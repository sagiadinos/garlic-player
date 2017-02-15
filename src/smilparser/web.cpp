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

#include "web.h"

TWeb::TWeb(TContainer *parent)
{
    parent_container = parent;
    initTimer();
    setObjectName("TWeb");
}

TWeb::~TWeb()
{
}


void TWeb::setDurationTimerBeforePlay()
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


/**
 * @brief TMedia::prepareLoad
 * @param manager
 */
void TWeb::registerFile(TFileManager *manager)
{
    Q_UNUSED(manager)
    return;
}


void TWeb::play()
{
    status = _playing;
    return;
}

void TWeb::pause()
{
    status = _paused;
    return;
}


void TWeb::stop()
{
    delete browser;
    status = _stopped;
    loaded = false;
    return;
}

bool TWeb::loadMedia()
{
    browser       = new QWebEngineView();
    browser->load(QUrl(MyFileManager->getLoadablePath(src)));

    browser->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);
    browser->settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    qDebug() << getID() << QTime::currentTime().toString()  << "maybe ;) loaded: " << src;
    return true;
}

void TWeb::setAttributes()
{
    setBaseMediaAttributes();
    return;
}
