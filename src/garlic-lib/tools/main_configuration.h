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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QSettings>
#include <QTimeZone>
#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QUuid>
#include <QString>
#include "logger.h"
#include "version.h"


/**
 * @brief The TConfiguration class
 */
class MainConfiguration  : public QObject
{
    Q_OBJECT
    public:
        const     QString        OS_ANDROID  = "Android";
        const     QString        OS_DARWIN   = "Darwin";
        const     QString        OS_HURD     = "Hurd";
        const     QString        OS_IOS      = "iOS";
        const     QString        OS_LINUX    = "Linux";
        const     QString        OS_NETBSD   = "BSD";
        const     QString        OS_OSX      = "macOS";
        const     QString        OS_WINDOWS  = "Windows";
        const     QString        OS_UNKNOWN  = "Unknown";

        explicit        MainConfiguration(QSettings *uc, QObject *parent = Q_NULLPTR);
        QString         getVersion(){return version;}
        void            setAdditionalVersion(QString value);
        void            setAppName(QString value){app_name = value;}
        QString         getAppName(){return app_name;}
        QString         getDescription() {return "SMIL Media Player for Digital Signage";}

        static QString  log_directory;
        static QString  getLogDir();

        void            setLastPlayedIndexPath(const QString &value);
        QString         getUserConfigByKey(QString key);
        void            setUserConfigByKey(QString key, QString value);
        QString         createUuid();
        void            setUuid(const QString &value);
        void            setPlayerName(const QString &value);
        void            determinePlayerName();

        // easy getter
        QString         getUuid() const {return uuid;}
        QString         getPlayerName() const {return player_name;}
        void            setLogDir(const QString &value){log_dir = value;}
        void            setUserAgent(const QString &value){user_agent = value;}
        QString         getUserAgent() const {return user_agent;}
        QString         getIndexUri(){return index_uri;}
        QString         getOS() const {return os;}
        QString         getValidatedContentUrl() const {return validated_content_url;}
        QString         getIndexPath(){return index_path;}
        QString         getTimeZone() const {return time_zone;}
        QString         getBasePath() const{return base_path;};
        QString         getErrorText() const {return error_text;};

        void            setValidatedContentUrl(const QString &value);
        QString         getLastPlayedIndexPath();
        QString         getStartTime() const;
        void            setStartTime(const QString &value);
        QString         getPaths(QString path_name);
        void            setIndexUri(const QString &value);
        void            setIndexPath(const QString &value);
        void            setNetworkInterface(const QString &value);
        QString         getNetworkInterface();
        void            setBasePath(const QString &value);
        void            determineBasePath(QString absolute_path_to_bin);
        void            determineIndexUri(const QString &value);
        void            createDirectories();
        void            determineUserAgent();
        bool            validateContentUrl(QString url_string);

protected:
        QSettings      *UserConfig;
        QString         uuid = "";
        QString         player_name = "";
        QString         version = version_from_git;
        QString         user_agent = "";
        QString         os = "";
        QString         base_path = "";
        QString         index_uri = "";
        QString         index_path = "";
        QString         validated_content_url = "";
        QString         start_time = "";
        QString         time_zone = "";
        QString         cache_dir = "";
        QString         log_dir = "";
        QString         app_name = "garlic-player";
        QString         error_text = "";
        void            createDirectoryIfNotExist(QString path);
        void            determineIndexPath();
        void            determineOS();
};

#endif // CONFIGURATION_H
