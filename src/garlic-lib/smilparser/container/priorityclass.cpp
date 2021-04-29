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
    setParentTag("excl");
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

bool TPriorityClass::findElement(QDomElement dom_element)
{
    for (iterator =  childs_list.begin(); iterator < childs_list.end(); iterator++)
    {
        if (dom_element == *iterator)
            return true;
    }
    return false;
}

void TPriorityClass::insertDeferQueue(BaseTimings *element)
{
    ar_defer.push(element);
}

void TPriorityClass::insertPauseQueue(BaseTimings *element)
{
    ar_pause.push(element);
}

int TPriorityClass::countQueue()
{
    if (ar_defer.size() > 0 && (peers == "defer" || lower == "defer"))
        return ar_defer.size();

    if (ar_pause.size() > 0 && (peers == "pause" || lower == "pause"))
        return ar_pause.size();
    return 0;
}

BaseTimings *TPriorityClass::getFromQueue()
{
    if (peers == "defer" || lower == "defer")
    {
        if (ar_defer.size() > 0)
            return ar_defer.pop();
    }
    if (peers == "pause" || lower == "pause")
    {
        if (ar_pause.size() > 0)
            return ar_pause.pop();
    }
    return Q_NULLPTR;
}


QList<QDomElement> TPriorityClass::getChildList()
{
    return childs_list;
}

void TPriorityClass::setAttributes()
{
    setBaseAttributes();
    peers  = getAttributeFromRootElement("peers", "stop");
    higher = getAttributeFromRootElement("higher", "pause");
    lower  = getAttributeFromRootElement("lower", "defer");
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
