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

        void    preloadParse(QDomElement element);
        void    next(BaseTimings *ended_element);
        bool    determineContinue(BaseTimings *new_element);
        void    pause();
        void    play();
        void    resume();
        void    collectActivatedChilds();

    public slots:
        void    prepareDurationTimerBeforePlay();

    private:
        TPriorityClass               *CurrentPriority, *NewPriority;
        QHash<int, TPriorityClass *>  priorities_list;
        TPriorityClass               *findPriorityClass(QDomElement dom_element);

        QString endsync = "last";
        void    traverseChilds();
        void    traversePriorityClasses(QDomNodeList priority_class_childs);
        void    parsePriorityClass(QDomElement element);
        void    priorityStop();
        void    priorityPause();
        void    priorityNever(BaseTimings *new_element);
        void    priorityDefer(BaseTimings *new_element);
};

#endif // TEXCL_H
