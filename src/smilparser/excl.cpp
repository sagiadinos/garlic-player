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

#include "excl.h"

TExcl::TExcl(QObject *parent)
{
    Q_UNUSED(parent);
    setObjectName("TExcl");
}


bool TExcl::parse(QDomElement element)
{
    actual_element = element; // must set to get inherited Attributed
    setBaseAttributes();
    if (element.hasChildNodes())
    {
        setPlaylist();
        QDomNodeList childs = element.childNodes();
        int count_childs    = childs.length();
        for (int i = 0; i < count_childs; i++)
        {
            // ToDo: Determine which of the Child elements shoud be active
            actual_element = childs.item(i).toElement();
        }

    }
    return false;
}

void TExcl::beginPlay()
{
    // ToDo: get Info about begin from TTiming.
    reactByTag();
}

void TExcl::play()
{

}


/**
 * @brief TExcl::next means to stop, pause, defer the active element
 *        and set another element to active.
 *
 * @return
 */
bool TExcl::next()
{
//    if (active_childs == 0)
//    {
//        if(checkRepeatCountStatus())
//        {
//            for (int i = 0; i < count_childs; i++)
//            {
//                actual_element = childs.item(i).toElement();
//                reactByTag();
//            }
//        }
//        else
//        {
//            // ToDo: get Info about end from TTiming. may be is it necessary not to end the playlist.
//            end();
//        }
//    }
    return false;
}

void TExcl::setPlaylist()
{
    // put all playlist elements into a QList
    QDomNodeList childs = actual_element.childNodes();
    count_childs        = childs.length();
    for (int i = 0; i < count_childs; i++)
    {
        ar_playlist.append(childs.item(i).toElement());
    }
}
