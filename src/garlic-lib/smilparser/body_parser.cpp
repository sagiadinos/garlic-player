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

BodyParser::BodyParser(ElementFactory *ef, Files::MediaManager *mm, ElementsContainer *ec, QObject *parent) : QObject(parent)
{
    MyElementFactory      = ef;
    MyMediaManager        = mm;
    MyElementsContainer   = ec;
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
void BodyParser::beginPreloading(QDomElement body)
{
    MyBody.reset(new TBody(Q_NULLPTR));
    connect(MyBody.data(), SIGNAL(preloadElementSignal(TContainer*,QDomElement)), this, SLOT(preloadElement(TContainer*,QDomElement)));
    connect(MyBody.data(), SIGNAL(finishPreloadSignal()), this, SLOT(emitPreloadingBodyCompleted()));
    // pause and presume not neccessary
    connect(MyBody.data(), SIGNAL(startElementSignal(BaseTimings*)), this, SLOT(startElement(BaseTimings*)));
    connect(MyBody.data(), SIGNAL(stopElementSignal(BaseTimings*)), this, SLOT(stopElement(BaseTimings*)));

    MyBody.data()->preloadParse(body);
    return;
}

void BodyParser::preloadElement(TContainer *ParentContainer, QDomElement dom_element)
{
    BaseTimings *MyBaseTimings = MyElementFactory->createBase(dom_element, ParentContainer, this);
    connectSlots(MyBaseTimings);

    // Important! Slots needs to be connected before parsing!
    MyBaseTimings->preloadParse(dom_element);

    // media must be initialised after parse
    if (MyBaseTimings->getBaseType() == "media")
    {
        initMedia(qobject_cast<BaseMedia *> (MyBaseTimings));
    }
    ParentContainer->insertDomChild(MyBaseTimings);
    MyElementsContainer->insertSmilElement(MyBaseTimings);
    return;
}

void BodyParser::emitPreloadingBodyCompleted()
{   
    emit preloadingBodyCompleted();
}

/**
 * @brief BodyParser::initMedia media must be init e.g. to start download
 * @param MyMedia
 */
void BodyParser::initMedia(BaseMedia *MyMedia)
{
    // media must be initialised after parse, because register needs src
    // and insertSmilMedia needs the region
    QString type   = MyMedia->objectName();
    if (type == "TImage" || type == "TAudio"  || type == "TVideo"  || type == "TWidget")
    {
        MyMedia->registerInMediaManager();
    }
    else if (type == "TWeb" || type == "TRefCommand")
    {
        MyMedia->registerInMediaManagerAsUncachable();
    }
    // do nothing with unknown
    MyElementsContainer->insertSmilMedia(MyMedia);
}

/**
 * start initial playing smil after preoload
 */
void BodyParser::startPlayingBody()
{
    MyBody.data()->startTimers();
}

/**
 * end playing
 */
void BodyParser::endPlayingBody()
{
    stop_all = true;

    BaseMedia *MyMedia;
    while (MyCurrentPlayingMedia->count() > 0) // stop all currently playing media
    {
        MyMedia = MyCurrentPlayingMedia->getFirstPlayingObject();
        MyMedia->stopTimers();
        emitStopShowMedia(MyMedia);
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
void BodyParser::startElement(BaseTimings *element)
{
    TContainer *ParentContainer = qobject_cast<TContainer *> (element->getParentContainer());
    if (ParentContainer != Q_NULLPTR && ParentContainer->objectName() == "TExcl")
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (ParentContainer);
        // determine if stop or pause previous or next child
        if (!MyExclParent->determineContinue(element))
            return;
    }

    element->start();
    qDebug() << "Start: " + element->getID();
    if (element->getBaseType() == "media")
        emitStartShowMedia(qobject_cast<BaseMedia *> (element));
}

/**
 * @brief TSmil::finishedElement slot get called when emit finishedMedia | finishedContainer
 *        => Activice Duration
 * @param parent
 * @param element
 */
void BodyParser::stopElement(BaseTimings *element)
{
    element->stop();
    qDebug() << "Stop: " + element->getID();

    TContainer *ParentContainer = qobject_cast<TContainer *> (element->getParentContainer());
    if (element->getBaseType() == "media")
        emitStopShowMedia(qobject_cast<BaseMedia *> (element));

    if (ParentContainer == Q_NULLPTR) // that means TBody as parent
        return;

    ParentContainer->next(element);
}

void BodyParser::pauseElement(BaseTimings *element)
{
    qDebug() <<  "Pause " + element->getID();;

    element->pauseTimers();
    element->pause();
    if (element->getBaseType() == "media")
        emitStopShowMedia(qobject_cast<BaseMedia *> (element));
    else
        (qobject_cast<TContainer *> (element))->emitPauseToAllActivatedChilds();
}

void BodyParser::resumeQueuedElement(BaseTimings *element)
{
    if (element == Q_NULLPTR)
        return;

    qDebug() << "Resume: " + element->getID();
    element->resumeTimers();
    element->resume();
    if (element->getBaseType() == "media")
        emitStartShowMedia(qobject_cast<BaseMedia *> (element));
    else
    {
        TContainer *ParentContainer = qobject_cast<TContainer *> (element);
        if (ParentContainer->objectName() != "TSeq")
            ParentContainer->emitResumeToAllActivatedChilds();
        else
            ParentContainer->startTimersOfFirstActivatedChild();
    }
}

void BodyParser::connectSlots(BaseTimings *element)
{
    connect(element, SIGNAL(startElementSignal(BaseTimings*)), this, SLOT(startElement(BaseTimings*)));
    connect(element, SIGNAL(stopElementSignal(BaseTimings*)), this, SLOT(stopElement(BaseTimings*)));
    connect(element, SIGNAL(resumeElementSignal(BaseTimings*)), this, SLOT(resumeQueuedElement(BaseTimings*)));
    connect(element, SIGNAL(pauseElementSignal(BaseTimings*)), this, SLOT(pauseElement(BaseTimings*)));

    if (element->getBaseType() == "container")
        connect(element, SIGNAL(preloadElementSignal(TContainer*,QDomElement)), this, SLOT(preloadElement(TContainer*,QDomElement)));
}

void BodyParser::emitStartShowMedia(BaseMedia *media)
{
    if (stop_all) // preventing for starting media during stop process
        return;
    MyCurrentPlayingMedia->insert(media);
    emit startShowMedia(media);
}

void BodyParser::emitStopShowMedia(BaseMedia *media)
{
    MyCurrentPlayingMedia->remove(media);
    emit stopShowMedia(media);
}
