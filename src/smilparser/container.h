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

#ifndef TPLAYLIST_H
#define TPLAYLIST_H

#include "smilparser/base_timing.h"

/**
 * @brief The TBasePlaylist is an abstract class for Smil Playlists which is inherited by seq, par and excl.
 *        This class has functions for handling and media content. In case of media or playlist it returns the type and
 *        the actual DomElement, so that the caller can handle it
 *
 */
class TContainer : public TBaseTiming
{
    Q_OBJECT
public:
            TContainer(TBaseTiming * parent = 0);
    virtual void                          next(TBaseTiming *ended_element) = 0;
            void                          insertPlaylistObject(QString id, TBaseTiming *obj_element);
            QHash<QString, TBaseTiming *>       getPlaylistObjects();
            QString                       getIdOfActiveElement();
    virtual bool                          isChildPlayable(TBaseTiming *element) = 0;
            bool                          hasPlayingChilds();
            void                          childStarted(TBaseTiming *element);
            void                          childEnded(TBaseTiming *element);
            QString                       getBaseType() {return "playlist";}
            TBaseTiming                        *getChildElementFromList();
            void                          setPlayedElement(TBaseTiming *element);
            TBaseTiming                        *getPlayedElement();
            void                          setChildActive(bool active);
public slots:
            void                          emitfinished();
protected:
            TContainer                   *parent_container;
            QSet<TBaseTiming *>           activatable_childs;
            QSet<TBaseTiming *>::iterator childs_iterator;
            bool                          is_child_active    = false;
            TBaseTiming                  *played_element;
            QList<QDomElement>            dom_list;
            QList<QDomElement>::iterator  iterator;
            QHash<QString, TBaseTiming *> ar_elements;
            QDomElement                   active_element;
            void                          reactByTag();
signals:
            void                          foundElement(TContainer *, QDomElement );
            void                          startedPlaylist(TContainer * , TBaseTiming *);
            void                          finishedPlaylist(TContainer * , TBaseTiming *);

};

#endif // TPLAYLIST_H
