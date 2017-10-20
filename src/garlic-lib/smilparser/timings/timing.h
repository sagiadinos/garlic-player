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

#ifndef TTIMINGS_H
#define TTIMINGS_H

#include <QObject>
#include <QDateTime>
#include <QRegularExpression>
#include <QDebug>

#include "clock_value.h"

struct IsoPeriod
{
    int    years   = 0;
    int    months  = 0;
    qint64 days    = 0;
    qint64 hours   = 0;
    qint64 minutes = 0;
    qint64 seconds = 0;
};

struct WallClock
{
    bool      active  = false;
    int       repeats = 0;
    QDateTime datetime;
    IsoPeriod period;
};


/**
 * @brief The Timing class parses values of begin, end and dur. It differs between full/partial clock, timecount or wallclock values
 *        in ISO 8601 datetime-format with periods and repeats enhancements.  ISo 8601 Datetimes can only be in begin or end attributes.
 *
 *
 * When Timer first event started in past and has repeats the class has to calculate rest of repeats until end or indefinite
 *
 * Notice: maybe bedder should work only with oneShot. Periods of months cannot be handled with continuesly
 * millisecods cause months and years have different days. So next event has to be calculated everytime after fired actual or in a QList.
 *
 */
class TTiming : public TClockValue
{
    Q_OBJECT
public:
    explicit TTiming(QObject *parent = Q_NULLPTR);
    ~TTiming();
    void       calculate();
    void       parse(QString svalue);
    WallClock  getWallClock();
    qint64     getNextTrigger(QDateTime actual);
    bool       remainingRepeats();
protected:
    WallClock  parseWallclock(QString iso_date);
private:
    WallClock  wall_clock;
    qint64     period     = 0;
    int        remaining_repeats;
    IsoPeriod  analysePeriods(QString p_value);
    int        analyseRepeats(QString r_value);
    qint64     analyseRemainingRepeats(QDateTime actual);
    QDateTime  analyseDate(QString date);
    QDateTime  addWallClockInterval(QDateTime calculated);
    QDateTime  addWallClockIntervalOptimized(QDateTime actual, QDateTime calculated);
signals:

public slots:
};

#endif // TTIMINGS_H
