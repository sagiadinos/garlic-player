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

#include "body.h"

TBody::TBody(QObject *parent) : TContainer(parent)
{
    parent_container       = Q_NULLPTR;
    setParentTag("seq");
    setObjectName("TBody");
}

TBody::~TBody()
{
    childs_list.clear();
}

void TBody::preloadParse(QDomElement element)
{
    root_element   = element;
    parseTimingAttributes();
    id             = "body";  // useful for debug
    if (root_element.hasChildNodes())
    {
        active_element   = root_element.firstChildElement();
        traverseChilds();
    }
    else
    {
        active_element = root_element;
        finishedActiveDuration();
    }
    emit finishPreloadSignal   ();
}

void TBody::startTimers()
{
   prepareDurationTimerBeforePlay(); // currently there is no timing in body tag
}

void TBody::prepareDurationTimerBeforePlay()
{
    if (childs_list.size() > 0)
    {
        resetInternalRepeatCount();
        emitStartElementSignal(this);
    }
    else
    {
        skipElement();
    }
    return;
}

void TBody::next(BaseTimings *ended_element)
{
    removeActivatedChild(ended_element);

    if (hasActivatedChild())
    {
        startTimersOfFirstActivatedChild();
        return;
    }

    finishedActiveDuration();
}


void TBody::start()
{
    collectActivatedChilds();
    status = _playing;
    startTimersOfFirstActivatedChild();
}

void TBody::resume()
{
    status = _playing;
}

void TBody::pause()
{
    status = _paused;
}

void TBody::stop()
{
    status = _stopped;
}

void TBody::interruptByEndSync()
{
    status = _stopped;
}

void TBody::collectActivatedChilds()
{
    for (childs_list_iterator = childs_list.begin(); childs_list_iterator < childs_list.end(); childs_list_iterator++)
    {
        active_element = *childs_list_iterator;
        activateFoundElement();
    }
}

void TBody::traverseChilds()
{
    QDomNodeList childs = active_element.parentNode().childNodes();
    int          length = childs.length();
    QDomElement  element;
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "")
        {
            childs_list.append(element);
            emit preloadElementSignal(this, element);
        }
    }
    childs_list_iterator = childs_list.begin();
}

void TBody::finishedSimpleDuration()
{
    //dummy cause body cannot have duration etc.
}
