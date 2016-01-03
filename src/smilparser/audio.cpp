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

#include "audio.h"

TAudio::TAudio(QObject *parent)
{
    parent_playlist = parent;
    setObjectName("TAudio");
}

TAudio::~TAudio()
{
    delete media_player;
    delete output;
}

bool TAudio::parse(QDomElement domelement)
{
    actual_element = domelement;
    setAttributes();
    setBaseParameters();
    return true;
}

showAudio TAudio::getMediaForShow()
{
    return show_audio;
}

void TAudio::play()
{
   media_player->play();
   return;
}

// ====================  protected methods =================================

void TAudio::setAttributes()
{
    setBaseMediaAttributes();
    show_audio.region      = region;
    output           = new QtAV::VideoOutput();
    media_player     = new QtAV::AVPlayer();
    connect(media_player, SIGNAL(stopped()), this, SLOT(emitfinished())); // 10s
    media_player->setRenderer(output);

    if (src.mid(0, 4) == "http")
        media_player->setFile(src);
    else // get relative paths
        media_player->setFile(index_path+src);

    return;
}

// ====================  private methods =================================

void TAudio::emitfinished()
{
    emit finished(parent_playlist, this);
}

