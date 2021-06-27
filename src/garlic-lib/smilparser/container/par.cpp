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

#include "par.h"

TPar::TPar(TContainer *pc, QObject *parent) : TContainer(parent)
{
    parent_container = pc;
    setParentTag(pc->getRootElement().nodeName());
    setObjectName("TPar");
}

TPar::~TPar()
{
}

void TPar::preloadParse(QDomElement element)
{
    root_element   = element;
    parseTimingAttributes();

    if (element.hasAttribute("endsync"))
        endsync = element.attribute("endsync");

    if (element.hasChildNodes())
    {
        traverseChilds();
    }
}

void TPar::prepareDurationTimerBeforePlay()
{
    // when a durtimer exists use it!
    if (hasDurTimer() && !startDurTimer() && !isEndTimerActive())
    {
        skipElement();
        return;
    }

    if (childs_list.size() > 0)
    {
        resetInternalRepeatCount();
        emitStartElementSignal(this);
    }
    else
    {
        skipElement();
    }
}

/**
 * @brief TPar::next means that it looks if there are no active elements
 *        when nothing is active (anymore) and no repeat possible
 *        then the finished signal will emitted;
 * @return
 */
void TPar::next(BaseTimings *ended_element)
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

    // do nothing when childs are active endsyc
    if (hasActivatedChild())
        return;

    // not stopped by parent
    if (status == _playing)
    {
        finishIntrinsicDuration(); // check for dur and then for repeat play
        return;
    }

}

void TPar::start()
{
    if (childs_list.size() == 0)
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
    status       = _playing;
    startTimersOfAllActivatedChilds();
}

void TPar::stop()
{
    status = _stopped;
    stopTimers();               // because there can be a dur or begin timer active
    stopTimersOfAllActivatedChilds();
    emitStopToAllActivatedChilds();
    removeAllActivatedChilds();
}

void TPar::interruptByEndSync()
{
    stop();
    if (handleRepeatCountStatus())
        start();
}

void TPar::resume()
{
    status = _playing;
}

void TPar::pause()
{
    status = _paused;
}

void TPar::collectActivatedChilds()
{
    for (childs_list_iterator = childs_list.begin(); childs_list_iterator < childs_list.end(); childs_list_iterator++)
    {
        active_element = *childs_list_iterator;
        activateFoundElement();
    }
}

void TPar::traverseChilds()
{
    // put all playlist elements into a QList
    QDomNodeList childs = root_element.childNodes();
    QString tag_name = "";
    for (int i = 0; i < childs.length(); i++)
    {
        tag_name = childs.item(i).toElement().tagName();
        if (tag_name != "") // e.g. comments
        {
            childs_list.append(childs.item(i).toElement());
            emit preloadElementSignal(this, childs.item(i).toElement());
        }
    }
}

