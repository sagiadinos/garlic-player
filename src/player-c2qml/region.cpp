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

TRegion::TRegion(LibFacade *lf, QObject *parent)
{
    MyLibFacade = lf;
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

void TRegion::setRegion(Region r, Launcher *lc, QQmlEngine *e)
{
    region = r;
    engine = e;
    rectangle.reset(new QQmlComponent(engine));
    media_component.reset(new QQmlComponent(engine));

    if (region.backgroundColor.isEmpty()) // otherwise  component is not created
    {
        region.backgroundColor = "black";
    }
    QString str("import QtQuick 2.12\n \
    Rectangle { \
        id: region_" + region.regionName + "; \
        color: \""+region.backgroundColor+"\"; \
        signal qmlClick()\n \
        MouseArea{ \
            anchors.fill: parent; \
            onClicked: { region_" + region.regionName + ".qmlClick()}\n}\n}");
    rectangle.data()->setData(str.toUtf8(), QUrl());
    rectangle_item.reset(qobject_cast<QQuickItem *>(rectangle.data()->create()));
    rectangle_item.data()->setParentItem(root_item);

    connect(rectangle_item.data(), SIGNAL(qmlClick()), this, SLOT(clickSlot()));
    MyMediaFactory.reset(new MediaFactory(media_component.data(), r.regionName, lc, this));
}


void TRegion::clickSlot()
{
    qint64 delay = QDateTime::currentMSecsSinceEpoch() - last_touch;
    if (delay < 500)
    {
        count_touch++;
    }
    else
    {
        count_touch = 0;
    }

    if (count_touch > 2)
    {
        count_touch = 0;
    }

    last_touch = QDateTime::currentMSecsSinceEpoch();
    if (count_touch < 2 && MyMedia != Q_NULLPTR)
    {
        MyMedia->getSmilMedia()->emitActivated();
    }
}

void TRegion::startShowMedia(BaseMedia *media)
{
    MyMedia = MyMediaFactory.data()->initMedia(media);
    if (MyMedia != Q_NULLPTR)
    {
        MyMedia->setParentItem(rectangle_item.data());
    }
}

void TRegion::stopShowMedia(BaseMedia *media)
{
    if (MyMedia == Q_NULLPTR)
        return;

    // 2021-07-21 see tests/data/smil/par/3_changes.smil brush over image
    // we need to stop the media in parameter if differs
    // 2021-10-01 enhancemend cause of syn-bug see tests/data/smil/par/bugs/2_pseudo_sync.smil

    if (MyMedia->getSmilMedia() != media)
    {
        if (MyMedia->getSmilMedia()->objectName() != media->objectName())
            secureStopDisplayingMedia(MyMediaFactory.data()->initMedia(media));
        return;
    }

    secureStopDisplayingMedia(MyMedia);}

void TRegion::secureStopDisplayingMedia(PlayerBaseMedia *TmpMedia)
{
    if (TmpMedia == Q_NULLPTR)
    {
        return;
    }

    TmpMedia->setParentItem(NULL);
    TmpMedia->deinit();
}

void TRegion::resizeGeometry()
{
    qreal  xr, yr, wr, hr = 0.0;
    xr = (root_width_px*region.left);
    yr = (root_height_px*region.top);
    wr = (root_width_px*region.width*1);
    hr = (root_height_px*region.height*1);
    rectangle_item.data()->setX(xr);
    rectangle_item.data()->setY(yr);
    rectangle_item.data()->setWidth(wr);
    rectangle_item.data()->setHeight(hr);
}

void TRegion::registerEventStarts()
{
    qint64 delay = QDateTime::currentMSecsSinceEpoch() - last_touch;
    if (delay < 500)
    {
        count_touch++;
    }
    else
    {
        count_touch = 0;
    }

    if (count_touch > 2)
    {
        count_touch = 0;
    }
}

void TRegion::registerEventEnds()
{
    last_touch = QDateTime::currentMSecsSinceEpoch();
    if (count_touch < 2 && MyMedia != Q_NULLPTR)
    {
        MyMedia->getSmilMedia()->emitActivated();
    }
}
