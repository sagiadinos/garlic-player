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

#include "playlist.h"

TPlaylist::TPlaylist(QObject * parent)
{
    Q_UNUSED(parent);
}

TFoundTag TPlaylist::getFoundTag()
{
    return found_tag;
}

void TPlaylist::reactByTag()
{
    if(actual_element.tagName() == "img")
        doImage();
    else if(actual_element.tagName() == "video")
        doVideo();
    else if(actual_element.tagName() == "audio")
        doAudio();
    else if(actual_element.tagName() == "text")
        doWeb();
    else if(actual_element.tagName() == "ref")
        doRef();
    else if(actual_element.tagName() == "prefetch")
        doPrefetch();
    else if(actual_element.tagName() == "seq")
        doSeq();
    else if(actual_element.tagName() == "par")
        doPar();
    else if(actual_element.tagName() == "excl")
        doExcl();
}


void TPlaylist::doRef()
{
    if (actual_element.hasAttribute("type"))
    {
        if(actual_element.attribute("type").contains("image"))
            doImage();
        else if(actual_element.attribute("type").contains("video"))
            doVideo();
        else if(actual_element.attribute("type").contains("audio"))
            doAudio();
        else if(actual_element.attribute("type").contains("text"))
            doWeb();
    }
}

void TPlaylist::doImage()
{
    found_tag.name         = "img";
    found_tag.dom_element  = actual_element;
    emit foundMedia(this, found_tag);
    return;
}

void TPlaylist::doVideo()
{
    found_tag.name         = "video";
    found_tag.dom_element  = actual_element;
    emit foundMedia(this, found_tag);
    return;
}

void TPlaylist::doAudio()
{
    found_tag.name         = "audio";
    found_tag.dom_element  = actual_element;
    emit foundMedia(this, found_tag);
    return;
}

void TPlaylist::doWeb()
{
    found_tag.name         = "web";
    found_tag.dom_element  = actual_element;
    emit foundMedia(this, found_tag);
    return;
}

void TPlaylist::doSeq()
{
    found_tag.name         = "seq";
    found_tag.dom_element  = actual_element;
    emit foundPlaylist(this, found_tag);
    return;
}

void TPlaylist::doPar()
{
    found_tag.name         = "par";
    found_tag.dom_element  = actual_element;
    emit foundPlaylist(this, found_tag);
    return;
}

void TPlaylist::doExcl()
{
    found_tag.name         = "excl";
    found_tag.dom_element  = actual_element;
    emit foundPlaylist(this, found_tag);
    return;
}

void TPlaylist::doPrefetch()
{
    found_tag.name         = "prefetch";
    found_tag.dom_element  = actual_element;
    emit foundMedia(this, found_tag);
    return;
}

