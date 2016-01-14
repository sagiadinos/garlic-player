/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-.control.com>
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
    const int _stopped  = 0;
    const int _playing  = 1;
    const int _pausing  = 2;
    explicit TBase(QObject * parent = 0);
    virtual  QString  getType() = 0;
    virtual  bool     parse(QDomElement element) = 0; // prepare for begin
    virtual  void     beginPlay() = 0;      // what to do when parent sends an order to begin executions
             QString  getID(){return id;}
             QString  getTitle(){return title;}
public slots:
    virtual void        emitfinished() = 0;
protected:
            QTimer      begin_timer, end_timer;
            QDomElement actual_element;
            QString     id                   = "";
            TClockValue dur, min, max;
            TTiming     begin, end;
            QObject    *parent_playlist;
            int         status         = 0;
            QString     title          = "";
            int         repeatCount    = 0;
            int         internal_count = 1;
            bool        indefinite     = false;
            void        setTimedStart();
            bool        setTimedEnd();
            void        setBaseAttributes();
            bool        checkRepeatCountStatus();
            qint64      calculateDuration(QString duration);
protected slots:
    virtual void play() = 0;
private:
            void        setRepeatCount(QString rC);
signals:
            void        started(QObject * , QObject *);
            void        finished(QObject * , QObject *);
};

#endif // TBASE_H
