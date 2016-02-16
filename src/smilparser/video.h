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

#ifndef TVIDEO_H
#define TVIDEO_H

#include "media.h"
#include <QtAV>
#include <QtAVWidgets>

struct showVideo
{
    QString media_type;
    QString region;
    QString fit;
    QtAV::GraphicsItemRenderer *video_item;
};

class TVideo : public TMedia
{
    Q_OBJECT
public:
    TVideo(TBase *parent = 0);
    ~TVideo();
    showVideo       getMediaForShow();
    QString         getFit();
    bool            load(QString index_path);
    void            pause();
    void            stop();
    void            play();
protected:
    void            setDurationTimerBeforePlay();
    QtAV::AVPlayer *media_player;
    showVideo       show_video;
    void            setAttributes();
};

#endif // TVIDEO_H
