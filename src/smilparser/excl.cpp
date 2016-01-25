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

TExcl::TExcl(QObject *parent)
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

void TExcl::beginPlay()
{
    setBeginEndTimer();
    return;
}

QObject *TExcl::getPlayedObject()
{
    return played_element;
}

void TExcl::setDurationTimerBeforePlay()
{
    if (hasDurAttribute() || end_timer->isActive() || ar_priorities.size() > 0)
    {
        emit startedPlaylist(parent_playlist, this);
        play();
    }
    else // when end or duration is not specified or no child elements stop imediately
    {
        ActivePriorityClass    = NULL;
        NewActivePriorityClass = NULL;
        emitfinished();
    }
    return;
}


int TExcl::interruptActualPlaying(QDomElement dom_element, QObject *element)
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
            if (ActivePriorityClass->isChildActive())
                ret = _interrupt_active;
            else
                ret = _play_this;
            played_element     = element;
            played_dom_element = dom_element;
        }
        else if (peers == "pause") // pause current active element
        {
            ActivePriorityClass->pauseElement(element);
            ActivePriorityClass->enqueueElement(element);
            ret = true;
        }
        else if (peers == "defer") // pause new element
        {
            ActivePriorityClass->enqueueElement(element);
            ret = _no_interrupting;
        }
        else if (peers == "never") // ignore new element when there is another element played
        {
            QString id = parseID(dom_element);
            if (ActivePriorityClass->isChildActive())
            {
                ret = _no_interrupting;
            }
            else
            {
                ret                = _play_this;
                played_element     = element;
                played_dom_element = dom_element;
            }
        }
    }
    /*    else if (ActivePriorityClass < NewActivePriorityClass) // new priority Class is higher then current active
        {
            QString higher = ActivePriorityClass->getHigher();
            if (higher == "pause")
            {
                NewActivePriorityClass = *ar_priorities_iterator;
                ActivePriorityClass->pauseActiveElement();
                ActivePriorityClass    = NewActivePriorityClass;

                return true;
            }
            else if (higher == "stop")
            {
                NewActivePriorityClass = *ar_priorities_iterator;
                ActivePriorityClass->stopActiveElement();
                ActivePriorityClass    = NewActivePriorityClass;
                return true;
            }
        }
        else if (ActivePriorityClass > NewActivePriorityClass) // new priority Class is lover then current active
        {
            QString lower = ActivePriorityClass->getLower();
            if (lower == "defer") // pause new element
            {
                NewActivePriorityClass->enqueueElement(element);
                return false;
            }
            else if (peers == "never") // ignore new element
            {
                return false;
            }
        }
     */
    return ret;
}

QString TExcl::getPeers()
{
    return ActivePriorityClass->getPeers();
}

void TExcl::decActivatableChilds()
{
    QString id = parseID(played_dom_element);
    activatable_childs--;
    return;
}

void TExcl::setChildActive(bool active)
{
    ActivePriorityClass->setActiveChilds(active);
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
    if (activatable_childs == 0)
    {
        if(checkRepeatCountStatus())
            play();
        else
            emitfinished();
    }
    return;
}

void TExcl::play()
{
    ActivePriorityClass    = NULL;
    NewActivePriorityClass = NULL;
    activatable_childs       = 0;
    if (ar_priorities.size() > 0)
    {
        QList<QDomElement> childs;
        QList<QDomElement>::iterator i;
        for (ar_priorities_iterator =  ar_priorities.begin(); ar_priorities_iterator != ar_priorities.end(); ar_priorities_iterator++ )
        {
            ActivePriorityClass       = *ar_priorities_iterator;
            childs                    = ActivePriorityClass->getChilds();
            for (i = childs.begin(); i != childs.end();i++)
            {
                active_element = *i;
                if (active_element.tagName() != "") // e.g. comments
                {
                    activatable_childs++;
                    reactByTag();
                }
            }
        }

    }
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

