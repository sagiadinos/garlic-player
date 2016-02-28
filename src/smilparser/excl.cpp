/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/

#include "excl.h"

TExcl::TExcl(TContainer *parent)
{
    parent_container        = parent;
    ActivePriorityClass    = NULL;
    NewActivePriorityClass = NULL;
    initTimer();
    setObjectName("TExcl");
}


bool TExcl::parse(QDomElement element)
{
    root_element   = element;
    setTimingAttributes();
    if (element.hasChildNodes())
        setPlaylist();
    else
        return false;
    return true;
}

void TExcl::setDurationTimerBeforePlay()
{
    if (hasDurAttribute() || end_timer->isActive() || ar_priorities.size() > 0)
    {
        if (!is_resumed)
            emit startedPlaylist(parent_container, this);
    }
    else // when end or duration is not specified or no child elements stop imediately
    {
        ActivePriorityClass    = NULL;
        NewActivePriorityClass = NULL;
        finishedActiveDuration();
    }
    return;
}

TPriorityClass *TExcl::findPriorityClass(QDomElement dom_element)
{
    // found priorityClass of new checking element
    TPriorityClass *MyPriorityClass = NULL;
    QHash<int, TPriorityClass *>::iterator      ar_priorities_iterator;
    for (ar_priorities_iterator =  ar_priorities.begin(); ar_priorities_iterator != ar_priorities.end(); ar_priorities_iterator++)
    {
        MyPriorityClass = *ar_priorities_iterator;
        if (MyPriorityClass->findElement(dom_element))
            break;
    }
    return MyPriorityClass;
}

int TExcl::interruptActualPlaying(QDomElement dom_element, TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << " checked if it should interupt actual playing";
    int ret = 0;
    NewActivePriorityClass = findPriorityClass(dom_element);
    qDebug() << "Active Prio ID: " << ActivePriorityClass->getID() << "New Prio ID: " << NewActivePriorityClass->getID() << "before check Interupt";

    if (ar_priorities.key(ActivePriorityClass) == ar_priorities.key(NewActivePriorityClass))      // same priorityClass is currently active => look at peers attribute:
    {
        QString peers = ActivePriorityClass->getPeers();
        if (peers == "stop")                                // stop current active element
            ret = priorityStop(dom_element, element);
        else if (peers == "pause")                          // pause current active element
            ret = priorityPause(dom_element, element);
        else if (peers == "defer")                          // delay new element and enqueue it
            ret = priorityDefer(dom_element, element);
        else if (peers == "never")                          // ignore new element when there is another element played
            ret = priorityNever(dom_element, element);
        else                                                // peers == "stop" is default => stop current active element
            ret = priorityStop(dom_element, element);
    }
    else if (ar_priorities.key(ActivePriorityClass) > ar_priorities.key(NewActivePriorityClass)) // new priority Class is higher then current active
    {
        QString higher = ActivePriorityClass->getHigher();
        if (higher == "stop")
            ret = priorityStop(dom_element, element);
        else // higher == "pause" is default
            ret = priorityPause(dom_element, element);

        qDebug() << NewActivePriorityClass->getID() << "is the new ActivePriorityClass after paused " << ActivePriorityClass->getID();
    }
    else if (ar_priorities.key(ActivePriorityClass) < ar_priorities.key(NewActivePriorityClass)) // new priority Class is lover then current active
    {
        QString lower = ActivePriorityClass->getLower();
        if (lower == "never")
            ret = priorityNever(dom_element, element);
        else
            ret = priorityDefer(dom_element, element);
    }
    return ret;
}

bool TExcl::isChildPlayable(TBaseTiming *element)
{
    TBaseTiming   *played_element = getPlayedElement(); // must set before interrupting
    int      interrupt      = interruptActualPlaying(element->getRootElement(), element);
    bool     playable       = false;
    QString  str_int        = "";
    switch (interrupt)
    {
            case 1: str_int = "stop active";
                    break;
            case 3: str_int = "pause active";
                    break;
            case 2: str_int = "play this";
                    break;
            case 0: str_int = "stop new";
                    break;
            case 4: str_int = "pause new";
                    break;
            default: str_int = "nothing";
                    break;
    }
    qDebug() << element->getID() << QTime::currentTime().toString() << "is ChildPlayable in excl with interuppt = " << str_int;
    if (interrupt == _stop_active) // stop active
    {
        childEnded(played_element);
        emit stopElement(played_element);
        playable  = true;
        setChildActive(true);
    }
    if (interrupt == _pause_active) // stop active
    {
        playable  = true;
        setChildActive(true);
        emit pauseElement(played_element);
    }
    else if (interrupt == _play_this)
    {
        setChildActive(true);
        playable = true;
    }
    else if (interrupt == _stop_new) // stop caller when peers = never
    {
        childEnded(element);
        emit stopElement(element);
        if(played_element == element)
            setChildActive(false);
    }
    else if (interrupt == _pause_new)
        emit pauseElement(element);
    qDebug() << NewActivePriorityClass->getID() << "is the new ActivePriorityClass after check interrupt " << ActivePriorityClass->getID();
    ActivePriorityClass = NewActivePriorityClass; // Change Active everytime!
    return playable;
}

