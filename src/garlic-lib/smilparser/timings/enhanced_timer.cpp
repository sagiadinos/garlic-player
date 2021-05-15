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
#include "enhanced_timer.h"

Timings::EnhancedTimer::EnhancedTimer(QObject *parent)
{
    Q_UNUSED(parent);
}

Timings::EnhancedTimer::~EnhancedTimer()
{
    deleteTimer();
}

void Timings::EnhancedTimer::initTimer(int type, QString value)
{
    TriggerStruct *ts = new TriggerStruct();
    ts->type        = type;
    ts->remaining   = 0;

    ts->MyTimer     = new QTimer(this);
    ts->MyTimer->setTimerType(Qt::PreciseTimer);
    ts->MyTimer->setSingleShot(true);
    connect(ts->MyTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));

    switch (ts->type)
    {
        case TYPE_OFFSET:
            ts->MyClockValue = new ClockValue();
            ts->MyClockValue->parse((value));
        break;
        case TYPE_WALLCLOCK:
            ts->MyWallClock = new WallClock();
            ts->MyWallClock->parse(value);
        break;
        case TYPE_SYNCBASE:
            ts->MySyncBase = new SyncBase();
            ts->MySyncBase->parse(value);
            if (ts->MySyncBase->hasExternTrigger())
                has_external_trigger = true;
        break;
    }

    MyTriggerList.append(ts);
}

void Timings::EnhancedTimer::deleteTimer()
{
    if (MyTriggerList.size() == 0)
        return;
    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {
        ts->MyTimer->stop();
        switch (ts->type)
        {
            case TYPE_OFFSET:
                delete ts->MyClockValue;
            break;
            case TYPE_WALLCLOCK:
                delete ts->MyWallClock;
            break;
            case TYPE_SYNCBASE:
                delete ts->MySyncBase;
            break;
        }
        delete ts->MyTimer;
        delete ts;
    }
    MyTriggerList.clear();
}

void Timings::EnhancedTimer::parse(QString attr_value)
{
    // do not toLower here, we need origial values e.g. for ISO 8601
    QStringList value_list = attr_value.simplified().split(';');
    for (const auto& value : qAsConst(value_list))
    {
        if (value.isEmpty()) // some might end with semikolon
            continue;

        if (value.contains("indefinite"))
        {
            is_indefinite = true;
        }
        else if (value.mid(0, 9) == "wallclock" )
        {
            initTimer(TYPE_WALLCLOCK, value.mid(10,value.length()-11));
        }
        else if (value.at(0) == '+' || value.at(0) == '-')
        {
            // we do not support negative values currently
            initTimer(TYPE_OFFSET, value);
        }
        else if (value.at(0).isDigit())
        {
            initTimer(TYPE_OFFSET, value);
        }
        else if (value.endsWith("begin") || value.endsWith("end"))
        {
            initTimer(TYPE_SYNCBASE, value);
        }
        else if (value.mid(0, 9) == "accesskey" )
        {
            // todo: add support for accesskeys
        }
        else if (value.at(0).isLetter() && value.contains("."))
        {
            // todo support for event ( click, load, focus )
        }
        else
        {
            // todo maybe something default?
        }
    }

    if (value_list.size() > 1) // in a value an indefinite can be ignored
        is_indefinite = false;

    return;
}

void Timings::EnhancedTimer::start()
{
    if (MyTriggerList.size() == 0)
        return;
    qint64 next_trigger;
    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {
        switch (ts->type)
        {
            case TYPE_OFFSET:
                  next_trigger = ts->MyClockValue->getTriggerInMSec();
                  if (next_trigger == 0)
                     emitTimeout();
                  else
                     ts->MyTimer->start(next_trigger);
            break;
            case TYPE_WALLCLOCK:
                  // we need to calculate a previous trigger, cause it can be possible that an
                  // element triggrered active in the past and last on due to a high duration
                  next_trigger     = ts->MyWallClock->calculateNextTrigger();
                  if (next_trigger > 0)
                     ts->MyTimer->start(next_trigger);
            case TYPE_SYNCBASE:
                ts->MySyncBase->setActive(true); // rest is done by Timings::EnhancedTimer::startFromExternalTrigger
            break;
        }
    }
}

void Timings::EnhancedTimer::startFromExternalTrigger(QString source_id)
{
    if (MyTriggerList.size() == 0)
        return;
    qint64 next_trigger;
    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {
        switch (ts->type)
        {
            case TYPE_SYNCBASE:
                if (ts->MySyncBase->getSourceId() == source_id)
                {
                    next_trigger = ts->MySyncBase->getTimeTrigger();
                    if (next_trigger == 0)
                        emitTimeout();
                    else
                       ts->MyTimer->start(next_trigger);
                }
            break;
            case TYPE_EVENT:
                // ToDo
                break;

        }
    }
}

void Timings::EnhancedTimer::pause()
{
    if (MyTriggerList.size() == 0)
        return;
    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {        
        ts->remaining = ts->MyTimer->remainingTime();
        ts->MyTimer->stop();
    }
    pause_start = QDateTime::currentMSecsSinceEpoch();
}

void Timings::EnhancedTimer::stop()
{
    if (MyTriggerList.size() == 0)
        return;

    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {
        switch (ts->type)
        {
            case TYPE_OFFSET:
            case TYPE_WALLCLOCK:
                ts->MyTimer->stop();
                ts->remaining = 0;
            break;
            case TYPE_SYNCBASE:
                ts->MySyncBase->setActive(false);
            break;
        }
    }
}

bool Timings::EnhancedTimer::resume()
{
    qint64 elapsed     = QDateTime::currentMSecsSinceEpoch() - pause_start;
    bool   ret         = false;
    if (MyTriggerList.size() == 0)
        return ret;
    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {
        switch (ts->type)
        {
            case TYPE_OFFSET:
            case TYPE_WALLCLOCK:
                if ((ts->remaining - elapsed) > 0)
                {
                    ts->MyTimer->start(ts->remaining - elapsed);
                    ret = true;
                }
            break;
            case TYPE_INDEFINITE:
                ret = true;
            break;
        }
    }
    return ret;
}

bool Timings::EnhancedTimer::isActive()
{
    if (MyTriggerList.size() == 0)
        return false;

    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {
        switch (ts->type)
        {
            case TYPE_OFFSET:
                if (ts->MyTimer->isActive())
                    return true;
            break;
            case TYPE_WALLCLOCK:
                if (ts->MyTimer->isActive())
                    return true;
            break;
            case TYPE_INDEFINITE:
                return true;
            case TYPE_SYNCBASE:
                if (ts->MySyncBase->isActive())
                    return true;
            break;
        }
    }
    return false;

}

bool Timings::EnhancedTimer::hasExternalTrigger()
{
    return has_external_trigger;
}

QHash<QString, QString> Timings::EnhancedTimer::fetchTriggerList()
{
    QHash<QString, QString> ret;
    if (MyTriggerList.size() == 0)
        return ret;

    for (TriggerStruct *ts : qAsConst(MyTriggerList))
    {
        switch (ts->type)
        {
            case TYPE_SYNCBASE:
                if (ts->MySyncBase->hasExternTrigger())
                    ret.insert(ts->MySyncBase->getSourceId(), ts->MySyncBase->getSymbol());
            break;
        }
    }
    return ret;


}

void Timings::EnhancedTimer::emitTimeout()
{
    emit timeout();
}
