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
}

TTiming::~TTiming()
{
    WallClock wall_clock1;
    wall_clock = wall_clock1;
    period     = 0;
    status     = "";
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
        else
        {
            ms = calculateMilliseconds(s_val);
            status = "ms";
        }
        // media should be returned ms like waste cause this class is only for begin or end attribute
    }
    return;
}

qint64 TTiming::getNextTrigger(QDateTime datetime)
{
    qint64    ret       = 0;
    QDateTime calculate = wall_clock.datetime;
    if (datetime > wall_clock.datetime)
    {
        // if wallclock date is in past and repeats -1 (indefinite) get time for the next event/trigger/shot from period
        if (wall_clock.repeats == -1)
        {
            // find out the next possible trigger based on the datetime of the wallclock
            // Not very efficient when start date is years ago but premature optimization is the root of all evil ;-)
            int i = 0;
            do
            {
                calculate = addWallClockInterval(calculate);
                i++;
            }
            while (datetime > calculate);
            qDebug() << i;
            ret = datetime.msecsTo(calculate);
        }
        // if wallclock date is in past and repeats > 0 calculate elapsed time since init and look if there is be another event possible
        else if (wall_clock.repeats > 0)
        {
            int i = 0;
            do
            {
                calculate = addWallClockInterval(calculate);
                i++;
            }
            while (datetime > calculate && i < wall_clock.repeats+1);
            if (datetime < calculate)
                ret = datetime.msecsTo(calculate);
        }
        // repeat 0 return 0 ms
    }
    else // datetime of wallclock is in future
        ret = datetime.msecsTo(wall_clock.datetime);
    return ret;
}

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
        if (tmp.at(0) > 0 && !wall_clock.datetime.isValid())
            wall_clock.datetime = analyseDate(tmp);
        else
            wall_clock.period = analysePeriods(ar.at(2));

        if (ar.size() == 3)
            wall_clock.period = analysePeriods(ar.at(2));
    }
    else
        wall_clock.datetime = analyseDate(iso_date);
    wall_clock.active = true;
    return wall_clock;
}

int TTiming::analyseRepeats(QString r_value)
{
    int  repeats    = 0;
    if(r_value.length() == 1)
        repeats = -1;
    else
        repeats = r_value.mid(1, r_value.length()-1).toInt();
    return repeats;
}

// ================================ private methods ===============================================

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

QDateTime TTiming::addWallClockInterval(QDateTime datetime)
{
    datetime = datetime.addYears(wall_clock.period.years);
    datetime = datetime.addMonths(wall_clock.period.months);
    datetime = datetime.addDays(wall_clock.period.days);
    return datetime.addSecs((wall_clock.period.hours*3600) + (wall_clock.period.minutes*60) + (wall_clock.period.seconds));
}



