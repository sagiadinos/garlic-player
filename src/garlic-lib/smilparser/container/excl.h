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

#include "priorityclass.h"

class TExcl : public TContainer
{
        Q_OBJECT
    public:
        explicit TExcl(TContainer *pc, QObject *parent = Q_NULLPTR);
                ~TExcl();
        const       int         _stop_new          = 0; // never
        const       int         _stop_active       = 1;
        const       int         _play_this         = 2;
        const       int         _pause_active      = 3;
        const       int         _pause_new         = 4; // defer

        void                 preloadParse(QDomElement element);
        void                 next(BaseTimings *ended_element);
        int                  interruptActualPlaying(QDomElement started_element, BaseTimings *element);
        bool                 isChildPlayable(BaseTimings *element);
        void                 pause();
        void                 stop();
        void                 play();
        void                 resume();
    public slots:
        void                 prepareDurationTimerBeforePlay();
        void                 finishedDuration();
    protected:
        QDomElement                           played_dom_element;
        TPriorityClass                       *ActivePriorityClass, *NewActivePriorityClass;
        QHash<int, TPriorityClass *>          ar_priorities;
        TPriorityClass                       *findPriorityClass(QDomElement dom_element);
        void                                  traverseChilds();
        void                                  traversePriorityClasses(QDomNodeList priority_class_childs);
        void                                  parsePriorityClass(QDomElement element);
        int                                   priorityStop(QDomElement dom_element, BaseTimings *element);
        int                                   priorityPause(QDomElement dom_element, BaseTimings *element);
        int                                   priorityNever(QDomElement dom_element, BaseTimings *element);
        int                                   priorityDefer(QDomElement dom_element, BaseTimings *element);
        void         handlePossibleRepeat();

    signals:
        void resumeElement(BaseTimings *element);
        void stopElement(BaseTimings *element);
        void pauseElement(BaseTimings *element);
};

#endif // TEXCL_H
