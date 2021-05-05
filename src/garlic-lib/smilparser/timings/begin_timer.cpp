#include "begin_timer.h"

Timings::BeginTimer::BeginTimer(SimpleTimer *dt, QObject *parent) : EnhancedTimer(parent)
{
    DurTimer = dt;
}

Timings::BeginTimer::~BeginTimer()
{
    deleteTimer();
}


void Timings::BeginTimer::start()
{
    if (MyTriggerList.size() == 0)
        return;


    qint64 next_trigger, previous_trigger, rest_duration;
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
                  ts->MyWallClock->calculateCurrentTrigger();

                  next_trigger     = ts->MyWallClock->getNextTimerTrigger();
                  previous_trigger = ts->MyWallClock->getPreviousTimerTrigger();
                  if (next_trigger > 0)
                     ts->MyTimer->start(next_trigger);

                  if (DurTimer == Q_NULLPTR)
                      break;

                  if (DurTimer->getType() == DurTimer->TYPE_INDEFINITE)
                      emitTimeout();

                  if (DurTimer->getType() == DurTimer->TYPE_CLOCKVALUE)
                  {
                      rest_duration = DurTimer->getOriginalTimeClock() + previous_trigger; // + due to previous trigger is always negative
                      if (rest_duration > 0)
                      {
                          DurTimer->setExternCalculatedTimeClock(rest_duration);
                          emitTimeout();
                      }
                   }

            break;
        }
    }
}