/**
 * @brief TExcl::next means to stop, pause, defer the active element
 *        and set another element to active.
 *
 * @return
 */
void TExcl::next(TBaseTiming *ended_element)
{
    qDebug() << ended_element->getID() << QTime::currentTime().toString() << "ended Element (TExcl::next)";
    setChildActive(false);
    childEnded(ended_element);
    // if elements are in queues starts resume or starts them
    QHash<int, TPriorityClass *>::iterator      ar_priorities_iterator;
    TPriorityClass                             *MyPriorityClass;
    for (ar_priorities_iterator =  ar_priorities.begin(); ar_priorities_iterator != ar_priorities.end(); ar_priorities_iterator++ )
    {
        MyPriorityClass       = *ar_priorities_iterator;
        if (MyPriorityClass->countQueue() > 0)
        {
            qDebug() << MyPriorityClass->getID() << "is the new ActivePriorityClass after get from Queue";
            ActivePriorityClass = MyPriorityClass; // important to change the now active Playlist
            setChildActive(true);
            played_element = MyPriorityClass->getFromQueue();
            emit resumeElement(played_element);
            return; // make sure do not move, until queue is not epty
        }
    }
    if (!hasPlayingChilds())
    {
        if(checkRepeatCountStatus())
        {
            play();
        }
        else
        {
            finishedActiveDuration();
        }
    }
    return;
}

void TExcl::play()
{
    ActivePriorityClass    = NULL;
    NewActivePriorityClass = NULL;
    activatable_childs.clear();
    if (ar_priorities.size() > 0)
    {
        QList<QDomElement> childs;
        QList<QDomElement>::iterator i;
        QHash<int, TPriorityClass *>::iterator      ar_priorities_iterator;
        for (ar_priorities_iterator =  ar_priorities.begin(); ar_priorities_iterator != ar_priorities.end(); ar_priorities_iterator++ )
        {
            ActivePriorityClass       = *ar_priorities_iterator;
            childs                    = ActivePriorityClass->getChildList();
            for (i = childs.begin(); i != childs.end(); i++)
            {
                active_element = *i;
                if (active_element.tagName() != "") // e.g. comments
                {
                    reactByTag();
                }
            }
        }

    }
    status = _playing;
    return;
}

void TExcl::resume()
{
    status = _playing;
    return;
}

void TExcl::stop()
{
    status = _stopped;
    return;
}

void TExcl::pause()
{
    status = _paused;
    return;
}

// ============================ protected methods ================================================================
/**
 * @brief TExcl::setPlaylist selects the priority Class objects
 *   This method check if a there are priorityClass childs and create the priority class objects
 *   When there are no prioritiyClass Tags check if there are direct elements and puts them into a default priority class object
 *   Remark: this ignores direct childs when there is at least one priorityClass according to W3-Document not permitting mixed
 *   https://www.w3.org/TR/REC-smil/smil-timing.html#edef-priorityClass
 */
void TExcl::setPlaylist()
{
    QDomNodeList  priority_class_childs       = root_element.elementsByTagName("priorityClass");
    int           count_priority_class_childs = priority_class_childs.length();
    if (count_priority_class_childs > 0)
    {
        for (int i = 0; i < count_priority_class_childs; i++)
            parsePriorityClass(priority_class_childs.item(i).toElement());
    }
    else
    {
        if (root_element.childNodes().length() > 0) // put the eLements directly into one standard priorityclass object;
            parsePriorityClass(root_element);
    }
    return;
}

void TExcl::parsePriorityClass(QDomElement element)
{
    TPriorityClass * MyPriorityClass = new TPriorityClass(this);
    ar_priorities.insert(ar_priorities.size(), MyPriorityClass); // create a priority class object
    MyPriorityClass->parse(element);
    return;
}

int TExcl::priorityStop(QDomElement dom_element, TBaseTiming *element)
{
    int ret;
    if (is_child_active)
        ret = _stop_active;
    else
        ret = _play_this;
    played_element     = element;
    played_dom_element = dom_element;
    return ret;
}

int TExcl::priorityPause(QDomElement dom_element, TBaseTiming *element)
{
    int ret;
    if (is_child_active)
    {
        qDebug() << ActivePriorityClass->getID() << "insert in Queue: " << played_element->getID();
        ActivePriorityClass->insertQueue(played_element);
        ret = _pause_active;
    }
    else
        ret = _play_this;
    played_element     = element;
    played_dom_element = dom_element;
    return ret;
}

int TExcl::priorityNever(QDomElement dom_element, TBaseTiming *element)
{
    int ret;
    if (is_child_active)
        ret = _stop_new;
    else
    {
        ret                = _play_this;
        played_element     = element;
        played_dom_element = dom_element;
    }
    return ret;
}

int TExcl::priorityDefer(QDomElement dom_element, TBaseTiming *element)
{
    int ret;
    if (is_child_active)
    {
        ActivePriorityClass->insertQueue(element);
        ret = _pause_new;
    }
    else
    {
        ret                = _play_this;
        played_element     = element;
        played_dom_element = dom_element;
    }
    return ret;
}

