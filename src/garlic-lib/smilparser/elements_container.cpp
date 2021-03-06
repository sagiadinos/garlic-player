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
#include "elements_container.h"

ElementsContainer::ElementsContainer(QObject *parent) : QObject(parent)
{

}

ElementsContainer::~ElementsContainer()
{
    qDeleteAll(elements_list);
    elements_list.clear();
}

BaseTimings *ElementsContainer::findSmilElementByDom(QDomElement dom_element)
{
    return findSmilElementById(TBase::parseID(dom_element));
}

BaseTimings *ElementsContainer::findSmilElementById(QString id)
{
    BaseTimings                             *MyBaseTimings     = Q_NULLPTR;
    QHash<QString, BaseTimings *>::iterator  elements_iterator = elements_list.find(id);
    if (elements_iterator != elements_list.end())
    {
        MyBaseTimings = *elements_iterator;
    }
    return MyBaseTimings;
}


void ElementsContainer::insertSmilElement(BaseTimings *MyBaseTimings)
{
    elements_list.insert(MyBaseTimings->getID(), MyBaseTimings);
}

void ElementsContainer::distributeTriggers()
{
    QHash<QString, BaseTimings *>::Iterator i = elements_list.begin();
    BaseTimings *bt_target = Q_NULLPTR;
    // traverse elements_container to find possible trigger set in begin or end values
    // then find the trigger source ids and set them which element they must informed when a trigger point fired
    //for example
    // target begin = source.end or source.begin
    // target end   = source.end or source.begin
    while (i != elements_list.end())
    {
        bt_target = i.value();
        // fetch all target id for their begin triggers
        distributeBeginTrigger(bt_target);

        // fetch all target id for their end triggers
        distributeEndTrigger(bt_target);
        i++;
    }
}

BaseTimings *ElementsContainer::findAccessKeysForBegin(QChar key)
{
    QMap<QChar, BaseTimings *>::Iterator i = begin_accesskey_list.find(key);
    if(i != begin_accesskey_list.end())
    {
        return  i.value();
    }
    return Q_NULLPTR;
}

BaseTimings *ElementsContainer::findAccessKeysForEnd(QChar key)
{
    QMap<QChar, BaseTimings *>::Iterator i = end_accesskey_list.find(key);
    if(i != end_accesskey_list.end())
    {
        return i.value();
    }

    return Q_NULLPTR;
}

void ElementsContainer::distributeBeginTrigger(BaseTimings *bt_target)
{
    QHash<QString, QString>  target_trigger_list = bt_target->fetchExternalBegins();
    QString                                           target_id = bt_target->getID();
    if (!target_trigger_list.isEmpty())
    {
        BaseTimings *bt_source = Q_NULLPTR;
        QHash<QString, QString> ::Iterator j = target_trigger_list.begin();
        while (j != target_trigger_list.end())
        {
            if (j.key() == "accesskey")
            {
                begin_accesskey_list.insert(j.value().at(0), bt_target);
            }
            else
            {
                QHash<QString, BaseTimings *> ::Iterator test;
                test = elements_list.find(j.key());
                if (test != elements_list.end())
                {
                    bt_source = test.value();
                    bt_source->addToExternalBegins(j.value(),target_id);
                }
            }
            j++;
        }
    }
}

void ElementsContainer::distributeEndTrigger(BaseTimings *bt_target)
{
    QHash<QString, QString> target_trigger_list = bt_target->fetchExternalEnds();
    QString                                          target_id = bt_target->getID();
    if (!target_trigger_list.isEmpty())
    {
        BaseTimings *bt_source = Q_NULLPTR;
        QHash<QString, QString> ::Iterator j = target_trigger_list.begin();
        while (j != target_trigger_list.end())
        {
            if (j.key() == "accesskey")
            {
                end_accesskey_list.insert(j.value().at(0), bt_target);
            }
            else
            {
                QHash<QString, BaseTimings *> ::Iterator test;
                test = elements_list.find(j.key());
                if (test != elements_list.end())
                {
                    bt_source = test.value();
                    bt_source->addToExternalEnds(j.value(),target_id);
                }
            }
            j++;
        }
    }
}
