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

#ifndef TBASETIMING_H
#define TBASETIMING_H

#include <QTimer>
#include "smilparser/timings/timing.h"
#include "smilparser/base.h"

/**
 * @brief The abstract TBaseTiming class should inherited for begin end duration capable smil elements in body section
 *        Full support as in http://www.w3.org/TR/SMIL3/ described for:
 *        dur          Clock Value in ms, media indefinite
 *        repeatCount  indefinite, numeric value)
 *
 *        Partial support Clock Value in ms, media indefinite and wallcock with repeating periods
 *        begin, end
 *
 */
class TBaseTiming : public TBase
{
    Q_OBJECT
public:
    const     int        _stopped  = 0;
    const     int        _waiting  = 1;
    const     int        _playing  = 2;
    const     int        _paused   = 3;

    explicit              TBaseTiming(QObject * parent = 0);
            bool          prepareTimerBeforePlaying();      // what to do when parent sends an order to begin executions
            void          prepareTimerBeforePausing();
            void          prepareTimerBeforeStop();      // what to do when parent sends an order to begin executions
            void          prepareTimerBeforeResume();      // what to do when parent sends an order to begin executions
            int           getStatus(){return status;}
            bool          isRepeatable(){return repeatable;}
    virtual void          play()        = 0;
    virtual void          pause()       = 0;
    virtual void          stop()        = 0;
    virtual void          resume()      = 0;
    virtual QString       getBaseType() = 0;
    virtual TBaseTiming  *getChildElementFromList() = 0;
    virtual bool          hasPlayingChilds() = 0;
public slots:
    virtual void          emitfinished() = 0;
            void          finishedSimpleDuration();
protected:
            QTimer       *begin_timer, *end_timer, *dur_timer;
            qint64        pause_start;
            int           begin_remaining, end_remaining, dur_remaining;
            TClockValue   dur, min, max;
            TTiming       begin, end;
            int           status         = 0;
            int           repeatCount    = 0;
            bool          indefinite     = false;
            bool          is_resumed     = false;
            int           internal_count = 1;
            void          setInternalDefaultDur();
            void          setTimingAttributes();
            void          resetInternalRepeatCount();
            bool          hasDurAttribute();
            bool          checkRepeatCountStatus();
            void          initTimer();
   virtual  void          setDurationTimerBeforePlay() = 0; // called from begin-Timer to check if
protected slots:
            void          releasePlay();
            void          finishedActiveDuration();
private:
            bool          repeatable;
            void          setRepeatCount(QString rC);
};

#endif // TBASETIMING_H
