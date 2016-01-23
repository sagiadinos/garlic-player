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

void TPriorityClass::stopPlayingChild(QDomElement new_dom_element)
{
    return;
}

void TPriorityClass::pausePlayingChild(QDomElement new_dom_element)
{
    return;
}

void TPriorityClass::pauseElement(QObject *element)
{
    return;
}

void TPriorityClass::enqueueElement(QObject *element)
{
    queue.enqueue(element);
    return;
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


bool TPriorityClass::play()
{
    if (ar_playlist.length() > 0)
    {
        iterator =  ar_playlist.begin();
        active_element = *iterator;
        return true;
    }
   return false;
}

bool TPriorityClass::next()
{
    iterator++;
    if (iterator != ar_playlist.end())
    {
        active_element = *iterator;
        return true;
    }
    return false;
}


void TPriorityClass::setActiveChilds(bool active)
{
    is_child_active = active;
    return;
}

int TPriorityClass::decPlayableChilds()
{
    playable_childs--;
    return playable_childs;
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
