#include "begin_timer.h"

Timings::BeginTimer::BeginTimer(QObject *parent) : Timings::EnhancedTimer(parent)
{

}

qint64 Timings::BeginTimer::getElapsedButActiveTimeTrigger()
{
    return negative_trigger;
}


bool Timings::BeginTimer::fireImmediately()
{
    if (fire_immediately)
        return true;

    return (negative_trigger <= 0);
}

