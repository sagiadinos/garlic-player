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

#include "smilparser.h"

TSmil::TSmil(MediaManager *mm, QObject *parent) : QObject(parent)
{
    MyMediaManager   = mm;
}

TSmil::~TSmil()
{
    clearLists();
    delete MyMediaManager;
}

void TSmil::init()
{
    return;
}

void TSmil::clearLists()
{
    QSet<TBaseTiming *>::iterator i;
    while (ar_played_media.size() > 0) // stop actual played media
    {
        i = ar_played_media.begin();
        stopPlayingElement(*i);
    }
    qDeleteAll(ar_elements);
    ar_elements.clear();
    qDeleteAll(ar_played_media);
    ar_played_media.clear();
    return;
}


void TSmil::beginSmilParsing(QDomElement body)
{
    MyBody = new TBody(this);
    connect(MyBody, SIGNAL(foundElement(TContainer *, QString, QDomElement )), this, SLOT(foundElement(TContainer *, QString, QDomElement )));
    connect(MyBody, SIGNAL(finishedContainer(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    if (MyBody->parse(body))
        MyBody->prepareTimerBeforePlaying();
    return;
}

/**
 * @brief TSmil::foundElement slot catches the signal from playlist, prepare elements for plaing and create a hash map
 * @param parent
 * @param found_tag
 */
void TSmil::foundElement(TContainer *parent_container, QString type, QDomElement dom_element)
{
    TBaseTiming                              *MyBaseTiming;
    QHash<QString, TBaseTiming *>::iterator  ar_elements_iterator = ar_elements.find(TBase::parseID(dom_element));
    QString                                  base_type            = "";
    if (ar_elements_iterator == ar_elements.end())
    {
        MyBaseTiming     = TFactory::createBase(type, parent_container, this);
        base_type        = MyBaseTiming->getBaseType();
        MyBaseTiming->parse(dom_element);
        if (MyBaseTiming != NULL)
        {
            ar_elements_iterator = insertIntoObjectContainer(parent_container, MyBaseTiming);
            if (base_type == "media")
                connectMediaSlots(qobject_cast<TMedia *> (MyBaseTiming));
            else if (base_type == "container")
                connectContainerSlots(qobject_cast<TContainer *> (MyBaseTiming));
        }
    }
    else
    {
        MyBaseTiming     = *ar_elements_iterator;
        base_type        = MyBaseTiming->getBaseType();
    }

    qDebug() << MyBaseTiming->getID() << " foundElement";

    MyBaseTiming->prepareTimerBeforePlaying();

    QString object_name = parent_container->objectName();
    if (object_name == "TExcl")  // increment active child to determine end of a excl
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (parent_container);
        MyExclParent->childStarted(*ar_elements_iterator);
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
void TSmil::startElement(TContainer *parent_container, TBaseTiming *element)
{
    qDebug() << element->getID() << " startElement";
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
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<TMedia *> (element));
        else
            element->play();
    }
    return;
}

/**
 * @brief TSmil::finishedElement slot get called when emit finishedMedia finishedContainer active duration (finish playing)
 *        finishElement should kill the timer (TSmil::stopElement(TBase *element)) of his child elements not his own
 *        except in the case is in an excl container and stpped by a peer
 * @param parent
 * @param element
 */
void TSmil::finishElement(TContainer *parent_container, TBaseTiming *element)
{
    qDebug() << element->getID() <<  " finishElement";
    if (element->objectName() != "TBody") // when TBody ends there is no parent and nothing todo anymore
    {
        qDebug()<< element->getID() << "Not kill Timer";
        stopElement(element);

        parent_container->next(element);
    }
    else
    {
        ar_elements.clear();
        delete MyBody;
    }
    return;
}


/**
 * @brief TSmil::handlePause prepares elements for pause, pause them and recurses their childs
 * @param parent
 * @param element
 */
void TSmil::pausePlayingElement(TBaseTiming *element)
{
    qDebug() << element->getID() << "pause Element";
    element->prepareTimerBeforePausing();
    if (element->hasPlayingChilds())
    {
        TContainer  *MyContainer      = qobject_cast<TContainer *> (element);
        pausePlayingElement(MyContainer->getChildElementFromList());
    }
    element->pause();
    if (element->getBaseType() == "media")
        emitStopShowMedia(qobject_cast<TMedia *> (element));
    return;
}

void TSmil::resumeQueuedElement(TBaseTiming *element)
{
    qDebug() << element->getID() << "resume Element";
    element->prepareTimerBeforeResume();
    if (element->getStatus() == element->_paused)
    {
        if (element->hasPlayingChilds())
            resumeQueuedElement(element->getChildElementFromList());
        element->resume();
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<TMedia *> (element));
    }
    return;
}

/**
 * @brief TSmil::handleStop checks when an element stopped or finished and recurses to stop all active child elements e.g. timers, too.
 *        If the element is a media a signal will emitted to an output interface which can stop the show the media e.g. player-widget
 * @param parent
 * @param element
 */
void TSmil::stopPlayingElement(TBaseTiming *element)
{
    killTimer(element);
    stopElement(element);
    return;
}

// ============================== private methods =======================================

void TSmil::stopElement(TBaseTiming *element)
{
    if (element->getStatus() != element->_stopped)
    {
        if (element->hasPlayingChilds())
        {
            stopPlayingElement(element->getChildElementFromList()); // recurse with stopPlaylingElement to kill timer of childs
        }
        element->stop();
        if (element->getBaseType() == "media")
            emitStopShowMedia(qobject_cast<TMedia *> (element));
    }
    return;
}

void TSmil::killTimer(TBaseTiming *element)
{
    element->prepareTimerBeforeStop();
    return;
}

QHash<QString, TBaseTiming *>::iterator TSmil::insertIntoObjectContainer(TBaseTiming *parent_container, TBaseTiming *child_container)
{
    QString id   = child_container->getID();
    if (parent_container->getBaseType() == "container")
    {
        TContainer *MyContainer  = qobject_cast<TContainer *> (parent_container);
        MyContainer->insertContainerObject(id, child_container);
    }
    return ar_elements.insert(id, child_container);
}

void TSmil::connectContainerSlots(TContainer *MyContainer)
{
    connect(MyContainer, SIGNAL(foundElement(TContainer *, QString, QDomElement )), this, SLOT(foundElement(TContainer *, QString, QDomElement )));
    connect(MyContainer, SIGNAL(startedContainer(TContainer *, TBaseTiming *)), this, SLOT(startElement(TContainer *, TBaseTiming *)));
    connect(MyContainer, SIGNAL(finishedContainer(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    if (MyContainer->objectName() == "TExcl")
    {
        connect(MyContainer, SIGNAL(resumeElement(TBaseTiming *)), this, SLOT(resumeQueuedElement(TBaseTiming *)));
        connect(MyContainer, SIGNAL(pauseElement(TBaseTiming *)), this, SLOT(pausePlayingElement(TBaseTiming *)));
        connect(MyContainer, SIGNAL(stopElement(TBaseTiming *)), this, SLOT(stopPlayingElement(TBaseTiming *)));
    }
    return;
}

void TSmil::connectMediaSlots(TMedia *media)
{
    media->registerFile(MyMediaManager);
    connect(media, SIGNAL(startedMedia(TContainer *, TBaseTiming *)), this, SLOT(startElement(TContainer *, TBaseTiming *)));
    connect(media, SIGNAL(finishedMedia(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    return;
}

void TSmil::emitStartShowMedia(TMedia *media)
{
    media->play();
    qDebug() << media->getID() << "startShowMedia";
    ar_played_media.insert(media);
    emit startShowMedia(media);
    return;
}

void TSmil::emitStopShowMedia(TMedia *media)
{
    if (ar_played_media.find(media) != ar_played_media.end())
    {
        ar_played_media.remove(media);
        qDebug() << media->getID() << "stopShowMedia";
        emit stopShowMedia(media);
    }
    return;
}
