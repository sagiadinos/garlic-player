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
    explicit TBase(QObject * parent = 0);
    virtual  QString getType() = 0;
    virtual  bool    parse(QDomElement element) = 0;

    QString     getDur(){return dur;}
    qint64      getDuration(){return duration;}
    QString     getID(){return id;}
    QString     getBegin(){return begin;}
    QString     getEnd(){return end;}
    QString     getTitle(){return title;}
protected:
    QDomElement actual_element;
    QString     dur            = "";
    qint64      duration       = 0;  // conversion in milliseconds
    QString     id             = "";
    QString     begin          = "0";
    qint64      begin_clock    = 0;  // use when begin has normal clock values like duration
    int         begin_repeat   = 0;  // use when wallclock with repeat
    int         begin_period   = 0;  // in millisceonds use when wallclock has period

    QString     end            = "";
    qint64      end_clock      = 0;  // use when end has normal clock values like duration
    int         end_repeat     = 0;  // use when end wallclock with repeat
    int         end_period     = 0;  // in millisceonds use when wallclock has period

    QString     title          = "";

    int         repeatCount    = 0;
    int         internal_count = 1;
    bool        indefinite     = false;
    void        setBaseAttributes();
    bool        checkRepeatCountStatus();
    qint64      calculateDuration(QString duration);
private:
    void        setRepeatCount(QString rC);
signals:
    void        finished(QObject * , QObject *);
};

#endif // TBASE_H
