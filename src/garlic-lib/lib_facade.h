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

#include "files/index_manager.h"
#include "files/media_manager.h"
#include "smilparser/head_parser.h"
#include "smil.h"
#include "body_parser.h"
#include "tools/resource_monitor.h"
#include "head/placeholder.h"

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
        void               init(MainConfiguration *config);
        MainConfiguration *getConfiguration() const {return MyConfiguration.data();}
        HeadParser        *getHead() const {return MyHeadParser.data();}
        void               setConfigFromExternal(QString config_path, bool restart_smil_parsing = true);
        void               toggleLauncher(bool value){has_launcher = value;}

        void               transferAccessKey(QChar key);
        void               reloadWithNewIndex(QString index_path);
        void               beginSmilPlaying();
        QString            requestLoaddableMediaPath(QString path);
        void               shutDownParsing();
    public slots:
        void               initParser();
    protected:
        int                                     resource_monitor_timer_id;
        bool                                    has_launcher = false;
        QScopedPointer<DB::InventoryTable>      MyInventoryTable;
        QScopedPointer<ElementsContainer>       MyElementsContainer;
        QScopedPointer<ElementFactory>          MyElementFactory;
        QScopedPointer<CurrentPlayingMedia>     MyCurrentPlayingMedia;
        QScopedPointer<MainConfiguration>       MyConfiguration;
        QScopedPointer<MediaModel>              MyMediaModel;
        QScopedPointer<DownloadQueue>           MyDownloadQueue;
        QScopedPointer<Files::IndexManager>     MyIndexManager ;
        QScopedPointer<Files::MediaManager>     MyMediaManager;
        QScopedPointer<HeadParser>              MyHeadParser;
        QScopedPointer<SmilHead::TaskScheduler> MyTaskScheduler;
        QScopedPointer<SmilHead::PlaceHolder>   MyPlaceHolder;
        QScopedPointer<SmilHead::XMLConfiguration> MyXMLConfiguration;
        QScopedPointer<BodyParser>              MyBodyParser;
        ResourceMonitor                         MyResourceMonitor;
        void               initFileManager();
        void               processHeadParsing();
        void               timerEvent(QTimerEvent *event);

    protected slots:
        void               loadIndex();
        void               changeConfig();
        void               emitInstallSoftware(QString file_path);
        void               reboot(QString task_id);
        void               emitStartShowMedia(BaseMedia *media);
        void               emitStopShowMedia(BaseMedia *media);
        void               processBodyParsing();
    private:
        QScopedPointer<Smil> MySmil;
    signals:
        void               startShowMedia(BaseMedia *media);
        void               stopShowMedia(BaseMedia *media);
        void               readyForPlaying();
        void               newConfig();
        void               rebootOS(QString task_id);
        void               installSoftware(QString file_path);
};

#endif // LIB_FACADE_H
