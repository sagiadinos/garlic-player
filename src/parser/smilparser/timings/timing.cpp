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

#include "timing.h"

TTiming::TTiming(QObject *parent)
{
    Q_UNUSED(parent);
    period            = 0;
    status            = "";
    remaining_repeats = 0;
}

TTiming::~TTiming()
{
    WallClock wall_clock1;
    wall_clock        = wall_clock1;
    period            = 0;
    status            = "";
    remaining_repeats = 0;
}

void TTiming::parse(QString s_val)
{
    if (s_val == "" || s_val == "indefinite")
        status = s_val;
    else
    {
        if (s_val.mid(0, 9) == "wallclock")
        {
            parseWallclock(s_val.mid(10, s_val.length()-11)); // wallclock(the_interesting stuff)
            status    = "wallclock";
        }
        else // default an unrecognized set to 0 except on excl-element (indefinite). This ist set in TExcl->parse
        {
            ms = calculateMilliseconds(s_val);
            status = "ms";
        }
    }
    return;
}

qint64 TTiming::getNextTrigger(QDateTime actual)
{
    qint64    ret       = 0;
    QDateTime calculate = wall_clock.datetime;
    if (actual > wall_clock.datetime)
    {
        // if wallclock date is in past and repeats -1 (indefinite) get time for the next event/trigger/shot from period
        if (wall_clock.repeats == -1)
        {
            // find out the next possible trigger based on the datetime of the wallclock
            do
            {
                calculate = addWallClockIntervalOptimized(actual, calculate); // do some shortcuts cause addWallClockInterval is extremly slow on long times
            }
            while (actual > calculate);
            ret = actual.msecsTo(calculate);
        }
        // if wallclock date is in past and repeats > 0 calculate elapsed time since init and look if there is be another event possible
        else if (wall_clock.repeats > 0)
            ret = analyseRemainingRepeats(actual);
        // repeat 0 return 0 ms
    }
    else // datetime of wallclock is in future
        ret = actual.msecsTo(wall_clock.datetime);
    return ret;
}

bool TTiming::remainingRepeats()
{
    if (wall_clock.repeats == -1)
        return true;
    if (remaining_repeats > 0)
        return true;
    return false;
}

// ================================ proteced methods ===============================================


WallClock TTiming::parseWallclock(QString iso_date)
{
    QStringList ar;
    if (iso_date.contains("/"))
        ar = iso_date.split("/");
    if (ar.length() > 1)
    {
        QString tmp = ar.at(0);
        if (tmp.length() > 0 && tmp.at(0) == 'R')
           wall_clock.repeats = analyseRepeats(tmp);
        else
            wall_clock.datetime = analyseDate(tmp);

        tmp = ar.at(1);
        if (tmp.at(0) != 0 && !wall_clock.datetime.isValid())
            wall_clock.datetime = analyseDate(tmp);
        else
            wall_clock.period = analysePeriods(ar.at(2));

        if (ar.size() == 3)
            wall_clock.period = analysePeriods(ar.at(2));
    }
    else
        wall_clock.datetime = analyseDate(iso_date);
    wall_clock.active = true;
    if (wall_clock.repeats > 0)
        analyseRemainingRepeats(QDateTime::currentDateTime());
    return wall_clock;
}


// ================================ private methods ===============================================

int TTiming::analyseRepeats(QString r_value)
{
    int  repeats    = 0;
    if(r_value.length() == 1)
        repeats = -1;
    else
        repeats = r_value.mid(1, r_value.length()-1).toInt();
    return repeats;
}

qint64 TTiming::analyseRemainingRepeats(QDateTime actual)
{
    QDateTime calculate = wall_clock.datetime;
    qint64    ret       = 0;
    int       i         = 0;
    remaining_repeats   = 0;
    do
    {
        calculate = addWallClockInterval(calculate);
        i++;
    }
    while (actual > calculate && i < wall_clock.repeats+1);
    if (actual < calculate)
    {
        ret = actual.msecsTo(calculate);
        remaining_repeats = wall_clock.repeats - i;
    }
    return ret;
}

IsoPeriod TTiming::analysePeriods(QString p_value)
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
 * @brief analyseDate actually supporting only ISO 8601 Dates in the format yyyy-MM-ddTHH:mm:ss
 *          ToDo: research for more ISO 8601 compatibility
 * @param date
 * @return
 */
QDateTime TTiming::analyseDate(QString date)
{
    if (date.length() > 12)
        return QDateTime::fromString(date, "yyyy-MM-ddTHH:mm:ss");
    else
        return QDateTime::fromString(date, "yyyy-MM-dd");
}

QDateTime TTiming::addWallClockInterval(QDateTime calculated)
{
    calculated = calculated.addYears(wall_clock.period.years);
    calculated = calculated.addMonths(wall_clock.period.months);
    calculated = calculated.addDays(wall_clock.period.days);
    return calculated.addSecs((wall_clock.period.hours*3600) + (wall_clock.period.minutes*60) + (wall_clock.period.seconds));
}

/**
 * @brief TTiming::addWallClockIntervalOptimized skips to long distances
 * @param actual
 * @param calculated
 * @return
 */
QDateTime TTiming::addWallClockIntervalOptimized(QDateTime actual, QDateTime calculated)
{
    if (wall_clock.period.years == 0)
        calculated = calculated.addYears(actual.date().year()-calculated.date().year()); // should give 0 on iterations
    else
        calculated = calculated.addYears(wall_clock.period.years);
    if (wall_clock.period.months == 0)
        calculated = calculated.addMonths(actual.date().month()-calculated.date().month());
    else
        calculated = calculated.addMonths(wall_clock.period.months);

    if (wall_clock.period.days == 0)
        calculated = calculated.addDays(actual.date().day()-calculated.date().day());
    else
        calculated = calculated.addDays(wall_clock.period.days);
    if (wall_clock.period.hours == 0)
        calculated = calculated.addSecs((actual.time().hour()-calculated.time().hour())*3600);
    else
        calculated = calculated.addSecs(wall_clock.period.hours*3600);
    if (wall_clock.period.minutes == 0)
        calculated = calculated.addSecs((actual.time().minute()-calculated.time().minute())*60);
    else
        calculated = calculated.addSecs(wall_clock.period.minutes*60);
    return calculated.addSecs(wall_clock.period.seconds);
}


