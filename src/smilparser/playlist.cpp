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

#include "playlist.h"

TPlaylist::TPlaylist(TBase * parent)
{
    Q_UNUSED(parent);
}

TFoundTag TPlaylist::getFoundTag()
{
    return found_tag;
}

void TPlaylist::insertPlaylistObject(QString id, TBase *obj_element)
{
    ar_elements.insert(id, obj_element);
    return;
}

QString TPlaylist::getIdOfActiveElement()
{
    return parseID(active_element);
}

TBase* TPlaylist::getActiveObject()
{
    QHash<QString, TBase *>::iterator i = ar_elements.find(getIdOfActiveElement());
    return *i;
}


QHash<QString, TBase *> TPlaylist::getPlaylistObjects()
{
    return ar_elements;
}

void TPlaylist::reactByTag()
{
    if(active_element.tagName() == "img")
        doImage();
    else if(active_element.tagName() == "video")
        doVideo();
    else if(active_element.tagName() == "audio")
        doAudio();
    else if(active_element.tagName() == "text")
        doWeb();
    else if(active_element.tagName() == "ref")
        doRef();
    else if(active_element.tagName() == "prefetch")
        doPrefetch();
    else if(active_element.tagName() == "seq")
        doSeq();
    else if(active_element.tagName() == "par")
        doPar();
    else if(active_element.tagName() == "excl")
        doExcl();
    return;
}

void TPlaylist::emitfinished()
{
    stop();
    qDebug() << getID() << "finished Playlist";
    emit finishedPlaylist(parent_playlist, this);
    return;
}

void TPlaylist::doRef()
{
    if (active_element.hasAttribute("type"))
    {
        if(active_element.attribute("type").contains("image"))
            doImage();
        else if(active_element.attribute("type").contains("video"))
            doVideo();
        else if(active_element.attribute("type").contains("audio"))
            doAudio();
        else if(active_element.attribute("type").contains("text"))
            doWeb();
    }
    return;
}

void TPlaylist::doImage()
{
    found_tag.name         = "img";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

void TPlaylist::doVideo()
{
    found_tag.name         = "video";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

void TPlaylist::doAudio()
{
    found_tag.name         = "audio";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

void TPlaylist::doWeb()
{
    found_tag.name         = "web";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

void TPlaylist::doSeq()
{
    found_tag.name         = "seq";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

void TPlaylist::doPar()
{
    found_tag.name         = "par";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

void TPlaylist::doExcl()
{
    found_tag.name         = "excl";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

void TPlaylist::doPrefetch()
{
    found_tag.name         = "prefetch";
    found_tag.dom_element  = active_element;
    emit foundElement(this, found_tag);
    return;
}

