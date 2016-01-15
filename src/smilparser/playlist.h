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

struct TFoundTag
{
    QString     name;
    QDomElement dom_element;
};

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
               TPlaylist(QObject * parent = 0);
    TFoundTag  getFoundTag();
    QString    getType(){ return "base_playlist";}
    void       beginPlay(){}
public slots:
    void       emitfinished(){}
protected:
    QList<QDomElement>            ar_playlist;
    QList<QDomElement>::iterator  iterator;
    TFoundTag  found_tag;
    QObject   *parent_playlist;
    void       reactByTag();
    void       doRef();
    void       doImage();
    void       doVideo();
    void       doAudio();
    void       doWeb();
    void       doPrefetch();
    void       doSeq();
    void       doPar();
    void       doExcl();
protected slots:
    void play(){}
signals:
    void       foundElement(QObject *, TFoundTag found_tag);

};

#endif // TPLAYLIST_H
