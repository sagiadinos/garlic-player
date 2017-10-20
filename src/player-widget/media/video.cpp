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

Video::Video(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyVideo  = qobject_cast<TVideo *>(media);
    connect(&MediaDecoder, SIGNAL(finished()), this, SLOT(finished()));
}

Video::~Video()
{
    MediaDecoder.unload();
    delete VideoWidget;
}

void Video::init()
{
   VideoWidget    = new MediaViewWrapper;
   MediaDecoder.setVideoOutput(VideoWidget);
   MediaDecoder.load(MyVideo->getLoadablePath());
   MediaDecoder.play();
}

void Video::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
    if (MyVideo->getFit() == "fill")
        VideoWidget->setAspectRatioFill();
    else if (MyVideo->getFit() == "meet")
        VideoWidget->setAspectRatioMeet();
    else if (MyVideo->getFit() == "meetbest")
        VideoWidget->setAspectRatioMeetBest();
}

QWidget *Video::getView()
{
    return VideoWidget->getVideoWidget();
}

void Video::finished()
{
   MyVideo->finishedSimpleDuration();
}
