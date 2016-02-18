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
    setBaseAttributes();
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


int TExcl::interruptActualPlaying(QDomElement dom_element, TBase *element)
{
    // found priorityClass of new checking element
    TPriorityClass *MyPriorityClass;
    int ret = 0;
    QHash<int, TPriorityClass *>::iterator      ar_priorities_iterator;
    for (ar_priorities_iterator =  ar_priorities.begin(); ar_priorities_iterator != ar_priorities.end(); ar_priorities_iterator++)
    {
        MyPriorityClass = *ar_priorities_iterator;
        if (MyPriorityClass->findElement(dom_element))
        {
            NewActivePriorityClass = *ar_priorities_iterator;
            break;
        }
    }

    if (ActivePriorityClass == NewActivePriorityClass)      // same priorityClass is currently active => look at peers attribute:
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

        ActivePriorityClass = NewActivePriorityClass;
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

bool TExcl::isChildPlayable(TBase *element)
{
    TBase   *played_element = getPlayedElement(); // must set before interrupting
    int      interrupt      = interruptActualPlaying(element->getRootElement(), element);
    bool     playable       = false;
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
    return playable;
}


/**
 * @brief TExcl::next means to stop, pause, defer the active element
 *        and set another element to active.
 *
 * @return
 */
void TExcl::next(TBase *ended_element)
{
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
            emit resumeElement(MyPriorityClass->getFromQueue());
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
            childs                    = ActivePriorityClass->getChilds();
            for (i = childs.begin(); i != childs.end();i++)
            {
                active_element = *i;
                if (active_element.tagName() != "") // e.g. comments
                {
                    QString id = parseID(active_element);
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
// ============================ private methods ==================================================================

void TExcl::setPlaylist()
{
    QDomNodeList childs = root_element.childNodes();
    int count           = childs.length();
    QDomElement  element;
    for (int i = 0; i < count; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() == "priorityClass")
            setPriorityClass(element);
        else // if there is no priorityClass childs will be grouped to a "virtually priorityClass"
        {
           setPriorityClass(root_element);
           break; // mixing of elements with or no priorityClass are not allowed, cause it is a kind of nesting http://www.w3.org/TR/REC-smil/smil-timing.html#Timing-priorityClassElement
        }
    }
    return;
}

void TExcl::setPriorityClass(QDomElement element)
{
    TPriorityClass *MyPriorityClass = new TPriorityClass(this);
    if (MyPriorityClass->parse(element))
        ar_priorities.insert(ar_priorities.size(), MyPriorityClass);
    return;
}

int TExcl::priorityStop(QDomElement dom_element, TBase *element)
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

int TExcl::priorityPause(QDomElement dom_element, TBase *element)
{
    int ret;
    if (is_child_active)
    {
        ActivePriorityClass->insertQueue(played_element);
        ret = _pause_active;
    }
    else
        ret = _play_this;
    played_element     = element;
    played_dom_element = dom_element;
    return ret;
}

int TExcl::priorityNever(QDomElement dom_element, TBase *element)
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

int TExcl::priorityDefer(QDomElement dom_element, TBase *element)
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

