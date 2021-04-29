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
#ifndef TPRIORITYCLASS_H
#define TPRIORITYCLASS_H
#include <QQueue>
#include <QList>
#include <QStack>
#include "container.h"

class TPriorityClass : public TBase
{
    Q_OBJECT
public:
    explicit TPriorityClass(QObject *parent = 0);
    ~TPriorityClass();
    void                          preloadParse(QDomElement element);

    QString                       getPeers(){return peers;}
    QString                       getHigher(){return higher;}
    QString                       getLower(){return lower;}
    bool                          findElement(QDomElement dom_element);
    void                          insertDeferQueue(BaseTimings *element);
    void                          insertPauseQueue(BaseTimings *element);
    int                           countQueue();
    BaseTimings                  *getFromQueue();
    QList<QDomElement>            getChildList();
protected:
    QList<QDomElement>            childs_list;
    QList<QDomElement>::iterator  iterator;
    QStack<BaseTimings *>         ar_defer;
    QStack<BaseTimings *>         ar_pause;
private:
    QString                       peers  = "stop";   // how elements inside a group interrupts each other
    QString                       higher = "pause";  // how a group with hier priority interrupts this group
    QString                       lower  = "defer";  // how a group with lower priority interrupts this group
    int                           count_childs       = 0;
    void                          setAttributes();
    void                          traverseChilds();
};

#endif // TPRIORITYCLASS_H
