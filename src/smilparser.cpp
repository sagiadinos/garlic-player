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
    connect(MyBody, SIGNAL(foundElement(TBase *, TFoundTag )), this, SLOT(foundElement(TBase *, TFoundTag)));
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

void TSmil::next(TExcl *MyExcl)
{
    MyExcl->setChildActive(false); // gibt Probleme bei peers never wenn
    MyExcl->decActivatableChilds();
    MyExcl->next();
    return;
}

/**
 * @brief TSmil::handleFoundElement catch the signal from playlist and starts to play found element
 * @param parent
 * @param found_tag
 */
void TSmil::foundElement(TBase *parent, TFoundTag found_tag)
{
    QHash<QString, TBase *>::iterator ar_elements_iterator = ar_elements.find(TBase::parseID(found_tag.dom_element));
    QString parent_type  = parent->objectName();
    if (found_tag.name == "img")
    {
        TImage      *MyImage;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyImage = new TImage(parent);
            MyImage->setIndexPath(index_path); // must set before parse to find media
            MyImage->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyImage->getID(), parent, MyImage);
            connect(MyImage, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
            connect(MyImage, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
        }
        else
            MyImage = qobject_cast<TImage *> (*ar_elements_iterator);
        MyImage->preparePlay();
    }
    else if (found_tag.name == "video")
    {
        TVideo      *MyVideo;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyVideo = new TVideo(parent);
            MyVideo->setIndexPath(index_path);
            MyVideo->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyVideo->getID(), parent, MyVideo);
            connect(MyVideo, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
            connect(MyVideo, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
        }
        else
            MyVideo = qobject_cast<TVideo*> (*ar_elements_iterator);
        MyVideo->preparePlay();

    }
    else if (found_tag.name == "audio")
    {
        TAudio      *MyAudio;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyAudio = new TAudio(parent);
            MyAudio->setIndexPath(index_path);
            MyAudio->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyAudio->getID(), parent, MyAudio);
            connect(MyAudio, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
            connect(MyAudio, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
        }
        else
            MyAudio = qobject_cast<TAudio*> (*ar_elements_iterator);
        MyAudio->preparePlay();
    }
    else if (found_tag.name == "web")
    {
        TWeb        *MyWeb;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyWeb = new TWeb(parent);
            MyWeb->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyWeb->getID(), parent, MyWeb);
            connect(MyWeb, SIGNAL(startedMedia(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
            connect(MyWeb, SIGNAL(finishedMedia(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
        }
        else
            MyWeb = qobject_cast<TWeb*> (*ar_elements_iterator);
        MyWeb->preparePlay();
    }
    else if (found_tag.name == "seq")
    {
        TSeq        *MySeq;
        if (ar_elements_iterator == ar_elements.end())
        {
            MySeq  = new TSeq(parent);
            MySeq->parse(found_tag.dom_element);
            insertIntoObjectContainer(MySeq->getID(), parent, MySeq);
            connect(MySeq, SIGNAL(foundElement(TBase *, TFoundTag )), this, SLOT(foundElement(TBase *, TFoundTag)));
            connect(MySeq, SIGNAL(startedPlaylist(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
            connect(MySeq, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
        }
        else
            MySeq = qobject_cast<TSeq*> (*ar_elements_iterator);
        MySeq->preparePlay();
    }
    else if (found_tag.name == "par")
    {
        TPar        *MyPar;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyPar = new TPar(parent);
            MyPar->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyPar->getID(), parent, MyPar);
            connect(MyPar, SIGNAL(foundElement(TBase *, TFoundTag )), this, SLOT(foundElement(TBase *, TFoundTag)));
            connect(MyPar, SIGNAL(startedPlaylist(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
            connect(MyPar, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
        }
        else
            MyPar = qobject_cast<TPar*> (*ar_elements_iterator);
        MyPar->preparePlay();
    }
    else if (found_tag.name == "excl")
    {
        TExcl       *MyExcl;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyExcl = new TExcl(parent);
            MyExcl->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyExcl->getID(), parent, MyExcl);
            connect(MyExcl, SIGNAL(foundElement(TBase *, TFoundTag )), this, SLOT(foundElement(TBase *, TFoundTag)));
            connect(MyExcl, SIGNAL(startedPlaylist(TBase *, TBase *)), this, SLOT(startElement(TBase *, TBase *)));
            connect(MyExcl, SIGNAL(finishedPlaylist(TBase *, TBase *)), this, SLOT(finishElement(TBase *, TBase *)));
            connect(MyExcl, SIGNAL(resumeElement(TBase *)), this, SLOT(handleResumeElement(TBase *)));
        }
        else
            MyExcl = qobject_cast<TExcl*> (*ar_elements_iterator);
        MyExcl->preparePlay();
    }
     return;
}

/**
 * @brief TSmil::emitstartedElement catch the signal from a starting element. If the element is a media a signal
 *        will emitted to an output which can show the media e.g. player-widget
 *        if element is a playlist nothing will send
 *
 *        if parent of the element is a par container it increases the count of active elements in this playlist
 *        if parent is an excl container it check and changes the active priorityclass and stop/pause othe siblings
 *        if parent is a seq nothing happens, cause the start signal in a seq can only be emitted from a media, when another media was finished.
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
            MyExclParent->decActivatableChilds();
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
            MyExclParent->decActivatableChilds();
            handleStop(element);
            if(played_object == element)
                MyExclParent->setChildActive(false);
        }
        else if (interrupt == MyExclParent->_pause_new)
            handlePause(element);
    }
    else
        display_media  = true;

    if (display_media)
    {
        if (type == "TImage")
        {
            TImage *MyImage         = qobject_cast<TImage *> (element);
            MyImage->play();
            emitStartShowMedia(element);
        }
        else if (type == "TVideo")
        {
            TVideo *MyVideo         = qobject_cast<TVideo *> (element);
            MyVideo->play();
            emitStartShowMedia(element);
        }
        else if (type == "TAudio")
        {
            TAudio *MyAudio         = qobject_cast<TAudio *> (element);
            MyAudio->play();
            emitStartShowMedia(element);
        }
        else if (type == "TWeb")
        {
            TWeb *MyWeb             = qobject_cast<TWeb *> (element);
            MyWeb->play();
            emitStartShowMedia(element);
        }
    }
    return;
}

/**
 * @brief TSmil::emitfinishedElement emits the signal finished media, when a Media ended naturally
 * @param parent
 * @param element
 */
void TSmil::finishElement(TBase *parent, TBase *element)
{
    QString type        = element->objectName();

    if (type == "TBody") // when TBody ends there is no parent and nothing todo anymore
    {
        ar_elements.clear();
        delete MyBody;
        return;
    }
    if (type == "TImage" || type == "TVideo" ||type == "TAudio" || type == "TWeb")
        emitStopShowMedia(element);

    QString parent_type = parent->objectName();
    if (parent_type == "TSeq")
        next(qobject_cast<TSeq *> (parent));
    else if (parent_type == "TPar")
        next(qobject_cast<TPar *> (parent));
    else if (parent_type == "TExcl")
        next(qobject_cast<TExcl *> (parent));
    else if (parent_type == "TBody")
        next();

    return;
}

/**
 * @brief TSmil::handleStop emits finished media when an element stopped radically by excl/priority and
 *        recurses to stop all active child elements, too.
 * @param parent
 * @param element
 */
void TSmil::handleStop(TBase *element)
{
    QString type        = element->objectName();

    element->prepareStop();
    if (type == "TImage" || type == "TVideo" || type == "TAudio" || type == "TWeb")
    {
        if (element->getStatus() == element->_playing)
        {
            qDebug() << element->getID() << "handleStop emitStopShowMedia";
            emitStopShowMedia(element);
        }
    }
    if (type == "TImage")
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        MyImage->stop();
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        MyVideo->stop();
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        MyAudio->stop();
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        MyWeb->stop();
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

}

/**
 * @brief TSmil::handlePause pauses and recurses childs to pause all active child elements
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
        if (MyImage->getStatus() == MyImage->_playing)
            emitStopShowMedia(element);
        MyImage->pause();
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        if (MyVideo->getStatus() == MyVideo->_playing)
            emitStopShowMedia(element);
        MyVideo->pause();
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        if (MyAudio->getStatus() == MyAudio->_playing)
            emitStopShowMedia(element);
        MyAudio->pause();
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        if (MyWeb->getStatus() == MyWeb->_playing)
            emitStopShowMedia(element);
        MyWeb->pause();
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

/**
 * @brief TSmil::handlePause pauses and recurses childs to pause all active child elements
 * @param parent
 * @param element
 */
void TSmil::handleResumeElement(TBase *element)
{
    QString type        = element->objectName();
    element->prepareResume();
    if (type == "TImage")
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        if (MyImage->getStatus() == MyImage->_paused)
        {
            emitStartShowMedia(element);
        }
        MyImage->play();
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        if (MyVideo->getStatus() == MyVideo->_paused)
            emitStartShowMedia(element);
        MyVideo->play();
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        if (MyAudio->getStatus() == MyAudio->_paused)
            emitStartShowMedia(element);
        MyAudio->play();
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        if (MyWeb->getStatus() == MyWeb->_paused)
            emitStartShowMedia(element);
        MyWeb->play();
    }
    else if (type == "TSeq")
    {
        TSeq *MySeq = qobject_cast<TSeq *> (element);
        handleResumeElement(MySeq->getActiveObject());
        MySeq->play();
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
        MyPar->play();
    }
    else if (type == "TExcl")
    {
        TExcl *MyExcl = qobject_cast<TExcl *> (element);
        handleResumeElement(MyExcl->getPlayedObject());
        MyExcl->play();
    }
}


// ============================== private methods =======================================

void TSmil::insertIntoObjectContainer(QString id, TBase *parent, TBase *child)
{
    QString type = parent->objectName();
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
    ar_elements.insert(id, child);
    return;

}

void TSmil::emitStartShowMedia(TBase *media)
{
    ar_played_media.insert(media);

    emit startShowMedia(media);
    return;
}

void TSmil::emitStopShowMedia(TBase *media)
{
    if (ar_played_media.find(media) != ar_played_media.end())
    {
        ar_played_media.remove(media);
        emit stopShowMedia(media);
    }
    return;
}




