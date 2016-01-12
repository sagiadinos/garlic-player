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

#include "base.h"

TBase::TBase(QObject * parent)
{
    Q_UNUSED(parent);
}


// ========================= protected methods ======================================================

void TBase::setTimedStart()
{
    if (begin.getStatus() == "ms") // look if begin should be delayed
        QTimer::singleShot(begin.getMilliseconds(), this, SLOT(play())); // 10s
    else // play imediately
        play();
}

bool TBase::setTimedEnd()
{
    bool ret = false;
    if (dur.getStatus() == "ms") // set end of simple duration
    {
        if (end.getStatus() == "ms") // end attribute has priority over dur (simple duration
            QTimer::singleShot(end.getMilliseconds(), this, SLOT(emitfinished())); // 10s
        else
            QTimer::singleShot(dur.getMilliseconds(), this, SLOT(emitfinished())); // 10s
        ret = true;
    }
    else if (dur.getStatus() == "indefinite" || end.getStatus() == "indefinite")
        ret = true;  // end time is set so no media end can be used
    else if (end.getStatus() == "wallclock")
    {
        // Todo get
    }
    else
    {
        //  if end specified simple duration is indefinited
        if (end.getStatus() == "ms") // end attribute has priority over dur (simple duration
        {
            QTimer::singleShot(end.getMilliseconds(), this, SLOT(emitfinished())); // 10s
            ret = true;
        }

    }
    return ret;
}



void TBase::setBaseAttributes()
{
    if (actual_element.hasAttribute("id"))
        id = actual_element.attribute("id");
    if (actual_element.hasAttribute("xml:id")) // In SMIL 3.0 superset old SMIL 2.0 id.
        id = actual_element.attribute("xml:id");
    if (id == "") // get line and column number as alternative when no
    {
        id = "T"+QString(actual_element.lineNumber()) + QString(actual_element.columnNumber());
    }

    if (actual_element.hasAttribute("title"))
        title = actual_element.attribute("title");

    if (actual_element.hasAttribute("begin"))
        begin.parse(actual_element.attribute("begin"));
    if (actual_element.hasAttribute("end"))
        end.parse(actual_element.attribute("end"));
    if (actual_element.hasAttribute("dur"))
        dur.parse(actual_element.attribute("dur"));
    if (actual_element.hasAttribute("repeatCount"))
        setRepeatCount(actual_element.attribute("repeatCount"));
    return;
}

/**
 * @brief  TBasePlaylist::checkRepeatCountStatus returns true, when playlist can be repeated
 * @return bool
 */
bool TBase::checkRepeatCountStatus()
{
    bool ret = false;
    if (indefinite)
        ret = true;
    else if (repeatCount > 0 && internal_count < repeatCount)
    {
        internal_count++;
        ret = true;
    }
    return ret;
}

/**
 * @brief TBase::calculateDuration converts clock or timecount values to milliseconds
 *
 * @param  duration
 * @return qint64 millseconds of duration
 */
qint64 TBase::calculateDuration(QString duration)
{
    int length = duration.length();
    int ret    = 0;
    if (duration.contains(QChar(':'))) // Full clock or partial clock values
    {
        QStringList ar = duration.split(QChar(':'));
        if (ar.size() == 3) // Full
            ret = (ar[0].toInt()*3600000) + (ar[1].toInt()*60000) + (ar[2].toFloat()*1000);
        else if (ar.size() == 2) // partial
            ret = (ar[0].toInt()*60000) + (ar[1].toFloat()*1000);
        // size 1 not possible here, cause no ':' would be treated as timecount
    }
    else // Timecount values
    {
        if (duration[length-1] == QChar('s'))
            ret = duration.mid(0, length-1).toFloat()*1000;
        else if (duration[length-1] == QChar('h'))
            ret = duration.mid(0, length-1).toFloat()*3600000;
        else if (duration.contains("min"))
            ret = duration.mid(0, length-3).toFloat()*60000;
        else if (duration.contains("ms"))
             ret = duration.mid(0, length-2).toInt();
        else if (duration=="indefinite")
            ret = -1;
        else if (duration=="media")
            ret = -1;
        else // no marker means seconds
            ret = duration.toFloat()*1000;
    }
    return ret;
}


// ========================= private methods ======================================================

void TBase::setRepeatCount(QString rC)
{
    if (rC == "indefinite")
        indefinite = true;
    else
       indefinite = false;
    repeatCount = rC.toInt();
    return;
}

