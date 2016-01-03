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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QSettings>
#include <QDir>
#include <QUuid>

/**
 * @brief The TConfiguration class
 */
class TConfiguration
{

public:
    TConfiguration();
    static QString getVersion(){return "0.1.0";}
    static QString getAppName(){return "garlic player";}
    static QString getDescription() {return "SMIL Player for Digital Signage";}
    void    setBaseDirectory(QString directory);
    QString getSetting(QString setting_name);
    QString getPaths(QString path_name);
    QString getIndexServer();
    QString getUserAgent();
protected:
    void createDirectories();
private:
    QDir            dir;
    QString         base_path;
    QString         user_agent, index_server;
    QSettings      *user_settings;
};

#endif // CONFIGURATION_H
