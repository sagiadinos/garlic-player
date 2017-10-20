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
#include "subscription.h"

SubScription::SubScription(QObject *parent) : QObject(parent)
{
    setRefreshInterval(3600);
    setRefreshInterval(3600);
}

void SubScription::parse(QDomElement element)
{
    QDomNodeList childs = element.childNodes();
    for (int i = 0; i < childs.length(); i++)
    {
        element = childs.item(i).toElement();
        QString tag_name = element.tagName();
        if (element.tagName() == "type")
            setType(element.text());
        else if (element.tagName() == "action")
            setAction(element.text());
        else if (element.tagName() == "method")
            setMethod(element.text());
        else if (element.tagName() == "refreshInterval")
            setRefreshInterval(element.text().toInt());
        else if (element.tagName() =="retryInterval")
            setRefreshInterval(element.text().toInt());
    }
}

QString SubScription::getType() const
{
    return type;
}

void SubScription::setType(const QString &value)
{
    type = value;
}

QString SubScription::getAction() const
{
    return action;
}

void SubScription::setAction(const QString &value)
{
    action = value;
}

QString SubScription::getMethod() const
{
    return method;
}

void SubScription::setMethod(const QString &value)
{
    method = value;
}

int SubScription::getRefreshInterval() const
{
    return refresh_interval;
}

void SubScription::setRefreshInterval(int value)
{
    refresh_interval = value;
}

int SubScription::getRetryInterval() const
{
    return retry_interval;
}

void SubScription::setRetryInterval(int value)
{
    retry_interval = value;
}
