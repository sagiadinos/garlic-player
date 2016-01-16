#include "priorityclass.h"

TPriorityClass::TPriorityClass(QObject *parent)
{
    parent_playlist = parent;
    setObjectName("TExcl"); // priorityClass elements may only appear as immediate children of excl elements
}

bool TPriorityClass::parse(QDomElement element)
{
    root_element   = element;
    setAttributes();
    if (element.hasChildNodes())
        setPlaylist();
    else
        return false;
    return true;
}

void TPriorityClass::beginPlay()
{
    setTimedStart();
    return;
}

bool TPriorityClass::findElement(QDomElement element)
{
    for (iterator =  ar_playlist.begin(); iterator < ar_playlist.end(); iterator++)
    {
        if (element == *iterator)
            return true;
    }
    return false;
}

void TPriorityClass::play()
{
    if (setTimedEnd() || ar_playlist.length() > 0)
    {
        for (iterator =  ar_playlist.begin(); iterator < ar_playlist.end(); iterator++)
        {
            actual_element = *iterator;
            reactByTag();
        }
        status   = _playing;
        emit startedPlaylist(parent_playlist, parent_playlist); // priorityClass elements may only appear as immediate children of excl elements
    }
    else // when end or duration is not specified or no child elements stop imediately
        emitfinished();
    return;

}


void TPriorityClass::setPlaylist()
{
    QDomNodeList childs = actual_element.childNodes();
    count_childs        = childs.length();
    QDomElement  element;
    for (int i = 0; i < count_childs; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "priorityClass") // nesting of priorityClass not allowed
            ar_playlist.append(childs.item(i).toElement());

    }
}

void TPriorityClass::setAttributes()
{
    if (actual_element.hasAttribute("peers"))
        peers = actual_element.attribute("peers");
    if (actual_element.hasAttribute("higher"))
        higher = actual_element.attribute("higher");
    if (actual_element.hasAttribute("lower"))
        lower = actual_element.attribute("lower");
    return;
}
