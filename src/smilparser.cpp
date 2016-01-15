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
    connect(MyBody, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
    connect(MyBody, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
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

void TSmil::handleFoundElement(QObject *parent, TFoundTag found_tag)
{
    if (found_tag.name == "img")
    {
        TImage *MyImage = new TImage(parent);
        MyImage->setIndexPath(index_path); // must set before parse to find media
        MyImage->parse(found_tag.dom_element);
        if (ar_media.find(MyImage->getID()) != ar_media.end())
           ar_media.insert(MyImage->getID(), MyImage);
        connect(MyImage, SIGNAL(started(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
        connect(MyImage, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        MyImage->beginPlay();
    }
    else if (found_tag.name == "video")
    {
        TVideo *MyVideo = new TVideo(parent);
        MyVideo->setIndexPath(index_path);
        MyVideo->parse(found_tag.dom_element);
        if (ar_media.find(MyVideo->getID()) != ar_media.end())
           ar_media.insert(MyVideo->getID(), MyVideo);
        connect(MyVideo, SIGNAL(started(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
        connect(MyVideo, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        MyVideo->beginPlay();
    }
    else if (found_tag.name == "audio")
    {
        TAudio *MyAudio = new TAudio(parent);
        MyAudio->setIndexPath(index_path);
        MyAudio->parse(found_tag.dom_element);
        if (ar_media.find(MyAudio->getID()) != ar_media.end())
           ar_media.insert(MyAudio->getID(), MyAudio);
        connect(MyAudio, SIGNAL(started(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
        connect(MyAudio, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        MyAudio->beginPlay();
    }
    else if (found_tag.name == "web")
    {
        TWeb *MyWeb = new TWeb(parent);
        MyWeb->parse(found_tag.dom_element);
        if (ar_media.find(MyWeb->getID()) != ar_media.end())
           ar_media.insert(MyWeb->getID(), MyWeb);
        connect(MyWeb, SIGNAL(started(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
        connect(MyWeb, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        MyWeb->beginPlay();
    }
    if (found_tag.name == "seq") // if first Time then init
    {
        MySeq  = new TSeq(parent);
        MySeq->parse(found_tag.dom_element);
        if (ar_playlists.find(MySeq->getID()) != ar_playlists.end())
            ar_playlists.insert(MySeq->getID(), MySeq);
        connect(MySeq, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
        connect(MySeq, SIGNAL(started(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
        connect(MySeq, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        MySeq->beginPlay();
    }
    else if (found_tag.name == "par")
    {
        MyPar = new TPar(parent);
        MyPar->parse(found_tag.dom_element);
        if (ar_playlists.find(MyPar->getID()) != ar_playlists.end())
            ar_playlists.insert(MyPar->getID(), MyPar);
        connect(MyPar, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
        connect(MyPar, SIGNAL(started(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
        connect(MyPar, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        MyPar->beginPlay();
    }
    else if (found_tag.name == "excl")
    {
        MyExcl = new TExcl(parent);
        MyExcl->parse(found_tag.dom_element);
        if (ar_playlists.find(MyExcl->getID()) != ar_playlists.end())
           ar_playlists.insert(MyExcl->getID(), MyExcl);
        connect(MyExcl, SIGNAL(foundElement(QObject *, TFoundTag )), this, SLOT(handleFoundElement(QObject *, TFoundTag)));
        connect(MyExcl, SIGNAL(started(QObject *, QObject *)), this, SLOT(emitstartedElement(QObject *, QObject *)));
        connect(MyExcl, SIGNAL(finished(QObject *, QObject *)), this, SLOT(emitfinishedElement(QObject *, QObject *)));
        MyExcl->beginPlay();
    }
    return;
}


void TSmil::emitstartedElement(QObject *parent, QObject *element)
{
    QString type = element->objectName();
    if (type == "TImage")
    {
        emit playMedia(element);
    }
    else if (type == "TVideo")
    {
        emit playMedia(element);
    }
    else if (type == "TAudio")
    {
        emit playMedia(element);
    }
    else if (type == "TWeb")
    {
        emit playMedia(element);
    }
    type = parent->objectName();
    if (type == "TPar")  // increment active child to determine end of a par
    {
        MyPar  = qobject_cast<TPar *> (parent);
        MyPar->incActiveChilds();
    }

    if (type == "TExcl")  // stop or pause the actual active element
    {
        MyExcl  = qobject_cast<TExcl *> (parent);
        MyExcl->changeActiveElement(element); // stops or pause the other sibling elements
    }

    return;
}


void TSmil::emitfinishedElement(QObject *parent, QObject *element)
{
    QString type = element->objectName();
    if (type == "TImage")
    {
        emit finishedMedia(element);
        delete qobject_cast<TImage *> (element);
    }
    else if (type == "TVideo")
    {
        emit finishedMedia(element);
        delete qobject_cast<TVideo *> (element);
    }
    else if (type == "TAudio")
    {
        emit finishedMedia(element);
        delete qobject_cast<TAudio *> (element);
    }
    else if (type == "TWeb")
    {
        emit finishedMedia(element);
        delete qobject_cast<TWeb *> (element);
    }

    type = parent->objectName();
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


