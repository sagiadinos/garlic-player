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
void TSmil::next(TPar *MyPar)
{
    MyPar->decActiveChilds();
    MyPar->next();
    return;
}

void TSmil::next(TExcl *MyExcl, TBase *element)
{
    MyExcl->setChildActive(false); // gibt Probleme bei peers never wenn
    MyExcl->decActivatableChilds(element);
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
    QHash<QString, TBase *>::iterator ar_elements_iterator = ar_elements.find(TBase::parseID(dom_element));
    QString parent_type                                    = parent->objectName();
    bool playable                                          = false;
    if (dom_element.tagName() == "img")
    {
        TImage      *MyImage;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyImage = new TImage(parent);
            MyImage->setIndexPath(index_path); // must set before parse to find media
            MyImage->parse(dom_element);
            ar_elements_iterator = insertIntoObjectContainer(parent, MyImage);
            playable             = MyImage->checkifPlayable();
            if (playable)
            {
                connect(MyImage, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
                connect(MyImage, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
            }
        }
        else
        {
            MyImage  = qobject_cast<TImage *> (*ar_elements_iterator);
            playable = MyImage->checkifPlayable();
        }
        if (playable)
            MyImage->preparePlay();
    }
    else if (dom_element.tagName() == "video")
    {
        TVideo      *MyVideo;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyVideo = new TVideo(parent);
            MyVideo->setIndexPath(index_path);
            MyVideo->parse(dom_element);
            ar_elements_iterator = insertIntoObjectContainer(parent, MyVideo);
            playable             = MyVideo->checkifPlayable();
            if (playable)
            {
                connect(MyVideo, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
                connect(MyVideo, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
            }
        }
        else
        {
            MyVideo  = qobject_cast<TVideo*> (*ar_elements_iterator);
            playable = MyVideo->checkifPlayable();
        }
        if (playable)
            MyVideo->preparePlay();
    }
    else if (dom_element.tagName() == "audio")
    {
        TAudio      *MyAudio;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyAudio = new TAudio(parent);
            MyAudio->setIndexPath(index_path);
            MyAudio->parse(dom_element);
            ar_elements_iterator = insertIntoObjectContainer(parent, MyAudio);
            playable             = MyAudio->checkifPlayable();
            if (playable)
            {
                connect(MyAudio, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
                connect(MyAudio, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
            }
        }
        else
        {
            MyAudio  = qobject_cast<TAudio*> (*ar_elements_iterator);
            playable = MyAudio->checkifPlayable();
        }
        if (playable)
            MyAudio->preparePlay();
    }
    else if (dom_element.tagName() == "web")
    {
        TWeb        *MyWeb;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyWeb = new TWeb(parent);
            MyWeb->parse(dom_element);
            ar_elements_iterator = insertIntoObjectContainer(parent, MyWeb);
            playable = MyWeb->checkifPlayable();
            if (playable)
            {
                connect(MyWeb, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
                connect(MyWeb, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
            }
        }
        else
        {
            MyWeb    = qobject_cast<TWeb*> (*ar_elements_iterator);
            playable = MyWeb->checkifPlayable();
        }
        if (playable)
            MyWeb->preparePlay();
    }
    else if (dom_element.tagName() == "seq")
    {
        TSeq        *MySeq;
        if (ar_elements_iterator == ar_elements.end())
        {
            MySeq  = new TSeq(parent);
            MySeq->parse(dom_element);
            ar_elements_iterator = insertIntoObjectContainer(parent, MySeq);
            playable = MySeq->checkifPlayable();
            if (playable)
            {
                connect(MySeq, SIGNAL(foundElement(TBase *, QDomElement )), this, SLOT(foundElement(TBase *, QDomElement)));
                connect(MySeq, SIGNAL(startedPlaylist(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
                connect(MySeq, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
            }
        }
        else
        {
            MySeq = qobject_cast<TSeq*> (*ar_elements_iterator);
            playable = MySeq->checkifPlayable();
        }
        if (playable)
            MySeq->preparePlay();
    }
    else if (dom_element.tagName() == "par")
    {
        TPar        *MyPar;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyPar = new TPar(parent);
            MyPar->parse(dom_element);
            ar_elements_iterator = insertIntoObjectContainer(parent, MyPar);
            connect(MyPar, SIGNAL(foundElement(TBase *, QDomElement )), this, SLOT(foundElement(TBase *, QDomElement)));
            playable = MyPar->checkifPlayable();
            if (playable)
            {
                connect(MyPar, SIGNAL(startedPlaylist(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
                connect(MyPar, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
            }
        }
        else
        {
            MyPar = qobject_cast<TPar*> (*ar_elements_iterator);
            playable = MyPar->checkifPlayable();
        }
        if (playable)
            MyPar->preparePlay();
    }
    else if (dom_element.tagName() == "excl")
    {
        TExcl       *MyExcl;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyExcl               = new TExcl(parent);
            MyExcl->parse(dom_element);
            ar_elements_iterator = insertIntoObjectContainer(parent, MyExcl);
            playable = MyExcl->checkifPlayable();
            if (playable)
            {
                connect(MyExcl, SIGNAL(foundElement(TBase *, QDomElement )), this, SLOT(foundElement(TBase *, QDomElement)));
                connect(MyExcl, SIGNAL(startedPlaylist(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
                connect(MyExcl, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
                connect(MyExcl, SIGNAL(resumeElement(TBase *)), this, SLOT(handleResumeElement(TBase *)));
            }
        }
        else
        {
            MyExcl   = qobject_cast<TExcl*> (*ar_elements_iterator);
            playable = MyExcl->checkifPlayable();
        }
        if (playable)
            MyExcl->preparePlay();
    }
    if (parent_type == "TExcl" && playable)  // increment active child to determine end of a excl
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (parent);
        MyExclParent->incActivatableChilds(*ar_elements_iterator);
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
    QString      type            = element->objectName();
    QString      parent_type     = parent->objectName();
    bool         display_media   = false;
    QDomElement  root_domelement = element->getRootElement();

    if (parent_type == "TPar")  // increment active child to determine end of a par
    {
        TPar *MyPar  = qobject_cast<TPar *> (parent);
        MyPar->incActiveChilds();
    }
    if (parent_type == "TExcl")  // increment active child to determine end of a par
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (parent);
        TBase   *played_object  = MyExclParent->getPlayedObject(); // must set before interrupting
        int      interrupt      = MyExclParent->interruptActualPlaying(root_domelement, element);

        if (interrupt == MyExclParent->_stop_active) // stop active
        {
            MyExclParent->decActivatableChilds(played_object);
            handleStop(played_object);
            display_media  = true;
            MyExclParent->setChildActive(true);
        }
        if (interrupt == MyExclParent->_pause_active) // stop active
        {
            handlePause(played_object);
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
            MyExclParent->decActivatableChilds(element);
            handleStop(element);
            if(played_object == element)
                MyExclParent->setChildActive(false);
        }
        else if (interrupt == MyExclParent->_pause_new)
            handlePause(element);
    }
    else
        display_media  = true;

    if (type == "TSeq")
    {
        TSeq *MySeq = qobject_cast<TSeq *> (element);
        MySeq->play();
    }
    else if (type == "TPar")
    {
        TPar *MyPar = qobject_cast<TPar *> (element);
        MyPar->play();
    }
    else if (type == "TExcl")
    {
        TExcl *MyExcl = qobject_cast<TExcl *> (element);
        MyExcl->play();
    }
    else if (type == "TImage" && display_media)
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        MyImage->play();
        emitStartShowMedia(element);
    }
    else if (type == "TVideo" && display_media)
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        MyVideo->play();
        emitStartShowMedia(element);
    }
    else if (type == "TAudio" && display_media)
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        MyAudio->play();
        emitStartShowMedia(element);
    }
    else if (type == "TWeb" && display_media)
    {
        TWeb *MyWeb             = qobject_cast<TWeb *> (element);
        MyWeb->play();
        emitStartShowMedia(element);
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
        handleStop(element);

        QString parent_type = parent->objectName();
        if (parent_type == "TSeq")
            next(qobject_cast<TSeq *> (parent));
        else if (parent_type == "TPar")
            next(qobject_cast<TPar *> (parent));
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
void TSmil::handleStop(TBase *element)
{
    QString type = element->objectName();
    element->prepareStop();
    if (type == "TImage")
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        if (element->getStatus() != element->_stopped)
        {
            MyImage->stop();
            emitStopShowMedia(element);
        }
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        if (element->getStatus() != element->_stopped)
        {
            MyVideo->stop();
            emitStopShowMedia(element);
        }
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        if (element->getStatus() != element->_stopped)
        {
            MyAudio->stop();
            emitStopShowMedia(element);
        }
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        if (element->getStatus() != element->_stopped)
        {
            MyWeb->stop();
            emitStopShowMedia(element);
        }
    }
    else if (type == "TSeq")
    {
        TSeq *MySeq = qobject_cast<TSeq *> (element);
        handleStop(MySeq->getActiveObject());
        MySeq->stop();
    }
    else if (type == "TPar")
    {
        TPar *MyPar = qobject_cast<TPar *> (element);
        QHash<QString, TBase *>           list = MyPar->getPlaylistObjects();
        QHash<QString, TBase *>::iterator i;
        for (i = list.begin(); i != list.end(); i++)
        {
            handleStop(*i);
        }
        MyPar->stop();
    }
    else if (type == "TExcl")
    {
        TExcl *MyExcl = qobject_cast<TExcl *> (element);
        handleStop(MyExcl->getPlayedObject());
        MyExcl->stop();
    }
    return;
}

/**
 * @brief TSmil::handlePause prepares elements for pause, pause them and recurses their childs
 * @param parent
 * @param element
 */
void TSmil::handlePause(TBase *element)
{
    QString type        = element->objectName();
    element->preparePause();
    if (type == "TImage")
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        MyImage->pause();
        emitStopShowMedia(element);
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        MyVideo->pause();
        emitStopShowMedia(element);
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        MyAudio->pause();
        emitStopShowMedia(element);
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        MyWeb->pause();
        emitStopShowMedia(element);
    }
    else if (type == "TSeq")
    {
        TSeq *MySeq = qobject_cast<TSeq *> (element);
        handlePause(MySeq->getActiveObject());
        MySeq->pause();
    }
    else if (type == "TPar")
    {
        TPar *MyPar = qobject_cast<TPar *> (element);
        QHash<QString, TBase *>           list = MyPar->getPlaylistObjects();
        QHash<QString, TBase *>::iterator i;
        for (i = list.begin(); i != list.end(); i++)
        {
            handlePause(*i);
        }
        MyPar->pause();
    }
    else if (type == "TExcl")
    {
        TExcl *MyExcl = qobject_cast<TExcl *> (element);
        handlePause(MyExcl->getPlayedObject());
        MyExcl->pause();
    }
}

void TSmil::handleResumeElement(TBase *element)
{
    QString type        = element->objectName();
    element->prepareResume();
    if (type == "TImage")
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        if (element->getStatus() == element->_paused)
        {
            MyImage->play();
            emitStartShowMedia(element);
        }
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        if (element->getStatus() == element->_paused)
        {
            MyVideo->play();
            emitStartShowMedia(element);
        }
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        if (element->getStatus() == element->_paused)
        {
            MyAudio->play();
            emitStartShowMedia(element);
        }
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        if (element->getStatus() == element->_paused)
        {
            MyWeb->play();
            emitStartShowMedia(element);
        }
    }
    else if (type == "TSeq")
    {
        TSeq *MySeq = qobject_cast<TSeq *> (element);
        handleResumeElement(MySeq->getActiveObject());
        MySeq->resume();
    }
    else if (type == "TPar")
    {
        TPar *MyPar = qobject_cast<TPar *> (element);
        QHash<QString, TBase *>           list = MyPar->getPlaylistObjects();
        QHash<QString, TBase *>::iterator i;
        for (i = list.begin(); i != list.end(); i++)
        {
            handleResumeElement(*i);
        }
        MyPar->resume();
    }
    else if (type == "TExcl")
    {
        TExcl *MyExcl = qobject_cast<TExcl *> (element);
        handleResumeElement(MyExcl->getPlayedObject());
        MyExcl->resume();
    }
}


// ============================== private methods =======================================

QHash<QString, TBase *>::iterator TSmil::insertIntoObjectContainer(TBase *parent, TBase *child)
{
    QString type = parent->objectName();
    QString id   = child->getID();
    if (type == "TSeq")
    {
        TSeq *MySeq  = qobject_cast<TSeq *> (parent);
        MySeq->insertPlaylistObject(id, child);
    }
    else if (type == "TPar")
    {
        TPar *MyPar  = qobject_cast<TPar *> (parent);
        MyPar->insertPlaylistObject(id, child);
    }
    else if (type == "TExcl")
    {
        TExcl *MyExcl  = qobject_cast<TExcl *> (parent);
        MyExcl->insertPlaylistObject(id, child);
    }
    return ar_elements.insert(id, child);;

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




