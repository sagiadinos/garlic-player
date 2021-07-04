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

/**
 * Todo: Check how it is possible to inherit this Class from TPar
 * as there are some dublicate code
 *
 * @param pc
 * @param parent
 */
TExcl::TExcl(TBase *pc, QObject *parent) : TContainer(parent)
{
    parent_container       = pc;
    CurrentPriority    = NULL;
    NewPriority = NULL;
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
    if (element.hasAttribute("endsync"))
        endsync = element.attribute("endsync");

    traverseChilds();

    // collect elements from prioritclasses
    for (QMap<int, TPriorityClass *>::iterator i_priorities = PriorityClassList.begin(); i_priorities != PriorityClassList.end(); i_priorities++)
    {
        QList<QDomElement> element_list = i_priorities.value()->getChildList();
        for (QList<QDomElement>::iterator i = element_list.begin(); i != element_list.end(); i++)
        {
            emitPreloadElementSignal(this, *i);
        }
    }
}

void TExcl::prepareDurationTimerBeforePlay()
{
    // when a durtimer exists use it!
    if (hasDurTimer() && !startDurTimer() && !hasEndTimer())
    {
        skipElement();
        return;
    }

    // otherwise empty container with dur will not start
    if (PriorityClassList.size() > 0 || isDurTimerActive() || isEndTimerActive())
    {
        emitStartElementSignal(this);
    }
    else // when end or duration is not specified or no child elements stop imediately
    {
        CurrentPriority    = NULL;
        NewPriority = NULL;
        skipElement();
    }
}

/**
 * @brief TExcl::next means to stop, pause, defer the active element
 *        and set another element to active.
 *
 * @return
 */
void TExcl::next(BaseTimings *ended_element)
{
    if (status == _stopped)
        return;

    removeActivatedChild(ended_element);

    // Active DurTimer or EndTimer results in ignore endsync
    if ((endsync == "first" || endsync == ended_element->getID()) && !isDurTimerActive() && !isEndTimerActive())
    {
        interruptByEndSync();
        return;
    }

    // if elements are in queues starts resume or starts them
    QMap<int, TPriorityClass *>::iterator      it;
    TPriorityClass                             *MyPriorityClass;
    for (it =  PriorityClassList.begin(); it != PriorityClassList.end(); it++ )
    {
        MyPriorityClass       = *it;
        if (MyPriorityClass->countQueue() > 0)
        {
            CurrentPriority = MyPriorityClass; // important to change the now active Playlist
            current_activated_element = MyPriorityClass->getFromQueue();
            current_activated_element->resumeTimers();
            // to make sure that this child is not removed previous
            insertActivatedChild(current_activated_element);
            emitResumeElementSignal(current_activated_element);

            return; // make sure do not move, until queue is not empty
        }
    }

    // remove when it is sure, that there nothing to resume
    removeActivatedChild(ended_element);

    if (ended_element == getCurrentActivatedElement())
        setCurrentActivatedElement(Q_NULLPTR);

    // do nothing when childs are active
    if (hasActivatedChild())
        return;

    // not stopped by parent
    if (status == _playing)
    {
        finishIntrinsicDuration(); // check for dur and then for repeat play
        return;
    }
}

void TExcl::start()
{
    if (PriorityClassList.size() == 0) // childs_list will always be 0
        return;

    // check if this is a restart attempt and check restart attribute
    if (hasActivatedChild())
    {
        if (isRestartable())
        {
            interruptByRestart();
        }
        else
        {
            return;
        }
    }

    collectActivatedChilds();
    status          = _playing;
    startTimersOfAllActivatedChilds();
}

void TExcl::stop()
{
    status = _stopped;
    stopTimers();               // because there can be a dur or begin timer active
    emitStopToAllActivatedChilds();
    removeAllActivatedChilds();
    removeQueuedElements();
}

void TExcl::interruptByEndSync()
{
    stop();
    if (handleRepeatCountStatus())
        start();
}

void TExcl::resume()
{
    status = _playing;
}

void TExcl::pause()
{
    status = _paused;
}

void TExcl::collectActivatedChilds()
{
    CurrentPriority = NULL;
    NewPriority     = NULL;

    if (PriorityClassList.size() > 0)
    {
        QList<QDomElement> childs;
        QList<QDomElement>::iterator i;
        QMap<int, TPriorityClass *>::iterator it;
        for (it =  PriorityClassList.begin(); it != PriorityClassList.end(); it++ )
        {
            CurrentPriority       = *it;
            childs                = CurrentPriority->getChildList();
            for (i = childs.begin(); i != childs.end(); i++)
            {
                active_element = *i;
                if (active_element.tagName() != "") // e.g. comments
                {
                    activateFoundElement();
                }
            }
        }
    }
}

