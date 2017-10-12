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

#include "audio.h"

TAudio::TAudio(TContainer *pc, QObject *parent)  : TMedia(parent)
{
    parent_container = pc;
    initTimer();
    setObjectName("TAudio");
}

TAudio::~TAudio()
{
}

void TAudio::setDurationTimerBeforePlay()
{
    if (isDownloaded())
    {
        hasDurAttribute(); // it doesn' matter if there is an dur set cause videos/audio have an own media duration
        if (!is_resumed)
            emit startedMedia(parent_container, this);
    }
    else // set a duration otherwise it runs in a recursion stack overflow when download is not complete
        setInternalDefaultDur();
}

// ====================  protected methods =================================
void TAudio::play()
{
    status = _playing;
}

void TAudio::stop()
{
    status        = _stopped;
}


void TAudio::pause()
{
    status = _playing;
}


void TAudio::setAttributes()
{
    setBaseMediaAttributes();
    if (root_element.hasAttribute("soundLevel"))
        soundLevel = root_element.attribute("soundLevel");
}

// ====================  private methods =================================


