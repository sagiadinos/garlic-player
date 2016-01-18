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
    TExcl(QObject * parent = 0);
    bool      parse(QDomElement element);
    bool      next();
    QString   getType(){return "excl";}
    void      beginPlay();
    void      changeActiveChild(QDomElement element);    
public slots:
    void      checkBeforePlay();
protected:
    void     play();
    void     pause();
    void     stop();
private:
    TPriorityClass                       *ActivePriorityClass, *NewActivePriorityClass;
    QQueue<QDomElement>                   pause_queue;
    QSet<TPriorityClass *>                ar_priorities;
    QSet<TPriorityClass *>::iterator      ar_priorities_iterator;
    int       count_childs       = 0;
    void      setPlaylist();
    void      setPriorityClass(QDomElement element);
};

#endif // TEXCL_H
