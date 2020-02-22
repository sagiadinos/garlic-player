#include "enhanced_timer.h"

EnhancedTimer::EnhancedTimer(QObject *parent)
{
    Q_UNUSED(parent);
}

EnhancedTimer::~EnhancedTimer()
{
    deleteTimer();
}

void EnhancedTimer::initTimer()
{
    MyTimer = new QTimer(this);
    connect(MyTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));
    MyTimer->setSingleShot(true);
    MyTimer->setTimerType(Qt::PreciseTimer);
}

void EnhancedTimer::deleteTimer()
{
    if (MyTimer != Q_NULLPTR)
    {
        stop();
        delete MyTimer;
        MyTimer = Q_NULLPTR;
    }
}

void EnhancedTimer::parse(QString attr_value)
{
    if (attr_value == "" || attr_value == "indefinite")
    {
        type = TYPE_INDEFINITE;
    }
    else if (attr_value.mid(0, 9) == "wallclock")
    {
        type    = TYPE_WALLCLOCK;
        MyWallClock.parse(attr_value.mid(10, attr_value.length()-11));
        initTimer();
    }
    else
    {
        type = TYPE_OFFSET;
        MyClockValue.parse(attr_value);
        initTimer();
    }
    return;
}

void EnhancedTimer::start()
{
    switch (type)
    {
        case EnhancedTimer::TYPE_OFFSET:
              MyTimer->start(MyClockValue.getNextTimerTrigger());
        break;
        case TYPE_WALLCLOCK:
              MyTimer->start(MyWallClock.getNextTimerTrigger());
        break;
    }

}

void EnhancedTimer::pause()
{
    if (MyTimer->isActive())
    {
        remaining = MyTimer->remainingTime();
        MyTimer->stop();
    }
    pause_start = QDateTime::currentMSecsSinceEpoch();
}

void EnhancedTimer::stop()
{
    if (MyTimer->isActive())
    {
        MyTimer->stop();
    }
    remaining = 0;
}

bool EnhancedTimer::resume()
{
    qint64 elapsed     = QDateTime::currentMSecsSinceEpoch() - pause_start;
    bool   ret         = false;
    switch (type)
    {
        case TYPE_OFFSET:
            if ((remaining - elapsed) > 0)
            {
                MyTimer->start(remaining - elapsed);
                ret = true;
            }
        break;
        case TYPE_WALLCLOCK:
            MyTimer->start(MyWallClock.getNextTimerTrigger());
            ret = true;
        break;
        case TYPE_INDEFINITE:
            ret = true;
        break;
    }

    return ret;
}

bool EnhancedTimer::isActive()
{
    return (MyTimer != Q_NULLPTR && MyTimer->isActive());
}

bool EnhancedTimer::remainingRepeats()
{
    return MyWallClock.remainingRepeats();
}

void EnhancedTimer::emitTimeout()
{
    emit timeout();
}
