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

#ifndef TREGION_H
#define TREGION_H

#include <QWebView>
#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QWidget>
#include <QList>
#include "smilparser/head.h"
#include "smilparser.h"

class TRegion : public QGraphicsView
{
    Q_OBJECT
public:
    TRegion(QWidget *parent = 0);
    ~TRegion();
        void                setRootSize(int w, int h);
        void                setRegion(Region region);
        void                playImage(showImg structure);
        void                playVideo(showVideo structure);
        void                playAudio(showAudio structure);
        void                playWeb(showWeb structure);
        void                removeImage(showImg structure);
        void                removeVideo(showVideo structure);
        void                removeWeb(showWeb structure);
private:
        qreal               root_width_px, root_height_px = 0;
        QWidget            *parent;
        QString             actual_media = "";
        Region              region;
        showImg             show_image;
        showVideo           show_video;
        showAudio           show_audio;
        showWeb             show_web;
        QPixmap             image;
        QGraphicsScene     *scene;
        void                addItem(QString media_type, QGraphicsItem *item);
        void                removeItem(QString media_type, QGraphicsItem *item);
        void                resizeGeometry();
        void                resizeImage(int w, int h);
        void                resizeVideo(int w, int h);
        void                resizeWeb(int w, int h);
};

#endif // TREGION_H
