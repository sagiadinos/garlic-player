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
    connect(MyBody, SIGNAL(foundElement(TBase *, QDomElement )), this, SLOT(foundElement(TBase *, QDomElement)));
    connect(MyBody, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
    if (MyBody->parse(MyFile.getBody()))
        MyBody->preparePlay();
    return;
}

/**
 * @brief Overloaded TSmil::next() when a media emit finished get the next action
 *        For root (body) which childs is handled similar to sequential playlists this
 *        could be a new media or a finished. (no repeat possible)
 * @param void
 */
void TSmil::next()
{
    MyBody->next();
    return;
}

/**
 * @brief Overloaded TSmil::next(TSeq *) when a media emit finished get the next action
 *        For sequential playlists this could be a new media, a playlist repeat or a finished
 * @param MySeq
 */
void TSmil::next(TSeq *MySeq)
{
    MySeq->next();
    return;
}

/**
 * @brief Overloaded TSmil::next(TPar *) when a media emit finished decrease the active elements and
 *        get the next action. For a parallel playlist his could be nothing, cause there
 *        are active elements, a repeat of all active media or a finish.
 * @param MyPar
 */
void TSmil::next(TPar *MyPar, TBase *element)
{
    MyPar->childEnded(element);
    MyPar->next();
    return;
}

void TSmil::next(TExcl *MyExcl, TBase *element)
{
    MyExcl->setChildActive(false);
    MyExcl->childEnded(element);
    MyExcl->next();
    return;
}

/**
 * @brief TSmil::foundElement slot catches the signal from playlist, prepare elements for plaing and create a hash map
 * @param parent
 * @param found_tag
 */
void TSmil::foundElement(TBase *parent, QDomElement dom_element)
{
    TBase                             *MyBase;
    QString                            tag_name             = dom_element.tagName();
    bool                               playable             = false;
    QHash<QString, TBase *>::iterator  ar_elements_iterator = ar_elements.find(TBase::parseID(dom_element));
    if (ar_elements_iterator == ar_elements.end())
    {
        MyBase = TFactory::createBase(tag_name, parent);
        MyBase->parse(dom_element);
        QString id = TBase::parseID(dom_element);
        if (MyBase != NULL)
        {
            ar_elements_iterator = insertIntoObjectContainer(parent, MyBase);
            if (tag_name == "img" || tag_name == "video" || tag_name == "audio" || tag_name == "text")
                prepareMyMedia(MyBase);
            else if (tag_name == "seq" || tag_name == "par" || tag_name == "excl")
                prepareMyPlaylist(MyBase);
        }
    }
    else
        MyBase  = *ar_elements_iterator;

    if (MyBase->checkifPlayable())
        MyBase->preparePlay();


    if (parent->objectName() == "TExcl" && playable)  // increment active child to determine end of a excl
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
void TSmil::startElement(TBase *parent, TBase *element)
{
    QString      base_type       = element->getBaseType();
    QString      parent_type     = parent->objectName();
    bool         display_media   = false;
    QDomElement  root_domelement = element->getRootElement();

    if (parent_type == "TSeq")  // increment active child to determine end of a par
    {
        TSeq *MySeq  = qobject_cast<TSeq *> (parent);
        MySeq->childStarted(element);
        MySeq->setPlayedElement(element);
        display_media  = true;
    }
    if (parent_type == "TPar")  // increment active child to determine end of a par
    {
        TPar *MyPar  = qobject_cast<TPar *> (parent);
        MyPar->childStarted(element);
        display_media  = true;
    }
    else if (parent_type == "TExcl")
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (parent);
        TBase   *played_element = MyExclParent->getPlayedElement(); // must set before interrupting
        int      interrupt      = MyExclParent->interruptActualPlaying(root_domelement, element);
        QString id = TExcl::parseID(root_domelement);
        if (interrupt == MyExclParent->_stop_active) // stop active
        {
            MyExclParent->childEnded(played_element);
            stopElement(played_element);
            display_media  = true;
            MyExclParent->setChildActive(true);
        }
        if (interrupt == MyExclParent->_pause_active) // stop active
        {
            pauseElement(played_element);
            display_media  = true;
            MyExclParent->setChildActive(true);
        }
        else if (interrupt == MyExclParent->_play_this)
        {
            MyExclParent->setChildActive(true);
            display_media  = true;
        }
        else if (interrupt == MyExclParent->_stop_new) // stop caller when peers = never
        {
            MyExclParent->childEnded(element);
            stopElement(element);
            if(played_element == element)
                MyExclParent->setChildActive(false);
        }
        else if (interrupt == MyExclParent->_pause_new)
            pauseElement(element);
    }
    else
        display_media  = true;

    if (base_type == "media" && display_media)
    {
        TMedia *MyMedia = qobject_cast<TMedia *> (element);
        MyMedia->play();
        emitStartShowMedia(element);
    }
    else if (base_type == "playlist")
    {
        TPlaylist *MyPlaylist = qobject_cast<TPlaylist *> (element);
        MyPlaylist->play();
    }
    return;
}

/**
 * @brief TSmil::finishedElement slot get called when emit finishedMedia finishedPlaylist active duration (finish playing)
 * @param parent
 * @param element
 */
