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

TRegion::TRegion(LibFacade *lf, QWidget *parent) : QWidget(parent)
{
    setParent(parent);
    MyLibFacade = lf;
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
void TRegion::paintEvent(QPaintEvent *)
{
    QStyleOption o;
    o.init(this);
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
            secureStopDisplayingMedia(MyMediaFactory.initMedia(media));
        return;
    }

    secureStopDisplayingMedia(MyMedia);
}

bool TRegion::event(QEvent *event)
{

    if(event->type() == QEvent::TouchBegin || event->type() == QEvent::MouseButtonPress)
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
    if(event->type() == QEvent::TouchEnd || event->type() == QEvent::MouseButtonRelease)
    {
        last_touch = QDateTime::currentMSecsSinceEpoch();
        if (count_touch < 2 && MyMedia != Q_NULLPTR)
        {
            event->accept();
            MyMedia->getSmilMedia()->emitActivated();
        }
    }
    return QWidget::event(event);

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
    determineStylesheets();
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

void TRegion::secureStopDisplayingMedia(PlayerBaseMedia *TmpMedia)
{
    // MyMedia can be null, when file path not found
    if (TmpMedia == Q_NULLPTR)
        return;

    QWidget *widget = TmpMedia->getView();

    if (widget != Q_NULLPTR)
    {
        layout.data()->removeWidget(widget);
    }
    TmpMedia->deinit();
    TmpMedia = Q_NULLPTR;
}

void TRegion::determineStylesheets()
{
    QString style_sheet = "background: " + region.backgroundColor;
    style_sheet += " " + determineBackgroundImage();
    style_sheet += " " + determineBackgroundImageRepeat() + ";";
    setStyleSheet(style_sheet);
}

QString TRegion::determineBackgroundImage()
{
    QString ret = "";
    // currently we do not support sub regions (inherited)
    if (region.backgroundImage.toLower() != "none" && region.backgroundImage.toLower() != "inherited")
    {
        ret = "url(\""+MyLibFacade->requestLoaddableMediaPath(region.backgroundImage)+"\")";
        has_background_image = true;
    }
    return ret;
}

QString TRegion::determineBackgroundImageRepeat()
{
    if (!has_background_image)
        return "";

    QString ret = "";
    if (region.backgroundRepeat.toLower() == "repeat")
    {
        ret = "repeat";
    }
    else if (region.backgroundRepeat.toLower() == "repeatx")
    {
        ret = "repeat-x";
    }
    else if (region.backgroundRepeat.toLower() == "repeaty")
    {
        ret = "repeat-y";
    }
    else if (region.backgroundRepeat.toLower() == "norepeat")
    {
        ret = "no-repeat";
    }
  // sub regions snot supported so no inherited neccesary
    else if (region.backgroundRepeat.toLower() == "inherited")
    {
        ret = "repeat";
    }

    return ret;
}


