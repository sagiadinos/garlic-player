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
}

showAudio TAudio::getMediaForShow()
{
    return show_audio;
}

bool TAudio::load()
{
    QString file_path      = MyFileManager->getLoadablePath(src);
    output                 = new QtAV::VideoOutput();
    media_player           = new QtAV::AVPlayer();
    media_player->setRenderer(output);
    bool isload            = media_player->load(file_path);
    // deprecated status informed but there is not alternative to get load status
    if (isload)
        qDebug() << getID() << QTime::currentTime().toString()  << "loaded: " << file_path;
    else
        qDebug() << getID() << QTime::currentTime().toString()  << "not loaded: " << file_path;
    return isload;
}

void TAudio::setDurationTimerBeforePlay()
{
    if (loaded)
    {
        if (!hasDurAttribute() && !end_timer->isActive()) // when end or dur is not specified use video duration for simple duration
            dur_timer->start(media_player->duration()); // do not connect signal stopped cause it could be sended more than one and causes sync problems
        if (!is_resumed)
            emit startedMedia(parent_container, this);
    }
    else // set a duration otherwise it runs in a recursion stack overflow when load is not complete
        setInternalDefaultDur();
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


