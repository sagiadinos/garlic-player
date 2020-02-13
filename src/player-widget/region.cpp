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

TRegion::TRegion(QWidget *parent) : QWidget(parent)
{
    setParent(parent);
    layout.reset(new QStackedLayout(this));
    layout.data()->setMargin(0);
    setLayout(layout.data());
 }

TRegion::~TRegion()
{
}

/**
 * @brief TRegion::paintEvent needed to draw a background color setted in smil layout header
 * @param event
 */
void TRegion::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void TRegion::startShowMedia(BaseMedia *media)
{
    MyMedia = MyMediaFactory.initMedia(media);

    QWidget *widget = MyMedia->getView();
    if (widget != Q_NULLPTR)
    {
        layout.data()->addWidget(widget);
    }
}

void TRegion::stopShowMedia()
{
    // MyMedia can be null, when file path not found
    if (MyMedia == Q_NULLPTR)
    {
        return;
    }
    QWidget *widget = MyMedia->getView();
    if (widget != Q_NULLPTR)
    {
        layout.data()->removeWidget(widget);
    }
    MyMedia->deinit();
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
    if (MyMedia != Q_NULLPTR)
        MyMedia->changeSize(qRound(wr), qRound(hr));
}




