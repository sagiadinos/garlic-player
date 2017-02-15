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
#include "prefetch.h"

TPrefetch::TPrefetch(TContainer *parent)
{
    parent_container = parent;
    initTimer();
    setObjectName("TPrefetch");
}

void TPrefetch::pause()
{
    qDebug() << getID() << QTime::currentTime().toString() << "paused ";
    status = _paused;
    return;
}

void TPrefetch::stop()
{
    qDebug() << getID() << QTime::currentTime().toString() << "stopped";
    status = _stopped;
    return;
}

void TPrefetch::play()
{
    qDebug() << getID() << QTime::currentTime().toString()  << "prefetch play (load)";
    status = _playing;
    return;
}

bool TPrefetch::loadMedia()
{
    return true;
}

void TPrefetch::setDurationTimerBeforePlay()
{
    setInternalDefaultDur();
}

void TPrefetch::setAttributes()
{
    setBaseMediaAttributes();
    return;
}
