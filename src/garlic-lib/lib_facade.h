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

#ifdef QT_DEBUG
    #include "system_infos/memory.h"
    #include "system_infos/general.h"
#endif

class LibFacade : public QObject
{
        Q_OBJECT
    public:
        explicit LibFacade(TConfiguration *config, QObject *parent = nullptr);
        ~LibFacade();
        void               initIndex();
        TConfiguration    *getConfiguration() const;
        THead             *getHead() const;
        void               beginSmilParsing();
        TMedia            *getCurrentMedia();

    public slots:
        void               checkForNewSmilIndex();
        void               emitStartShowMedia(TMedia *media);
        void               emitStopShowMedia(TMedia *media);
        void               emitNewIndex(QList<Region> *region);

    protected:
        int                 timer_id;
        TMedia             *current_media;
        TConfiguration     *MyConfiguration = Q_NULLPTR;
        IndexManager       *MyIndexManager  = Q_NULLPTR;
        THead              *MyHead          = Q_NULLPTR;
        TSmil              *MySmil          = Q_NULLPTR;
        void                createParsingObjects();
        void                deleteParsingObjects();
#ifdef QT_DEBUG
        qint64               max_memory_used = 0;
        qint64               max_threads_used = 0;
        SystemInfos::Memory  MyMemoryInfos;
        SystemInfos::General MyGeneralInfos;
        void                timerEvent(QTimerEvent *event);
#endif
    signals:
        void                startShowMedia(TMedia *media);
        void                stopShowMedia(TMedia *media);
        void                newIndex(QList<Region> *region);
    protected slots:
        void setSmilIndex();
};

#endif // LIB_FACADE_H
