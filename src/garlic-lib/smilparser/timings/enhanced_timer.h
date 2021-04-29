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
#ifndef ENHANCEDTIMER_H
#define ENHANCEDTIMER_H

#include <QObject>

#include "timings/simple_timer.h"
#include "trigger/wallclock.h"
#include <QList>
namespace Timings
{
    class EnhancedTimer : public QObject
    {
        Q_OBJECT
    public:
        enum Constants
        {
            TYPE_NOT_SET        = 0,
            TYPE_OFFSET         = 1, // same as clock value but depends on type of container https://www.w3.org/TR/REC-smil/smil-timing.html#Timing-OffsetValueSyntax
            TYPE_SYNCBASE       = 2,
            TYPE_EVENT          = 3,
            TYPE_REPEAT         = 4,
            TYPE_ACCESSKEY      = 5,
            TYPE_MEDIAMARKER    = 6,
            TYPE_WALLCLOCK      = 7,
            TYPE_INDEFINITE     = 8
        };

        explicit EnhancedTimer(QObject *parent = nullptr);
        ~EnhancedTimer();
        void        parse(QString svalue);
        void        deleteTimer();
        void        start();
        void        pause();
        void        stop();
        bool        resume();
        bool        isActive();
    protected:
        qint64      pause_start;
    protected slots:
      void   emitTimeout();
    private:
      struct TimerStruct
      {
          QTimer     *MyTimer      = Q_NULLPTR;
          ClockValue *MyClockValue = Q_NULLPTR;
          WallClock  *MyWallClock  = Q_NULLPTR;
          int         type         = TYPE_NOT_SET;
          int         remaining    = 0;
      };
      bool        is_indefinite = false;
      QList<TimerStruct *> MyTimerList;
      void        initTimer(int type, QString value);

    signals:
        void timeout();
    };
}
#endif // ENHANCEDTIMER_H
