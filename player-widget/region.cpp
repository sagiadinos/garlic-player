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

#include "region.h"
#include <QLayout>

TRegion::TRegion(QWidget *p)
    //: QGraphicsView(parent)
{
    parent = p;
    scene = new QGraphicsScene();
    setScene(scene);
    setAlignment(Qt::AlignCenter);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void TRegion::playImage(showImg structure)
{
    show_image = structure;
    scene->addItem(show_image.image_item);
    actual_media  = "image";
}

void TRegion::playVideo(showVideo structure)
{
    show_video = structure;
    scene->addItem(show_video.video_item);
    actual_media = "video";
}

void TRegion::playAudio(showAudio structure)
{
    // nothing to see for audio, so no scene item added
    show_audio   = structure;
    actual_media = "audio";
}


void TRegion::playWeb(showWeb structure)
{
    show_web               = structure;
    show_web.browser_proxy = scene->addWidget(show_web.browser);
    actual_media  = "web";
    return;
}

void TRegion::removeImage(showImg structure)
{
    scene->removeItem(structure.image_item);
//    scene->clear();
    return;
}

void TRegion::removeVideo(showVideo structure)
{
    scene->removeItem(structure.video_item);
//    scene->clear();
    return;
}

void TRegion::removeWeb(showWeb structure)
{
    scene->removeItem(structure.browser_proxy->createProxyForChildWidget(structure.browser));
//    scene->clear();
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

TRegion::~TRegion()
{
}
