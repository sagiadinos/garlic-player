#include "syncbase.h"

SyncBase::SyncBase(QObject *parent) : QObject(parent)
{

}

void SyncBase::parse(QString sync_value)
{
    has_extern_trigger = false;
    QStringList sl = splitPossibleClockValue(sync_value).split(".");
    if (sl.length() <= 1)
        return;

    source_id = sl.at(0);

    // make sure that there is only begin or end and no hacking jokes
    if (sl.at(1) == "begin")
        symbol = "begin";
    else if (sl.at(1) == "end")
        symbol = "end";

    has_extern_trigger = true;
}

bool SyncBase::hasExternTrigger()
{
    return has_extern_trigger;
}

qint64 SyncBase::getTimeTrigger()
{
    if (MyClockValue == Q_NULLPTR)
        return 0;

    return MyClockValue->getTriggerInMSec();
}

QString SyncBase::getSourceId()
{
    return source_id;
}

QString SyncBase::getSymbol()
{
    return symbol;
}

void SyncBase::setActive(const bool &value)
{
    is_active = value;
}

bool SyncBase::isActive() const
{
    return is_active;
}

QString SyncBase::splitPossibleClockValue(QString sync_value)
{
    if (sync_value.contains("+") || sync_value.contains("-"))
          return splitClockValue(sync_value);
    else
        return sync_value;

}

QString SyncBase::splitClockValue(QString sync_value)
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
