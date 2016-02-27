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

#ifndef SMILPARSER_H
#define SMILPARSER_H

#include <smilparser/file.h>
#include <smilparser/head.h>
#include <smilparser/factory.h>
#include <QSet>

/**
 * @brief   The TSmil class is the interface between playlists,  medias and
 *          the communication with the graphical output in mainwindow
 *          It is emits a signal which informs about region and media
 *          has to be played.
 *
 */
class TSmil : public QObject
{
    Q_OBJECT
public:
    TSmil(QObject *parent = 0);
    ~TSmil();
    void                               init(QString smil_index = "");
    THead                              getHeader();
    void                               beginSmilParsing();
protected:
    QDomElement                        parser;
    QString                            index_path;
    QList<Region>                      region_list;
    TFile                              MyFile;
    THead                              MyHead;
    TBody                             *MyBody;
    QHash<QString, TBase *>            ar_elements;
    showImg                            ImgAttr;
protected slots:
    void                               foundElement(TContainer *parent, QDomElement dom_element);
    void                               startElement(TContainer *parent, TBase *element);
    void                               finishElement(TContainer *parent, TBase *element);

    void                               resumeElement(TBase *element);
    void                               pauseElement(TBase *element);
    void                               stopElement(TBase *element, bool kill_timer);
private:
    QSet<TBase *>                      ar_played_media;
    QHash<QString, TBase *>::iterator  insertIntoObjectContainer(TBase *parent, TBase *child);
    void                               emitStartShowMedia(TMedia *media);
    void                               emitStopShowMedia(TMedia *media);
    void                               connectMediaSlots(TMedia *MyMedia);
    void                               connectPlaylistSlots(TBase *element);

signals:
    void                               startShowMedia(TMedia *media);
    void                               stopShowMedia(TMedia *media);
};

#endif // SMILPARSER_H
