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

#ifndef HEAD_H
#define HEAD_H

#include <algorithm>
#include <QFile>
#include <QString>
#include <QtXml>
#include <QList>
#include <QTimer>


/**
 * @brief The Region struct
 * All values are QString cause there could be a percent sign (%) in SMIL
 */
struct Region
{
    QString regionName;
    qreal top;
    qreal left;
    qreal width;
    qreal height;
    int z_index;
    QString backgroundColor;
    bool operator<(const Region& other) const {return z_index < other.z_index;} // use for sorting a list of Regions
};


class THead: public QObject
{
    Q_OBJECT
public:
    explicit THead(QObject *parent = Q_NULLPTR);
     ~THead();
    void           setDefaultValues();
    void           parse(QDomElement head);
    int            getRefreshTime(){return refresh;}
    QString        getRootBackgroundColor();
    QString        getTitle();
    QList<Region>  getLayout();
    void           setRootLayout(int w, int h);
public slots:
    void           emitCheckForNewIndex();
protected:
    Region         default_region;
    QString        title;
    int            refresh, width, height;
    QString        backgroundColor;
    QDomElement    head;
    QList<Region>  region_list;
    QTimer        *refresh_timer;
    void           parseMeta(QDomElement element);
    void           parseLayout(QDomElement layout);
    void           parseRootLayout(QDomElement root_layout);
    void           parseRegions(QDomNodeList childs);
    void           setRefreshTimer();
    qreal          calculatePercentBasedOnRoot(QString value, qreal root);
signals:
    void           checkForNewIndex();
};

#endif // HEAD_H
