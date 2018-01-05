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
#include "smilparser/head.h"
#include "smilparser.h"

#include "system_infos/memory.h"
#include "system_infos/general.h"

/**
 * @brief The LibFacade class is the interface for a player component to the garlic parser
 *
 * It emits 4 signals:
 * Two for begin an start a meida playback
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
        THead             *getHead() const;
        void               beginSmilBodyParsing();
        void               checkForNewSmilIndex();

    public slots:
        void               loadIndex();
        void               emitStartShowMedia(TMedia *media);
        void               emitStopShowMedia(TMedia *media);

    protected:
        int                 timer_id;
        QScopedPointer<DB::InventoryTable> MyInventoryTable;
        QScopedPointer<TConfiguration>     MyConfiguration;
        QScopedPointer<MediaModel>         MyMediaModel;
        QScopedPointer<DownloadQueue>      MyDownloadQueue;
        QScopedPointer<IndexManager>       MyIndexManager ;
        QScopedPointer<MediaManager>       MyMediaManager;
        QScopedPointer<THead>              MyHead;
        QScopedPointer<TSmil>              MySmil;
        qint64               max_memory_used = 0;
        QString              max_memory_time = "";
        qint64               max_threads_used = 0;
        SystemInfos::Memory  MyMemoryInfos;
        SystemInfos::General MyGeneralInfos;
        void                timerEvent(QTimerEvent *event);

    signals:
        void                startShowMedia(TMedia *media);
        void                stopShowMedia(TMedia *media);
        void                newIndexLoaded();
};

#endif // LIB_FACADE_H
