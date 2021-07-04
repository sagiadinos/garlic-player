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
#include "priorityclass.h"

TPriorityClass::TPriorityClass(QObject *parent) : TBase(parent)
{
    setObjectName("TPriorityClass");
}

TPriorityClass::~TPriorityClass()
{
}

void TPriorityClass::preloadParse(QDomElement element)
{
    root_element   = element;
    setAttributes();
    traverseChilds();
}

QString TPriorityClass::getPeers()
{
    return peers;
}

QString TPriorityClass::getHigher()
{
    return higher;
}

QString TPriorityClass::getLower()
{
    return lower;
}

bool TPriorityClass::findElement(QDomElement dom_element)
{
    for (iterator =  childs_list.begin(); iterator < childs_list.end(); iterator++)
    {
        if (dom_element == *iterator)
            return true;
    }
    return false;
}

void TPriorityClass::insertQueue(BaseTimings *element)
{
    queue_stack.push(element);
}

void TPriorityClass::removeQueuedElements()
{
    for (BaseTimings *bt : qAsConst(queue_stack))
    {
       bt->stopTimers();
    }
}

int TPriorityClass::countQueue()
{
    if (queue_stack.size() > 0)
        return queue_stack.size();
    return 0;
}

BaseTimings *TPriorityClass::getFromQueue()
{
    if (queue_stack.size() > 0)
        return queue_stack.pop();

    return Q_NULLPTR;
}


QList<QDomElement> TPriorityClass::getChildList()
{
    return childs_list;
}

void TPriorityClass::setAttributes()
{
    setBaseAttributes();

    peers  = validatePeers();
    higher = validateHigher();
    lower  = validateLower();
}

void TPriorityClass::traverseChilds()
{
    QDomNodeList childs = root_element.childNodes();
    count_childs        = childs.length();
    QDomElement  element;
    for (int i = 0; i < count_childs; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "priorityClass" && element.tagName() != "")
        {
            childs_list.append(element);
        }
    }
}

QString TPriorityClass::validatePeers()
{
    QString attr  = getAttributeFromRootElement("peers", "stop");
    if (attr != "stop" && attr != "pause" && attr != "defer" && attr != "never")
        attr = "stop";

    return attr;
}

QString TPriorityClass::validateHigher()
{
    QString attr = getAttributeFromRootElement("higher", "pause");
    if (attr != "stop" && attr != "pause")
        attr = "pause";

    return attr;
}

QString TPriorityClass::validateLower()
{
    QString attr  = getAttributeFromRootElement("lower", "defer");
    if (attr != "defer" && attr != "never")
        attr = "defer";

    return attr;
}
