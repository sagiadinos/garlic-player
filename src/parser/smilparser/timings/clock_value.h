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

#ifndef TCLOCKVALUE_H
#define TCLOCKVALUE_H
#include <QObject>

/**
 * @brief The TClockValue class calculate timings for dur, min and max attributes.
 *        inherited by TTiming for begin and end attributes
 */
class TClockValue : public QObject
{
    Q_OBJECT
public:
             TClockValue(QObject *parent = 0);
    void     parse(QString s_value);
    QString  getStatus();
    qint64   getMilliseconds();
protected:
    QString   status      = "";
    qint64    ms          = 0;
    qint64    calculateMilliseconds(QString duration);
signals:

public slots:
};

#endif // TCLOCKVALUE_H
