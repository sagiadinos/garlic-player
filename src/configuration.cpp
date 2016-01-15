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

#include "configuration.h"
#include <QXmlStreamReader>
#include <QDebug>
#include <QString>
#include <memory>
#include <mutex>


TConfiguration::TConfiguration()
{
    user_settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player");
    index_server       = "";
    user_agent         = "";

    QString absolute_path = dir.absolutePath();
    if (absolute_path.contains("/bin"))
        base_path     = absolute_path.mid(0, absolute_path.length()-3); // "/" is set when xyz/bin
    else
        base_path     = absolute_path+"/";
}

QString TConfiguration::getSetting(QString setting_name)
{
    return user_settings->value(setting_name).toString();
}

/**
 * @brief TConfiguration::getPaths returns paths.
 *        Values can be config, var, media or base
 * @param path_name
 * @return
 */
QString TConfiguration::getPaths(QString path_name)
{
    QString ret = "";
    if (path_name == "config")
        ret = base_path + "configuration/";
    else if (path_name == "var")
        ret = base_path + "var/";
    else if (path_name == "media")
        ret = base_path + "var/media/";
    else if (path_name == "base")
        ret = base_path;

    return ret;
}

void TConfiguration::setBaseDirectory(QString directory)
{
    base_path = directory;
}

void TConfiguration::createDirectories()
{
    dir.setPath(getPaths("media"));  // this means ./var and ./var/media
    if (!dir.exists() && !dir.mkpath("."))
        qDebug() << "Failed to create var/media"<< dir.path() << "\r";

    dir.setPath(getPaths("config"));
    if (!dir.exists() && !dir.mkpath("."))
        qDebug() << "Failed to create " << dir.path() << "\r";
    return;

}

QString TConfiguration::getIndexServer()
{
    if (index_server != "")
        return index_server;
    QFile file(getPaths("config")+"config.xml");
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        index_server = "http://indexes.smil-admin.com";
        return index_server;
    }
    QXmlStreamReader reader(&file);
    while (!reader.atEnd())
    {
         reader.readNext();
         if (reader.isStartElement())
         {
             if (reader.name() == "prop" && reader.attributes().value("name") == "content.serverUrl")
                 index_server = reader.attributes().value("value").toString();
         }
     }
    return index_server;
}

QString TConfiguration::getUserAgent()
{
    if (user_agent != "")
        return user_agent;

    if (user_settings->value("uuid", "").toString() == "")
        user_settings->setValue("uuid", QUuid::createUuid().toString().mid(1, 36));
#if defined  Q_OS_ANDROID
    QString os("android");
#elif defined Q_OS_BSD4
    QString os("bsd4");
#elif defined Q_OS_DARWIN
    QString os("darwin");
#elif defined Q_OS_FREEBSD
    QString os("freebsd");
#elif defined Q_OS_HURD
    QString os("hurd");
#elif defined Q_OS_IOS
    QString os("ios");
#elif defined Q_OS_LINUX
    QString os("linux");
#elif defined Q_OS_NETBSD
    QString os("netbsd");
#elif defined Q_OS_OPENBSD
    QString os("openbsd");
#elif defined Q_OS_OSX
    QString os("osx");
#elif defined Q_OS_WIN32
    QString os("windows");
#elif defined Q_OS_WINPHONE
    QString os("winphone");
#elif defined Q_OS_WINRT
    QString os("winrt");
#else
    QString os("unknown");
#endif

    user_agent = "GAPI/1.0 (UUID:"+getSetting("uuid")+"; NAME:"+user_settings->value("player_name", getSetting("uuid").mid(24,12)).toString()+") "+os+"/"+getVersion()+" (MODEL:Garlic)";
   // ADAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:Player with known UUID) SK8855-ADAPI/2.0.5 (MODEL:XMP-330)
   return user_agent;
}

