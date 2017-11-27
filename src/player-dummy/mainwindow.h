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

#include <QMainWindow>
#include "../player-common/debug_infos.h"

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(LibFacade *lib_facade, QWidget *parent = 0);
        ~MainWindow();
        void                      openDebugInfos();
    protected:
        LibFacade                *MyLibFacade      = Q_NULLPTR;
        DebugInfos               *MyDebugInfos;
        QPointer<TMedia>          MyMedia;
        QString                   start_time;
        void                      outputResourcesUsage();
        void                      createRegions();
        void                      deleteRegions();
        Logger&                   MyLogger = Logger::getInstance();

    protected slots:
        void                      prepareParsing();
        void                      startShowMedia(TMedia *media);
        void                      stopShowMedia(TMedia *media);
        void                      finishMedia();

};

#endif // MAINWINDOW_H
