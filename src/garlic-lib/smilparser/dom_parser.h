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

#include "elements_container.h"
#include "current_playing_media.h"

/**
 * @brief   The TSmil class is the interface between container, media and
 *          the communication with the graphical output in mainwindow
 *          It is emits a signal which informs about media
 *          has to be played.
 *
 */
class DomParser : public QObject
{
    Q_OBJECT
public:
    explicit DomParser(MediaManager *mm, ElementsContainer *ec, QObject *parent = Q_NULLPTR);
    ~DomParser();
    void                               endSmilParsing();
    void                               beginSmilParsing(QDomElement body);

protected:
    QDomElement                        parser;
    QString                            index_path;
    QScopedPointer<TBody>              MyBody;
    MediaManager                      *MyMediaManager;
    bool                               stop_all = false;
    ElementsContainer                 *MyElementsContainer;
    CurrentPlayingMedia               *MyCurrentPlayingMedia;
    void                               stopAllPlayingMedia();
    BaseTimings                       *determineElements(TContainer *parent_container, QDomElement dom_element);
    void                               emitStartShowMedia(BaseMedia *media);
    void                               stopElement(BaseTimings *element);
    void                               emitStopShowMedia(BaseMedia *media);

    void                               connectSlots(BaseTimings *element);
    void                               connectMediaSlots(BaseMedia *media);
    void                               connectContainerSlots(TContainer *MyContainer);
protected slots:
    void                               foundElement(TContainer *parent_container, QDomElement dom_element);
    void                               startElement(TContainer *parent_container, BaseTimings *element);
    void                               finishElement(TContainer *parent_container, BaseTimings *element);

    void                               resumeQueuedElement(BaseTimings *element);
    void                               pausePlayingElement(BaseTimings *element);
    void                               stopPlayingElement(BaseTimings *element);

signals:
    void                               startShowMedia(BaseMedia *media);
    void                               stopShowMedia(BaseMedia *media);
};

#endif // SMILPARSER_H
