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

#include "base_timings.h"
#include <QList>

/**
 * @brief The TBasePlaylist is an abstract class for Smil Playlists which is inherited by seq, par and excl.
 *        This class has functions for handling and media content. In case of media or container it returns the type and
 *        the actual DomElement, so that the caller can handle it
 *
 */
class TContainer : public BaseTimings
{
    Q_OBJECT
    public:
        explicit TContainer(QObject *parent = Q_NULLPTR);
        virtual void                          next(BaseTimings *ended_element) = 0;
                QHash<QString, BaseTimings *> getContainerObjects();
                QString                       getIdOfActiveElement();
        virtual bool                          isChildPlayable(BaseTimings *element) = 0;
                bool                          hasPlayingChilds();
                void                          childStarted(BaseTimings *element);
                void                          childEnded(BaseTimings *element);
                QString                       getBaseType() {return "container";}
                BaseTimings                  *getChildElementFromList();
                void                          setPlayedElement(BaseTimings *element);
                BaseTimings                  *getPlayedElement();
                void                          setChildActive(bool active);
    public slots:
                void                          emitfinished();
    protected:
                TContainer                   *parent_container;
                QSet<BaseTimings *>           activatable_childs;
                QSet<BaseTimings *>::iterator childs_iterator;
                bool                          is_child_active    = false;
                BaseTimings                  *played_element;
                QList<QDomElement>            childs_list;
                QList<QDomElement>::iterator  childs_list_iterator;

                QDomElement                   active_element;
                virtual void                  traverseChilds() = 0;
                void                          emitFoundElement();
    signals:
                void                          foundElement(TContainer *, QDomElement);
                void                          startedContainer(TContainer * , BaseTimings *);
                void                          finishedContainer(TContainer * , BaseTimings *);
                void                          preloadElement(TContainer *parent, QDomElement);

};

#endif // TCONTAINER_H
