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

TSmil::TSmil(TConfiguration *config, QObject *parent)
{
    Q_UNUSED(parent)
    MyConfiguration = config;
    MyFileManager   = new TFileManager(MyConfiguration);
}

TSmil::~TSmil()
{
    clearLists();
}

void TSmil::init()
{
    MyFileManager->clearQueues();
    return;
}

void TSmil::clearLists()
{
    for (QHash<QString, TBaseTiming *>::iterator i = ar_elements.begin(); i != ar_elements.end(); i++)
    {
        delete ar_elements[i.key()];
    }
    ar_elements.clear();
    ar_played_media.clear();
    return;
}


void TSmil::beginSmilParsing(QDomElement body)
{
    MyBody = new TBody();
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
void TSmil::foundElement(TContainer *ParentContainer, QString type, QDomElement dom_element)
{
    TBaseTiming                              *MyBaseTiming;
    QHash<QString, TBaseTiming *>::iterator  ar_elements_iterator = ar_elements.find(TBase::parseID(dom_element));
    QString                                  base_type            = "";
    if (ar_elements_iterator == ar_elements.end())
    {
        MyBaseTiming     = TFactory::createBase(type, ParentContainer);
        base_type  = MyBaseTiming->getBaseType();
        MyBaseTiming->parse(dom_element);
        if (MyBaseTiming != NULL)
        {
            ar_elements_iterator = insertIntoObjectContainer(ParentContainer, MyBaseTiming);
            if (base_type == "media")
                connectMediaSlots(qobject_cast<TMedia *> (MyBaseTiming));
            else if (base_type == "container")
                connectContainerSlots(qobject_cast<TContainer *> (MyBaseTiming));
        }
    }
    else
    {
        MyBaseTiming     = *ar_elements_iterator;
        base_type  = MyBaseTiming->getBaseType();
    }
    qDebug() << MyBaseTiming->getID() << QTime::currentTime().toString()  << " foundElement";
    if (base_type == "media")
    {
        TMedia *MyMedia = qobject_cast<TMedia *> (MyBaseTiming);
        MyMedia->isLoaded();
    }
     MyBaseTiming->prepareTimerBeforePlaying();

    QString object_name = ParentContainer->objectName();
    if (object_name == "TExcl")  // increment active child to determine end of a excl
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (ParentContainer);
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
void TSmil::startElement(TContainer *parent, TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << " startElement";
    bool         playable      = true;
    if (parent->getBaseType() == "container")
    {

        TContainer *MyContainer = qobject_cast<TContainer *> (parent);
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
void TSmil::finishElement(TContainer *parent, TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << " finishElement";
    if (element->objectName() != "TBody") // when TBody ends there is no parent and nothing todo anymore
    {
        qDebug()<< element->getID() << QTime::currentTime().toString() << "Not kill Timer";
        stopElement(element);
        parent->next(element);
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
    qDebug() << element->getID() << QTime::currentTime().toString() << "pause Element";
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
    qDebug() << element->getID() << QTime::currentTime().toString() << "resume Element";
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
            qDebug()<< element->getID() << QTime::currentTime().toString() << "has Childs";
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
    qDebug() << element->getID() << QTime::currentTime().toString() << "kill Timer";
    element->prepareTimerBeforeStop();
    return;
}

QHash<QString, TBaseTiming *>::iterator TSmil::insertIntoObjectContainer(TBaseTiming *parent, TBaseTiming *child)
{
    QString type = parent->objectName();
    QString id   = child->getID();
    if (parent->getBaseType() == "container")
    {
        TContainer *MyPlaylist  = qobject_cast<TContainer *> (parent);
        MyPlaylist->insertContainerObject(id, child);
    }
    return ar_elements.insert(id, child);;
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

void TSmil::connectMediaSlots(TMedia *MyMedia)
{
    MyMedia->prepareLoad(MyFileManager);
    connect(MyMedia, SIGNAL(startedMedia(TContainer *, TBaseTiming *)), this, SLOT(startElement(TContainer *, TBaseTiming *)));
    connect(MyMedia, SIGNAL(finishedMedia(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    return;
}

void TSmil::emitStartShowMedia(TMedia *MyMedia)
{
    if (MyMedia->isLoaded())
    {
        MyMedia->play();
        qDebug() << MyMedia->getID() <<QTime::currentTime().toString() << "startShowMedia";
        ar_played_media.insert(MyMedia);
        emit startShowMedia(MyMedia);
    }
    return;
}

void TSmil::emitStopShowMedia(TMedia *MyMedia)
{
    if (ar_played_media.find(MyMedia) != ar_played_media.end())
    {
        ar_played_media.remove(MyMedia);
        qDebug() << MyMedia->getID() <<QTime::currentTime().toString() << "stopShowMedia";
        emit stopShowMedia(MyMedia);
    }
    return;
}
