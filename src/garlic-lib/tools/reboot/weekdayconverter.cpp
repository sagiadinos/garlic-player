/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2024 Nikolaos Saghiadinos <ns@smil-control.com>
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
#include "weekdayconverter.h"
#include "logging_categories.h"

WeekdayConverter::WeekdayConverter(QObject *parent) : QObject{parent}
{}

int WeekdayConverter::convertWeekDay(const QString &weekday_string) const
{
    int weekday_int = 0;

    QString weekday = weekday_string.toLower();

    QMap<QString, int> weekday_map;
    weekday_map["monday"]    = 1;
    weekday_map["tuesday"]   = 2;
    weekday_map["wednesday"] = 3;
    weekday_map["thursday"]  = 4;
    weekday_map["friday"]    = 5;
    weekday_map["saturday"]  = 6;
    weekday_map["sunday"]    = 7;

    if (weekday_map.contains(weekday))
    {
        weekday_int = weekday_map[weekday];
    }
    else
    {
        qWarning(System) << "The day of week string '" + weekday_string + "' is invalid!";
    }

    return weekday_int;
}
