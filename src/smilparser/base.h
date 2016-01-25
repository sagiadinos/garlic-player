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

#ifndef TBASE_H
#define TBASE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtXml>
#include <QDebug>
#include <QTimer>
#include <configuration.h>
#include "smilparser/timings/timing.h"

/**
 * @brief The abstract TBase class should inherited for all smil elements in body section
 *        Here comes is the init of all common attributes:
 *        id/xml:id, begin, end, title, repeatCount and dur
 *
 *        Full support as in http://www.w3.org/TR/SMIL3/ described for:
 *        dur          Clock Value in ms, media indefinite
 *        repeatCount  indefinite, numeric value)
 *        fallback for id with is superseted by SMIL3.0 xml:id
 *
 *        actual no support/functionality beside of reading for:
 *        begin, end, id/xml:id and title
 *
 *        Signals
 *        started: should be emitted when elements starts play
 *        finshed: should be emitted when element ends
 *
 */
class TBase : public QObject
{
    Q_OBJECT
public:
    const     int        _stopped  = 0;
    const     int        _waiting  = 1;
    const     int        _playing  = 2;
    const     int        _paused   = 3;

    explicit             TBase(QObject * parent = 0);
    virtual  bool        parse(QDomElement element) = 0; // prepare for begin
    virtual  void        beginPlay() = 0;      // what to do when parent sends an order to begin executions
             void        beginPause();
             void        beginStop();      // what to do when parent sends an order to begin executions
             void        beginResume();      // what to do when parent sends an order to begin executions
//    virtual  void        resume()    = 0;
             QDomElement getRootElement(){return root_element;}
             int         getStatus(){return status;}
             QString     getID(){return id;}
             QString     getTitle(){return title;}
    static   QString     parseID(QDomElement element);
    virtual  void        pause()    = 0;
    virtual  void        stop()     = 0;

public slots:
    virtual  void        emitfinished() = 0;
protected:
            QTimer       *begin_timer, *end_timer, *dur_timer;
            int          begin_remaining, end_remaining, dur_remaining;
            QDomElement  root_element;
            QString      id                   = "";
            TClockValue  dur, min, max;
            TTiming      begin, end;
            QObject     *parent_playlist;
            int          status         = 0;
            QString      title          = "";
            int          repeatCount    = 0;         // protected for testing
            bool         indefinite     = false;     // protected for testing
            int          internal_count = 1;         // protected for testing
            void         resetInternalRepeatCount();
            void         setBeginEndTimer();
            bool         hasDurAttribute();
            void         setBaseAttributes();
            bool         checkRepeatCountStatus();
            void         initTimer();
   virtual  void         play()     = 0;
protected slots:
    virtual void         setDurationTimerBeforePlay() = 0; // called from begin-Timer to check if
            void         finishedSimpleDuration();
private:
            void         setRepeatCount(QString rC);
};

#endif // TBASE_H
