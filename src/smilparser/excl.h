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

class TExcl : public TPlaylist
{
    Q_OBJECT
public:
    const     int        _no_interrupting  = 0;
    const     int        _interrupt_active   = 1;
    const     int        _play_this          = 2;
    TExcl(QObject * parent = 0);
    bool      parse(QDomElement element);
    void      next();
    void      beginPlay();
    QObject*  getPlayedObject();
    int       interruptActualPlaying(QDomElement started_element, QObject *element);
    void      decActivatableChilds();
    QString   getPeers();
    void      setChildActive(bool active);
    void      pause();
    void      stop();
public slots:
    void      setDurationTimerBeforePlay();
protected:
    void     play();
private:
    QObject                              *played_element;
    QDomElement                           played_dom_element;
    TPriorityClass                       *ActivePriorityClass, *NewActivePriorityClass;
    QQueue<TPriorityClass *>              queue;
    QHash<int, TPriorityClass *>                ar_priorities;
    QHash<int, TPriorityClass *>::iterator      ar_priorities_iterator;
    int       activatable_childs  = 0;
    void      setPlaylist();
    void      setPriorityClass(QDomElement element);
signals:
    void doStop(QObject *element);
    void doPause(QObject *element);
};

#endif // TEXCL_H
