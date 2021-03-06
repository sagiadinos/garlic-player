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

#ifndef TCONTAINER_H
#define TCONTAINER_H

#include <QList>
#include "base_timings.h"

/**
 * @brief The TComntainer is an abstract class for Playlists Container which is inherited by seq, par and excl.
 *
 * It holds two lists. Alle children as DomElements and one ActivatedChilds with TBaseTimings
 *
 * ActivatedChilds are the Childs which can be played/displayed
 *
 */
class TContainer : public BaseTimings
{
    Q_OBJECT
    public:
        explicit TContainer(QObject *parent = Q_NULLPTR);
        virtual void         next(BaseTimings *ended_element) = 0;

                QString      getBaseType() {return "container";}
                // next two used only in excl => move later when sure
                void         setCurrentActivatedElement(BaseTimings *element);
                BaseTimings *getCurrentActivatedElement();

                BaseTimings *findDomChild(QDomElement dom_element);
                void         insertDomChild(BaseTimings *element);
        virtual void         interruptByEndSync()   = 0;
                void         interruptByRestart();


        virtual void         collectActivatedChilds() = 0;
                void         removeActivatedChild(BaseTimings *element);
                void         removeAllActivatedChilds();

                void         startTimersOfAllActivatedChilds();
                void         startTimersOfFirstActivatedChild();
                void         stopTimersOfAllActivatedChilds();

                void         emitStopToAllActivatedChilds();
                void         emitPauseToAllActivatedChilds();
                void         emitResumeToAllActivatedChilds();
                bool         hasActivatedChild();

                void         emitPreloadElementSignal(TContainer* p, QDomElement e){emit preloadElementSignal(p, e);}
                void         emitPause();
                void         emitResume();
    public slots:
                void         emitfinishedActiveDuration();

    protected:
                QList<BaseTimings *>          activated_childs;
                bool                          is_child_active    = false;
                // used only in excl => move later
                BaseTimings                  *current_activated_element = Q_NULLPTR;

                QList<QDomElement>            childs_list;
                QList<QDomElement>::iterator  childs_list_iterator;
                QHash<QString, BaseTimings *> elements_list;
                QDomElement                   active_element;

        virtual void  traverseChilds() = 0;
                void  activateFoundElement();
                void  insertActivatedChild(BaseTimings *MyBaseTimings);
    signals:
                void  preloadElementSignal(TContainer*, QDomElement);

};

#endif // TCONTAINER_H
