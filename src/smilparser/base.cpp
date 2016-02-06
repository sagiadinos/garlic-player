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
    Q_UNUSED(parent);
}

bool TBase::checkifPlayable()
{
    playable = true;
    if (end.getStatus() == "ignore" || begin.getStatus() == "ignore")
        playable = false;
    return playable;
}

bool TBase::preparePlay()
{
    if (!playable)
        return false;

    bool    ret        = true;
    QString end_status = end.getStatus();
    is_resumed         = false;

    // set end here cause begin and end depending on start of the par/excl parent. With seq it depends on last element
    if (end_status == "ms") // end attribute has priority over dur (simple duration
        end_timer->start(end.getMilliseconds());
    else if (end_status == "wallclock")
    {
        end_timer->start(end.getNextTrigger(QDateTime::currentDateTime()));
    }
    if (end_status != "ignore")
    {
        // set if begin should be delayed
        QString begin_status = begin.getStatus();
        if (begin_status == "ms")
        {
            begin_timer->start(begin.getMilliseconds());
            status = _waiting;
        }
        else if (begin_status == "wallclock") // look if begin should be delayed
        {
            begin_timer->start(begin.getNextTrigger(QDateTime::currentDateTime()));
            qDebug() << "begin " << getID() << begin.getNextTrigger(QDateTime::currentDateTime());
            status = _waiting;
        }
        else if (begin_status == "indefinite")
            status = _waiting;
        else // play imediately
            setDurationTimerBeforePlay();
    }
    begin_remaining = 0;
    end_remaining   = 0;
    dur_remaining   = 0;
    pause_start     = 0;
    return ret;
}

void TBase::preparePause()
{
    if (begin_timer->isActive())
    {
        begin_remaining = begin_timer->remainingTime();
        begin_timer->stop();
    }
    if (dur_timer->isActive())
    {
        dur_remaining = dur_timer->remainingTime();
        dur_timer->stop();
    }
    if (end_timer->isActive())
    {
        end_remaining = end_timer->remainingTime();
        end_timer->stop();
    }
    pause_start = QDateTime::currentMSecsSinceEpoch();
    return;
}

void TBase::prepareStop()
{
    if (begin_timer->isActive())
        begin_timer->stop();
    if (end_timer->isActive())
        end_timer->stop();
    if (dur_timer->isActive())
        dur_timer->stop();
    begin_remaining = 0;
    end_remaining   = 0;
    dur_remaining   = 0;
    return;
}

void TBase::prepareResume()
{
    is_resumed = true;
    qint64 elapsed     = QDateTime::currentMSecsSinceEpoch() - pause_start;
    bool   ended       = false; // set true if there is a trigger and it is had ended
    if (end_remaining > 0)
    {
        qint64 end_trigger = 0;
        if (end.getStatus() == "ms") // end attribute has priority over dur (simple duration
            end_trigger = end_remaining - elapsed;
        else if (end.getStatus() == "wallclock")
            end_trigger = end.getNextTrigger(QDateTime::currentDateTime());

        qDebug() << getID() << " end remaining: " << end_remaining << "elapsed: " << elapsed << "trigger" << end_trigger;
        if (end_trigger > 0)
            end_timer->start(end_trigger);
        else
        {
            ended = true;
            finishedActiveDuration();
        }
    }

    if (!ended)
    {
        if (begin_remaining > 0)
        {
            qint64 begin_trigger = 0;
            QString begin_status = begin.getStatus();
            if (begin_status == "ms")
            {
                begin_trigger   = begin_remaining - elapsed;
                status          = _waiting;
            }
            else if(begin_status == "wallclock")
            {
                begin_trigger = begin.getNextTrigger(QDateTime::currentDateTime()) - elapsed;
            }
            if (begin_trigger > 0)
            {
                begin_timer->start(begin_trigger);
                status          = _waiting;
            }
            else if (begin_status == "indefinite")
                status = _waiting;
            else // play imediately when begin_trigger become negative and begin_status is not indefinite
                setDurationTimerBeforePlay();
          }
          else // play imediately when no begin trigger
              setDurationTimerBeforePlay();
    }

    begin_remaining = 0;
    end_remaining   = 0;
    return;
}

// ========================= protected methods ======================================================

void TBase::finishedSimpleDuration()
{
    if (!checkRepeatCountStatus() && !end_timer->isActive())
        finishedActiveDuration();
    return;
}

void TBase::finishedActiveDuration()
{
    emitfinished();
    return;
}

bool TBase::hasDurAttribute()
{
    bool ret = false;
    if (dur.getStatus() == "ms") // set end of simple duration
    {
        if (dur_remaining > 0)
        {
            dur_timer->start(dur_remaining);
        }
        else
        {
            dur_timer->start(dur.getMilliseconds());
        }
        ret = true;
    }
    else if (dur.getStatus() == "indefinite" || end.getStatus() == "indefinite")
        ret = true; // no timer
    dur_remaining = 0;
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
    begin_timer = new QTimer;
    connect(begin_timer, SIGNAL(timeout()), this, SLOT(checkBeginRepeat()));
    begin_timer->setSingleShot(true);
    begin_timer->setTimerType(Qt::PreciseTimer);

    end_timer = new QTimer;
    connect(end_timer, SIGNAL(timeout()), this, SLOT(finishedActiveDuration()));
    end_timer->setSingleShot(true);
    end_timer->setTimerType(Qt::PreciseTimer);

    dur_timer = new QTimer;
    connect(dur_timer, SIGNAL(timeout()), this, SLOT(finishedSimpleDuration()));
    dur_timer->setSingleShot(true);
    dur_timer->setTimerType(Qt::PreciseTimer);

    begin_remaining = 0;
    end_remaining   = 0;
    dur_remaining   = 0;
    return;
}

void TBase::checkBeginRepeat()
{
    if (begin.remainingRepeats())
    {
        begin_timer->start(begin.getNextTrigger(QDateTime::currentDateTime()));
        qDebug() << "check begin repeats " << getID() << begin.getNextTrigger(QDateTime::currentDateTime());
        if (end.remainingRepeats()) // this make sense only when begin timer is repeated
            end_timer->start(end.getNextTrigger(QDateTime::currentDateTime()));
    }
    setDurationTimerBeforePlay();
    return;
}

