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
#include "tools/downloader.h"
#include "configuration.h"

class TIndexFile: public QObject
{
    Q_OBJECT
public:
    TIndexFile();
    void            load(QString path, TConfiguration *config);
    QDomElement     getHead();
    QDomElement     getBody();
    QString         getFilePath();
protected:
    void            loadFromHttpToDom(QString uri);
    TDownloader    *MyDownloader;
    TConfiguration *MyConfiguration;
    QDomDocument    document;
    QDomElement     getTag(QString name);
protected slots:
    void            loadFromLocalToDom(QString filename);
signals:
    void isLoaded();
};

#endif // FILE_H
