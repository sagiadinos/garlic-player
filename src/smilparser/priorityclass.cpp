#include "priorityclass.h"

TPriorityClass::TPriorityClass(QObject *parent)
{
    Q_UNUSED(parent);
}

bool TPriorityClass::parse(QDomElement element)
{
    actual_element = element; // must set to get inherited Attributed
    setAttributes();
    if (element.hasChildNodes())
        setPlaylist();
    else
        return false;
    return true;
}

void TPriorityClass::beginPlay()
{
    if (begin.getStatus() == "ms") // look if begin should be delayed
        QTimer::singleShot(begin.getMilliseconds(), this, SLOT(play())); // 10s
    else // play imediately
        play();
}

void TPriorityClass::play()
{

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
