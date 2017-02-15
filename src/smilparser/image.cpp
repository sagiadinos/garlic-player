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

#include "image.h"

TImage::TImage(TContainer *parent)
{
    parent_container = parent;
    initTimer();
    setObjectName("TImage");
}

TImage::~TImage()
{
}

void TImage::setDurationTimerBeforePlay()
{
    if (load() && (hasDurAttribute() || end_timer->isActive()))
    {
        if (!is_resumed)
            emit startedMedia(parent_container, this);
    }
    else // set a duration otherwise it runs in a recursion stack overflow when no dur set or load is not complete
        setInternalDefaultDur();
    return;
}

void TImage::play()
{
    qDebug() << getID() << QTime::currentTime().toString()  << "play";
    status = _playing;
    return;
}

void TImage::pause()
{
    qDebug() << getID() << QTime::currentTime().toString() << "paused " << end_remaining;
    status = _paused;
    return;
}

void TImage::stop()
{
    qDebug() << getID() << QTime::currentTime().toString() << "stopped";
    // image  = QPixmap::QPixmap(); // unload
    status = _stopped;
    loaded = false;
    return;
}

bool TImage::loadMedia()
{
    QString file_path = MyFileManager->getLoadablePath(src);
    bool ret          = image.load(file_path);
    if (!ret)
        qCritical() << getID() << QTime::currentTime().toString()  << "not loaded: " << file_path;
    else
        qDebug() << getID() << QTime::currentTime().toString()  << "loaded: " << file_path;

    return ret;
}


// ====================  protected methods =================================

void TImage::setAttributes()
{
    setBaseMediaAttributes();
    return;
}

// ====================  private methods =================================
