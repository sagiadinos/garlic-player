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

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include "smilparser/head.h"
#include "smilparser.h"
#include "files/file_manager.h"

class TRegion : public QWidget
{
    Q_OBJECT
public:
    explicit TRegion(QWidget *parent, TFileManager *filemanager);
    ~TRegion();
    void                paintEvent(QPaintEvent *event);
    void                setRootSize(int w, int h);
    void                setRegion(Region region);
    void                playImage(TImage *structure);
    void                playVideo(TVideo *structure);
    void                playAudio(TAudio *structure);
    void                playWeb(TWeb *structure);
    void                removeImage();
    void                removeVideo();
    void                removeAudio();
    void                removeWeb();

protected:
    QLabel               *ImageWidget;
    MediaViewWrapper     *VideoWidget;
    TImage               *MyImage;
    TVideo               *MyVideo;
    TAudio               *MyAudio;
    TWeb                 *MyWeb;
    qreal                 root_width_px, root_height_px = 0;
    QString               actual_media = "";
    Region                region;
    QHBoxLayout          *layout;
    TFileManager         *MyFileManager;
    void                  resizeGeometry();
    void                  resizeImage(int w, int h);
    void                  resizeVideo(int w, int h);
    void                  resizeWeb(int w, int h);
};

#endif // TREGION_H
