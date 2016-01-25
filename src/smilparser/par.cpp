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

#include "par.h"

TPar::TPar(QObject *parent)
{
    initTimer();
    parent_playlist = parent;
    setObjectName("TPar");
}

bool TPar::parse(QDomElement element)
{
    root_element   = element;
    setBaseAttributes();
    if (element.hasChildNodes())
    {
        setPlaylist();
    }
    return false;
}

void TPar::beginPlay()
{
    setBeginEndTimer();
    return;
}

void TPar::setDurationTimerBeforePlay()
{
    if (hasDurAttribute() || end_timer->isActive() || ar_playlist.size() > 0)
    {
        resetInternalRepeatCount();
        play();
        status = _playing;
        emit startedPlaylist(parent_playlist, this);
    }
    else // when end or duration is not specified or no child elements stop imediately
        emitfinished();
    return;
}

/**
 * @brief TPar::next means that it looks if there are no active elements
 *        when nothing is active (anymore) and no repeat possible
 *        then the finished signal will emitted;
 * @return
 */
void TPar::next()
{
    if (active_childs == 0)
    {
        if(checkRepeatCountStatus())
        {
            play();
        }
        else
        {
            emitfinished();
        }
    }
    return;
}

void TPar::play()
{
    for (iterator =  ar_playlist.begin(); iterator < ar_playlist.end(); iterator++)
    {
        active_element = *iterator;
        reactByTag();
    }
    status = _playing;
    return;
}

void TPar::pause()
{
    status = _paused;
    return;
}

void TPar::stop()
{
    status = _stopped;
    return;
}


void TPar::setPlaylist()
{
    // put all playlist elements into a QList
    QDomNodeList childs = root_element.childNodes();
    count_childs        = childs.length();
    for (int i = 0; i < count_childs; i++)
    {
        if (childs.item(i).toElement().tagName() != "") // e.g. comments
            ar_playlist.append(childs.item(i).toElement());
    }
}


void TPar::incActiveChilds()
{
    active_childs++;
}

void TPar::decActiveChilds()
{
    active_childs--;
    return;
}


// next means here that it should be check only if playliste can be started from begin
