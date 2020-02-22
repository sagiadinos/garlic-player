/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/

#include "body_parser.h"

BodyParser::BodyParser(Files::MediaManager *mm, ElementsContainer *ec, QObject *parent) : QObject(parent)
{
    MyElementsContainer   = ec;
    MyMediaManager        = mm;
    MyCurrentPlayingMedia = new CurrentPlayingMedia(MyMediaManager, this);
    stop_all              = false;
}

BodyParser::~BodyParser()
{
}

/**
 * @brief TSmil::beginSmilParsing
 * Here starts the parsing withthe body Tag
 *
 * @param body
 */
void BodyParser::beginSmilParsing(QDomElement body)
{
    MyBody.reset(new TBody(this));
    connect(MyBody.data(), SIGNAL(foundElement(TContainer *, QDomElement )), this, SLOT(useElement(TContainer *, QDomElement )));
    connect(MyBody.data(), SIGNAL(finishedContainer(TContainer *, BaseTimings *)), this, SLOT(finishElement(TContainer *, BaseTimings *)));

    connect(MyBody.data(), SIGNAL(preloadElement(TContainer *, QDomElement)), this, SLOT(preloadElement(TContainer *, QDomElement)));
    connect(MyBody.data(), SIGNAL(finishPreload()), this, SLOT(beginPlaying()));

    MyBody.data()->preloadParse(body);
    return;
}

void BodyParser::endSmilParsing()
{
    stop_all = true;

    while (MyCurrentPlayingMedia->count() > 0) // stop all currently played media
    {
        stopPlayingElement(MyCurrentPlayingMedia->getFirstPlayingObject());
    }
    return;
}


void BodyParser::preloadElement(TContainer *parent_container, QDomElement element)
{
    BaseTimings *MyBaseTimings = ElementFactory::createBase(element, parent_container, this);
    connectSlots(MyBaseTimings);

    // Important! Slots needs to be connected before parsing!
    // parsing emits a foundElement signal so without connected Slot no one
    // can receive the signal and parsing will be interrupted
    MyBaseTimings->preloadParse(element);

    MyElementsContainer->insertSmilElement(MyBaseTimings);

    // media must be initialised after parse
    if (MyBaseTimings->getBaseType() == "media")
    {
        initMedia(qobject_cast<BaseMedia *> (MyBaseTimings));
    }

    qDebug(Develop) << MyBaseTimings->getID() << " preloaded";
    return;
}

void BodyParser::beginPlaying()
{
    MyBody.data()->prepareTimingsBeforePlaying();
}

/**
 * @brief BodyParser::initMedia media must be init e.g. to start download
 * @param MyMedia
 */
void BodyParser::initMedia(BaseMedia *MyMedia)
{
    // media must be initialised after parse, because register needs src
    // and insertSmilMedia needs the region
    MyElementsContainer->insertSmilMedia(MyMedia);
    MyMedia->registerInMediaManager(MyMediaManager);
}


/**
 * @brief TSmil::foundElement slot catches the signals from the containers and
 * @param parent
 * @param found_tag
 */
void BodyParser::useElement(TContainer *parent_container, QDomElement dom_element)
{
    BaseTimings *MyBaseTimings = MyElementsContainer->findSmilElement(dom_element);
    if (MyBaseTimings != Q_NULLPTR)
    {
        MyBaseTimings->prepareTimingsBeforePlaying(); // here we start
    }

    if (parent_container->objectName() == "TExcl")  // increment active child to determine end of a excl
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (parent_container);
        MyExclParent->childStarted(MyBaseTimings);
    }

    return;
}



/**
 * @brief TSmil::startedElement slot catch the start signal from a element and check if it is allowed to play.
 *        If the element is a media a signal will emitted to an output interface which can show the media e.g. player-widget
 *
 * @param parent
 * @param element
 */
void BodyParser::startElement(TContainer *parent_container, BaseTimings *element)
{
    bool         playable      = true;
    if (parent_container->getBaseType() == "container")
    {
        TContainer *MyContainer = qobject_cast<TContainer *> (parent_container);
        playable                = MyContainer->isChildPlayable(element);
        if (playable)
            MyContainer->setPlayedElement(element);
    }

    if (playable)
    {
        element->play();
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<BaseMedia *> (element));
    }
    return;
}

/**
 * @brief TSmil::finishedElement slot get called when emit finishedMedia finishedContainer active duration (finish playing)
 *        finishElement should kill the timer (TSmil::stopElement(TBase *element)) of his child elements not his own
 *        except in the case is in an excl container and stopped by a peer
 * @param parent
 * @param element
 */
