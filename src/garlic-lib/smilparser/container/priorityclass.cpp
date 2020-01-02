#include "priorityclass.h"

TPriorityClass::TPriorityClass(QObject *parent) : TBase(parent)
{
    setObjectName("TPriorityClass");
}

TPriorityClass::~TPriorityClass()
{
}

bool TPriorityClass::parse(QDomElement dom_element)
{
    root_element   = dom_element;
    setAttributes();
    setChildList();
    return true;
}


bool TPriorityClass::findElement(QDomElement dom_element)
{
    for (iterator =  ar_dom_childs.begin(); iterator < ar_dom_childs.end(); iterator++)
    {
        if (dom_element == *iterator)
            return true;
    }
    return false;
}

void TPriorityClass::insertQueue(BaseTimings *element)
{
    if (peers == "defer" || lower == "defer")
        return ar_defer.push(element);
    if (peers == "pause" || lower == "pause")
        ar_pause.enqueue(element);
}

int TPriorityClass::countQueue()
{
    if (peers == "defer" || lower == "defer")
        return ar_defer.size();
    if (peers == "pause" || lower == "pause")
        return ar_pause.size();
    return 0;
}

BaseTimings *TPriorityClass::getFromQueue()
{
    if (peers == "defer" || lower == "defer")
    {
        if (ar_defer.size() > 0)
            return ar_defer.pop();
    }
    if (peers == "pause" || lower == "pause")
    {
        if (ar_pause.size() > 0)
            return ar_pause.dequeue();
    }
    return NULL;
}


QList<QDomElement> TPriorityClass::getChildList()
{
    return ar_dom_childs;
}

void TPriorityClass::setAttributes()
{
    setBaseAttributes();
    if (root_element.hasAttribute("peers"))
        peers = root_element.attribute("peers");
    if (root_element.hasAttribute("higher"))
        higher = root_element.attribute("higher");
    if (root_element.hasAttribute("lower"))
        lower = root_element.attribute("lower");
}

void TPriorityClass::setChildList()
{
    QDomNodeList childs = root_element.childNodes();
    count_childs        = childs.length();
    QDomElement  element;
    for (int i = 0; i < count_childs; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "priorityClass" && element.tagName() != "")
            ar_dom_childs.append(element);
    }
}
