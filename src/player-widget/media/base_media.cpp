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
#include "base_media.h"

BaseMedia::BaseMedia(QObject *parent) : QObject(parent)
{

}

void BaseMedia::setStartTime()
{
    start_time = MyLogger.getCurrentIsoDateTime();;
}

bool BaseMedia::isFileExists(QString path)
{
    exists = QFileInfo::exists(path);
    if (!exists)
    {
        QStringList list;
        list  << "resourceURI" << MyMedia->getSrc();
        qCritical(MediaPlayer) << MyLogger.createEventLogMetaData("MEDIA_NOT_AVAILABLE", list);
    }
    return exists;
}

QString BaseMedia::createPlayLogXml()
{
    if (start_time == "")
        return "";

    QString xml = MyLogger.createPlayLogEntry(start_time, MyMedia->getLogContentId());

    // set times to Zero
    start_time = "";
    return xml;
}
