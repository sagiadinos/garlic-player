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

TRegion::TRegion(QObject *parent)
{
    root_item = qobject_cast<QQuickItem*>(parent);
}

TRegion::~TRegion()
{
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

void TRegion::setRegion(Region r, QQmlEngine *e)
{
    region = r;
    engine = e;
    rectangle       = new QQmlComponent(engine);
    media_component = new QQmlComponent(engine);
    QString str("import QtQuick 2.7\nRectangle {color: \""+region.backgroundColor+"\"}");
    rectangle->setData(str.toUtf8(), QUrl());
    rectangle_item = qobject_cast<QQuickItem *>(rectangle->create());
    rectangle_item->setParentItem(root_item);
}

void TRegion::playImage(TMedia *Media)
{
    MyMedia  = Media;
    QString fill_mode = determineQmlFillMode(MyMedia->getFit());
    QString anchors   = "";
    if (fill_mode != "Pad")
        anchors = "anchors.fill: parent;\n";

    QString str(
                "import QtQuick 2.7\n \
                    Image {\n " +
                            anchors+
                        "fillMode: Image."+fill_mode+";\n \
                        source: \"file:"+MyMedia->getLoadablePath()+"\";\n \
                    }\n"
    );
    handleMediaItem(str);
}

void TRegion::playVideo(TMedia *Media)
{
    MyMedia  = Media;
    QString fill_mode = determineQmlFillMode(MyMedia->getFit());
    QString anchors   = "";
    if (fill_mode != "Pad")
        anchors = "anchors.fill: parent;\n";
    QString str("import QtQuick 2.7\n \
                 import QtMultimedia 5.7\n \
                    Video {\n " +
                        anchors+
                        "autoPlay: true;\n \
                        fillMode: VideoOutput."+fill_mode+";\n \
                        source: \"file:"+MyMedia->getLoadablePath()+"\";\n \
                   }\n"
    );
    handleMediaItem(str);
    connect(media_item, SIGNAL(stopped()), this, SLOT(finished()));
}

void TRegion::playAudio(TMedia *Media)
{
    MyMedia        = Media;
    QString str("import QtQuick 2.7\n \
                 import QtMultimedia 5.7\n \
                    Audio {\n \
                        autoPlay: true;\n \
                        source: \"file:"+MyMedia->getLoadablePath()+"\";\n \
                   }\n"
    );
    handleMediaItem(str);
    connect(media_item, SIGNAL(stopped()), this, SLOT(finished()));
}

void TRegion::playWeb(TMedia *Media)
{
    MyMedia        = Media;
    QString str("import QtQuick 2.7\n \
                 import QtWebView 1.1\n \
                    WebView {\n \
                        anchors.fill: parent;\n \
                        url: \""+MyMedia->getLoadablePath()+"\";\n \
                   }\n"
    );
    handleMediaItem(str);
}


void TRegion::removeMedia()
{
   media_item->disconnect();
   media_item->deleteLater();
}

void TRegion::resizeGeometry()
{
    qreal  xr, yr, wr, hr = 0.0;
    xr = (root_width_px*region.left);
    yr = (root_height_px*region.top);
    wr = (root_width_px*region.width*1);
    hr = (root_height_px*region.height*1);
    rectangle_item->setX(xr);
    rectangle_item->setY(yr);
    rectangle_item->setWidth(wr);
    rectangle_item->setHeight(hr);
}

void TRegion::handleMediaItem(QString str)
{
    media_component->setData(str.toUtf8(), QUrl());
    media_item = qobject_cast<QQuickItem *>(media_component->create());
    media_item->setParentItem(rectangle_item);
}

QString TRegion::determineQmlFillMode(QString fill_mode)
{
    if (fill_mode == "fill")
       return "Stretch";
    else if (fill_mode == "meet")
        return "PreserveAspectCrop";
    else if (fill_mode == "meetbest")
        return "PreserveAspectFit";
    else
        return "Pad";
}

void TRegion::finished()
{
   MyMedia->finishedSimpleDuration();
}
