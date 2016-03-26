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

#include "region.h"
#include <QLayout>

TRegion::TRegion(QWidget *parent)
{
    Q_UNUSED(parent);
    scene = new QGraphicsScene();
    setScene(scene);
    setAlignment(Qt::AlignCenter);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

TRegion::~TRegion()
{
}

void TRegion::playImage(showImg structure)
{
    show_image = structure;
    actual_media  = "image";
    addItem("image", show_image.image_item);
}

void TRegion::playVideo(showVideo structure)
{
    show_video = structure;
    actual_media = "video";
    addItem("video", show_video.video_item);
}

void TRegion::playAudio(showAudio structure)
{
    // nothing to see for audio, so no scene item added
    show_audio   = structure;
    actual_media = "audio";
}


void TRegion::playWeb(showWeb structure)
{
    show_web = structure;
    actual_media  = "web";
    addItem("web", show_web.browser_proxy);
    return;
}

void TRegion::removeImage(showImg structure)
{
    removeItem("image", structure.image_item);
    return;
}

void TRegion::removeVideo(showVideo structure)
{
    removeItem("video", structure.video_item);
    return;
}

void TRegion::removeWeb(showWeb structure)
{
    removeItem("web", structure.browser_proxy);
    return;
}

void TRegion::addItem(QString media_type, QGraphicsItem *item)
{
    int before = scene->items().size();
    scene->addItem(item);
    qDebug() << QTime::currentTime().toString() << "insert " << item << " " << media_type << "before insert: " << before << " after insert: "<< scene->items().size();
    return;
}

void TRegion::removeItem(QString media_type, QGraphicsItem *item)
{
    int before = scene->items().size();
    scene->removeItem(item);
    qDebug() << QTime::currentTime().toString() << "remove " << item << " " << media_type << " before remove: " << before << " after remove: "<< scene->items().size();
    return;
}

/**
 * Needs the real pixel of the actual root size regardless
 * if fullscreen or in window
 *
 * @brief TRegion::setRootSize
 * @param w
 * @param h
 */
void TRegion::setRootSize(int w, int h)
{
    root_width_px  = w;
    root_height_px = h;
    resizeGeometry();
}

void TRegion::setRegion(Region r)
{
    region = r;
    setStyleSheet("background-color:"+region.backgroundColor+";");
}

void TRegion::resizeGeometry()
{
    qreal  xr, yr, wr, hr = 0.0;
    xr = (root_width_px*region.left);
    yr = (root_height_px*region.top);
    wr = (root_width_px*region.width);
    hr = (root_height_px*region.height);
    setGeometry(qRound(xr), qRound(yr), qRound(wr), qRound(hr));
    if (actual_media == "image")
        resizeImage(qRound(wr), qRound(hr));
    else if (actual_media == "video")
        resizeVideo(qRound(wr), qRound(hr));
    else if (actual_media == "web")
        resizeWeb(qRound(wr), qRound(hr));
}

void TRegion::resizeImage(int w, int h)
{
    if (show_image.fit == "fill")
       image = show_image.pixmap.scaled(w, h, Qt::IgnoreAspectRatio);
    else if (show_image.fit == "meet")
        image = show_image.pixmap.scaled(w, h, Qt::KeepAspectRatioByExpanding);
    else if (show_image.fit == "meetbest")
        image = show_image.pixmap.scaled(w, h, Qt::KeepAspectRatio);
    else
        image = show_image.pixmap;
    show_image.image_item->setPixmap(image);
    setSceneRect(0, 0, image.width()-2, image.height()-2);
}

void TRegion::resizeVideo(int w, int h)
{
    if (show_video.fit == "fill")
        show_video.video_item->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
    else if (show_video.fit == "meet")
        show_video.video_item->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
    else if (show_video.fit == "meetbest")
        show_video.video_item->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
    if (show_video.fit != "")
        show_video.video_item->resizeRenderer(w, h);
    setSceneRect(0, 0, show_video.video_item->rendererWidth()-2, show_video.video_item->rendererHeight()-2);
    return;
}

void TRegion::resizeWeb(int w, int h)
{
    setSceneRect(0, 0, w, h);
    show_web.browser->resize(w-2, h-2);
    return;
}

