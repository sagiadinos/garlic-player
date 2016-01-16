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

#include "base.h"

TBase::TBase(QObject * parent)
{
    parent_playlist = parent;
}

void TBase::pause()
{
    if (begin_timer.isActive())
    {
        begin_remaining = begin_timer.remainingTime();
        begin_timer.stop();
    }
    if (dur_timer.isActive())
    {
        dur_remaining = dur_timer.remainingTime();
        dur_timer.stop();
    }

    if (end_timer.isActive())
    {
        begin_remaining = begin_timer.remainingTime();
        end_timer.stop();
    }
    status = _paused;
    return;
}


void TBase::stop()
{
    if (begin_timer.isActive())
        begin_timer.stop();
    if (end_timer.isActive())
        end_timer.stop();
    if (dur_timer.isActive())
        dur_timer.stop();
    begin_remaining = 0;
    end_remaining   = 0;
    status = _stopped;
}

void TBase::resume()
{
    begin_remaining = 0;
    end_remaining   = 0;
    status          = _playing;
}


// ========================= protected methods ======================================================

void TBase::finishedImplicitDuration()
{
    if (!end_timer.isActive())
        emitfinished();
    return;
}

void TBase::setTimedStart()
{
    if (begin.getStatus() == "ms") // look if begin should be delayed
    {
        begin_timer.start(begin.getMilliseconds());
    }
    else if (begin.getStatus() == "wallclock") // look if begin should be delayed
    {
        begin_timer.start(begin.getNextTrigger(QDateTime::currentDateTime()));
    }
    else // play imediately
        play();
}


bool TBase::setTimedEnd()
{
    bool ret = false;
    if (dur.getStatus() == "ms") // set end of simple duration
    {
        if (end.getStatus() == "ms") // end attribute has priority over dur (simple duration
            end_timer.start(end.getMilliseconds());

        dur_timer.start(dur.getMilliseconds());
        ret = true;
    }
    else if (dur.getStatus() == "indefinite" || end.getStatus() == "indefinite")
        ret = true;  // end time is set so no media end can be used
    else if (end.getStatus() == "wallclock")
    {
        end_timer.start(end.getNextTrigger(QDateTime::currentDateTime()));
    }
    else
    {
        //  if end specified simple duration is indefinited
        if (end.getStatus() == "ms") // end attribute has priority over dur (simple duration
        {
            end_timer.start(end.getMilliseconds());
            ret = true;
        }

    }
    return ret;
}

QString TBase::parseID(QDomElement element)
{
    QString ret = "";
    if (element.hasAttribute("id"))
        ret = element.attribute("id");
    if (element.hasAttribute("xml:id")) // In SMIL 3.0 superset old SMIL 2.0 id.
        ret = element.attribute("xml:id");
    if (ret == "") // get line and column number as alternative when no
    {
        ret = "T"+QString(element.lineNumber()) + QString(element.columnNumber());
    }
    return ret;
}


void TBase::setBaseAttributes()
{
    id = parseID(root_element);
    if (root_element.hasAttribute("title"))
        title = root_element.attribute("title");
    if (root_element.hasAttribute("begin"))
        begin.parse(root_element.attribute("begin"));
    if (root_element.hasAttribute("end"))
        end.parse(root_element.attribute("end"));
    if (root_element.hasAttribute("dur"))
        dur.parse(root_element.attribute("dur"));
    if (root_element.hasAttribute("repeatCount"))
        setRepeatCount(root_element.attribute("repeatCount"));
    return;
}

/**
 * @brief  TBasePlaylist::checkRepeatCountStatus returns true, when playlist can be repeated
 * @return bool
 */
bool TBase::checkRepeatCountStatus()
{
    bool ret = false;
    if (indefinite)
        ret = true;
    else if (repeatCount > 0 && internal_count < repeatCount)
    {
        internal_count++;
        ret = true;
    }
    return ret;
}

// ========================= private methods ======================================================

void TBase::setRepeatCount(QString rC)
{
    if (rC == "indefinite")
        indefinite = true;
    else
       indefinite = false;
    repeatCount = rC.toInt();
    return;
}

void TBase::initTimer()
{
    connect(&begin_timer, SIGNAL(timeout()), this, SLOT(play()));
    begin_timer.setSingleShot(true);

    connect(&end_timer, SIGNAL(timeout()), this, SLOT(emitfinished()));
    end_timer.setSingleShot(true);

    connect(&dur_timer, SIGNAL(timeout()), this, SLOT(finishedImplicitDuration()));
    dur_timer.setSingleShot(true);

    begin_remaining = 0;
    end_remaining   = 0;
    return;
}

