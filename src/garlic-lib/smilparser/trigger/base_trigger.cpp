#include "base_trigger.h"

Trigger::BaseTrigger::BaseTrigger(QObject *parent) : QObject(parent)
{

}


QString Trigger::BaseTrigger::getSourceId()
{
    return source_id;
}

bool Trigger::BaseTrigger::hasExternTrigger()
{
    return has_extern_trigger;
}

qint64  Trigger::BaseTrigger::getTimeTrigger()
{
    if (MyClockValue == Q_NULLPTR)
        return 0;

    return MyClockValue->getTriggerInMSec();
}

void Trigger::BaseTrigger::setActive(const bool &value)
{
    is_active = value;
}

bool Trigger::BaseTrigger::isActive() const
{
    return is_active;
}

QString Trigger::BaseTrigger::splitPossibleClockValue(QString sync_value)
{
    if (sync_value.contains("+") || sync_value.contains("-"))
          return splitClockValue(sync_value);
    else
        return sync_value;

}

QString Trigger::BaseTrigger::splitClockValue(QString sync_value)
{

    QStringList sl;
    MyClockValue = new ClockValue;
    if (sync_value.contains("+"))
    {
        operant = '+';
        sl = sync_value.split("+");
    }
    else
    {
        operant = '-';
        sl = sync_value.split("-");
    }
    MyClockValue->parse(sl.at(1));

    return sl.at(0);
}
