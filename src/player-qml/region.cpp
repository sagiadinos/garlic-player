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
#include <unistd.h>

TRegion::TRegion(QObject *parent)
{
    root_item = qobject_cast<QQuickItem*>(parent);
}

TRegion::~TRegion()
{
//    video_item->deleteLater();
//    audio_item->deleteLater();
//    image_item->deleteLater();
//    web_item->deleteLater();
//    delete rectangle_item;
//    delete rectangle;
//    delete media_component;
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

void TRegion::playImage(TImage *Media)
{
    QString fill_mode = determineQmlFillMode(Media->getFit());
    QString anchors   = "";
    if (fill_mode != "Pad")
        anchors = "anchors.fill: parent;\n";
    QString str(
                "import QtQuick 2.7\n \
                    Image {\n " +
                            anchors+
                        "fillMode: Image."+fill_mode+";\n \
                        source: \"file:"+Media->getLoadablePath()+"\";\n \
                    }\n"
    );
    image_item = createMediaItem(str);
    image_item->setParentItem(rectangle_item);
}

void TRegion::playVideo(TVideo *Media)
{
    MyVideo  = Media;
    QString fill_mode = determineQmlFillMode(MyVideo->getFit());
    QString anchors   = "";
    if (fill_mode != "Pad")
        anchors = "anchors.fill: parent;\n";
    QString str("import QtQuick 2.7\n \
                 import  QtMultimedia 5.7\n \
                    Video {\n " +
                        anchors+
                        "autoPlay: true;\n \
                        fillMode: VideoOutput."+fill_mode+";\n \
                        source: \"file:"+MyVideo->getLoadablePath()+"\";\n \
                   }\n"
    );
    video_item = createMediaItem(str);
    connect(video_item, SIGNAL(stopped()), this, SLOT(finishedVideo()));
    connect(video_item, SIGNAL(destroyed(QObject*)), this, SLOT(doDestroyVideo(QObject *)));
    video_item->setParentItem(rectangle_item);
}

void TRegion::playAudio(TAudio *Media)
{
    MyAudio  = Media;
    QString str("import QtQuick 2.7\n \
                 import  QtMultimedia 5.7\n \
                    Audio {\n \
                        autoPlay: true;\n \
                        source: \"file:"+MyAudio->getLoadablePath()+"\";\n \
                   }\n"
    );
    audio_item = createMediaItem(str);
    connect(audio_item, SIGNAL(stopped()), this, SLOT(finishedAudio()));
    connect(audio_item, SIGNAL(destroyed(QObject*)), this, SLOT(doDestroyAudio(QObject *)));
    audio_item->setParentItem(rectangle_item);
}

void TRegion::playWeb(TWeb *Media)
{
    QString str("import QtQuick 2.7\n \
                 import QtWebView 1.1\n \
                    WebView {\n \
                        anchors.fill: parent;\n \
                        url: \""+Media->getLoadablePath()+"\";\n \
                   }\n"
    );
    web_item = createMediaItem(str);
    web_item->setParentItem(rectangle_item);
}

void TRegion::removeImage()
{
     delete image_item;
}

void TRegion::removeVideo()
{
}

void TRegion::removeAudio()
{
}

void TRegion::removeWeb()
{
    web_item->setParentItem(NULL);
    delete web_item;
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

QQuickItem *TRegion::createMediaItem(QString str)
{
    media_component->setData(str.toUtf8(), QUrl());
    double d = (double)getCurrentRSS() / (double)1048576;
    qDebug() << QString( "%1" ).arg(d, 0, 'f', 2) << " MByte RAM used";
    return qobject_cast<QQuickItem *>(media_component->create());
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

void TRegion::finishedVideo()
{
    video_item->deleteLater();
}

void TRegion::finishedAudio()
{
    audio_item->deleteLater();
}

void TRegion::doDestroyVideo(QObject *oo)
{
    MyVideo->finishedSimpleDuration();
}

void TRegion::doDestroyAudio(QObject *oo)
{
    MyAudio->finishedSimpleDuration();
}

qint64 TRegion::getCurrentRSS()
{
#if defined Q_OS_WIN32
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (qint64)info.WorkingSetSize;

#elif defined Q_OS_DARWIN
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
        return (qint64)0L;		/* Can't access? */
    return (qint64)info.resident_size;

#elif defined Q_OS_LINUX // work on Android too
    QFile fp("/proc/self/statm");
    if (!fp.open(QIODevice::ReadOnly))
        return 0;		/* Can't open? */
    QTextStream in(&fp);
    QStringList fields = in.readLine().split(" ");
    fp.close();
    return fields.at(1).toLong() * (qint64)sysconf(_SC_PAGESIZE);
#else
    /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (qint64)0L;			/* Unsupported. */
#endif
}
