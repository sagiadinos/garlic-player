#include "end_timer.h"

Timings::EndTimer::EndTimer(QObject *parent) : Timings::EnhancedTimer(parent)
{

}

void Timings::EndTimer::setActiveBeginTimeTrigger(qint64 value)
{
    active_begin_time_trigger = value;
}


bool Timings::EndTimer::fireImmediately()
{
    // if this is an end attribute and a clock with negative offset
    // then it does not matter => fire the end signal!
    if (has_negative_offset_trigger)
        return true;

    // And wallclock value is an interval with next and previous trigger
    if (has_wallclock_next_trigger &&
        (active_begin_time_trigger > negative_trigger))
        return false;

    // if end or begin have a negative trigger =>> fire it
    return (negative_trigger <= 0);
}


