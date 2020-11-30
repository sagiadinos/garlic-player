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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQuickView>
#include <QMap>

#include "region.h"
#include "../player-common/configdialog.h"
#include "../player-common/screen.h"
#include "../player-common/debug_infos.h"
#include "../player-common/interactions.h"
#include "../player-common/player_configuration.h"
#include "files/index_manager.h"
#include "files/media_manager.h"
#if defined  Q_OS_ANDROID
    #include <QtAndroidExtras>
#endif

#include "lib_facade.h"

class MainWindow : public QQuickView
{
    Q_OBJECT
    public:
        MainWindow(TScreen *screen, LibFacade *lib_facade, PlayerConfiguration *pc);
        ~MainWindow();
        void                      init();
        int                       openConfigDialog();
        void                      resizeAsNormalFullScreen();
        void                      resizeAsBigFullScreen();
        void                      resizeAsWindow();
        void                      setMainWindowSize(QSize size);
        QSize                     getMainWindowSize();

    protected:
        const int                 WINDOWED      = 0;
        const int                 FULLSCREEN    = 1;
        const int                 BIGFULLSCREEN = 2;
        Interactions             *MyInteractions;
        LibFacade                *MyLibFacade      = Q_NULLPTR;
        PlayerConfiguration      *MyPlayerConfiguration      = Q_NULLPTR;
        TScreen                  *MyScreen;
        static LibFacade         *MyStaticLibFacade;
        QMap<QString, TRegion *>  regions_list;
        QSize                     mainwindow_size;
        int                       screen_state = 0;
        QTime                     start_touch_time;
        void                      openDebugInfos();
        QString                   selectRegion(QString region_name);
        void                      resizeEvent(QResizeEvent * event);
        void                      keyPressEvent(QKeyEvent *ke);
        bool                      event(QEvent *event);
        void                      createRegions();
        void                      deleteRegions();
        void                      sendClosePlayerCorrect();
        void                      quitApplication();
    protected slots:
        void                      prepareParsing();
        void                      startShowMedia(BaseMedia *media);
        void                      stopShowMedia(BaseMedia *media);
        void                      doStatusChanged(QQuickView::Status status);
        void                      sendConfig();
        void                      rebootOS();
        void                      installSoftware(QString file_path);
};

#endif // MAINWINDOW_H
