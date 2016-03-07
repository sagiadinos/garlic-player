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

#include "audio.h"

TAudio::TAudio(TContainer *parent)
{
    parent_container = parent;
    initTimer();
    setObjectName("TAudio");
}

TAudio::~TAudio()
{
    delete media_player;
    delete output;
}

showAudio TAudio::getMediaForShow()
{
    return show_audio;
}

bool TAudio::load(QString file_path)
{
    output           = new QtAV::VideoOutput();
    media_player     = new QtAV::AVPlayer();
    media_player->setRenderer(output);
    media_player->setFile(file_path);
    return true;
}

void TAudio::setDurationTimerBeforePlay()
{
    if (!hasDurAttribute()  && !end_timer->isActive()) // when end or dur is not specified use audio duration for simple duration
        connect(media_player, SIGNAL(stopped()), this, SLOT(finishedSimpleDuration()));
    if (!is_resumed)
        emit startedMedia(parent_container, this);
    return;
}

// ====================  protected methods =================================
void TAudio::play()
{
    media_player->play();
    status = _playing;
}

void TAudio::stop()
{
    media_player->stop();
    status = _playing;
    return;
}


void TAudio::pause()
{
    media_player->pause();
    status = _playing;
}


void TAudio::setAttributes()
{
    setBaseMediaAttributes();
    show_audio.region      = region;
    return;
}

// ====================  private methods =================================


