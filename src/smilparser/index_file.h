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

#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QtXml>
#include <QDebug>
#include "files/downloader.h"
#include "tools/configuration.h"

class TIndexFile : public QObject
{
    Q_OBJECT
public:
    TIndexFile(TConfiguration *config, QObject *parent = 0);
    void            load();
    QDomElement     getHead();
    QDomElement     getBody();
protected:
    bool            loaded;
    TDownloader    *MyDownloader;
    TConfiguration *MyConfiguration;
    QDomDocument    document;
    void            loadFromHttpToDom(QString uri);
    void            loadFromLocalToDom(QString filename);
    QString         determineLocalFileName(QString uri);
    QDomElement     getTag(QString name);
protected slots:
    void            loadAfterDownload(QString filename);
    void            checkForLoaded(QString filename);
signals:
    void isLoaded();
};

#endif // FILE_H
