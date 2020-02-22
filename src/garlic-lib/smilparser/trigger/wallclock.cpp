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
        repeats = r_value.mid(1, r_value.length()-1).toInt();
    return repeats;
}

qint64 WallClock::analyseRemainingRepeats(QDateTime actual)
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
    while (actual > calculate && i < repeats+1);
    if (actual < calculate)
    {
        ret = actual.msecsTo(calculate);
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
 * @brief analyseDate actually supporting only ISO 8601 Dates in the format yyyy-MM-ddTHH:mm:ss
 *          ToDo: research for more ISO 8601 compatibility
 * @param date
 * @return
 */
QDateTime WallClock::analyseDate(QString date)
{
    if (date.length() > 12)
        return QDateTime::fromString(date, "yyyy-MM-ddTHH:mm:ss");
    else
        return QDateTime::fromString(date, "yyyy-MM-dd");
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
 * @param actual
 * @param calculated
 * @return
 */
QDateTime WallClock::addWallClockIntervalOptimized(QDateTime actual, QDateTime calculated)
{
    if (period.years == 0)
        calculated = calculated.addYears(actual.date().year()-calculated.date().year()); // should give 0 on iterations
    else
        calculated = calculated.addYears(period.years);
    if (period.months == 0)
        calculated = calculated.addMonths(actual.date().month()-calculated.date().month());
    else
        calculated = calculated.addMonths(period.months);

    if (period.days == 0)
        calculated = calculated.addDays(actual.date().day()-calculated.date().day());
    else
        calculated = calculated.addDays(period.days);
    if (period.hours == 0)
        calculated = calculated.addSecs((actual.time().hour()-calculated.time().hour())*3600);
    else
        calculated = calculated.addSecs(period.hours*3600);
    if (period.minutes == 0)
        calculated = calculated.addSecs((actual.time().minute()-calculated.time().minute())*60);
    else
        calculated = calculated.addSecs(period.minutes*60);
    return calculated.addSecs(period.seconds);
}


