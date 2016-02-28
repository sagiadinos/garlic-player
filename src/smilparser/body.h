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

#ifndef TBODY_H
#define TBODY_H
#include "container.h"
#include "seq.h"

class TBody : public TContainer
{
    Q_OBJECT
public:
    TBody(TContainer * parent = 0);
    ~TBody();
    bool      parse(QDomElement element);
    void      prepareTimerBeforPlaying();
    void      endPlay(){}
    void      next(TBaseTiming *ended_element);
    void      resume(){}
    void      play(){}
    void      pause(){}
    void      stop(){}
    bool      isChildPlayable(TBaseTiming *element);
protected:
public slots:
    void      setDurationTimerBeforePlay();
private:
    void      setPlaylist();
};

#endif // TBODY_H
