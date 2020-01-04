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
#ifndef LIB_FACADE_H
#define LIB_FACADE_H

#include <QObject>
#include "files/index_manager.h"
#include "files/media_manager.h"
#include "smilparser/head_parser.h"
#include "body_parser.h"
#include "tools/resource_monitor.h"

/**
 * @brief The LibFacade class is the interface for a player component to the garlic parser
 *
 * It emits 4 signals:
 * Two for begin to start a media playback
 *       void                startShowMedia(TMedia *media);
 *       void                stopShowMedia(TMedia *media);
 *
 * and two for let the player component clear
 *       void                newIndexLoaded();
 *       void                newIndexPrepared(QList<Region> *region);
 *
 */
class LibFacade : public QObject
{
        Q_OBJECT
    public:
        explicit LibFacade(QObject *parent = nullptr);
        ~LibFacade();
        void               init();
        TConfiguration    *getConfiguration() const;
        HeadParser             *getHead() const;
        void               beginSmilBodyParsing();
        void               checkForNewSmilIndex();
        // Interactions
        void               playNextSmilElement();
        void               playPreviousSmilElement();
        void               playSmilElement(int position, int zone = 1);

    public slots:
        void               loadIndex();
        void               emitStartShowMedia(BaseMedia *media);
        void               emitStopShowMedia(BaseMedia *media);

    protected:
        int                 timer_id;
        QScopedPointer<DB::InventoryTable>  MyInventoryTable;
        QScopedPointer<ElementsContainer>   MyElementsContainer;
        QScopedPointer<CurrentPlayingMedia> MyCurrentPlayingMedia;
        QScopedPointer<TConfiguration>      MyConfiguration;
        QScopedPointer<MediaModel>          MyMediaModel;
        QScopedPointer<DownloadQueue>       MyDownloadQueue;
        QScopedPointer<IndexManager>        MyIndexManager ;
        QScopedPointer<MediaManager>        MyMediaManager;
        QScopedPointer<HeadParser>          MyHeadParser;
        QScopedPointer<BodyParser>          MyBodyParser;
        ResourceMonitor                     MyResourceMonitor;
        void               timerEvent(QTimerEvent *event);
    signals:
        void               startShowMedia(BaseMedia *media);
        void               stopShowMedia(BaseMedia *media);
        void               newIndexLoaded();
};

#endif // LIB_FACADE_H
