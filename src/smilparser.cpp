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
    connect(MyBody, SIGNAL(finishedPlaylist(TContainer *, TBase *)), this, SLOT(finishElement(TContainer *, TBase *)));
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
    TBase                             *MyBase;
    QString                            tag_name             = dom_element.tagName();
    bool                               playable             = false;
    QHash<QString, TBase *>::iterator  ar_elements_iterator = ar_elements.find(TBase::parseID(dom_element));
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
void TSmil::startElement(TContainer *parent, TBase *element)
{
    bool         playable      = true;

    if (parent->getBaseType() == "playlist")
    {
        TContainer *MyPlaylist  = qobject_cast<TContainer *> (parent);
        playable               = MyPlaylist->isChildPlayable(element);
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
 * @param parent
 * @param element
 */
void TSmil::finishElement(TContainer *parent, TBase *element)
{
    /*
    ToDo:Problem:
    finishElement should only kill the timer (stopElement) of the child elements
    */

    if (element->objectName() != "TBody") // when TBody ends there is no parent and nothing todo anymore
    {
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
 * @brief TSmil::handleStop checks when an element stopped or finished and recurses to stop all active child elements e.g. timers, too.
 *        If the element is a media a signal will emitted to an output interface which can stop the show the media e.g. player-widget
 * @param parent
 * @param element
 */
void TSmil::stopElement(TBase *element)
{
    element->prepareTimerBeforeStop();
    if (element->getStatus() != element->_stopped)
    {
        if (element->hasPlayingChilds())
            stopElement(element->getPlayedElement());
        element->stop();
        if (element->getBaseType() == "media")
            emitStopShowMedia(qobject_cast<TMedia *> (element));
    }
    return;
}

/**
 * @brief TSmil::handlePause prepares elements for pause, pause them and recurses their childs
 * @param parent
 * @param element
 */
void TSmil::pauseElement(TBase *element)
{
    element->prepareTimerBeforePausing();
    if (element->hasPlayingChilds())
        pauseElement(element->getPlayedElement());
    element->pause();
    if (element->getBaseType() == "media")
        emitStopShowMedia(qobject_cast<TMedia *> (element));
    return;
}

void TSmil::resumeElement(TBase *element)
{
    element->prepareTimerBeforeResume();
    if (element->getStatus() == element->_paused)
    {
        if (element->hasPlayingChilds())
            resumeElement(element->getPlayedElement());
        element->resume();
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<TMedia *> (element));
    }
    return;
}


// ============================== private methods =======================================

QHash<QString, TBase *>::iterator TSmil::insertIntoObjectContainer(TBase *parent, TBase *child)
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

void TSmil::connectPlaylistSlots(TBase *element)
{
    TContainer *MyPlaylist = qobject_cast<TContainer *> (element);
    if (MyPlaylist->isPlayable())
    {
        connect(MyPlaylist, SIGNAL(foundElement(TContainer *, QDomElement )), this, SLOT(foundElement(TContainer *, QDomElement )));
        connect(MyPlaylist, SIGNAL(startedPlaylist(TContainer *, TBase *)), this, SLOT(startElement(TContainer *, TBase *)));
        connect(MyPlaylist, SIGNAL(finishedPlaylist(TContainer *, TBase *)), this, SLOT(finishElement(TContainer *, TBase *)));
        if (element->objectName() == "TExcl")
        {
            connect(MyPlaylist, SIGNAL(resumeElement(TBase *)), this, SLOT(resumeElement(TBase *)));
            connect(MyPlaylist, SIGNAL(pauseElement(TBase *)), this, SLOT(pauseElement(TBase *)));
            connect(MyPlaylist, SIGNAL(stopElement(TBase *)), this, SLOT(stopElement(TBase *)));
        }
    }
    return;
}

void TSmil::connectMediaSlots(TMedia *MyMedia)
{
    if (MyMedia->isPlayable())
    {
        MyMedia->load(index_path);
        connect(MyMedia, SIGNAL(startedMedia(TContainer *, TBase *)), this, SLOT(startElement(TContainer *, TBase *)));
        connect(MyMedia, SIGNAL(finishedMedia(TContainer *, TBase *)), this, SLOT(finishElement(TContainer *, TBase *)));
    }
    return;
}

void TSmil::emitStartShowMedia(TMedia *media)
{
    qDebug() << media->getID() << "startShowMedia";
    ar_played_media.insert(media);
    emit startShowMedia(media);
    return;
}

void TSmil::emitStopShowMedia(TMedia *media)
{
    qDebug() << media->getID() << "try stopShowMedia";
    if (ar_played_media.find(media) != ar_played_media.end())
    {
        ar_played_media.remove(media);
        qDebug() << media->getID() << "stopShowMedia";
        emit stopShowMedia(media);
    }
    return;
}
