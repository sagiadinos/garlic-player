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
#include "wallclock.h"

WallClock::WallClock(QObject *parent) : QObject(parent)
{

}

void WallClock::parse(QString iso_date)
{
    QStringList ar;
    if (iso_date.contains("/"))
        ar = iso_date.split("/");

    if (ar.length() > 1)
    {
        QString tmp = ar.at(0);
        if (tmp.length() > 0 && tmp.at(0) == 'R')
           repeats = analyseRepeats(tmp);
        else
            datetime = analyseDate(tmp);

        tmp = ar.at(1);
        if (tmp.at(0) != 0 && !datetime.isValid())
            datetime = analyseDate(tmp);
        else
            period = analysePeriods(ar.at(2));

        if (ar.size() == 3)
            period = analysePeriods(ar.at(2));
    }
    else
        datetime = analyseDate(iso_date);

    active = true;
    if (repeats > 0)
        analyseRemainingRepeats(QDateTime::currentDateTime());
    return;
}


bool WallClock::remainingRepeats()
{
    if (repeats == -1)
        return true;
    if (remaining_repeats > 0)
        return true;
    return false;
}

qint64 WallClock::getNextTimerTrigger(QDateTime current)
{
    return getNextTrigger(current);
}

// ================================ protected methods ===============================================

qint64 WallClock::getNextTrigger(QDateTime current)
{
    qint64    ret       = 0;
    QDateTime calculate = datetime;
    if (current > datetime)
    {
        // if wallclock date is in past and repeats -1 (indefinite) get time for the next event/trigger/shot from period
        if (repeats == -1)
        {
            // find out the next possible trigger based on the datetime of the wallclock
            do
            {
                calculate = addWallClockIntervalOptimized(current, calculate); // do some shortcuts cause addWallClockInterval is extremly slow on long times
            }
            while (current > calculate);
            ret = current.msecsTo(calculate);
        }
        // if wallclock date is in past and repeats > 0 calculate elapsed time since init and look if there is be another event possible
        else if (repeats > 0)
            ret = analyseRemainingRepeats(current);
        // repeat 0 return 0 ms
    }
    else // datetime of wallclock is in future
        ret = current.msecsTo(datetime);
    return ret;
}

int WallClock::analyseRepeats(QString r_value)
{
    int  repeats    = 0;
    if(r_value.length() == 1)
        repeats = -1;
    else
        repeats = r_value.midRef(1, r_value.length()-1).toInt();
    return repeats;
}

qint64 WallClock::analyseRemainingRepeats(QDateTime current)
{
    QDateTime calculate = datetime;
    qint64    ret       = 0;
    int       i         = 0;
    remaining_repeats   = 0;
    do
    {
        calculate = addWallClockInterval(calculate);
        i++;
    }
    while (current > calculate && i < repeats+1);
    if (current < calculate)
    {
        ret = current.msecsTo(calculate);
        remaining_repeats = repeats - i;
    }
    return ret;
}

IsoPeriod WallClock::analysePeriods(QString p_value)
{
    IsoPeriod               iso_period;
    QRegularExpression      re("^P(?:(\\d+)Y)?(?:(\\d+)M)?(?:(\\d+)W)?(?:(\\d+)D)?(?:T(?:(\\d+)H)?(?:(\\d+)M)?(?:(\\d+)S)?)?");
    QRegularExpressionMatch match  = re.match(p_value);
    if (match.hasMatch())
    {
        iso_period.years    = match.captured(1).toInt();
        iso_period.months   = match.captured(2).toInt();
        iso_period.days     = match.captured(3).toInt()*7; // weeks
        iso_period.days    += match.captured(4).toInt();
        iso_period.hours    = match.captured(5).toInt();
        iso_period.minutes  = match.captured(6).toInt();
        iso_period.seconds  = match.captured(7).toInt();
    }
    return iso_period;
}

/**
 * @brief This format yyyy-MM-dd(+/-)w(1..7)THH:mm:ss is supported
 *
 * @param date
 * @return
 */
