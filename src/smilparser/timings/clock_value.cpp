#include "clock_value.h"

TClockValue::TClockValue(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent);
}

void TClockValue::parse(QString s_val)
{
    if (s_val == "" || s_val == "indefinite" || s_val == "media")
        status = s_val;
    else
    {
        ms     = calculateMilliseconds(s_val);
        status = "ms";
    }
    // status will become ms and returns 0 when there will be iserted rubbish
    return;
}

QString TClockValue::getStatus()
{
    return status;
}

qint64 TClockValue::getMilliseconds()
{
    return ms;
}


/**
 * @brief TTiming::calculateDuration converts clock or timecount values to milliseconds
 *
 * @param  dur
 * @return
 */
qint64 TClockValue::calculateMilliseconds(QString dur)
{
    int length = dur.length();
    int duration = 0;
    if (dur.contains(QChar(':'))) // Full clock or partial clock values
    {
        QStringList ar = dur.split(QChar(':'));
        if (ar.size() == 3) // Full
            duration = (ar[0].toInt()*3600000) + (ar[1].toInt()*60000) + (ar[2].toFloat()*1000);
        else if (ar.size() == 2) // partial
            duration = (ar[0].toInt()*60000) + (ar[1].toFloat()*1000);
        // size 1 not possible here, cause no ':' would be treated as timecount
    }
    else // Timecount values
    {
        if (dur[length-1] == QChar('s'))
            duration = dur.mid(0, length-1).toFloat()*1000;
        else if (dur[length-1] == QChar('h'))
            duration = dur.mid(0, length-1).toFloat()*3600000;
        else if (dur.contains("min"))
            duration = dur.mid(0, length-3).toFloat()*60000;
        else if (dur.contains("ms"))
             duration = dur.mid(0, length-2).toInt();
        else if (dur=="indefinite")
            duration = -1;
        else if (dur=="media")
            duration = -1;
        else // no marker means seconds
            duration = dur.toFloat()*1000;
    }
    return duration;
}
