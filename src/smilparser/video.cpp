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

#include "video.h"

TVideo::TVideo(TContainer *parent)
{
    parent_container = parent;
    initTimer();
    setObjectName("TVideo");
}

TVideo::~TVideo()
{
    delete media_player;
    delete show_video.video_item;
}


showVideo TVideo::getMediaForShow()
{
    return show_video;
}

void TVideo::setDurationTimerBeforePlay()
{
    if (!hasDurAttribute() && !end_timer->isActive()) // when end or dur is not specified use video duration for simple duration
        connect(media_player, SIGNAL(stopped()), this, SLOT(finishedSimpleDuration()));
    if (!is_resumed)
        emit startedMedia(parent_container, this);
    return;
}

void TVideo::play()
{
    media_player->play();
    status = _playing;
    return;
}

void TVideo::stop()
{
    media_player->stop();
    status = _stopped;
    return;
}

void TVideo::pause()
{
    media_player->pause();
    status = _paused;
    return;
}


QString TVideo::getFit()
{
    return show_video.fit;
}

bool TVideo::load(QString index_path)
{
    show_video.video_item  = new QtAV::GraphicsItemRenderer;
    media_player           = new QtAV::AVPlayer;
    media_player->setRenderer(show_video.video_item);

    if (src.mid(0, 4) == "http")
        media_player->setFile(src);
    else // get relative paths
        media_player->setFile(index_path+src);
    return true;
}

// ====================  protected methods =================================

void TVideo::setAttributes()
{
    show_video.fit  = "";
    setBaseMediaAttributes();

    show_video.region      = region;

    if (root_element.hasAttribute("fit"))
        show_video.fit = root_element.attribute("fit");
    return;
}

// ====================  private methods =================================

