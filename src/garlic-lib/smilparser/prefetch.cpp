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

TPrefetch::TPrefetch(TContainer *pc, QObject *parent) : TMedia(parent)
{
    parent_container = pc;
    initTimer();
    setObjectName("TPrefetch");
}

TPrefetch::~TPrefetch()
{
    deleteTimer();
}

void TPrefetch::pause()
{
    qDebug() << getID() << "paused ";
    status = _paused;
}

void TPrefetch::stop()
{
    qDebug() << getID() << "stopped";
    status = _stopped;
}

void TPrefetch::play()
{
    qDebug() << getID()  << "prefetch play";
    status = _playing;
}

void TPrefetch::setDurationTimerBeforePlay()
{
    MyMediaManager->registerFile(src); // register file again to start check if changed";
    setInternalDefaultDur();
}

void TPrefetch::setAttributes()
{
    setBaseMediaAttributes();
}
