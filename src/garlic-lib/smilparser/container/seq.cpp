/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2021 Nikolaos Sagiadinos <ns@smil-control.com>
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

#include "seq.h"

TSeq::TSeq(TContainer *pc, QObject *parent) : TContainer(parent)
{
    parent_container = pc;
    setParentTag(pc->getRootElement().nodeName());
    setObjectName("TSeq");
}

TSeq::~TSeq()
{
}

void TSeq::preloadParse(QDomElement element)
{
    root_element   = element;
    parseTimingAttributes();
    if (root_element.hasChildNodes())
    {
        active_element = root_element.firstChildElement();
        traverseChilds();
        if (active_element.tagName() == "metadata")
        {
            MyShuffle = new TShuffle(childs_list, this);
            MyShuffle->parse(active_element);
        }
    }
}

void TSeq::next(BaseTimings *ended_element)
{
    if (status == _stopped)
        return;

    removeActivatedChild(ended_element);

    if (hasActivatedChild())
    {
        startTimersOfFirstActivatedChild();
        return;
    }

    // not stopped by parent
    if (status == _playing)
        finishIntrinsicDuration(); // check for dur and the for repeat play
}

void TSeq::start()
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
    status = _playing;
    startTimersOfFirstActivatedChild();
}

void TSeq::stop()
{
    status = _stopped;
    stopTimers();               // because there can be a dur or begin timer active
    stopTimersOfAllActivatedChilds();
    emitStopToAllActivatedChilds();
    removeAllActivatedChilds();
}

void TSeq::interruptByEndSync()
{
    stop();
}

void TSeq::pause()
{
    status = _paused;
}

void TSeq::resume()
{
    status = _playing;
}

void TSeq::collectActivatedChilds()
{
    QList<QDomElement>            list;
    QList<QDomElement>::iterator  iterator;

    if (MyShuffle == Q_NULLPTR)
        list = childs_list;
    else
        list = MyShuffle->getShuffeledList();

    for (iterator = list.begin(); iterator < list.end(); iterator++)
    {
        active_element = *iterator;
        activateFoundElement();
    }
}

void TSeq::prepareDurationTimerBeforePlay()
{
    if (startDurTimer() || isEndTimerActive() || childs_list.size() > 0)
    {
        resetInternalRepeatCount();
        emitStartElementSignal(this);
    }
    else
    {
        skipElement();
    }
}

void TSeq::traverseChilds()
{
    QDomNodeList childs = root_element.childNodes();
    QDomElement  element;
    for (int i = 0; i < childs.length(); i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "metadata" && element.tagName() != "") // e.g. comments
        {
            childs_list.append(element);
            emit preloadElementSignal(this, element);
        }
    }
    childs_list_iterator = childs_list.begin();
}
