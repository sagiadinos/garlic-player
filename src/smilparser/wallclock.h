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

#ifndef TWALLCLOCK_H
#define TWALLCLOCK_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QTimerEvent>

/**
 * @brief The TWallClock class parses ISO 8601 datetime-format with periods and repeats enhancements
 *        inits QTimer event for given value
 *
 * When Timer first event started in past and has repeats the class has to calculate rest of repeats until end or indefinite
 *
 * Notice: maybe bedder should work only with oneShot. Periods of months cannot be handled with continuesly
 * millisecods cause months and years have different days. So next event has to be calculated everytime after fired actual or in a QList.
 *
 */
class TWallClock : public QObject
{
    Q_OBJECT
public:
    explicit TWallClock(QObject *parent = 0);
    bool     parse(QString iso_date);
private:
    QDateTime datetime;
    qint64    period  = 0;
    int       repeats = 0;

signals:

public slots:
};

#endif // TWALLCLOCK_H
