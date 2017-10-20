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
#include "media_factory.h"

MediaFactory::MediaFactory(QObject *parent) : QObject(parent)
{

}

BaseMedia *MediaFactory::createMedia(TMedia *media, QObject *parent)
{
    QString type   = media->objectName();
    if (type == "TImage")
        return new Image(media, parent);
    else if (type == "TVideo")
        return new Video(media, parent);
    else if (type == "TAudio")
        return new Audio(media, parent);
    else if (type == "TWeb")
        return new Web(media, parent);
    else
        return Q_NULLPTR;
}
