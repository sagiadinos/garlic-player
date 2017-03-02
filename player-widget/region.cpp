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

TRegion::TRegion(QWidget *parent, TFileManager *filemanager)
{
    MyFileManager = filemanager;
    setParent(parent);
    layout = new QHBoxLayout;
    layout->setMargin(0);
    setLayout(layout);
}

TRegion::~TRegion()
{
    if (actual_media == "image")
    {
        MyImage->stop();
        removeImage();
    }
    else if (actual_media == "video")
    {
        MyVideo->stop();
        removeVideo();
    }
    else if (actual_media == "MyAudio")
    {
        MyAudio->stop();
        removeAudio();
    }
    else if (actual_media == "web")
    {
        MyWeb->stop();
        removeWeb();
    }
}

void TRegion::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void TRegion::playImage(TImage *structure)
{
    MyImage        = structure;
    actual_media   = "image";
    ImageWidget.setPixmap(MyImage->getImage());
    layout->addWidget(&ImageWidget);
}

void TRegion::playVideo(TVideo *structure)
{
    MyVideo        = structure;
    actual_media   = "video";
    MyVideo->setRenderer(&VideoWidget);
    VideoWidget.setParentWidget(this);
    layout->addWidget(VideoWidget.getVideoWidget());
}

void TRegion::playAudio(TAudio *structure)
{
    MyAudio        = structure;
    actual_media   = "audio";
}

void TRegion::playWeb(TWeb *structure)
{
    actual_media  = "web";
    MyWeb        = structure;
    layout->addWidget(MyWeb->getBrowser());
}

void TRegion::removeImage()
{
    layout->removeWidget(&ImageWidget);
}

void TRegion::removeVideo()
{
    layout->removeWidget(VideoWidget.getVideoWidget());
}

void TRegion::removeAudio()
{
    return;
}

void TRegion::removeWeb()
{
    layout->removeWidget(MyWeb->getBrowser());
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
    wr = (root_width_px*region.width*1);
    hr = (root_height_px*region.height*1);
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
    if (MyImage->getFit() == "fill")
       ImageWidget.setPixmap(MyImage->getImage().scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else if (MyImage->getFit() == "meet")
        ImageWidget.setPixmap(MyImage->getImage().scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else if (MyImage->getFit() == "meetbest")
        ImageWidget.setPixmap(MyImage->getImage().scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        ImageWidget.setPixmap(MyImage->getImage());
}

void TRegion::resizeVideo(int w, int h)
{
    if (MyVideo->getFit() == "fill")
        VideoWidget.setAspectRatioFill();
    else if (MyVideo->getFit() == "meet")
        VideoWidget.setAspectRatioMeet();
    else if (MyVideo->getFit() == "meetbest")
        VideoWidget.setAspectRatioMeetBest();
    return;
}

void TRegion::resizeWeb(int w, int h)
{
    MyWeb->getBrowser()->resize(w-2, h-2);
}

