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
#ifndef BASE_MANAGER_H
#define BASE_MANAGER_H

#include <QObject>
#include <QFile>
class BaseManager : public QObject
{
        Q_OBJECT
    public:
        BaseManager(QObject *parent = Q_NULLPTR);

    protected:
        bool     isRemote(QString src);
        bool     isRelative(QString src);
};

#endif // BASE_MANAGER_H
