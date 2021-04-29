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
#include "smilparser/base.h"
#include "smilparser/timings/enhanced_timer.h"

/**
 * @brief The abstract BaseTimings class should inherited for begin end duration capable smil elements in body section
 *        Full support as in http://www.w3.org/TR/SMIL3/ described for:
 *        dur          Clock Value in ms, media indefinite
 *        repeatCount  indefinite, numeric value)
 *
 *        Partial support Clock Value in ms, media indefinite and wallcock with repeating periods
 *        begin, end
 *
 */
class BaseTimings : public TBase
{
        Q_OBJECT
    public:
        enum RESTART
        {
            ALWAYS             = 0,
            NEVER              = 1,
            WHEN_NOT_ACTIVE    = 2
        };
        const     int        _stopped  = 0;
        const     int        _waiting  = 1; // wait for first start
        const     int        _playing  = 2;
        const     int        _paused   = 3;
        // means: simple duration ended, but begin or end timer are active or . Can be restarted by begin-valuelist
        const     int        _pending  = 4;

        explicit               BaseTimings(QObject * parent = Q_NULLPTR);
                              ~BaseTimings();
                void           preparePlaying();      // what to do when parent sends an order to begin executions
                void           preparePausing();
                void           prepareStop();      // what to do when parent sends an order to begin executions
                void           prepareResume();      // what to do when parent sends an order to begin executions
                int            getStatus(){return status;}
                void           finishedNotFound();
                void           skipElement();
                BaseTimings   *getParentContainer(){return parent_container;}

                void           stop();
        virtual void           play()        = 0;
        virtual void           pause()       = 0;
        virtual void           resume()      = 0;
        virtual QString        getBaseType() = 0;

        virtual void           emitPause() = 0;
        virtual void           emitResume() = 0;

                void  emitStartElementSignal(BaseTimings* bt){emit startElementSignal(bt);}
                void  emitStopElementSignal(BaseTimings* bt){emit stopElementSignal(bt);}
                void  emitResumeElementSignal(BaseTimings* bt){emit resumeElementSignal(bt);}
                void  emitPauseElementSignal(BaseTimings* bt){emit pauseElementSignal(bt);}


                void           finishIntrinsicDuration();
    public slots:
        virtual void           emitfinishedActiveDuration() = 0;
        virtual void           prepareDurationTimerBeforePlay() = 0; // called from begin-Timer to check if
                void           finishedSimpleDuration();
    protected:
                BaseTimings   *parent_container;
                QTimer                 *InternalTimer = Q_NULLPTR;
                int            restart = ALWAYS;// WHEN_NOT_ACTIVE;
                QString        fill    = "remove";
                int            status         = 0;
                bool           is_repeatable  = false;
                int            repeatCount    = 0;
                bool           indefinite     = false;
                int            internal_count = 1;
                void           parseTimingAttributes();
                void           resetInternalRepeatCount();
                bool           startDurTimer();
                bool           checkRepeatCountStatus();
                bool           isEndTimerActive();
                bool           isDurTimerActive();
                bool           isRestartable();
    protected slots:
                void           finishedActiveDuration();
    private:
                Timings::EnhancedTimer *BeginTimer = Q_NULLPTR;
                Timings::EnhancedTimer *EndTimer   = Q_NULLPTR;
                Timings::SimpleTimer   *DurTimer   = Q_NULLPTR;
           //     Timings::SimpleTimer   *MinTimer   = Q_NULLPTR;
           //     Timings::SimpleTimer   *MaxTimer   = Q_NULLPTR;
           //     Timings::SimpleTimer   *repeatDur  = Q_NULLPTR;
                void           setRepeatCount(QString rC);
                void           handleBeginTimer();
                void           setRestart(QString attr_value);
    signals:
                void           finishActiveDurationSignal(BaseTimings*);
                void           finishSimpleDurationSignal(BaseTimings*);

                void  startElementSignal(BaseTimings*);
                void  stopElementSignal(BaseTimings*);
                void  resumeElementSignal(BaseTimings*);
                void  pauseElementSignal(BaseTimings*);

};

#endif // TBASETIMING_H
