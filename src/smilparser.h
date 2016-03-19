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

#include <smilparser/factory.h>
#include <QSet>

/**
 * @brief   The TSmil class is the interface between container, media and
 *          the communication with the graphical output in mainwindow
 *          It is emits a signal which informs about media
 *          has to be played.
 *
 */
class TSmil : public QObject
{
    Q_OBJECT
public:
    TSmil(TConfiguration *config, QObject *parent = 0);
    ~TSmil();
    void                               init();
    void                               clearLists();
    void                               beginSmilParsing(QDomElement body);
protected:
    QDomElement                        parser;
    QString                            index_path;
    TBody                             *MyBody;
    QHash<QString, TBaseTiming *>      ar_elements;
    TConfiguration                    *MyConfiguration;
    TFileManager                      *MyFileManager;
protected slots:
    void                               foundElement(TContainer *ParentContainer, QString type, QDomElement dom_element);
    void                               startElement(TContainer *parent, TBaseTiming *element);
    void                               finishElement(TContainer *parent, TBaseTiming *element);

    void                               resumeQueuedElement(TBaseTiming *element);
    void                               pausePlayingElement(TBaseTiming *element);
    void                               stopPlayingElement(TBaseTiming *element);
private:
    QSet<TBaseTiming *>                ar_played_media;
    QHash<QString, TBaseTiming *>::iterator  insertIntoObjectContainer(TBaseTiming *parent, TBaseTiming *child);
    void                               emitStartShowMedia(TMedia *MyMedia);
    void                               stopElement(TBaseTiming *element);
    void                               killTimer(TBaseTiming *element);
    void                               emitStopShowMedia(TMedia *media);
    void                               connectMediaSlots(TMedia *MyMedia);
    void                               connectContainerSlots(TContainer *MyContainer);

signals:
    void                               startShowMedia(TMedia *media);
    void                               stopShowMedia(TMedia *media);
};

#endif // SMILPARSER_H
