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

Video::Video(QObject *parent) : BaseMedia(parent)
{
    MediaDecoder.reset(new MediaDecoderWrapper(this));
    connect(MediaDecoder.data(), SIGNAL(finished()), this, SLOT(finished()));

    VideoWidget.reset(new MediaViewWrapper);
    MediaDecoder.data()->setVideoOutput(VideoWidget.data());
}

Video::~Video()
{
    MediaDecoder.reset();
    VideoWidget.reset();
}

void Video::init(TMedia *media)
{
   ParserVideo  = qobject_cast<TVideo *>(media);
   MediaDecoder.data()->load(ParserVideo->getLoadablePath());
   MediaDecoder.data()->play();}

void Video::deinit()
{
    MediaDecoder.data()->stop();
    MediaDecoder.data()->unload();
}


void Video::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
    if (ParserVideo->getFit() == "fill")
        VideoWidget.data()->setAspectRatioFill();
    else if (ParserVideo->getFit() == "meet")
        VideoWidget.data()->setAspectRatioMeet();
    else if (ParserVideo->getFit() == "meetbest")
        VideoWidget.data()->setAspectRatioMeetBest();
}

QWidget *Video::getView()
{
    return VideoWidget.data()->getVideoWidget();
}

void Video::finished()
{
   ParserVideo->finishedSimpleDuration();
}