void BodyParser::finishElement(TContainer *parent_container, BaseTimings *element)
{
    qDebug() << element->getID() <<  " finishElement";
    if (element->objectName() != "TBody") // when TBody ends there is no parent and nothing todo anymore
    {
        // Not kill a Timer here
        stopElement(element);

        parent_container->next(element);
    }
    return;
}

/**
 * @brief TSmil::handlePause prepares elements for pause, pause them and recurses their childs
 * @param parent
 * @param element
 */
void BodyParser::pausePlayingElement(BaseTimings *element)
{
    qDebug() << element->getID() << "pause Element";
    element->prepareTimingsBeforePausing();
    if (element->hasPlayingChilds())
    {
        TContainer  *MyContainer      = qobject_cast<TContainer *> (element);
        pausePlayingElement(MyContainer->getChildElementFromList());
    }
    element->pause();
    if (element->getBaseType() == "media")
        emitStopShowMedia(qobject_cast<BaseMedia *> (element));
    return;
}

void BodyParser::resumeQueuedElement(BaseTimings *element)
{
    qDebug() << element->getID() << "resume Element";
    element->prepareTimingsBeforeResume();
    if (element->getStatus() == element->_paused)
    {
        if (element->hasPlayingChilds())
            resumeQueuedElement(element->getChildElementFromList());
        element->resume();
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<BaseMedia *> (element));
    }
    return;
}

/**
 * @brief TSmil::handleStop checks when an element stopped or finished and recurses to stop all active child elements e.g. timers, too.
 *        If the element is a media a signal will emitted to an output interface which can stop the show the media e.g. player-widget
 * @param parent
 * @param element
 */
void BodyParser::stopPlayingElement(BaseTimings *element)
{
    if (element == Q_NULLPTR)
        return;

    element->prepareTimingsBeforeStop();
    stopElement(element);
    return;
}

// ============================== private methods =======================================

void BodyParser::stopElement(BaseTimings *element)
{
    if (element->getStatus() != element->_stopped)
    {
        if (element->hasPlayingChilds())
        {
            stopPlayingElement(element->getChildElementFromList()); // recurse with stopPlaylingElement to kill timer of childs
        }
        element->stop();
        if (element->getBaseType() == "media")
            emitStopShowMedia(qobject_cast<BaseMedia *> (element));
    }
    return;
}

void BodyParser::connectSlots(BaseTimings *element)
{
    QString      base_type  = element->getBaseType();
    if (base_type == "media")
    {
        connectMediaSlots(qobject_cast<BaseMedia *> (element));
    }
    else if (base_type == "container")
    {
        connectContainerSlots(qobject_cast<TContainer *> (element));
    }
}

void BodyParser::connectContainerSlots(TContainer *MyContainer)
{
    connect(MyContainer, SIGNAL(preloadElement(TContainer *, QDomElement )), this, SLOT(preloadElement(TContainer *, QDomElement )));
    connect(MyContainer, SIGNAL(foundElement(TContainer *, QDomElement )), this, SLOT(useElement(TContainer *, QDomElement )));
    connect(MyContainer, SIGNAL(startedContainer(TContainer *, BaseTimings *)), this, SLOT(startElement(TContainer *, BaseTimings *)));
    connect(MyContainer, SIGNAL(finishedContainer(TContainer *, BaseTimings *)), this, SLOT(finishElement(TContainer *, BaseTimings *)));
    if (MyContainer->objectName() == "TExcl")
    {
        connect(MyContainer, SIGNAL(resumeElement(BaseTimings *)), this, SLOT(resumeQueuedElement(BaseTimings *)));
        connect(MyContainer, SIGNAL(pauseElement(BaseTimings *)), this, SLOT(pausePlayingElement(BaseTimings *)));
        connect(MyContainer, SIGNAL(stopElement(BaseTimings *)), this, SLOT(stopPlayingElement(BaseTimings *)));
    }
    return;
}

void BodyParser::connectMediaSlots(BaseMedia *media)
{
    connect(media, SIGNAL(startedMedia(TContainer *, BaseTimings *)), this, SLOT(startElement(TContainer *, BaseTimings *)));
    connect(media, SIGNAL(finishedMedia(TContainer *, BaseTimings *)), this, SLOT(finishElement(TContainer *, BaseTimings *)));
    return;
}

void BodyParser::emitStartShowMedia(BaseMedia *media)
{
    if (stop_all) // preventing for starting media during stop process
        return;
    MyCurrentPlayingMedia->insert(media);
    emit startShowMedia(media);
    return;
}

void BodyParser::emitStopShowMedia(BaseMedia *media)
{
    MyCurrentPlayingMedia->remove(media);
    emit stopShowMedia(media);
    return;
}
