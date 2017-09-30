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

#include <QQuickItem> // interates QQmlComponent

#include <head.h>
#include <smilparser.h>
#include <media_manager.h>



class TRegion : public QQuickItem
{
    Q_OBJECT
public:
    explicit TRegion(QObject *parent);
    ~TRegion();
    void                setRootSize(int w, int h);
    void                setRegion(Region r, QQmlEngine *e);
    void                playImage(TImage *Media);
    void                playVideo(TVideo *Media);
    void                playAudio(TAudio *Media);
    void                playWeb(TWeb *Media);
    void                removeImage();
    void                removeVideo();
    void                removeAudio();
    void                removeWeb();

protected:
    QQmlComponent        *media_component, *rectangle;
    QQuickItem           *root_item, *rectangle_item, *image_item, *web_item, *audio_item, *video_item = NULL;
//    QPointer<QQuickItem>  video_item;
    QQmlEngine           *engine;
    TVideo               *MyVideo;
    TAudio               *MyAudio;
    qreal                 root_width_px, root_height_px = 0;
    Region                region;
    QString               actual_media = "";
    void                  resizeGeometry();
    void                  handleMediaItem(QString str);
    QQuickItem           *createMediaItem(QString str);
    QString               determineQmlFillMode(QString fill_mode);
protected slots:
    void                  finishedVideo();
    void                  finishedAudio();
    void                  doDestroyVideo(QObject *oo);
    void                  doDestroyAudio(QObject *oo);
private:
    mutable QMutex mutex;
    qint64                getCurrentRSS();
};

#endif // TREGION_H
