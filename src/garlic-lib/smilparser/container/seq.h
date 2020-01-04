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

#ifndef SEQ_H
#define SEQ_H
#include "container.h"
#include "tools/shuffle.h"

/**
 * @brief   The TSeq class parses seq-Tag
 *
 */
class TSeq : public TContainer
{
        Q_OBJECT
    public:
        explicit TSeq(TContainer *pc, QObject *parent = Q_NULLPTR);
                ~TSeq();
        bool         parse(QDomElement element);
        void         preload();
        void         next(BaseTimings *ended_element);
        void         pause();
        void         stop();
        void         play();
        void         resume();
        bool         isChildPlayable(BaseTimings *element);
    public slots:
        void         setDurationTimerBeforePlay();
    protected:
        TShuffle    *MyShuffle = Q_NULLPTR;
        int          count             = 0;
        bool         canGetNextItem();
        QDomElement  getNextItem();
        void         handlePossibleRepeat();
        void         traverseChilds();
};

#endif // SEQ_H
