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
