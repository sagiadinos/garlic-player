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
    setParent(parent);
    layout = new QHBoxLayout;
    layout->setMargin(0);
    setLayout(layout);
    connect(&MediaDecoder, SIGNAL(finished()), this, SLOT(finished()));
}

TRegion::~TRegion()
{
    if (actual_media == "image")
    {
        MyImage->stop();
    }
    else if (actual_media == "video")
    {
        MediaDecoder.stop();
        MyVideo->stop();
    }
    else if (actual_media == "MyAudio")
    {
        MediaDecoder.stop();
        MyAudio->stop();
    }
    else if (actual_media == "web")
    {
        MyWeb->stop();
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
    ImageWidget    = new QLabel;
    ImageWidget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    image.load(MyImage->getLoadablePath());
    ImageWidget->setPixmap(image);
    layout->addWidget(ImageWidget);
}

void TRegion::playVideo(TVideo *structure)
{
    MyVideo        = structure;
    actual_media   = "video";
    VideoWidget    = new MediaViewWrapper(this);
    MediaDecoder.setVideoOutput(VideoWidget);
    MediaDecoder.load(MyVideo->getLoadablePath());
    MediaDecoder.play();
    layout->addWidget(VideoWidget->getVideoWidget());
}

void TRegion::playAudio(TAudio *structure)
{
    MyAudio        = structure;
    actual_media   = "audio";
    MediaDecoder.load(MyAudio->getLoadablePath());
    MediaDecoder.play();
}

void TRegion::playWeb(TWeb *structure)
{
    MyWeb        = structure;
    actual_media = "web";
    browser      = new QWebEngineView;
    browser->load(QUrl(MyWeb->getLoadablePath()));
    browser->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);
    browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    layout->addWidget(browser);
}

void TRegion::removeImage()
{
    layout->removeWidget(ImageWidget);
    delete ImageWidget;
}

void TRegion::removeVideo()
{
    layout->removeWidget(VideoWidget->getVideoWidget());
    delete VideoWidget;
}

void TRegion::removeAudio()
{
    return;
}

void TRegion::removeWeb()
{
    layout->removeWidget(browser);
    delete browser;
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
       ImageWidget->setPixmap(image.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else if (MyImage->getFit() == "meet")
        ImageWidget->setPixmap(image.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else if (MyImage->getFit() == "meetbest")
        ImageWidget->setPixmap(image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        ImageWidget->setPixmap(image);
}

void TRegion::resizeVideo(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
    if (MyVideo->getFit() == "fill")
        VideoWidget->setAspectRatioFill();
    else if (MyVideo->getFit() == "meet")
        VideoWidget->setAspectRatioMeet();
    else if (MyVideo->getFit() == "meetbest")
        VideoWidget->setAspectRatioMeetBest();
}

void TRegion::resizeWeb(int w, int h)
{
    browser->resize(w-2, h-2);
}

void TRegion::finished()
{
    if (actual_media == "video")
        MyVideo->finishedSimpleDuration();
    else if (actual_media == "audio")
        MyAudio->finishedSimpleDuration();
}

