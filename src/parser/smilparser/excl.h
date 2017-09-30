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

#ifndef TEXCL_H
#define TEXCL_H

#include "smilparser/priorityclass.h"

class TExcl : public TContainer
{
    Q_OBJECT
public:
    explicit TExcl(TContainer * parent = 0);

    const       int         _stop_new          = 0; // never
    const       int         _stop_active       = 1;
    const       int         _play_this         = 2;
    const       int         _pause_active      = 3;
    const       int         _pause_new         = 4; // defer

    bool                 parse(QDomElement element);
    void                 next(TBaseTiming *ended_element);
    int                  interruptActualPlaying(QDomElement started_element, TBaseTiming *element);
    bool                 isChildPlayable(TBaseTiming *element);
    void                 pause();
    void                 stop();
    void                 play();
    void                 resume();
public slots:
    void      setDurationTimerBeforePlay();
protected:
    QDomElement                           played_dom_element;
    TPriorityClass                       *ActivePriorityClass, *NewActivePriorityClass;
    QHash<int, TPriorityClass *>          ar_priorities;
    TPriorityClass                       *findPriorityClass(QDomElement dom_element);
    void                                  setPlaylist();
    void                                  parsePriorityClass(QDomElement element);
    int                                   priorityStop(QDomElement dom_element, TBaseTiming *element);
    int                                   priorityPause(QDomElement dom_element, TBaseTiming *element);
    int                                   priorityNever(QDomElement dom_element, TBaseTiming *element);
    int                                   priorityDefer(QDomElement dom_element, TBaseTiming *element);

signals:
    void resumeElement(TBaseTiming *element);
    void stopElement(TBaseTiming *element);
    void pauseElement(TBaseTiming *element);
};

#endif // TEXCL_H
