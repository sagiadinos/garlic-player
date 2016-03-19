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


TConfiguration::TConfiguration(QString path)
{
    user_settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player");
    setBasePath();
    setFullIndexPath(path);
    setIndexPath();
    setUserAgent();
    createDirectories();
}

QString TConfiguration::getSetting(QString setting_name)
{
    return user_settings->value(setting_name).toString();
}

QString TConfiguration::getFullIndexPath()
{
    return full_index_path;
}

QString TConfiguration::getIndexPath()
{
    return index_path;
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
    if (path_name == "configuration")
        ret = base_path + "configuration/";
    else if (path_name == "var")
        ret = base_path + "var/";
    else if (path_name == "media")
        ret = base_path + "var/media/";
    else if (path_name == "base")
        ret = base_path;

    return ret;
}

void TConfiguration::setFullIndexPath(QString path)
{
    if (path != "")
        full_index_path = path;
    else
    {
        QFile file(getPaths("configuration")+"config.xml");
        if (!file.exists() || !file.open(QIODevice::ReadOnly))
            full_index_path = "http://indexes.smil-admin.com";

        QXmlStreamReader reader(&file);
        while (!reader.atEnd())
        {
             reader.readNext();
             if (reader.isStartElement())
             {
                 if (reader.name() == "prop" && reader.attributes().value("name") == "content.serverUrl")
                     full_index_path = reader.attributes().value("value").toString();
             }
         }
    }
    if (full_index_path.mid(0, 4) != "http" && full_index_path.mid(0, 3) != "ftp")
        full_index_path = base_path+full_index_path;
    return;
}

void TConfiguration::setIndexPath()
{
    if (full_index_path.mid(0, 4) == "http" || full_index_path.mid(0, 3) == "ftp")
    {
        QUrl url(full_index_path);
        index_path = url.adjusted(QUrl::RemoveFilename).toString();
    }
    else
    {
        QFileInfo fi(full_index_path);
        index_path = fi.path();
    }
    if (index_path.mid(index_path.length()-1, 1) != "/")
        index_path += "/";
    return;
}


void TConfiguration::setBasePath()
{
    QDir            dir;
    QString absolute_path = dir.absolutePath();
    if (absolute_path.contains("/bin"))
        base_path     = absolute_path.mid(0, absolute_path.length()-3); // "/" is set when xyz/bin
    else
        base_path     = absolute_path+"/";
    return;
}

void TConfiguration::createDirectories()
{
    QDir            dir;
    dir.setPath(getPaths("var"));
    if (!dir.exists() && !dir.mkpath("."))
        qDebug() << "Failed to create var"<< dir.path() << "\r";

    dir.setPath(getPaths("configuration"));
    if (!dir.exists() && !dir.mkpath("."))
        qDebug() << "FaigetIndexPath" << dir.path() << "\r";
    return;

}


QString TConfiguration::getUserAgent()
{
   return user_agent;
}

void TConfiguration::setUserAgent()
{
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

    user_agent = "GAPI/1.0 (UUID:"+getSetting("uuid")+"; NAME:"+user_settings->value("player_name", getSetting("uuid").mid(24,12)).toString()+") garlic-"+os+"/"+getVersion()+" (MODEL:Garlic)";
   // ADAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:Player with known UUID) SK8855-ADAPI/2.0.5 (MODEL:XMP-330)
   return;
}

