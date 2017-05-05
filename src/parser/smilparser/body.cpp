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

TBody::TBody(TContainer * parent)
{
    Q_UNUSED(parent);
    setObjectName("TBody");
}

TBody::~TBody()
{
    dom_list.clear();
}


bool TBody::parse(QDomElement element)
{
    bool ret       = false;
    root_element   = element;
    active_element = element;
    setTimingAttributes();
    id             = "body";  // useful for debug
    if (element.hasChildNodes())
    {
        active_element = element.firstChildElement();
        setPlaylist();
        iterator       = dom_list.begin();
        active_element = *iterator;
        ret = true;
    }
    else
       finishedActiveDuration();
    return ret;
}

void TBody::prepareTimerBeforePlaying()
{
   setDurationTimerBeforePlay();
}

bool TBody::isChildPlayable(TBaseTiming *element)
{
    qDebug(SmilParser) << element->getID() << "is ChildPlayable in body";
    childStarted(element);
    return true;
}



void TBody::setDurationTimerBeforePlay()
{
    reactByTag();
    emit startedContainer(parent_container, this);
    return;
}


void TBody::next(TBaseTiming *ended_element)
{
    Q_UNUSED(ended_element);
    iterator++; // inc iterator first only when inc result smaller than  .end()
    if (iterator < dom_list.end())  // cause .end() pointing to the imaginary item after the last item in the vector
    {
        active_element = *iterator;
        reactByTag();
    }
    else
        finishedActiveDuration();
}

void TBody::setPlaylist()
{
    QDomNodeList childs = active_element.parentNode().childNodes();
    int          length = childs.length();
    QDomElement  element;
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "")
            dom_list.append(element);
    }
}
