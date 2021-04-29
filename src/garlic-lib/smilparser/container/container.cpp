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

BaseTimings *TContainer::findDomChild(QDomElement dom_element)
{
    BaseTimings                             *MyBaseTimings     = Q_NULLPTR;
    QHash<QString, BaseTimings *>::iterator  elements_iterator = elements_list.find(TBase::parseID(dom_element));
    if (elements_iterator != elements_list.end())
    {
        MyBaseTimings = *elements_iterator;
    }
    return MyBaseTimings;
}

void TContainer::insertDomChild(BaseTimings *MyBaseTimings)
{
    elements_list.insert(MyBaseTimings->getID(), MyBaseTimings);
}

void TContainer::setCurrentActivatedElement(BaseTimings *element)
{
    current_activated_element = element;
}

BaseTimings *TContainer::getCurrentActivatedElement()
{
    return current_activated_element;
}

void TContainer::removeActivatedChild(BaseTimings *element)
{
    qDebug() << "remove: " + element->getID() + " parent: " + element->getParentContainer()->getID();
    if (activated_childs.indexOf(element) >= 0)
        activated_childs.removeOne(element);
}

void TContainer::startAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for ( BaseTimings *bt : qAsConst(activated_childs))
    {
        bt->preparePlaying();
    }
}

void TContainer::startFirstActivatedChild()
{
    if (activated_childs.size() == 0)
        return;

    QList<BaseTimings *>::iterator i = activated_childs.begin();
    BaseTimings *bt = *i;
    bt->preparePlaying();
}

void TContainer::stopAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for ( BaseTimings *bt : qAsConst(activated_childs))
    {
        if (bt->getStatus() == _playing || bt->getStatus() == _waiting)
        {
            bt->emitfinishedActiveDuration();
        }
    }
}

void TContainer::pauseAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for ( BaseTimings *bt : qAsConst(activated_childs))
    {
        if (bt->getStatus() == _playing || bt->getStatus() == _waiting)
        {
            bt->emitPause();
        }
    }
}

void TContainer::resumeAllActivatedChilds()
{
    if (activated_childs.size() == 0)
        return;

    for ( BaseTimings *bt : qAsConst(activated_childs))
    {
        if (bt->getStatus() == _paused)
        {
            bt->emitResume();
        }
    }
}

bool TContainer::hasActivatedChild()
{
    return (activated_childs.size() > 0);
}

void TContainer::emitfinishedActiveDuration() // slot called from EndTimer
{
    emitStopElementSignal(this);
}

void TContainer::emitPause()
{
    emitPauseElementSignal(this);
}

void TContainer::emitResume()
{
    emitResumeElementSignal(this);
}

void TContainer::activateFoundElement()
{
    insertActivatedChild(findDomChild(active_element));
}

void TContainer::insertActivatedChild(BaseTimings *MyBaseTimings)
{
    if (MyBaseTimings == Q_NULLPTR)
        return;

    qDebug() << "Parent: " + getID() << "activated: " +  MyBaseTimings->getID();

    if (activated_childs.indexOf(MyBaseTimings) < 0) // do not have double items
        activated_childs.append(MyBaseTimings);
}
