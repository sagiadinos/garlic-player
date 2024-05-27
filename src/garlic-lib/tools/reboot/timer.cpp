#include "timer.h"
#include <QDateTime>

Timer::Timer(QObject *parent) : QObject(parent)
{
    connect(&timer, &QTimer::timeout, this, &Timer::checkRebootTime);
    timer.start(10000); // check every 10s
}

void Timer::setRebootTime(const QDateTime &rebootDateTime)
{
    this->rebootDateTime = rebootDateTime;
}

void Timer::checkRebootTime()
{
    if (QDateTime::currentDateTime() >= rebootDateTime)
    {
        timer.stop();
        emit rebootTimeReached();
    }
}
