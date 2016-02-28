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

TSmil::TSmil(QObject *parent)
{
    Q_UNUSED(parent)
}

TSmil::~TSmil()
{
    ar_elements.clear();
}

void TSmil::init(QString smil_index)
{
    index_path = MyFile.loadToDom(smil_index);
    MyHead.parse(MyFile.getHead());
    return;
}

THead TSmil::getHeader()
{
    return MyHead;
}

void TSmil::beginSmilParsing()
{
    MyBody = new TBody();
    connect(MyBody, SIGNAL(foundElement(TContainer *, QDomElement )), this, SLOT(foundElement(TContainer *, QDomElement )));
    connect(MyBody, SIGNAL(finishedPlaylist(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    if (MyBody->parse(MyFile.getBody()))
        MyBody->prepareTimerBeforPlaying();
    return;
}

/**
 * @brief TSmil::foundElement slot catches the signal from playlist, prepare elements for plaing and create a hash map
 * @param parent
 * @param found_tag
 */
void TSmil::foundElement(TContainer *parent, QDomElement dom_element)
{
    TBaseTiming                             *MyBase;
    QString                            tag_name             = dom_element.tagName();
    bool                               playable             = false;
    QHash<QString, TBaseTiming *>::iterator  ar_elements_iterator = ar_elements.find(TBase::parseID(dom_element));
    if (ar_elements_iterator == ar_elements.end())
    {
        MyBase = TFactory::createBase(tag_name, parent);
        MyBase->parse(dom_element);
        if (MyBase != NULL)
        {
            ar_elements_iterator = insertIntoObjectContainer(parent, MyBase);
            QString base_type    = MyBase->getBaseType();
            if (base_type == "media")
                connectMediaSlots(qobject_cast<TMedia *> (MyBase));
            else if (base_type == "playlist")
                connectPlaylistSlots(MyBase);
        }
    }
    else
        MyBase  = *ar_elements_iterator;

    playable    = MyBase->isPlayable();
    if (playable)
        MyBase->prepareTimerBeforPlaying();

    QString object_name = parent->objectName();
    if (object_name == "TExcl" && playable)  // increment active child to determine end of a excl
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (parent);
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
    bool         playable      = true;

    if (parent->getBaseType() == "playlist")
    {
        TContainer *MyContainer = qobject_cast<TContainer *> (parent);
        playable                = MyContainer->isChildPlayable(element);
        if (playable)
            MyContainer->setPlayedElement(element);
    }

    if (playable)
    {
        element->play();
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<TMedia *> (element));
    }
    return;
}

/**
 * @brief TSmil::finishedElement slot get called when emit finishedMedia finishedPlaylist active duration (finish playing)
 *        finishElement should kill the timer (TSmil::stopElement(TBase *element)) of his child elements not his own
 *        except in the case is in an excl container and stpped by a peer
 * @param parent
 * @param element
 */
void TSmil::finishElement(TContainer *parent, TBaseTiming *element)
{
    //   finishElement should kill the timer (TSmil::stopElement(TBase *element)) of his child elements not his own
    //   except the case that the element itself is child in an excl container and stopped by a peer
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
void TSmil::pausePlaylingElement(TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << "pause Element";
    element->prepareTimerBeforePausing();
    if (element->hasPlayingChilds())
    {
        TContainer  *MyContainer      = qobject_cast<TContainer *> (element);
        pausePlaylingElement(MyContainer->getChildElementFromList());
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
void TSmil::stopPlaylingElement(TBaseTiming *element)
{
    qDebug()<< element->getID() << QTime::currentTime().toString() << "Kill Timer";
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
            stopPlaylingElement(element->getChildElementFromList()); // recurse with stopPlaylingElement to kill timer of childs
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
    if (parent->getBaseType() == "playlist")
    {
        TContainer *MyPlaylist  = qobject_cast<TContainer *> (parent);
        MyPlaylist->insertPlaylistObject(id, child);
    }
    return ar_elements.insert(id, child);;
}

void TSmil::connectPlaylistSlots(TBaseTiming *element)
{
    TContainer *MyPlaylist = qobject_cast<TContainer *> (element);
    if (MyPlaylist->isPlayable())
    {
        connect(MyPlaylist, SIGNAL(foundElement(TContainer *, QDomElement )), this, SLOT(foundElement(TContainer *, QDomElement )));
        connect(MyPlaylist, SIGNAL(startedPlaylist(TContainer *, TBaseTiming *)), this, SLOT(startElement(TContainer *, TBaseTiming *)));
        connect(MyPlaylist, SIGNAL(finishedPlaylist(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
        if (element->objectName() == "TExcl")
        {
            connect(MyPlaylist, SIGNAL(resumeElement(TBaseTiming *)), this, SLOT(resumeQueuedElement(TBaseTiming *)));
            connect(MyPlaylist, SIGNAL(pauseElement(TBaseTiming *)), this, SLOT(pausePlaylingElement(TBaseTiming *)));
            connect(MyPlaylist, SIGNAL(stopElement(TBaseTiming *)), this, SLOT(stopPlaylingElement(TBaseTiming *)));
        }
    }
    return;
}

void TSmil::connectMediaSlots(TMedia *MyMedia)
{
    if (MyMedia->isPlayable())
    {
        MyMedia->load(index_path);
        connect(MyMedia, SIGNAL(startedMedia(TContainer *, TBaseTiming *)), this, SLOT(startElement(TContainer *, TBaseTiming *)));
        connect(MyMedia, SIGNAL(finishedMedia(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    }
    return;
}

void TSmil::emitStartShowMedia(TMedia *media)
{
    qDebug() << media->getID() <<QTime::currentTime().toString() << "startShowMedia";
    ar_played_media.insert(media);
    emit startShowMedia(media);
    return;
}

void TSmil::emitStopShowMedia(TMedia *media)
{
    if (ar_played_media.find(media) != ar_played_media.end())
    {
        ar_played_media.remove(media);
        qDebug() << media->getID() <<QTime::currentTime().toString() << "stopShowMedia";
        emit stopShowMedia(media);
    }
    return;
}
