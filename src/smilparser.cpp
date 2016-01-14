/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-.control.com>
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

TSmil::TSmil(QObject * parent)
{
    Q_UNUSED(parent)
}

TSmil::~TSmil()
{
    ar_playlists.clear();
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
    connect(MyBody, SIGNAL(foundMedia(QObject *, TFoundTag )), this, SLOT(handleMedia(QObject *, TFoundTag)));
    connect(MyBody, SIGNAL(finished(QObject *, QObject *)), this, SLOT(finishedPlaylist(QObject *, QObject *)));
    connect(MyBody, SIGNAL(foundPlaylist(QObject *, TFoundTag )), this, SLOT(handlePlaylist(QObject *, TFoundTag)));
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
    Q_UNUSED(MyExcl);
    return;
}

void TSmil::handleMedia(QObject *parent, TFoundTag found_tag)
{
    if (found_tag.name == "img")
    {
        TImage *MyImage = new TImage(parent);
        MyImage->setIndexPath(index_path);
        connect(MyImage, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedMedia(QObject *, QObject *)));
        MyImage->parse(found_tag.dom_element);
        MyImage->play();
        emit playMedia(MyImage);
    }
    else if (found_tag.name == "video")
    {
        TVideo *MyVideo = new TVideo(parent);
        MyVideo->setIndexPath(index_path);
        connect(MyVideo, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedMedia(QObject *, QObject *)));
        MyVideo->parse(found_tag.dom_element);
        MyVideo->play();
        emit playMedia(MyVideo);
    }
    else if (found_tag.name == "audio")
    {
        TAudio *MyAudio = new TAudio(parent);
        MyAudio->setIndexPath(index_path);
        connect(MyAudio, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedMedia(QObject *, QObject *)));
        MyAudio->parse(found_tag.dom_element);
        MyAudio->play();
        emit playMedia(MyAudio);
    }
    else if (found_tag.name == "web")
    {
        TWeb *MyWeb = new TWeb(parent);
        connect(MyWeb, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedMedia(QObject *, QObject *)));
        MyWeb->parse(found_tag.dom_element);
        MyWeb->play();
        emit playMedia(MyWeb);
    }

    QString playlist_type = parent->objectName();
    if (playlist_type == "TPar")  // increment active child to determine end of a par
    {
        MyPar  = qobject_cast<TPar *> (parent);
        MyPar->incActiveChilds();
    }
    return;
}

void TSmil::handlePlaylist(QObject *parent, TFoundTag found_tag)
{
    QDomElement dom_element = found_tag.dom_element;
    if (found_tag.name == "seq") // if first Time then init
    {
        MySeq  = new TSeq(parent);
        MySeq->parse(dom_element);
        if (ar_playlists.find(MySeq->getID()) != ar_playlists.end())
            ar_playlists.insert(MySeq->getID(), MySeq);
        connect(MySeq, SIGNAL(foundMedia(QObject *, TFoundTag )), this, SLOT(handleMedia(QObject *, TFoundTag)));
        connect(MySeq, SIGNAL(finished(QObject *, QObject *)), this, SLOT(finishedPlaylist(QObject *, QObject *)));
        connect(MySeq, SIGNAL(foundPlaylist(QObject *, TFoundTag )), this, SLOT(handlePlaylist(QObject *, TFoundTag)));
        MySeq->beginPlay();
    }
    else if (found_tag.name == "par")
    {
        MyPar = new TPar(parent);
        MyPar->parse(dom_element);
        if (ar_playlists.find(MyPar->getID()) != ar_playlists.end())
            ar_playlists.insert(MyPar->getID(), MyPar);
        connect(MyPar, SIGNAL(foundMedia(QObject *, TFoundTag )), this, SLOT(handleMedia(QObject *, TFoundTag)));
        connect(MyPar, SIGNAL(finished(QObject *, QObject *)), this, SLOT(finishedPlaylist(QObject *, QObject *)));
        connect(MyPar, SIGNAL(foundPlaylist(QObject *, TFoundTag )), this, SLOT(handlePlaylist(QObject *, TFoundTag)));
        MyPar->beginPlay();
    }
    else if (found_tag.name == "excl")
    {
        MyExcl = new TExcl(parent);
        MyExcl->parse(dom_element);
        if (ar_playlists.find(MyExcl->getID()) != ar_playlists.end())
           ar_playlists.insert(MyExcl->getID(), MyExcl);
        connect(MyExcl, SIGNAL(foundMedia(QObject *, TFoundTag )), this, SLOT(handleMedia(QObject *, TFoundTag)));
        connect(MyExcl, SIGNAL(finished(QObject *, QObject *)), this, SLOT(finishedPlaylist(QObject *, QObject *)));
        connect(MyExcl, SIGNAL(foundPlaylist(QObject *, TFoundTag )), this, SLOT(handlePlaylist(QObject *, TFoundTag)));
    }

    QString playlist_type = parent->objectName();
    if (playlist_type == "TPar") // increment active child to determine end of par
    {
        MyPar  = qobject_cast<TPar *> (parent);
        MyPar->incActiveChilds();
    }
    return;
}

void TSmil::finishedPlaylist(QObject *parent, QObject *playlist)
{
 //   ar_playlists.remove(playlist);
    selectPlaylistForNextAction(parent);
}

void TSmil::emitfinishedMedia(QObject *parent, QObject *media)
{
    emit finishedMedia(media);

    QString media_type = media->objectName();
    if (media_type == "TImage")
        delete qobject_cast<TImage *> (media);
    else if (media_type == "TVideo")
        delete qobject_cast<TVideo *> (media);
    else if (media_type == "TAudio")
        delete qobject_cast<TAudio *> (media);
    else if (media_type == "TWeb")
        delete qobject_cast<TWeb *> (media);
    selectPlaylistForNextAction(parent);
}

void TSmil::selectPlaylistForNextAction(QObject *parent)
{
    QString type = parent->objectName();
    if (type == "TSeq")
        next(qobject_cast<TSeq *> (parent));
    else if (type == "TPar")
        next(qobject_cast<TPar *> (parent));
    else if (type == "TExcl")
        next(qobject_cast<TExcl *> (parent));
    else if (type == "TBody")
        next();
    return;
}


THead TSmil::getHeader()
{
    return MyHead;
}


