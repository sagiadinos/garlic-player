/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2021 Nikolaos Sagiadinos <ns@smil-control.com>
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
#ifndef WALLCLOCK_H
#define WALLCLOCK_H

#include <QObject>
#include <QDateTime>
#include <QRegularExpression>

struct IsoPeriod
{
    int    years   = 0;
    int    months  = 0;
    qint64 days    = 0;
    qint64 hours   = 0;
    qint64 minutes = 0;
    qint64 seconds = 0;
};

class WallClock : public QObject
{
    Q_OBJECT
public:
    explicit  WallClock(QObject *parent = nullptr);
    void      parse(QString iso_date);
    IsoPeriod getPeriod(){return period;}
    bool      isActive(){return active;}
    int       getRepeats(){return repeats;}
    QDateTime getDateTime(){return datetime;}
    bool      remainingRepeats();
    qint64    getNextTimerTrigger(QDateTime current = QDateTime::currentDateTime());
protected:
    bool      active  = false;
    int       repeats = 0;
    QDateTime datetime;
    IsoPeriod period;
    int       remaining_repeats = 0;
    qint64    getNextTrigger(QDateTime current);
    int       analyseRepeats(QString r_value);
    qint64    analyseRemainingRepeats(QDateTime current);
    IsoPeriod analysePeriods(QString p_value);
    QDateTime analyseDate(QString date);
    QDateTime addWallClockInterval(QDateTime calculated);
    QDateTime addWallClockIntervalOptimized(QDateTime current, QDateTime calculated);

private:
    QDate determineDate(QString the_date);
    QDate calculateDateWithWeekDayDifference(QString the_date, int w_position);
    int determineDayDiffByOperator(QChar op, int day_diff);
    QDate seperateDateFromOperator(QChar op, QString the_date, int w_position);
    QTime determineTime(QStringList sl);
    int calculateDiffAfter(int day_diff);
    int calculateDiffBefore(int day_diff);
signals:

};

#endif // WALLCLOCK_H
