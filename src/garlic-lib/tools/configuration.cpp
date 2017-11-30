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
#include <QString>
#include <memory>
#include <mutex>

// need to define static variable
QString TConfiguration::log_directory = "";


/**
 * @brief TConfiguration::TConfiguration
 * @param UserConfig
 */
TConfiguration::TConfiguration(QSettings *UserConfig, QObject *parent) : QObject(parent)
{
    setUserConfig(UserConfig);
    determineUuid();
    determinePlayerName();
    determineOS();
    setTimeZone(QTimeZone::systemTimeZoneId());
    // ugly workaround from https://stackoverflow.com/questions/21976264/qt-isodate-formatted-date-time-including-timezone
    // cause otherwise we get no time zone in date string
    setStartTime(QDateTime::currentDateTime().toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate));
    determineUserAgent();
}

QString TConfiguration::getLogDir()
{
    return TConfiguration::log_directory;
}

void TConfiguration::setLogDir(QString path)
{
    TConfiguration::log_directory = path;
}

QString TConfiguration::getLastPlayedIndexPath()
{
    return getUserConfigByKey("last_played_index_path");
}

void TConfiguration::setLastPlayedIndexPath(const QString &value)
{
    setUserConfigByKey("last_played_index_path", value);
}

QSettings *TConfiguration::getUserConfig() const
{
    return UserConfig;
}

void TConfiguration::setUserConfig(QSettings *value)
{
    UserConfig = value;
}

QString TConfiguration::getUserConfigByKey(QString key)
{
    return UserConfig->value(key, "").toString();
}

void TConfiguration::setUserConfigByKey(QString key, QString value)
{
    UserConfig->setValue(key, value);
}


QString TConfiguration::getPlayerName() const
{
    return player_name;
}

void TConfiguration::setPlayerName(const QString &value)
{
    player_name = value;
    setUserConfigByKey("player_name", value);
}

QString TConfiguration::createUuid()
{
    return QUuid::createUuid().toString().mid(1, 36);
}

QString TConfiguration::getUuid() const
{
    return uuid;
}

void TConfiguration::setUuid(const QString &value)
{
    uuid = value;
    setUserConfigByKey("uuid", value);
}

void TConfiguration::determineUuid()
{
    setUuid(getUserConfigByKey("uuid"));
    if (getUuid() == "")
    {
        setUuid(createUuid());
        UserConfig->setValue("uuid", getUuid());
    }
}

QString TConfiguration::getUserAgent() const
{
   return user_agent;
}

void TConfiguration::setUserAgent(const QString &value)
{
    user_agent = value;
}

QString TConfiguration::getIndexUri()
{
    return index_uri;
}

void TConfiguration::setIndexUri(const QString &value)
{
    index_uri = value;
}

void TConfiguration::determineIndexUri(QString path)
{
    if (path != "")
    {
        setUserConfigByKey("index_uri", path);
        setIndexUri(path);
    }
    else if (getUserConfigByKey("index_uri") != "") // get from intern config
    {
        setIndexUri(getUserConfigByKey("index_uri"));
    }
    else
    {
        checkConfigXML();
    }

    if (index_uri != "" && index_uri.mid(0, 4) != "http"  && index_uri.mid(0, 3) != "ftp" && index_uri.mid(0, 1) != "/") // https is includered in http!
    {
       setUserConfigByKey("index_uri", base_path+index_uri);
       setIndexUri( base_path+index_uri);
    }
    determineIndexPath();
}

void TConfiguration::setIndexPath(const QString &value)
{
    index_path = value;
}
/**
 * @brief TConfiguration::determineIndexPath
 * determine the clean index path
 * e.g only the scheme://domain.tld/path_to/
 */
void TConfiguration::determineIndexPath()
{
    if (index_uri.mid(0, 4) == "http" || index_uri.mid(0, 3) == "ftp")
    {
        QUrl url(index_uri);
        setIndexPath(url.adjusted(QUrl::RemoveFilename |
                                  QUrl::RemoveQuery |
                                  QUrl::RemoveFragment |
                                  QUrl::RemovePort).toString());
        if (index_path.mid(index_path.length()-1, 1) != "/")
            setIndexPath(index_path+"/");

    }
    else
    {
        QFileInfo fi(index_uri);
        setIndexPath(fi.path()+"/");
    }
}


QString TConfiguration::getIndexPath()
{
    return index_path;
}

QString TConfiguration::getOS() const
{
    return os;
}

void TConfiguration::setOS(const QString &value)
{
    os = value;
}

