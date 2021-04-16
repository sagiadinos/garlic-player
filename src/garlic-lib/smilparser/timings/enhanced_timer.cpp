#include "enhanced_timer.h"

Timings::EnhancedTimer::EnhancedTimer(QObject *parent)
{
    Q_UNUSED(parent);
}

Timings::EnhancedTimer::~EnhancedTimer()
{
    deleteTimer();
}

void Timings::EnhancedTimer::initTimer(int type)
{
    TimerStruct *ts = new TimerStruct();
    ts->MyTimer     = new QTimer(this);
    ts->type        = type;
    ts->remaining   = 0;

    connect(ts->MyTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));
    ts->MyTimer->setSingleShot(true);
    ts->MyTimer->setTimerType(Qt::PreciseTimer);

    MyTimerList.append(ts);
}

void Timings::EnhancedTimer::deleteTimer()
{
    if (MyTimerList.size() == 0)
        return;
    for (TimerStruct *ts : qAsConst(MyTimerList))
    {
        ts->MyTimer->stop();
        delete ts->MyTimer;
        delete ts;
    }
    MyTimerList.clear();
}

void Timings::EnhancedTimer::parse(QString attr_value)
{
    QStringList value_list = attr_value.split(';');
    for (QString value : qAsConst(value_list))
    {
        if (attr_value == "" || attr_value == "indefinite")
        {
//            type = TYPE_INDEFINITE;
        }
        else if (attr_value.mid(0, 9) == "wallclock" )
        {
            MyWallClock.parse(attr_value.mid(10, attr_value.length()-11));
            initTimer(TYPE_WALLCLOCK);
        }
        else
        {
            MyClockValue.parse(attr_value);
            initTimer(TYPE_OFFSET);
        }
    }
    return;
}

void Timings::EnhancedTimer::start()
{
    if (MyTimerList.size() == 0)
        return;

    qint64 next_trigger;
    for (TimerStruct *ts : qAsConst(MyTimerList))
    {
        switch (ts->type)
        {
            case EnhancedTimer::TYPE_OFFSET:
                  next_trigger = MyClockValue.getNextTimerTrigger();
                  if (next_trigger == 0)
                     emitTimeout();
                  else
                     ts->MyTimer->start(next_trigger);
            break;
            case TYPE_WALLCLOCK:
                  next_trigger = MyWallClock.getNextTimerTrigger();
                  if (next_trigger == 0)
                     emitTimeout();
                  else
                     ts->MyTimer->start(next_trigger);
            break;
        }
     }
}

void Timings::EnhancedTimer::pause()
{
    if (MyTimerList.size() == 0)
        return;
    for (TimerStruct *ts : qAsConst(MyTimerList))
    {
        ts->remaining = ts->MyTimer->remainingTime();
        ts->MyTimer->stop();
    }
    pause_start = QDateTime::currentMSecsSinceEpoch();
}

void Timings::EnhancedTimer::stop()
{
    if (MyTimerList.size() == 0)
        return;
    for (TimerStruct *ts : qAsConst(MyTimerList))
    {
        ts->MyTimer->stop();
        ts->remaining = 0;
    }
}

bool Timings::EnhancedTimer::resume()
{
    qint64 elapsed     = QDateTime::currentMSecsSinceEpoch() - pause_start;
    bool   ret         = false;
    if (MyTimerList.size() == 0)
        return ret;
    for (TimerStruct *ts : qAsConst(MyTimerList))
    {
        switch (ts->type)
        {
            case TYPE_OFFSET:
                if ((ts->remaining - elapsed) > 0)
                {
                    ts->MyTimer->start(ts->remaining - elapsed);
                    ret = true;
                }
            break;
            case TYPE_WALLCLOCK:
                ts->MyTimer->start(MyWallClock.getNextTimerTrigger());
                ret = true;
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
    if (MyTimerList.size() == 0)
        return false;

    for (TimerStruct *ts : qAsConst(MyTimerList))
    {
        if (ts->MyTimer->isActive())
            return true;
    }
    return false;
}

bool Timings::EnhancedTimer::remainingRepeats()
{
    return MyWallClock.remainingRepeats();
}

void Timings::EnhancedTimer::emitTimeout()
{
    emit timeout();
}
