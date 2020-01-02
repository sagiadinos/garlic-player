#include "simple_timer.h"

SimpleTimer::SimpleTimer(QObject *parent)
{
    Q_UNUSED(parent);
 }

SimpleTimer::~SimpleTimer()
{
    deleteTimer();
}


void SimpleTimer::parse(QString attr_value)
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

void SimpleTimer::initTimer()
{
    MyTimer = new QTimer(this);
    connect(MyTimer, SIGNAL(timeout()), this, SLOT(emitTimeout()));
    MyTimer->setSingleShot(true);
    MyTimer->setTimerType(Qt::PreciseTimer);
}

void SimpleTimer::deleteTimer()
{
    if (MyTimer != Q_NULLPTR)
    {
        stop();
        delete MyTimer;
        MyTimer = Q_NULLPTR;
    }
}

void SimpleTimer::start()
{
    if (type != TYPE_CLOCKVALUE)
        return;

    MyTimer->start(MyClockValue.getNextTimerTrigger());
}

void SimpleTimer::resume()
{
    if (type != TYPE_CLOCKVALUE)
        return;

    MyTimer->start(remaining);
}


void SimpleTimer::pause()
{
    if (MyTimer->isActive())
    {
        remaining = MyTimer->remainingTime();
        MyTimer->stop();
    }
    pause_start = QDateTime::currentMSecsSinceEpoch();
}

void SimpleTimer::stop()
{
    if (MyTimer->isActive())
    {
        MyTimer->stop();
    }
    remaining = 0;
}

int SimpleTimer::getRemaining() const
{
    return remaining;
}

void SimpleTimer::emitTimeout()
{
    emit timeout();
}
