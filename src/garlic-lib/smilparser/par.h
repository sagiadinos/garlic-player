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

#ifndef TPAR_H
#define TPAR_H

#include "smilparser/container.h"

class TPar : public TContainer
{
    Q_OBJECT
public:
    explicit TPar(TContainer *pc, QObject *parent = Q_NULLPTR);
    ~TPar();
    bool                parse(QDomElement element);
    void                next(TBaseTiming *ended_element);
    TBaseTiming        *getChildElementFromList();
    void                pause();
    void                stop();
    void                play();
    void                resume();
    bool                isChildPlayable(TBaseTiming *element);
protected slots:
    void                setDurationTimerBeforePlay();
private:
    QSet<TBaseTiming *> activatable_childs;
    QString             endsync            = "last";
    QDomNodeList        childs;
    void                setPlaylist();
    int                 count_childs       = 0;

};

#endif // TPAR_H
