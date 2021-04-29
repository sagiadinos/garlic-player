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

ElementsContainer::ElementsContainer(HeadParser *hp, QObject *parent) : QObject(parent)
{
    MyHeadParser = hp;
    QList<Region> *layout = MyHeadParser->getLayout();
    for (int i = 0; i < layout->length(); i++)
    {
        regions.append(layout->at(i).regionName);
    }
}

ElementsContainer::~ElementsContainer()
{
    qDeleteAll(elements_list);
    elements_list.clear();

    // not neccesary do delete here cause same objects are in all_elements_list
    media_list.clear();
}

BaseTimings *ElementsContainer::findSmilElement(QDomElement dom_element)
{
    BaseTimings                             *MyBaseTimings     = Q_NULLPTR;
    QHash<QString, BaseTimings *>::iterator  elements_iterator = elements_list.find(TBase::parseID(dom_element));
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

void ElementsContainer::insertSmilMedia(BaseMedia *MyBaseMedia)
{
    if (!MyBaseMedia->isMedia())
    {
        return;
    }

    QString s = MyBaseMedia->getRegion();

    QMap<QString, QVector<BaseMedia *>*>::iterator i = media_list.find(s);
    if (i != media_list.end())
    {
        i.value()->append(MyBaseMedia);
    }
    else
    {
        QVector<BaseMedia *> *vec = new QVector<BaseMedia *>;
        vec->append(MyBaseMedia);
        media_list.insert(s, vec);
    }
}

BaseMedia *ElementsContainer::getMediaOnZoneAndPosition(int position, int zone)
{
    QMap<QString, QVector<BaseMedia *> *>::iterator i =  media_list.find(determineZoneName(zone));

    if (i != media_list.end())
    {
        return getMediaOnPosition(position, *i.value());
    }
    return Q_NULLPTR;
}

BaseMedia *ElementsContainer::getMediaOnPosition(int position, QVector<BaseMedia *> list)
{
    int array_position = position - 1;

    if (array_position < 0)
    {
        return list.first();
    }
    if (array_position > list.size())
    {
        return list.last();
    }
    return list.at(array_position);

}

QString ElementsContainer::determineZoneName(int zone)
{
    int array_position = zone - 1;
    if (array_position < 1)
    {
        return regions.first();
    }

    if (array_position > regions.size())
    {
        return regions.last();
    }
    // zone-1 cause arrays start as 0
    return regions.at(zone-1);
}


