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
TExcl::TExcl(QObject *parent) : TContainer(parent)
{
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
        return;

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

void TExcl::prepareDurationTimers()
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

    if (interruptByEndSync(ended_element->getID()))
        return;


    if (areQueuesToProceed())
        return;

    if (ended_element->isBeginTimerActive())
    {
        startTimersOfAllActiveChilds();
        return;
    }
    else
        removeActiveChild(ended_element);

    if (ended_element == getCurrentActiveElement())
        setCurrentActivedElement(Q_NULLPTR);

    // do nothing when childs are active
    if (hasActivatedChild())
        return;

    // not stopped by parent
    if (status == _active)
        finishIntrinsicDuration(); // check for dur and then for repeat play
}

bool TExcl::interruptByEndSync(QString id)
{
    // Active DurTimer or EndTimer results in ignore endsync
    if ((endsync == "first" || endsync == id) && !isDurTimerActive() && !isEndTimerActive())
    {
        stopTimersOfAllActivatedChilds();
        removeAllActiveChilds();
        return true;
    }
    return false;
 }

void TExcl::start()
{
    if (PriorityClassList.size() == 0) // childs_list will always be 0 in excl
        return;

    // check if this is a restart attempt and check restart attribute
    if (!proceedStart())
    {
       return;
    }

    startTimersOfAllActiveChilds();
}

void TExcl::repeat()
{
    if (PriorityClassList.size() == 0) // childs_list will always be 0 in excl
        return;

    collectActivatableChilds();
    startTimersOfAllActiveChilds();
}

void TExcl::stop()
{
    handleTriggerStops();
    removeQueuedElements();
}

void TExcl::pause()
{
    status = _paused;
    pauseAllTimers();
    pauseTimersOfAllActivatedChilds();
}

void TExcl::resume()
{
    status = _active;
    resumeAllTimers();
    resumeTimersOfAllActivatedChilds();
}

void TExcl::collectActivatableChilds()
{
    CurrentPriority = NULL;
    NewPriority     = NULL;

    if (PriorityClassList.size() > 0)
    {
        QList<QDomElement> childs;
        QList<QDomElement>::iterator i;
        QMap<int, TPriorityClass *>::iterator it;
        QDomElement e;
        for (it =  PriorityClassList.begin(); it != PriorityClassList.end(); it++ )
        {
            CurrentPriority       = *it;
            childs                = CurrentPriority->getChildList();
            for (i = childs.begin(); i != childs.end(); i++)
            {
                e = *i;
                if (e.tagName() != "") // e.g. comments
                {
                    insertAsActiveChildFromDom(e);
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
        setCurrentActivedElement(new_element);
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
        setCurrentActivedElement(new_element);
        ret = true;
    }
    else if (attribute_value == "pause")  // pause current active element
    {
        priorityPause();
        setCurrentActivedElement(new_element);
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


void TExcl::setCurrentActivedElement(BaseTimings *element)
{
    current_activated_element = element;
}

BaseTimings *TExcl::getCurrentActiveElement()
{
    return current_activated_element;
}

bool TExcl::areQueuesToProceed()
{
    // if elements are in queues starts resume or starts them
    QMap<int, TPriorityClass *>::iterator      it;
    TPriorityClass                             *MyPriorityClass;
    for (it =  PriorityClassList.begin(); it != PriorityClassList.end(); it++ )
    {
        MyPriorityClass       = *it;
        if (MyPriorityClass->countQueue() > 0)
        {
            saveRemoveActivated(current_activated_element);
            CurrentPriority = MyPriorityClass; // important to change the now active Playlist
            current_activated_element = MyPriorityClass->getFromQueue();
            current_activated_element->resumeAllTimers();
            emitResumeElementSignal(current_activated_element);
            return true; // make sure do not move, until queue is not empty
        }
    }

    return false;
}

void TExcl::saveRemoveActivated(BaseTimings *element)
{
    if (!element->isBeginTimerActive())
    {
        removeActiveChild(element);
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
    // interrupt playing old element
    // send stop signal cause element could be shown
   saveRemoveActivated(current_activated_element);
   emitStopElementSignal(current_activated_element, true); // set true not to jump to next
}

void TExcl::priorityPause()
{
    // pause old element and queue it
    // emit because there can be a displayed media on screen
    emitPauseElementSignal(current_activated_element);
    NewPriority->insertQueue(current_activated_element);
}

void TExcl::priorityDefer(BaseTimings *new_element)
{
    if (new_element->getBaseType() == "container")
    {
        // as element has not the chance to reach element->start() collect the childs here
        TContainer *MyContainer = qobject_cast<TContainer *> (new_element);
        MyContainer->collectActivatableChilds();
        // start and stop all child timers, because pause will fail on not activated childs
        // a follow up resume will result an immediately dur-timeout
        MyContainer->deferTimersOfActivatedChilds();
    }
    // the element itself media has already started timers, so there is only need to pause here
    // as no media has started a emitPauseElementSignal is not necessary
    new_element->pause();

    NewPriority->insertQueue(new_element);
}

void TExcl::priorityNever(BaseTimings *new_element)
{
    // ignore new Element
    // emit stop is not necessary cause no start was emitted
    // do not stop or remove element, because it can be played later
    saveRemoveActivated(new_element);
}

