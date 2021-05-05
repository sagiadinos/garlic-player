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

void TBase::setParentTag(QString tag)
{
    if (tag == "seq")
        parent_tag = SMIL_TAG::seq;
    else if (tag == "par")
        parent_tag = SMIL_TAG::par;
    else if (tag == "excl")
        parent_tag = SMIL_TAG::excl;
    else if (tag == "body")
        parent_tag = SMIL_TAG::seq;
    else
        parent_tag = SMIL_TAG::unknown;
}

SMIL_TAG TBase::getParentTag()
{
   return parent_tag;
}

QString TBase::parseID(QDomElement element)
{
    QString ret = "";
    if (element.hasAttribute("id"))
        ret = element.attribute("id");
    if (element.hasAttribute("xml:id")) // In SMIL 3.0 superset old SMIL 2.0 id.
        ret = element.attribute("xml:id");
    if (ret == "") // get line and column number as alternative id when no one is set in SMIL
    {
        ret = element.tagName()+"_"+QString::number(element.lineNumber()) + "_" + QString::number(element.columnNumber());
    }
    return ret;
}

void TBase::setBaseAttributes()
{
    id       = parseID(root_element);
    title    = getAttributeFromRootElement("title", "");
    a_class  = getAttributeFromRootElement("class", "");
    lang     = getAttributeFromRootElement("xml:lang", "");
    longdesc = getAttributeFromRootElement("longdesc", "");
    alt      = getAttributeFromRootElement("alt", "");
}

QString TBase::getAttributeFromRootElement(const QString attribute_name, const QString default_value)
{
    QString ret = default_value;
    if (root_element.hasAttribute(attribute_name))
    {
        ret = root_element.attribute(attribute_name);
    }
    return ret;
}

