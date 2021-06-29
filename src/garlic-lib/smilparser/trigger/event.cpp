#include "event.h"

Event::Event(QObject *parent) : Trigger::BaseTrigger(parent)
{

}

bool Event::parse(QString sync_value)
{
    has_extern_trigger = false;
    QStringList sl = splitPossibleClockValue(sync_value).split(".");
    if (sl.length() < 2)
        return false;

    source_id = sl.at(0);
    bool ret = true;;
    // make sure that there are no hacking jokes
    if (sl.at(1) == "beginEvent")
        nm_token = "beginEvent";
    else if (sl.at(1) == "endEvent")
        nm_token = "endEvent";
    else if (sl.at(1) == "activateEvent")
        nm_token = "activateEvent";
    else if (sl.at(1) == "click")
        nm_token = "click";
    else
        ret = false;

    if (ret)
        has_extern_trigger = true;

    return ret;
}

QString Event::getNmToken()
{
    return nm_token;
}

