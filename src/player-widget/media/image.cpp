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

Image::Image(QObject *parent) : BaseMedia(parent)
{
    ImageWidget.reset(new QLabel);
}

Image::~Image()
{
}

void Image::init(TMedia *media)
{
    MyMedia = media;

    QString path = MyMedia->getLoadablePath();
    if (isFileExists(path))
    {
        loaded_image.load(path);
        ImageWidget.data()->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ImageWidget.data()->setPixmap(loaded_image);
        if (MyMedia->getLogContentId() != "")
            setStartTime();
    }
}

void Image::deinit()
{
    loaded_image.load("");
    ImageWidget.data()->setPixmap(loaded_image);
    if (MyMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void Image::changeSize(int w, int h)
{
    if (!exists)
        return;

    QString fit = MyMedia->getFit();
    if (fit == "fill")
       ImageWidget.data()->setPixmap(loaded_image.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else if (fit == "meet")
        ImageWidget.data()->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else if (fit == "meetbest")
        ImageWidget.data()->setPixmap(loaded_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        ImageWidget.data()->setPixmap(loaded_image);
}

QWidget *Image::getView()
{
    if (!exists)
        return Q_NULLPTR;

    return ImageWidget.data();
}
