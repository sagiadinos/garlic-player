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
#include <QList>
#include "smilparser/playlist.h"

/**
 * @brief   The TSeq class parses Seq-Tag
 *
 */
class TSeq : public TPlaylist
{
    Q_OBJECT
public:
    TSeq(TBase * parent = 0);
    bool     parse(QDomElement element);
    void     next();
    bool     previous();
    void     pause();
    void     stop();
    void     play();
public slots:
    void     setDurationTimerBeforePlay();
private:
    bool     random            = false;
    int      pickNumber        = 0;
    int      internal_pick     = 0;
    QString  pickingAlgorithm  = "";
    QString  pickingBehavior   = "";
    void     setPlaylist();
    void     randomizePlaylist();
    void     doMetaData();
};

#endif // SEQ_H
