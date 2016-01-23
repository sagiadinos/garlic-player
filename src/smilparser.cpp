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

void TSmil::beginSmilParsing()
{
    MyBody = new TBody();
    connect(MyBody, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
    connect(MyBody, SIGNAL(finishedPlaylist(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
    if (MyBody->parse(MyFile.getBody()))
        MyBody->beginPlay();
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
    MyExcl->decActivatableChilds();
    MyExcl->next();
    return;
}

/**
 * @brief TSmil::handleFoundElement catch the signal from playlist and starts to play found element
 * @param parent
 * @param found_tag
 */
void TSmil::handleFoundElement(QObject *parent, TFoundTag found_tag)
{
    ar_elements_iterator = ar_elements.find(TBase::parseID(found_tag.dom_element));
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
            connect(MyImage, SIGNAL(startedMedia(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
            connect(MyImage, SIGNAL(finishedMedia(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        }
        else
            MyImage = qobject_cast<TImage *> (*ar_elements_iterator);
        MyImage->beginPlay();
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
            connect(MyVideo, SIGNAL(startedMedia(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
            connect(MyVideo, SIGNAL(finishedMedia(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        }
        else
            MyVideo = qobject_cast<TVideo*> (*ar_elements_iterator);
        MyVideo->beginPlay();

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
            connect(MyAudio, SIGNAL(startedMedia(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
            connect(MyAudio, SIGNAL(finishedMedia(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        }
        else
            MyAudio = qobject_cast<TAudio*> (*ar_elements_iterator);
        MyAudio->beginPlay();
    }
    else if (found_tag.name == "web")
    {
        TWeb        *MyWeb;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyWeb = new TWeb(parent);
            MyWeb->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyWeb->getID(), parent, MyWeb);
            connect(MyWeb, SIGNAL(startedMedia(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
            connect(MyWeb, SIGNAL(finishedMedia(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        }
        else
            MyWeb = qobject_cast<TWeb*> (*ar_elements_iterator);
        MyWeb->beginPlay();
    }
    else if (found_tag.name == "seq")
    {
        TSeq        *MySeq;
        if (ar_elements_iterator == ar_elements.end())
        {
            MySeq  = new TSeq(parent);
            MySeq->parse(found_tag.dom_element);
            insertIntoObjectContainer(MySeq->getID(), parent, MySeq);
            connect(MySeq, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
            connect(MySeq, SIGNAL(startedPlaylist(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
            connect(MySeq, SIGNAL(finishedPlaylist(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        }
        else
            MySeq = qobject_cast<TSeq*> (*ar_elements_iterator);
        MySeq->beginPlay();
    }
    else if (found_tag.name == "par")
    {
        TPar        *MyPar;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyPar = new TPar(parent);
            MyPar->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyPar->getID(), parent, MyPar);
            connect(MyPar, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
            connect(MyPar, SIGNAL(startedPlaylist(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
            connect(MyPar, SIGNAL(finishedPlaylist(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        }
        else
            MyPar = qobject_cast<TPar*> (*ar_elements_iterator);
        MyPar->beginPlay();
    }
    else if (found_tag.name == "excl")
    {
        TExcl       *MyExcl;
        if (ar_elements_iterator == ar_elements.end())
        {
            MyExcl = new TExcl(parent);
            MyExcl->parse(found_tag.dom_element);
            insertIntoObjectContainer(MyExcl->getID(), parent, MyExcl);
            connect(MyExcl, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
            connect(MyExcl, SIGNAL(startedPlaylist(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
            connect(MyExcl, SIGNAL(finishedPlaylist(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        }
        else
            MyExcl = qobject_cast<TExcl*> (*ar_elements_iterator);
        MyExcl->beginPlay();
    }
     return;
}

void TSmil::insertIntoObjectContainer(QString id, QObject *parent, QObject *child)
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
void TSmil::emitstartedElement(QObject *parent, QObject *element)
{
    QString type        = element->objectName();
    QString parent_type = parent->objectName();
    QDomElement root_domelement;
    QString id;
    if (type == "TImage")
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        root_domelement = MyImage->getRootElement();
        id= MyImage->getID();
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        root_domelement = MyVideo->getRootElement();
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        root_domelement = MyAudio->getRootElement();
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        root_domelement = MyWeb->getRootElement();
    }

    if (parent_type == "TPar")  // increment active child to determine end of a par
    {
        TPar *MyPar  = qobject_cast<TPar *> (parent);
        MyPar->incActiveChilds();
    }
    if (parent_type == "TExcl")  // increment active child to determine end of a par
    {
        TExcl *MyExclParent   = qobject_cast<TExcl *> (parent);
        QObject *played_object = MyExclParent->getPlayedObject(); // must set before interrupting
        int interrupt = MyExclParent->interruptActualPlaying(root_domelement, element);
        if (interrupt == MyExclParent->_interrupt_active)
        {
            handleStop(parent, played_object);
            emit playMedia(element);
            MyExclParent->setChildActive(true);
        }
        else if (interrupt == MyExclParent->_play_this)
        {
            MyExclParent->setChildActive(true);
            emit playMedia(element);
        }
        else if (interrupt == MyExclParent->_no_interrupting)
        {
            handleStop(parent, element);
        }

        // _interrupt_new => let it flow
    }
    else
        emit playMedia(element);
    return;
}


void TSmil::emitfinishedElement(QObject *parent, QObject *element)
{
    QString type        = element->objectName();
    if (type == "TBody") // when TBody ends there is no parent and nothing todo anymore
        return;

    QString parent_type = parent->objectName();
    if (type == "TImage")
    {
        emit finishedMedia(element);
    }
    else if (type == "TVideo")
    {
        emit finishedMedia(element);
    }
    else if (type == "TAudio")
    {
        emit finishedMedia(element);
    }
    else if (type == "TWeb")
    {
        emit finishedMedia(element);
    }
    else if (type == "TBody")
    {
        delete MyBody;
        ar_elements.clear();
    }


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

void TSmil::handleStop(QObject *parent, QObject *element)
{
    QString type        = element->objectName();
    QString parent_type = parent->objectName();
    if (parent_type == "TExcl")
    {
        TExcl *MyExclParent   = qobject_cast<TExcl *> (parent);
        // set false only when an active element ends, cause peers="never" gets upset
        if (element == MyExclParent->getActiveObject())
            MyExclParent->setChildActive(false);
    }

    if (type == "TImage")
    {
        TImage *MyImage = qobject_cast<TImage *> (element);
        if (MyImage->getStatus() == MyImage->_playing)
        {
            MyImage->beginStop();
        }
    }
    else if (type == "TVideo")
    {
        TVideo *MyVideo = qobject_cast<TVideo *> (element);
        if (MyVideo->getStatus() == MyVideo->_playing)
        {
            MyVideo->beginStop();
        }
    }
    else if (type == "TAudio")
    {
        TAudio *MyAudio = qobject_cast<TAudio *> (element);
        if (MyAudio->getStatus() == MyAudio->_playing)
        {
            MyAudio->beginStop();
        }
    }
    else if (type == "TWeb")
    {
        TWeb *MyWeb = qobject_cast<TWeb *> (element);
        if (MyWeb->getStatus() == MyWeb->_playing)
        {
            MyWeb->beginStop();
        }
    }
    else if (type == "TSeq")
    {
        TSeq *MySeq = qobject_cast<TSeq *> (element);
        handleStop(parent, MySeq->getActiveObject());
        MySeq->beginStop();
    }
    else if (type == "TPar")
    {
        TPar *MyPar = qobject_cast<TPar *> (element);
        QHash<QString, QObject *>           list = MyPar->getPlaylistObjects();
        QHash<QString, QObject *>::iterator i;
        for (i = list.begin(); i != list.end(); i++)
        {
            handleStop(parent, *i);
        }
        MyPar->beginStop();
    }
    else if (type == "TExcl")
    {
        TExcl *MyExcl = qobject_cast<TExcl *> (element);
        handleStop(parent, MyExcl->getPlayedObject());
        MyExcl->beginStop();
    }

}

void TSmil::handlePause(QObject *element)
{

}



THead TSmil::getHeader()
{
    return MyHead;
}


