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

#include "body.h"

TBody::TBody(QObject * parent)
{
    Q_UNUSED(parent);
    setObjectName("TBody");
}

bool TBody::parse(QDomElement element)
{
    actual_element = element; // must set to get inherited Attributed
    setBaseAttributes();
    if (element.hasChildNodes())
    {
        actual_element = element.firstChildElement();
        setPlaylist();
        iterator       = ar_playlist.begin();
        actual_element = *iterator;
    }
    else
        return false;
    return true;
}

void TBody::beginPlay()
{
   play();
   return;
}

void TBody::play()
{
    reactByTag();
    emit started(parent_playlist, this);
    return;
}


bool TBody::next()
{
    bool ret = false;
    iterator++; // inc iterator first only when inc result smaller than  .end()
    if (iterator < ar_playlist.end())  // cause .end() pointing to the imaginary item after the last item in the vector
    {
        actual_element = *iterator;
        ret            = true;
        reactByTag();
    }
    return ret;
}

bool TBody::previous()
{
     return true;
}

void TBody::setPlaylist()
{
    QDomNodeList childs = actual_element.parentNode().childNodes();
    int          length = childs.length();
    QDomElement  element;
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        ar_playlist.append(element);
    }
    return;
}
