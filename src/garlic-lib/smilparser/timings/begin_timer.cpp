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
    return (negative_trigger <= 0);
}