QDateTime WallClock::analyseDate(QString date)
{
    // we must do some work arounds cause QDateTime is pretty useless for wallclock
    // btw. also not recognizing all iso 8601 dates

    QStringList sl = date.split('T');

    return QDateTime(determineDate(sl.at(0)), determineTime(sl));
}

QDate WallClock::determineDate(QString the_date)
{
    int w_position = the_date.indexOf('w');
    QDate ret;
    if (w_position > -1)
        ret =  calculateDateWithWeekDayDifference(the_date, w_position);
    else
        ret =  QDate::fromString(the_date, "yyyy-MM-dd");

    // a valid date is mandatory otherwise the app react unpredictable
    if (ret.isValid())
        return ret;
    else
        return QDate::fromString("2000-01-01", "yyyy-MM-dd");

}

QTime WallClock::determineTime(QStringList sl)
{
    QTime ret;
    if (sl.size() > 1)
        ret = QTime::fromString(sl.at(1), "HH:mm:ss");
    else
        ret = QTime::fromString("00:00:00", "HH:mm:ss");

    // valid time mandatory otherwise the app react unpredictable
    if (ret.isValid())
        return ret;
    else
       return QTime::fromString("00:00:00", "HH:mm:ss");

}

QDate WallClock::calculateDateWithWeekDayDifference(QString the_date, int w_position)
{
    QChar op           = the_date.at(w_position-1);
    int target_weekday = QString(the_date.at(w_position+1)).toInt();
    QDate temp         = seperateDateFromOperator(op, the_date, w_position);

    return temp.addDays(determineDayDiffByOperator(op, target_weekday - temp.dayOfWeek()));

}

QDate WallClock::seperateDateFromOperator(QChar op, QString the_date, int w_position)
{
    if (op == '+' || op == '-')
        return QDate::fromString(the_date.mid(0, w_position-1), "yyyy-MM-dd");
    else
        return QDate::fromString(the_date.mid(0, w_position), "yyyy-MM-dd");
}

int WallClock::determineDayDiffByOperator(QChar op, int day_diff)
{
    // if the is no +/- we assume a missing operator so we set default to +
    if (op == '-')
        return calculateDiffBefore(day_diff);
    else
        return calculateDiffAfter(day_diff);
}

int WallClock::calculateDiffBefore(int day_diff)
{
    // because modulo % fails
    if (day_diff >= 0)
          day_diff -= 7;
    return day_diff;
}

int WallClock::calculateDiffAfter(int day_diff)
{
    // because modulo % fails
    if (day_diff <= 0)
          day_diff += 7;

    return day_diff;
}

QDateTime WallClock::addWallClockInterval(QDateTime calculated)
{
    calculated = calculated.addYears(period.years);
    calculated = calculated.addMonths(period.months);
    calculated = calculated.addDays(period.days);
    return calculated.addSecs((period.hours*3600) + (period.minutes*60) + (period.seconds));
}

/**
 * @brief WallClock::addWallClockIntervalOptimized skips to long distances
 * @param current
 * @param calculated
 * @return
 */
QDateTime WallClock::addWallClockIntervalOptimized(QDateTime current, QDateTime calculated)
{
    if (period.years == 0)
        calculated = calculated.addYears(current.date().year()-calculated.date().year()); // should give 0 on iterations
    else
        calculated = calculated.addYears(period.years);

    if (period.months == 0)
        calculated = calculated.addMonths(current.date().month()-calculated.date().month());
    else
        calculated = calculated.addMonths(period.months);

    if (period.days == 0)
        calculated = calculated.addDays(current.date().day()-calculated.date().day());
    else
        calculated = calculated.addDays(period.days);

    if (period.hours == 0)
        calculated = calculated.addSecs((current.time().hour()-calculated.time().hour())*3600);
    else
        calculated = calculated.addSecs(period.hours*3600);
    if (period.minutes == 0)
        calculated = calculated.addSecs((current.time().minute()-calculated.time().minute())*60);
    else
        calculated = calculated.addSecs(period.minutes*60);

    return calculated.addSecs(period.seconds);
}


