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

TAudio::TAudio(TContainer *parent)
{
    parent_container = parent;
    initTimer();
    setObjectName("TAudio");
}

TAudio::~TAudio()
{
}

void TAudio::setDurationTimerBeforePlay()
{
    if (load())
    {
        hasDurAttribute(); // it doesn' matter if there is an dur set cause videos/audio have an own media duration
        if (!is_resumed)
            emit startedMedia(parent_container, this);
    }
    else // set a duration otherwise it runs in a recursion stack overflow when load is not complete
        setInternalDefaultDur();
}

void TAudio::finished()
{
    finishedSimpleDuration();
}

// ====================  protected methods =================================
void TAudio::play()
{
    MediaDecoderW->play();
    status = _playing;
}

void TAudio::stop()
{
    MediaDecoderW->stop();
    disconnect(MediaDecoderW, SIGNAL(finished()), this, SLOT(finished()));
    MediaDecoderW->deleteLater();
    status        = _stopped;
    loaded        = false;
}


void TAudio::pause()
{
    MediaDecoderW->pause();
    status = _playing;
}

bool TAudio::loadMedia()
{
    QString file_path  = MyFileManager->getLoadablePath(src);
    MediaDecoderW      = new MediaDecoderWrapper;
    connect(MediaDecoderW, SIGNAL(finished()), this, SLOT(finished()));
    bool ret           = MediaDecoderW->load(file_path);

    if (!ret)
        qCritical() << getID() << QTime::currentTime().toString()  << "not loaded: " << file_path;
    else
        qDebug() << getID() << QTime::currentTime().toString()  << "loaded: " << file_path;
    return ret;
}



void TAudio::setAttributes()
{
    setBaseMediaAttributes();
    if (root_element.hasAttribute("soundLevel"))
        soundLevel = root_element.attribute("soundLevel");
}

// ====================  private methods =================================


