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
    MyImage.reset(new PlayerImage(this));
    MyVideo.reset(new PlayerVideo(this));
    MyAudio.reset(new PlayerAudio(this));
    MyWeb.reset(new PlayerWeb(this));
}

PlayerBaseMedia *MediaFactory::initMedia(BaseMedia *media)
{
    QString type   = media->objectName();
    if (type == "TImage")
    {
        MyImage.data()->init(media);
        return MyImage.data();
    }
    else if (type == "TVideo")
    {
        MyVideo.data()->init(media);
        return MyVideo.data();
    }
    else if (type == "TAudio")
    {
        MyAudio.data()->init(media);
        return MyAudio.data();
    }
    else if (type == "TWeb")
    {
        MyWeb.data()->init(media);
        return MyWeb.data();
    }
    else if (type == "TWidget")
    {
        MyWeb.data()->init(media);
        return MyWeb.data();
    }
    else
        return Q_NULLPTR;
}
