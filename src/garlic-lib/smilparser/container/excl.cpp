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
TExcl::TExcl(TContainer *pc, QObject *parent) : TContainer(parent)
{
    parent_container       = pc;
    setParentTag(pc->getRootElement().nodeName());
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
    for (QHash<int, TPriorityClass *>::iterator i_priorities = priorities_list.begin(); i_priorities != priorities_list.end(); i_priorities++)
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
    if (startDurTimer() || isEndTimerActive() || priorities_list.size() > 0)
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
    QHash<int, TPriorityClass *>::iterator      ar_priorities_iterator;
    TPriorityClass                             *MyPriorityClass;
    for (ar_priorities_iterator =  priorities_list.begin(); ar_priorities_iterator != priorities_list.end(); ar_priorities_iterator++ )
    {
        MyPriorityClass       = *ar_priorities_iterator;
        if (MyPriorityClass->countQueue() > 0)
        {
            CurrentPriority = MyPriorityClass; // important to change the now active Playlist
            current_activated_element = MyPriorityClass->getFromQueue();
            current_activated_element->resumeTimers();
            // to make sure that this child is not removed previous

            // Sometimes a Null Pointer will got from queue. Currently do not know exactly why
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
    if (priorities_list.size() == 0) // childs_list will always be 0
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
    // check for repeat
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

    if (priorities_list.size() > 0)
    {
        QList<QDomElement> childs;
        QList<QDomElement>::iterator i;
        QHash<int, TPriorityClass *>::iterator it;
        for (it =  priorities_list.begin(); it != priorities_list.end(); it++ )
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

// children of excl => begin has default value indefinite

bool TExcl::determineContinue(BaseTimings *new_element)
{
    if (current_activated_element == Q_NULLPTR) // if first element we can continue to show
    {
        setCurrentActivatedElement(new_element);
        return true;
    }

    // get position from new element to current element
    // this determine which priority attribute should be used
    NewPriority = findPriorityClass(new_element->getRootElement());
    QString attribute_value = "";
    if (priorities_list.key(CurrentPriority) == priorities_list.key(NewPriority))      // same priorityClass is currently active => look at peers attribute:
    {
        attribute_value = CurrentPriority->getPeers();
    }
    else if (priorities_list.key(CurrentPriority) > priorities_list.key(NewPriority)) // new priority Class is higher then current active
    {
        attribute_value= CurrentPriority->getHigher();
    }
    else if (priorities_list.key(CurrentPriority) < priorities_list.key(NewPriority)) // new priority Class is lover then current active
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


TPriorityClass *TExcl::findPriorityClass(QDomElement dom_element)
{
    // found priorityClass of new checking element
    TPriorityClass *MyPriorityClass = NULL;
    QHash<int, TPriorityClass *>::iterator      ar_priorities_iterator;
    for (ar_priorities_iterator =  priorities_list.begin(); ar_priorities_iterator != priorities_list.end(); ar_priorities_iterator++)
    {
        MyPriorityClass = *ar_priorities_iterator;
        if (MyPriorityClass->findElement(dom_element))
            break;
    }
    return MyPriorityClass;
}

void TExcl::parsePriorityClass(QDomElement element)
{
    TPriorityClass * MyPriorityClass = new TPriorityClass(this);
    priorities_list.insert(priorities_list.size(), MyPriorityClass); // create a priority class object
    MyPriorityClass->preloadParse(element);
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
    CurrentPriority->insertPauseQueue(current_activated_element);
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

    NewPriority->insertDeferQueue(new_element);
}


