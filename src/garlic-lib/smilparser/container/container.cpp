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

#include "container.h"

TContainer::TContainer(QObject *parent) : BaseTimings(parent)
{
}

BaseTimings *TContainer::findChildByDom(QDomElement dom_element)
{
    BaseTimings                             *MyBaseTimings     = Q_NULLPTR;
    QHash<QString, BaseTimings *>::iterator  elements_iterator = childs_list.find(TBase::parseID(dom_element));
    if (elements_iterator != childs_list.end())
    {
        MyBaseTimings = *elements_iterator;
    }
    return MyBaseTimings;
}

void TContainer::appendChild(BaseTimings *MyBaseTimings)
{
    childs_list.insert(MyBaseTimings->getID(), MyBaseTimings);
}

void TContainer::interruptByRestart()
{
    emitStopToAllActivatedChilds();
    // maybe useless when emitted stop
      //  stopTimersOfAllActivatedChilds(); // must be after emit stop
    removeAllActiveChilds();
}

void TContainer::removeActiveChild(BaseTimings *element)
{
    qDebug() << "remove: " + element->getID() + " parent: " + element->getParentContainer()->getID();
    if (activated_childs.indexOf(element) >= 0)
        activated_childs.removeOne(element);
}

void TContainer::removeAllActiveChilds()
{
    activated_childs.clear();
}

void TContainer::startTimersOfFirstActivatedChild()
{
    status = _active;
    if (activated_childs.size() == 0)
        return;

    BaseTimings *bt = activated_childs.first();
    bt->activateTriggerTimers();
}

void TContainer::pauseTimersOfFirstActivatedChild()
{
    if (activated_childs.size() == 0)
        return;

    BaseTimings *bt = activated_childs.first();
    bt->pauseAllTimers();
}

void TContainer::resumeTimersOfFirstActivatedChild()
{
    status = _active;
    if (activated_childs.size() == 0)
        return;

    BaseTimings *bt = activated_childs.first();
    bt->resumeAllTimers();
}

void TContainer::startTimersOfAllActiveChilds()
{
    status      = _active;
    if (activated_childs.size() == 0)
        return;

    for ( BaseTimings *bt : qAsConst(activated_childs))
    {
        if (bt->getStatus() == _stopped)
            bt->activateTriggerTimers();
    }
}

void TContainer::stopTimersOfAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for (BaseTimings *bt : qAsConst(activated_childs))
    {
        bt->stopAllTimers();
    }
}

void TContainer::pauseTimersOfAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for (BaseTimings *bt : qAsConst(activated_childs))
    {
        bt->pauseAllTimers();
    }
}

void TContainer::resumeTimersOfAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for (BaseTimings *bt : qAsConst(activated_childs))
    {
        bt->resumeAllTimers();
    }
}

void TContainer::deferTimersOfActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

// ToDo: Think about a more elegant solution to this and the first/all shit.
// Mybe excl and par should inherited from a middle Object

    if (objectName() == "TSeq")
    {
        BaseTimings *bt = activated_childs.first();
        bt->deferAllTimers();
    }
    else
    {
        for ( BaseTimings *bt : qAsConst(activated_childs))
        {
            if (bt->getStatus() == _stopped)
                bt->deferAllTimers();
        }
    }
}

void TContainer::emitStopToAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for (BaseTimings *bt : qAsConst(activated_childs))
    {
        if (bt->getStatus() == _active || bt->getStatus() == _pending)
        {
            bt->emitfinishedElement();
        }
    }
}

void TContainer::emitPauseToAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for (BaseTimings *bt : qAsConst(activated_childs))
    {
        if (bt->getStatus() == _active || bt->getStatus() == _pending)
        {
            bt->emitPause();
        }
    }
}

void TContainer::emitResumeToAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for (BaseTimings *bt : qAsConst(activated_childs))
    {
        if (bt->getStatus() == _paused)
        {
            bt->emitResume();
        }
    }
}

bool TContainer::hasActivatedChild()
{
    return (activated_childs.size() > 0);
}

void TContainer::emitfinishedElement() // slot called from EndTimer
{
    emitStopElementSignal(this, false);
}

void TContainer::handleTriggerStops()
{
    // if a repeated timer trigger like begin-end wallclock or a begin-end list perform a check
    selectWhichTimerShouldStop();
    emitStopToAllActivatedChilds();

    // commented maybe useless when emitted stop
    //  stopTimersOfAllActivatedChilds(); // must be after emit stop
}

bool TContainer::proceedStart()
{
    if (has_started)
    {
        // check if this is a restart attempt and check restart attribute
        if (isRestartable())
        {
            interruptByRestart();
        }
        else
        {
            return false;
        }
    }
    collectActivatableChilds();
    has_started = true;

    return true;
}

void TContainer::emitPause()
{
    emitPauseElementSignal(this);
}

void TContainer::emitResume()
{
    emitResumeElementSignal(this);
}

void TContainer::insertAsActiveChildFromDom(QDomElement e)
{
    insertActivatedChild(findChildByDom(e));
}

void TContainer::insertActivatedChild(BaseTimings *MyBaseTimings)
{
    if (MyBaseTimings == Q_NULLPTR)
        return;

    qDebug() << "Parent: " + getID() << "insert activated: " +  MyBaseTimings->getID();
    if (activated_childs.indexOf(MyBaseTimings) < 0) // do not have double items
        activated_childs.append(MyBaseTimings);
}
