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

#include "smilparser/base.h"

/**
 * @brief The TBasePlaylist is an abstract class for Smil Playlists which is inherited by seq, par and excl.
 *        This class has functions for handling and media content. In case of media or playlist it returns the type and
 *        the actual DomElement, so that the caller can handle it
 *
 */
class TPlaylist : public TBase
{
    Q_OBJECT
public:
            TPlaylist(TBase * parent = 0);
            void                          insertPlaylistObject(QString id, TBase *obj_element);
            QHash<QString, TBase *>       getPlaylistObjects();
            QString                       getIdOfActiveElement();
            TBase                        *getPlayedElement();
            void                          setPlayedElement(TBase *element);
    virtual void                          childStarted(TBase *element) = 0;
    virtual void                          childEnded(TBase *element) = 0;
            QString                       getBaseType() {return "playlist";}
public slots:
            void                          emitfinished();
protected:
            QList<QDomElement>            ar_playlist;
            QList<QDomElement>::iterator  iterator;
            QHash<QString, TBase *>       ar_elements;
            QDomElement                   active_element;
            TBase                        *parent_playlist;
            TBase                        *played_element;
            QString                       found_tag;
            void                          reactByTag();
            void                          doRef();
            void                          doImage();
            void                          doVideo();
            void                          doAudio();
            void                          doWeb();
            void                          doPrefetch();
            void                          doSeq();
            void                          doPar();
            void                          doExcl();
signals:
            void                          foundElement(TBase *, QDomElement element);
            void                          startedPlaylist(TBase * , TBase *);
            void                          finishedPlaylist(TBase * , TBase *);

};

#endif // TPLAYLIST_H
