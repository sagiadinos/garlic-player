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

TExcl::TExcl(QObject *parent)
{
    parent_playlist = parent;
    setObjectName("TExcl");
}


bool TExcl::parse(QDomElement element)
{
    actual_element = element; // must set to get inherited Attributed
    setBaseAttributes();
    if (element.hasChildNodes())
    {
        setPlaylist();
    }
    else
        return false;
    return true;
}

void TExcl::beginPlay()
{
    setTimedStart();
    return;
 }

void TExcl::play()
{

}


void TExcl::changeActiveElement(QObject *element)
{
    Q_UNUSED(element);
}




/**
 * @brief TExcl::next means to stop, pause, defer the active element
 *        and set another element to active.
 *
 * @return
 */
bool TExcl::next()
{
    return false;
}

void TExcl::setPlaylist()
{
    QDomNodeList childs = actual_element.childNodes();
    count_childs        = childs.length();
    QDomElement  element;
    for (int i = 0; i < count_childs; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() == "priorityClass")
            setPriorityClass(element);
        else // if there is no priorityClass childs will be grouped to a "virtually priorityClass"
        {
           setPriorityClass(actual_element);
           break; // mixing of elements with or no priorityClass are not allowed, cause it is a kind of nesting http://www.w3.org/TR/REC-smil/smil-timing.html#Timing-priorityClassElement
        }
    }
    return;
}

void TExcl::setPriorityClass(QDomElement element)
{
    TPriorityClass *MyPriorityClass = new TPriorityClass(this);
    if (MyPriorityClass->parse(element))
        ar_priorities.insert(MyPriorityClass);
    return;
}

