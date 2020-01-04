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


QString TContainer::getIdOfActiveElement()
{
    return parseID(active_element);
}

BaseTimings *TContainer::getChildElementFromList()
{
    BaseTimings *ret = NULL;
    if (childs_iterator != activatable_childs.end())
    {
        ret = *childs_iterator;
        childs_iterator++;
    }
    else
    {
        childs_iterator = activatable_childs.begin();
        ret             = *childs_iterator;
    }
    return ret;
}

void TContainer::setPlayedElement(BaseTimings *element)
{
    played_element = element;
    return;
}

BaseTimings *TContainer::getPlayedElement()
{
    return played_element;
}


void TContainer::childStarted(BaseTimings *element)
{
    activatable_childs.insert(element);
    childs_iterator = activatable_childs.begin();
}

void TContainer::childEnded(BaseTimings *element)
{
    if (activatable_childs.find(element) != activatable_childs.end())
    {
        activatable_childs.remove(element);
    }
}

void TContainer::setChildActive(bool active)
{
    is_child_active = active;
}

void TContainer::emitPreLoad()
{
    emit preloadElement(this, active_element);
}

bool TContainer::hasPlayingChilds()
{
    if (activatable_childs.size() > 0)
        return true;
    return false;
}

void TContainer::emitFoundElement()
{
    emit foundElement(this, active_element);
    return;
}

void TContainer::emitfinished() // called from finishedActiveDuration() TBase
{
    qDebug(Develop) << getID() << "finished container tag";
    emit finishedContainer(parent_container, this);
}

