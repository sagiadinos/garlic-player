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
        MainConfiguration *getConfiguration() const;
        HeadParser        *getHead() const;
        void               setConfigFromExternal(QString config_path, bool restart_smil_parsing = true);
        void               setIndexFromExternal(QString index_path);
        void               beginSmilBodyParsing();
        QString            requestLoaddableMediaPath(QString path);
        // Interactions
        void               nextSmilMedia(int zone = 1);
        void               previousSmilMedia(int zone = 1);
        void               jumpToSmilMedia(int position, int zone = 1);
        void               shutDownParsing();
public slots:
        void               initParser();
protected:
        int                                     resource_monitor_timer_id;
        QScopedPointer<DB::InventoryTable>      MyInventoryTable;
        QScopedPointer<ElementsContainer>       MyElementsContainer;
        QScopedPointer<CurrentPlayingMedia>     MyCurrentPlayingMedia;
        QScopedPointer<MainConfiguration>       MyConfiguration;
        QScopedPointer<MediaModel>              MyMediaModel;
        QScopedPointer<DownloadQueue>           MyDownloadQueue;
        QScopedPointer<Files::IndexManager>     MyIndexManager ;
        QScopedPointer<Files::MediaManager>     MyMediaManager;
        QScopedPointer<HeadParser>              MyHeadParser;
        QScopedPointer<SmilHead::TaskScheduler> MyTaskScheduler;
        QScopedPointer<SmilHead::XMLConfiguration> MyXMLConfiguration;
        QScopedPointer<BodyParser>              MyBodyParser;
        ResourceMonitor                         MyResourceMonitor;
        void               initInventoryDataTable();
        void               initFileManager();
        void               processHeader();
        void               checkForNewSmilIndex();
        void               timerEvent(QTimerEvent *event);

    protected slots:
        void               loadIndex();
        void               emitInstallSoftware(QString file_path);
        void               reboot();
        void               emitStartShowMedia(BaseMedia *media);
        void               emitStopShowMedia(BaseMedia *media);

    signals:
        void               startShowMedia(BaseMedia *media);
        void               stopShowMedia(BaseMedia *media);
        void               newIndexLoaded();
        void               rebootOS();
        void               installSoftware(QString file_path);
};

#endif // LIB_FACADE_H
