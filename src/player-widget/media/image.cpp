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
#include "media/image.h"

Image::Image(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    ParserImage  = qobject_cast<TImage *>(media);
}

Image::~Image()
{
    loaded_image.load("");
    delete ImageWidget;
}

void Image::init()
{
    QString path = ParserImage->getLoadablePath();
    loaded_image.load(path);
    ImageWidget    = new QLabel;
    ImageWidget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ImageWidget->setPixmap(loaded_image);
}

void Image::changeSize(int w, int h)
{
    if (ParserImage->getFit() == "fill")
       ImageWidget->setPixmap(loaded_image.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else if (ParserImage->getFit() == "meet")
        ImageWidget->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else if (ParserImage->getFit() == "meetbest")
        ImageWidget->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        ImageWidget->setPixmap(loaded_image);
}

QWidget *Image::getView()
{
    return ImageWidget;
}
