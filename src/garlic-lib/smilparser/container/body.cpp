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
    emit finishPreload();
}

void TBody::prepareTimingsBeforePlaying()
{
   prepareDurationTimerBeforePlay();
}

bool TBody::isChildPlayable(BaseTimings *element)
{
    childStarted(element);
    return true;
}

void TBody::prepareDurationTimerBeforePlay()
{
    emitFoundElement();
  //  emit startedContainer(parent_container, this);
    return;
}


void TBody::next(BaseTimings *ended_element)
{
    Q_UNUSED(ended_element);
    childs_list_iterator++; // inc iterator first only when inc result smaller than  .end()
    if (childs_list_iterator < childs_list.end())  // cause .end() pointing to the imaginary item after the last item in the vector
    {
        active_element = *childs_list_iterator;
        QString s = active_element.tagName();
        emitFoundElement();
    }
    else
        finishedActiveDuration();
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
            emit preloadElement(this, element);
        }
    }
    childs_list_iterator = childs_list.begin();
}
