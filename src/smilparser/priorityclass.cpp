#include "priorityclass.h"

TPriorityClass::TPriorityClass(QObject *parent)
{
    Q_UNUSED(parent);
    setObjectName("TExcl"); // priorityClass elements may only appear as immediate children of excl elements
}

bool TPriorityClass::parse(QDomElement dom_element)
{
    root_element   = dom_element;
    setAttributes();
    setPlaylist();
    return true;
}


bool TPriorityClass::findElement(QDomElement dom_element)
{
    for (iterator =  ar_playlist.begin(); iterator < ar_playlist.end(); iterator++)
    {
        if (dom_element == *iterator)
            return true;
    }
    return false;
}

void TPriorityClass::insertQueue(TBase *element)
{
    if (peers == "defer" || lower == "defer")
        return ar_defer.push(element);
    if (peers == "pause" || lower == "pause")
        ar_pause.enqueue(element);
    return;
}

int TPriorityClass::countQueue()
{
    if (peers == "defer" || lower == "defer")
        return ar_defer.size();
    if (peers == "pause" || lower == "pause")
        return ar_pause.size();
    return 0;
}

TBase *TPriorityClass::getFromQueue()
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


QList<QDomElement> TPriorityClass::getChilds()
{
    return ar_playlist;
}


void TPriorityClass::setAttributes()
{
    if (root_element.hasAttribute("peers"))
        peers = root_element.attribute("peers");
    if (root_element.hasAttribute("higher"))
        higher = root_element.attribute("higher");
    if (root_element.hasAttribute("lower"))
        lower = root_element.attribute("lower");
    return;
}

void TPriorityClass::setPlaylist()
{
    QDomNodeList childs = root_element.childNodes();
    count_childs        = childs.length();
    QDomElement  element;
    for (int i = 0; i < count_childs; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "priorityClass")
            ar_playlist.append(element);
    }
    return;
}
