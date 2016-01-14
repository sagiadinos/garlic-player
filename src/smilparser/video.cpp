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

#include "video.h"

TVideo::TVideo(QObject *parent)
{
    parent_playlist = parent;
    setObjectName("TVideo");
}

TVideo::~TVideo()
{
    delete media_player;
    delete show_video.video_item;
}

bool TVideo::parse(QDomElement domelement)
{
    actual_element = domelement;
    setAttributes();
    setBaseParameters();
    return true;
}

showVideo TVideo::getMediaForShow()
{
    return show_video;
}

void TVideo::beginPlay()
{
    setTimedStart();
    return;
}

void TVideo::play()
{
    if (!setTimedEnd()) // when end or duration is not specified end on video duration
        connect(media_player, SIGNAL(stopped()), this, SLOT(emitfinished()));
    media_player->play();
    status = _playing;
    emit started(parent_playlist, this);
    return;
}


QString TVideo::getFit()
{
    return show_video.fit;
}

// ====================  protected methods =================================

void TVideo::setAttributes()
{
    show_video.fit  = "";
    setBaseMediaAttributes();

    show_video.region      = region;
    show_video.video_item  = new QtAV::GraphicsItemRenderer;
    media_player           = new QtAV::AVPlayer;
    media_player->setRenderer(show_video.video_item);

    if (src.mid(0, 4) == "http")
        media_player->setFile(src);
    else // get relative paths
        media_player->setFile(index_path+src);

    if (actual_element.hasAttribute("fit"))
        show_video.fit = actual_element.attribute("fit");
    return;
}

// ====================  private methods =================================

void TVideo::emitfinished()
{
    status = _stopped;
    emit finished(parent_playlist, this);
}

