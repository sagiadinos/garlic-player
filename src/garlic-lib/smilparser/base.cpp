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

#include "base.h"

TBase::TBase(QObject *parent) : QObject(parent)
{
}

QString TBase::parseID(QDomElement element)
{
    QString ret = "";
    if (element.hasAttribute("id"))
        ret = element.attribute("id");
    if (element.hasAttribute("xml:id")) // In SMIL 3.0 superset old SMIL 2.0 id.
        ret = element.attribute("xml:id");
    if (ret == "") // get line and column number as alternative when no
    {

        ret = element.tagName()+"_"+QString::number(element.lineNumber()) + "_" + QString::number(element.columnNumber());
    }
    return ret;
}

void TBase::setBaseAttributes()
{
    id = parseID(root_element);
    if (root_element.hasAttribute("title"))
        title = root_element.attribute("title");
    if (root_element.hasAttribute("class"))
        a_class = root_element.attribute("class");
    if (root_element.hasAttribute("xml:lang"))
        lang = root_element.attribute("xml:lang");
}
