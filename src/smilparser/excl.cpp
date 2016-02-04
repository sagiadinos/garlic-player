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

TExcl::TExcl(TBase *parent)
{
    parent_playlist        = parent;
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
    {
        setPlaylist();
    }
    else
        return false;
    return true;
}

TBase *TExcl::getPlayedObject()
{
    return played_element;
}

void TExcl::setDurationTimerBeforePlay()
{
    if (hasDurAttribute() || end_timer->isActive() || ar_priorities.size() > 0)
    {
        if (!is_resumed)
            emit startedPlaylist(parent_playlist, this);
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
    /*
         found priorityClass of new checking element

        - same priorityClass is currently active => look at peers attribute:
            1. stop  => stop current active element
            2. pause => pause current active element and put it into a queue
            3. defer => put new element into queue
            4. never => ignore new element when there is another element played

        - an other priorityClass is currently active and higher => look at higher attribute for current
            1. stop  => current active element
            2. pause => current active element and put it into a queue
        - an other priorityClass is currently active and lower => look at lower attribute for current
            1.  defer => put new element into  queue
            2.  never => ignore new element when there is another element played
    */

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

    if (ActivePriorityClass == NewActivePriorityClass) // same priorityClass is currently active => look at peers attribute:
    {
        QString peers = getPeers();
        if (peers == "stop") // stop current active element
        {
            if (is_child_active)
                ret = _stop_active;
            else
                ret = _play_this;
            played_element     = element;
            played_dom_element = dom_element;
        }
        else if (peers == "pause") // pause current active element
        {
            if (is_child_active)
            {
                ActivePriorityClass->insertQueue(played_element);
                ret = _pause_active;
            }
            else
                ret = _play_this;
            played_element     = element;
            played_dom_element = dom_element;
        }
        else if (peers == "defer") // delay new element and enqueue it
        {
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
        }
        else if (peers == "never") // ignore new element when there is another element played
        {
            if (is_child_active)
                ret = _stop_new;
            else
            {
                ret                = _play_this;
                played_element     = element;
                played_dom_element = dom_element;
            }
        }
        else // peers == "stop" is default => stop current active element
        {
            if (is_child_active)
                ret = _stop_active;
            else
                ret = _play_this;
            played_element     = element;
            played_dom_element = dom_element;
        }
    }
    else if (ar_priorities.key(ActivePriorityClass) > ar_priorities.key(NewActivePriorityClass)) // new priority Class is higher then current active
    {
        QString higher = ActivePriorityClass->getHigher();
        if (higher == "stop")
        {
            if (is_child_active)
                ret = _stop_active;
            else
                ret = _play_this;
            played_element     = element;
            played_dom_element = dom_element;
        }
        else // higher == "pause" is default
        {
            if (is_child_active)
            {
                ActivePriorityClass->insertQueue(played_element);
                ret = _pause_active;
            }
            else
                ret = _play_this;
            played_element     = element;
            played_dom_element = dom_element;
        }
        ActivePriorityClass = NewActivePriorityClass;
    }
    else if (ar_priorities.key(ActivePriorityClass) < ar_priorities.key(NewActivePriorityClass)) // new priority Class is lover then current active
    {
        QString lower = ActivePriorityClass->getLower();
        if (lower == "never") // ignore new element
        {
            if (is_child_active)
                ret = _stop_new;
            else
            {
                ret                = _play_this;
                played_element     = element;
                played_dom_element = dom_element;
            }
        }
        else // lower == "defer" is default
        {
            if (is_child_active)
            {
                NewActivePriorityClass->insertQueue(element);
                ret = _pause_new;
            }
            else
            {
                ret                = _play_this;
                played_element     = element;
                played_dom_element = dom_element;
            }
        }
    }
    return ret;
}

QString TExcl::getPeers()
{
    return ActivePriorityClass->getPeers();
}

void TExcl::decActivatableChilds(TBase *element)
{
    QString id = element->getID();
    if (activatable_childs.find(element) != activatable_childs.end())
        activatable_childs.remove(element);
    return;
}

void TExcl::incActivatableChilds(TBase *element)
{
    activatable_childs.insert(element);
    return;
}

void TExcl::setChildActive(bool active)
{
    is_child_active = active;
    return;
}


/**
 * @brief TExcl::next means to stop, pause, defer the active element
 *        and set another element to active.
 *
 * @return
 */
void TExcl::next()
{
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
    if (activatable_childs.size() < 1)
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