bool TExcl::determineContinue(BaseTimings *new_element)
{
    if (current_activated_element == new_element) // if element has already been activated from a trigger previously
    {
        return true;
    }

    if (current_activated_element == Q_NULLPTR) // if first element we can continue to show
    {
        CurrentPriority = findPriorityClass(new_element->getRootElement());
        setCurrentActivatedElement(new_element);
        return true;
    }

    // get position from new element to current element
    // this determine which priority attribute should be used
    NewPriority = findPriorityClass(new_element->getRootElement());
    QString attribute_value = "";

    // find if element is in higher, lower or same priorityClass
    // lower key number has higher priority as priority descend from top to down

    int cur_prio = PriorityClassList.key(CurrentPriority);
    int new_prio = PriorityClassList.key(NewPriority);
    if (cur_prio == new_prio)      // same priorityClass is currently active => look at peers attribute:
    {
        attribute_value = CurrentPriority->getPeers();
    }
    else if (cur_prio > new_prio) // new priority Class is higher then current
    {
        attribute_value= CurrentPriority->getHigher();
    }
    else if (cur_prio < new_prio) // new priority Class is lower then current active
    {
        attribute_value = CurrentPriority->getLower();
    }

    bool ret = true;
    if (attribute_value == "stop")        // stop current active element
    {
        priorityStop();
        setCurrentActivatedElement(new_element);
        ret = true;
    }
    else if (attribute_value == "pause")  // pause current active element
    {
        priorityPause();
        setCurrentActivatedElement(new_element);
        ret = true;
    }
    else if (attribute_value == "defer")  // delay new element and enqueue it
    {
        priorityDefer(new_element);
        ret = false;
    }
    else if (attribute_value == "never")  // ignore new element when there is another element played
    {
        priorityNever(new_element);
        ret = false;
    }

    if (ret)
        CurrentPriority = NewPriority; // even when priority is the same

    return ret;
}



/**
 *   Check for priorityClass childs and create the priority objects
 *
 *   When no priorityClass Tags put direct elements them into a default priority class object
 *
 *   Remark: mixing childs out of priority-element where ignored
 *   https://www.w3.org/TR/REC-smil/smil-timing.html#edef-priorityClass
 */
void TExcl::traverseChilds()
{
    QDomNodeList  priority_classes       = root_element.elementsByTagName("priorityClass");

    // a nested excl can cause problems with elementsByTagName
    // so we need to secure that are only
    // priorityClasses with this excl (root_element) as parent.
    QList<QDomElement> priority_childs;
    for (int i = 0; i < priority_classes.length(); i++)
    {
        if (priority_classes.item(i).parentNode().toElement() == root_element)
            priority_childs.append(priority_classes.item(i).toElement());
    }

    if (priority_childs.length() > 0)
    {
        traversePriorityClasses(priority_childs);
    }
    else
    {
        parsePriorityClass(root_element);
    }
}

void TExcl::traversePriorityClasses(QList<QDomElement>  priority_class_childs)
{
    for (auto& de : priority_class_childs)
    {
        parsePriorityClass(de);
    }
}


TPriorityClass *TExcl::findPriorityClass(QDomElement dom_element)
{
    // found priorityClass of new checking element
    TPriorityClass *MyPriorityClass = NULL;
    QMap<int, TPriorityClass *>::iterator      ar_priorities_iterator;
    for (ar_priorities_iterator =  PriorityClassList.begin(); ar_priorities_iterator != PriorityClassList.end(); ar_priorities_iterator++)
    {
        MyPriorityClass = *ar_priorities_iterator;
        if (MyPriorityClass->findElement(dom_element))
            break;
    }
    return MyPriorityClass;
}

void TExcl::removeQueuedElements()
{
    for (TPriorityClass *prio : qAsConst(PriorityClassList))
    {
       prio->removeQueuedElements();
    }

}

void TExcl::parsePriorityClass(QDomElement element)
{
    TPriorityClass * MyPriorityClass = new TPriorityClass(this);
    MyPriorityClass->preloadParse(element);
    PriorityClassList.insert(PriorityClassList.size(), MyPriorityClass); // create a priority class object
    return;
}

void TExcl::priorityStop()
{
    // stop old element
    // send signal cause element could be shown
    emitStopElementSignal(current_activated_element);
}

void TExcl::priorityPause()
{
    // pause old element and queue it
    emitPauseElementSignal(current_activated_element);
    // when peers NewPriority is Current Priority
    NewPriority->insertQueue(current_activated_element);
}

void TExcl::priorityNever(BaseTimings *new_element)
{
    // stop new element and remove it
    // emit stop is not necessary cause no start was emitted
    new_element->stopTimers();
    removeActivatedChild(new_element);
}

void TExcl::priorityDefer(BaseTimings *new_element)
{
    // pause new element and queue it
    // element->play in BodyParser::startElement will not reached so we collect the childs here
    emitPauseElementSignal(new_element);
    if (new_element->getBaseType() == "container")
        (qobject_cast<TContainer *> (new_element))->collectActivatedChilds();

    NewPriority->insertQueue(new_element);
}


