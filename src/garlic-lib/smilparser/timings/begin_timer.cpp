#include "begin_timer.h"

Timings::BeginTimer::BeginTimer(QObject *parent) : Timings::EnhancedTimer(parent)
{

}

qint64 Timings::BeginTimer::getElapsedButActiveTimeTrigger()
{
    return negative_trigger;
}

void Timings::BeginTimer::setDurTimer(Timings::SimpleTimer *dur)
{
    DurTimer = dur;
}


bool Timings::BeginTimer::fireImmediately()
{
    if (fire_immediately)
        return true;

    if (DurTimer == Q_NULLPTR)
        return false;

    int type = DurTimer->getType();
    switch (type)
    {
        case SimpleTimer::TYPE_INDEFINITE:
            return true;
        case SimpleTimer::TYPE_CLOCKVALUE:
            if (negative_trigger < 0)
                return (negative_trigger + DurTimer->getParsedClock() > 0);
    }

    return false;
}

