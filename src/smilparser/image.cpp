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

#include "image.h"

TImage::TImage(QObject *parent)
{
    parent_playlist = parent;
    setObjectName("TImage");
}

TImage::~TImage()
{
    delete show_img.image_item;
}


bool TImage::parse(QDomElement domelement)
{
    actual_element = domelement;
    setAttributes();
    setBaseParameters();
    return true;
}

showImg TImage::getMediaForShow()
{
    return show_img;
}

void TImage::beginPlay()
{
    setTimedStart();
    return;
}

void TImage::play()
{
    if (!setTimedEnd()) // when end or duration is not specified stop imediately
        QTimer::singleShot(100, this, SLOT(emitfinished())); // 10s
    emit started(parent_playlist, this);
    return;
}

QString TImage::getFit()
{
    return show_img.fit;
}

// ====================  protected methods =================================

void TImage::setAttributes()
{
    show_img.fit    = "";
    //show_img.media  = QPixmap::detach();
    setBaseMediaAttributes();

    show_img.region = region;
    if (src.mid(0, 4) == "http")
        show_img.pixmap.load(src);
    else // get relative paths
        show_img.pixmap.load(index_path+src);

    show_img.image_item = new QGraphicsPixmapItem();
    show_img.image_item->setPixmap(show_img.pixmap);
    if (actual_element.hasAttribute("fit"))
        show_img.fit = actual_element.attribute("fit");
    return;
}

// ====================  private methods =================================

void TImage::emitfinished()
{
    emit finished(parent_playlist, this);
}

