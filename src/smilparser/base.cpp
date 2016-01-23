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

void TBase::beginPause()
{
//    if (begin_timer.isActive())
//    {
//        begin_remaining = begin_timer.remainingTime();
//        begin_timer.stop();
//    }
//    if (dur_timer.isActive())
//    {
//        dur_remaining = dur_timer.remainingTime();
//        dur_timer.stop();
//    }
//    if (end_timer.isActive())
//    {
//        begin_remaining = begin_timer.remainingTime();
//        end_timer.stop();
//    }
//    status = _paused;
    return;
}


void TBase::beginStop()
{
    if (begin_timer.isActive())
        begin_timer.stop();
    if (end_timer.isActive())
        end_timer.stop();
    if (dur_timer.isActive())
        dur_timer.stop();
    begin_remaining = 0;
    end_remaining   = 0;
    dur_remaining   = 0;
    stop();
    return;
}

void TBase::beginResume()
{
//    if (begin_remaining > 0 && begin.getStatus() == "ms")
//    {
//        begin_timer.start(begin_remaining);
//        status          = _waiting;
//        begin_remaining = 0;
//    }
//    else if(begin.getStatus() == "wallclock")
//    {
//        begin_timer.start(begin.getNextTrigger(QDateTime::currentDateTime()));
//        status          = _waiting;
//        begin_remaining = 0;
//    }
    return;
}

// ========================= protected methods ======================================================

void TBase::finishedSimpleDuration()
{
    if (!checkRepeatCountStatus() && !end_timer.isActive())
        emitfinished();
    return;
}

void TBase::setTimedStart()
{
    // set end here cause begin and end depending on start of the par/excl parent. With seq it depends on last element
    if (end.getStatus() == "ms") // end attribute has priority over dur (simple duration
        end_timer.start(end.getMilliseconds());
    else if (end.getStatus() == "wallclock")
        end_timer.start(end.getNextTrigger(QDateTime::currentDateTime()));

    // set if begin should be delayed
    if (begin.getStatus() == "ms")
    {
        begin_timer.start(begin.getMilliseconds());
        status = _waiting;
    }
    else if (begin.getStatus() == "wallclock") // look if begin should be delayed
    {
        begin_timer.start(begin.getNextTrigger(QDateTime::currentDateTime()));
        status = _waiting;
    }
    else // play imediately
        setDurationTimerBeforePlay();

    begin_remaining = 0;
    end_remaining   = 0;
    dur_remaining   = 0;
    return;
}


bool TBase::setTimedEnd()
{
    bool ret = false;
    if (dur.getStatus() == "ms") // set end of simple duration
    {
        dur_timer.start(dur.getMilliseconds());
        ret = true;
    }
    else if (dur.getStatus() == "indefinite" || end.getStatus() == "indefinite")
        ret = true; // no timer
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

void TBase::resetInternalRepeatCount()
{
    internal_count = 1;
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
    connect(&begin_timer, SIGNAL(timeout()), this, SLOT(setDurationTimerBeforePlay()));
    begin_timer.setSingleShot(true);

    connect(&end_timer, SIGNAL(timeout()), this, SLOT(emitfinished()));
    end_timer.setSingleShot(true);

    connect(&dur_timer, SIGNAL(timeout()), this, SLOT(finishedSimpleDuration()));
    dur_timer.setSingleShot(true);

    begin_remaining = 0;
    end_remaining   = 0;
    return;
}

