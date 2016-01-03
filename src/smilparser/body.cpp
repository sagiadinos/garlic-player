/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-.control.com>
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

TBody::TBody(QObject * parent)
{
    Q_UNUSED(parent);
    setObjectName("TBody");
}

bool TBody::parse(QDomElement element)
{
    bool ret = false;
    if (element.hasChildNodes())
    {
         actual_element = element.firstChildElement();
         if (!actual_element.isNull())
         {
             reactByTag();
             ret = true;
         }
    }
    return ret;
}

bool TBody::next()
{
    QDomElement next = actual_element.nextSiblingElement();
    if (next.isNull())
        return false;
    actual_element = next.toElement();
    reactByTag();
    return true;
}

bool TBody::previous()
{
    QDomElement next = actual_element.previousSiblingElement();
    if (next.isNull())
        return false;
    actual_element = next;
    reactByTag();
    return true;
}
