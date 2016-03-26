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
}


showVideo TVideo::getMediaForShow()
{
    return show_video;
}

void TVideo::setDurationTimerBeforePlay()
{
    if (loaded)
    {
        if (!hasDurAttribute() && !end_timer->isActive()) // when end or dur is not specified use video duration for simple duration
            dur_timer->start(media_player->duration()); // do not connect signal stopped it could be sended more than one and causes sync problems
        if (!is_resumed)
            emit startedMedia(parent_container, this);
    }
    else // set a duration otherwise it runs in a recursion stack overflow when load is not complete
        setInternalDefaultDur();

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

bool TVideo::load()
{
    QString file_path      = MyFileManager->getLoadablePath(src);
    show_video.video_item  = new QtAV::GraphicsItemRenderer;
    media_player           = new QtAV::AVPlayer;
    media_player->setRenderer(show_video.video_item);
    bool isload            = media_player->load(file_path);
    // deprecated status informed but there is not alternative to get load status
    if (isload)
        qDebug() << getID() << QTime::currentTime().toString()  << "loaded: " << file_path;
    else
        qDebug() << getID() << QTime::currentTime().toString()  << "not loaded: " << file_path;
    return isload;
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

