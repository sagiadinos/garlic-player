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

TContainer::TContainer(TBase *parent)
{
    Q_UNUSED(parent);
}

void TContainer::insertPlaylistObject(QString id, TBase *obj_element)
{
    ar_elements.insert(id, obj_element);
    return;
}

QString TContainer::getIdOfActiveElement()
{
    return parseID(active_element);
}

QHash<QString, TBase *> TContainer::getPlaylistObjects()
{
    return ar_elements;
}

TBase *TContainer::getPlayedElement()
{
    return played_element;
}

void TContainer::setPlayedElement(TBase *element)
{
    played_element = element;
    return;
}

void TContainer::childStarted(TBase *element)
{
    activatable_childs.insert(element);
    return;
}

void TContainer::childEnded(TBase *element)
{
    if (activatable_childs.find(element) != activatable_childs.end())
        activatable_childs.remove(element);
    return;
}

void TContainer::setChildActive(bool active)
{
    is_child_active = active;
    return;
}

bool TContainer::hasPlayingChilds()
{
    if (activatable_childs.size() > 0)
        return true;
    return false;
}

bool TContainer::isChildPlayable(TBase *element)
{
    childStarted(element);
    return true;
}

void TContainer::reactByTag()
{
    QString tag_name = active_element.tagName();
    if(tag_name == "img" || tag_name == "video" || tag_name == "audio" || tag_name == "text" || tag_name == "prefetch" ||
            tag_name == "seq"|| tag_name == "par" || tag_name == "excl")
    {
        emit foundElement(this, active_element);
    }
    else if(tag_name == "ref" && active_element.hasAttribute("type"))
    {
        if(active_element.attribute("type").contains("image") || active_element.attribute("type").contains("video") ||
                active_element.attribute("type").contains("audio") || active_element.attribute("type").contains("text"))
        {
            emit foundElement(this, active_element);
        }
    }
    return;
}

void TContainer::emitfinished() // called from finishedActiveDuration() TBase
{
    qDebug() << getID() << "finished Playlist";
    emit finishedPlaylist(parent_container, this);
    return;
}