void TSmil::finishElement(TBase *parent, TBase *element)
{
    if (element->objectName() != "TBody") // when TBody ends there is no parent and nothing todo anymore
    {
        stopElement(element);
        QString parent_type = parent->objectName();
        if (parent_type == "TSeq")
            next(qobject_cast<TSeq *> (parent));
        else if (parent_type == "TPar")
            next(qobject_cast<TPar *> (parent), element);
        else if (parent_type == "TExcl")
            next(qobject_cast<TExcl *> (parent), element);
        else if (parent_type == "TBody")
            next();
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
    QString base_type   = element->getBaseType();
    element->prepareStop();
    if (base_type == "media")
    {
        TMedia *MyMedia = qobject_cast<TMedia *> (element);
        if (element->getStatus() != element->_stopped)
        {
            MyMedia->stop();
            emitStopShowMedia(element);
        }
    }
    else if (base_type == "playlist")
    {
        QString type = element->objectName();
        if (type == "TSeq")
        {
            TSeq *MySeq = qobject_cast<TSeq *> (element);
            stopElement(MySeq->getPlayedElement());
            MySeq->stop();
        }
        else if (type == "TPar")
        {
            TPar *MyPar = qobject_cast<TPar *> (element);
            QHash<QString, TBase *>           list = MyPar->getPlaylistObjects();
            QHash<QString, TBase *>::iterator i;
            for (i = list.begin(); i != list.end(); i++)
            {
                stopElement(*i);
            }
            MyPar->stop();
        }
        else if (type == "TExcl")
        {
            TExcl *MyExcl = qobject_cast<TExcl *> (element);
            stopElement(MyExcl->getPlayedElement());
            MyExcl->stop();
        }
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
    QString base_type   = element->getBaseType();
    element->preparePause();
    if (base_type == "media")
    {
        TMedia *MyMedia = qobject_cast<TMedia *> (element);
        MyMedia->pause();
        emitStopShowMedia(element);
    }
    else if (base_type == "playlist")
    {
        QString type        = element->objectName();
        if (type == "TSeq")
        {
            TSeq *MySeq = qobject_cast<TSeq *> (element);
            pauseElement(MySeq->getPlayedElement());
            MySeq->pause();
        }
        else if (type == "TPar")
        {
            TPar *MyPar = qobject_cast<TPar *> (element);
            QHash<QString, TBase *>           list = MyPar->getPlaylistObjects();
            QHash<QString, TBase *>::iterator i;
            for (i = list.begin(); i != list.end(); i++)
            {
                pauseElement(*i);
            }
            MyPar->pause();
        }
        else if (type == "TExcl")
        {
            TExcl *MyExcl = qobject_cast<TExcl *> (element);
            pauseElement(MyExcl->getPlayedElement());
            MyExcl->pause();
        }
    }
    return;
}

void TSmil::resumeElement(TBase *element)
{
    QString base_type   = element->getBaseType();
    element->prepareResume();
    if (base_type == "media")
    {
        TMedia *MyMedia = qobject_cast<TMedia *> (element);
        if (MyMedia->getStatus() == element->_paused)
        {
            MyMedia->play();
            emitStartShowMedia(element);
        }
    }
    else if (base_type == "playlist")
    {
        QString type        = element->objectName();
        if (type == "TSeq")
        {
            TSeq *MySeq = qobject_cast<TSeq *> (element);
            resumeElement(MySeq->getPlayedElement());
            MySeq->resume();
        }
        else if (type == "TPar")
        {
            TPar *MyPar = qobject_cast<TPar *> (element);
            QHash<QString, TBase *>           list = MyPar->getPlaylistObjects();
            QHash<QString, TBase *>::iterator i;
            for (i = list.begin(); i != list.end(); i++)
            {
                resumeElement(*i);
            }
            MyPar->resume();
        }
        else if (type == "TExcl")
        {
            TExcl *MyExcl = qobject_cast<TExcl *> (element);
            resumeElement(MyExcl->getPlayedElement());
            MyExcl->resume();
        }
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
        TPlaylist *MyPlaylist  = qobject_cast<TPlaylist *> (parent);
        MyPlaylist->insertPlaylistObject(id, child);
    }
    return ar_elements.insert(id, child);;
}

void TSmil::prepareMyPlaylist(TBase *element)
{
    TPlaylist *MyPlaylist = qobject_cast<TPlaylist *> (element);
    if (MyPlaylist->checkifPlayable())
    {
        connect(MyPlaylist, SIGNAL(foundElement(TBase *, QDomElement )), this, SLOT(foundElement(TBase *, QDomElement)));
        connect(MyPlaylist, SIGNAL(startedPlaylist(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
        connect(MyPlaylist, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
        if (element->objectName() == "TExcl")
            connect(MyPlaylist, SIGNAL(resumeElement(TBase *)), this, SLOT(resumeElement(TBase *)));
    }
    return;
}

void TSmil::prepareMyMedia(TBase *element)
{
    TMedia *MyMedia = qobject_cast<TMedia *> (element);
    if (MyMedia->checkifPlayable())
    {
        MyMedia->load(index_path);
        connect(MyMedia, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
        connect(MyMedia, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
    }
    return;
}

void TSmil::emitStartShowMedia(TBase *media)
{
    qDebug() << media->getID() << "startShowMedia";
    ar_played_media.insert(media);
    emit startShowMedia(media);
    return;
}

void TSmil::emitStopShowMedia(TBase *media)
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