QString TConfiguration::getBasePath() const
{
    return base_path;
}

void TConfiguration::setBasePath(const QString &value)
{
    base_path = value;
}

void TConfiguration::determineBasePath(QString absolute_path_to_bin)
{
    if (absolute_path_to_bin.contains("/bin"))
        setBasePath(absolute_path_to_bin.mid(0, absolute_path_to_bin.length()-3)); // "/" is set when xyz/bin
    else
        setBasePath(absolute_path_to_bin);

    if (base_path.mid(base_path.length()-1, 1) != "/")
        setBasePath(base_path+"/");
}


/**
 * @brief TConfiguration::getPaths returns paths.
 *        Values can be config, var, media or base
 * @param path_name
 * @return
 */
QString TConfiguration::getPaths(QString path_name)
{
    QString ret = base_path;
    if (path_name == "cache")
        ret = cache_dir;
    else if (path_name == "logs")
        ret = log_dir;

    return ret;
}

void TConfiguration::createDirectories()
{
#if defined Q_OS_WIN32  // QStandardPaths::CacheLocation in windows 7 is set to appdir (bin), which should not be writable after installation in Program Files
    cache_dir = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QString(), QStandardPaths::LocateDirectory) +  getAppName() + "/cache/";
    log_dir   = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QString(), QStandardPaths::LocateDirectory) +  getAppName() + "/logs/";
#elif defined  Q_OS_ANDROID
    // Using CacheLocation in Android is dangerous, cause that is limited App-Storage which flooding soon and crash the Player
    // GenericDataLocation should be /sdcard
    cache_dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/" +getAppName() + "/cache/";
    log_dir   = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/" + getAppName() + "/logs/";
#else
    cache_dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+ "/";
    log_dir   = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs/";
#endif
    createDirectoryIfNotExist(cache_dir);
    createDirectoryIfNotExist(log_dir);
    setLogDir(log_dir);
}

void TConfiguration::determineUserAgent()
{
    setUserAgent("GAPI/1.0 (UUID:"+getUuid()+"; NAME:"+getPlayerName()+") garlic-"+getOS()+"/"+getVersion()+" (MODEL:Garlic)");
}


void TConfiguration::checkConfigXML()
{
    QFile file(base_path+"config.xml");
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader reader(&file);
        while (!reader.atEnd())
        {
             reader.readNext();
             if (reader.isStartElement())
             {
                 if (reader.name() == "prop" && reader.attributes().value("name") == "content.serverUrl")
                 {
                     setUserConfigByKey("index_uri", reader.attributes().value("value").toString());
                     setIndexUri(reader.attributes().value("value").toString());
                 }
             }
         }
        file.rename(base_path+"config.xml", base_path+"config_readd0aeec17b69aed.xml");
    }
}

QString TConfiguration::getStartTime() const
{
    return start_time;
}

void TConfiguration::setStartTime(const QString &value)
{
    start_time = value;
}

QString TConfiguration::getTimeZone() const
{
    return time_zone;
}

void TConfiguration::setTimeZone(const QString &value)
{
    time_zone = value;
}


void TConfiguration::createDirectoryIfNotExist(QString path)
{
    QDir dir;
    dir.setPath(path);
    if (!dir.exists() && !dir.mkpath("."))
        qCritical(SmilParser) << "Failed to create " << dir.path() << "\r";
    return;
}

void TConfiguration::determinePlayerName()
{
    setPlayerName(getUserConfigByKey("player_name"));
    if (getPlayerName() == "")
    {
        setPlayerName(getUuid().mid(24,12));
        UserConfig->setValue("player_name", getPlayerName());
    }
}

void TConfiguration::determineOS()
{
#if defined  Q_OS_ANDROID
    setOS("android");
#elif defined Q_OS_BSD4
    setOS("bsd4");
#elif defined Q_OS_DARWIN
    setOS("darwin");
#elif defined Q_OS_FREEBSD
    setOS("freebsd");
#elif defined Q_OS_HURD
    setOS("hurd");
#elif defined Q_OS_IOS
    setOS("ios");
#elif defined Q_OS_LINUX
    setOS("linux");
#elif defined Q_OS_NETBSD
    setOS("netbsd");
#elif defined Q_OS_OPENBSD
    setOS("openbsd");
#elif defined Q_OS_OSX
    setOS("osx");
#elif defined Q_OS_WIN32
    setOS("windows");
#elif defined Q_OS_WINPHONE
    setOS("winphone");
#elif defined Q_OS_WINRT
    setOS("winrt");
#else
    setOS("unknown");
#endif
}

