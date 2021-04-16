#include "simple_timer.h"

Timings::SimpleTimer::SimpleTimer(QObject *parent)
{
    Q_UNUSED(parent);
 }

Timings::SimpleTimer::~SimpleTimer()
{
    deleteTimer();
}


void Timings::SimpleTimer::parse(QString attr_value)
{
    if (attr_value == "" || attr_value == "indefinite")
    {
        type = TYPE_INDEFINITE;
    }
    else if (attr_value == "media")
    {
        type = TYPE_MEDIA;
    }
    else
    {
        type = TYPE_CLOCKVALUE;
        MyClockValue.parse(attr_value);
        initTimer();
    }
    return;
}

void Timings::SimpleTimer::start()
{
    if (MyTimer == Q_NULLPTR && type != TYPE_CLOCKVALUE)
        return;

    MyTimer->start(MyClockValue.getNextTimerTrigger());
}

void Timings::SimpleTimer::resume()
{
    if (MyTimer == Q_NULLPTR && type != TYPE_CLOCKVALUE)
        return;

    MyTimer->start(remaining);
}


void Timings::SimpleTimer::pause()
{
    if (MyTimer != Q_NULLPTR && MyTimer->isActive())
    {
        remaining = MyTimer->remainingTime();
        MyTimer->stop();
    }
    pause_start = QDateTime::currentMSecsSinceEpoch();
}

void Timings::SimpleTimer::stop()
{
    if (MyTimer != Q_NULLPTR && MyTimer->isActive())
    {
        MyTimer->stop();
    }
    remaining = 0;
}

int Timings::SimpleTimer::getRemaining() const
{
    return remaining;
}

void Timings::SimpleTimer::initTimer()
{
    MyTimer = new QTimer(this);
    connect(MyTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));
    MyTimer->setSingleShot(true);
    MyTimer->setTimerType(Qt::PreciseTimer);
}

void Timings::SimpleTimer::deleteTimer()
{
    if (MyTimer != Q_NULLPTR)
    {
        stop();
        delete MyTimer;
        MyTimer = Q_NULLPTR;
    }
}
void Timings::SimpleTimer::emitTimeout()
{
    emit timeout();
}
