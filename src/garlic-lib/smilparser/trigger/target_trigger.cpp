#include "target_trigger.h"

TargetTrigger::TargetTrigger(QObject *parent) : QObject(parent)
{

}

TargetTrigger::TargetTrigger()
{
    trigger_list.clear();
}

void TargetTrigger::insert(QString trigger, QString target_id)
{
    if (!trigger.isEmpty() && !target_id.isEmpty())
        trigger_list.insert(trigger, target_id);
}

QStringList TargetTrigger::findTargetIDsByTrigger(QString trigger)
{
    QStringList sl = {};
    if (trigger_list.isEmpty())
        return sl;

    QMap<QString, QString>::iterator i =  trigger_list.find(trigger);

    while (i != trigger_list.end() && i.key() == trigger)
    {
        sl.append(i.value());
        i++;
    }

    return sl;
}
