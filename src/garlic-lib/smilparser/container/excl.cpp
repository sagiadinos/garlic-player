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

TExcl::TExcl(TContainer *pc, QObject *parent) : TContainer(parent)
{
    parent_container       = pc;
    ActivePriorityClass    = NULL;
    NewActivePriorityClass = NULL;
    setObjectName("TExcl");
}

TExcl::~TExcl()
{
}

void TExcl::preloadParse(QDomElement element)
{
    root_element   = element;
    if (!root_element.hasChildNodes())
    {
        active_element = root_element;
        return;
    }

    parseTimingAttributes();
    traverseChilds();

    // collect from prioritclasses
    for (QHash<int, TPriorityClass *>::iterator i_priorities = ar_priorities.begin(); i_priorities != ar_priorities.end(); i_priorities++)
    {
        QList<QDomElement> element_list = i_priorities.value()->getChildList();
        for (QList<QDomElement>::iterator i = element_list.begin(); i != element_list.end(); i++)
        {
            emit preloadElement(this, *i);
        }
    }
}

void TExcl::prepareDurationTimerBeforePlay()
{
    if (startDurTimer() || isEndTimerActive() || ar_priorities.size() > 0)
    {
        emit startedContainer(parent_container, this);
    }
    else // when end or duration is not specified or no child elements stop imediately
    {
        ActivePriorityClass    = NULL;
        NewActivePriorityClass = NULL;
        skipElement();
    }
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

int TExcl::interruptActualPlaying(QDomElement dom_element, BaseTimings *element)
{
    qDebug() << element->getID() << " checked if it should interupt actual playing";
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

bool TExcl::isChildPlayable(BaseTimings *element)
{
    BaseTimings   *played_element = getPlayedElement(); // must set before interrupting
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
    qDebug() << element->getID() << "is ChildPlayable in excl with interuppt = " << str_int;
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
        if(played_element == element)
        {
            setChildActive(false);
        }
        // element->prepareTimingsBeforeStart() was called so we need to stop here the timings;
        element->prepareTimingsBeforeStop();
    }
    else if (interrupt == _pause_new)
    {
        emit pauseElement(element);
    }

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
void TExcl::next(BaseTimings *ended_element)
{
    qDebug() << ended_element->getID() << "ended Element (TExcl::next)";
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
                    emitFoundElement();
                }
            }
        }

    }
    qDebug(Develop) << getID() << "played";
    status = _playing;
}

void TExcl::resume()
{
    qDebug(Develop) << getID() << "resumed";
    status = _playing;
}

void TExcl::stop()
{
    qDebug(Develop) << getID() << "stopped";
    status = _stopped;
}

void TExcl::pause()
{
    qDebug(Develop) << getID() << "paused";
    status = _paused;
}

// ============================ protected methods ================================================================
/**
 * @brief TExcl::setPlaylist selects the priority Class objects
 *   This method check if there are priorityClass childs and create the priority class objects
 *   When there are no prioritiyClass Tags check if there are direct elements and puts them into a default priority class object
 *   Remark: this ignores direct childs when there is at least one priorityClass according to W3-Document not permitting mixed
 *   https://www.w3.org/TR/REC-smil/smil-timing.html#edef-priorityClass
 */
void TExcl::traverseChilds()
{
    QDomNodeList  priority_class_childs       = root_element.elementsByTagName("priorityClass");
    if (priority_class_childs.length() > 0)
    {
        traversePriorityClasses(priority_class_childs);
    }
    else
    {
        parsePriorityClass(root_element);
    }
}

void TExcl::traversePriorityClasses(QDomNodeList  priority_class_childs)
{
    for (int i = 0; i < priority_class_childs.length(); i++)
    {
        parsePriorityClass(priority_class_childs.item(i).toElement());
    }
}


void TExcl::parsePriorityClass(QDomElement element)
{
    TPriorityClass * MyPriorityClass = new TPriorityClass(this);
    ar_priorities.insert(ar_priorities.size(), MyPriorityClass); // create a priority class object
    MyPriorityClass->preloadParse(element);
    return;
}

int TExcl::priorityStop(QDomElement dom_element, BaseTimings *element)
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

int TExcl::priorityPause(QDomElement dom_element, BaseTimings *element)
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

int TExcl::priorityNever(QDomElement dom_element, BaseTimings *element)
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

int TExcl::priorityDefer(QDomElement dom_element, BaseTimings *element)
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

