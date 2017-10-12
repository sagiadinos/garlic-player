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
#include <QMap>

#include "head.h"
#include "region.h"
#include "video.h"
#include "../player-common/configdialog.h"
#include "../player-common/screen.h"
#include "index_manager.h"
#include "media_manager.h"
#include "smilparser.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TConfiguration *config, TScreen *screen);
    ~MainWindow();
public slots:
    void                      setSmilIndex();
    void                      checkForNewSmilIndex();
    void                      resizeEvent(QResizeEvent * event);
    void                      keyPressEvent(QKeyEvent *ke);
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
    QSize                     mainwindow_size;
    QMap<QString, TRegion *>  ar_regions;
    TSmil                    *MySmil          = Q_NULLPTR;
    Network                  *MyNetwork       = Q_NULLPTR;
    NetworkQueue             *MyNetworkQueue  = Q_NULLPTR;
    THead                    *MyHead          = Q_NULLPTR;
    IndexManager             *MyIndexManager  = Q_NULLPTR;
    MediaManager             *MyMediaManager  = Q_NULLPTR;
    MediaModel               *MyMediaModel    = Q_NULLPTR;
    TConfiguration           *MyConfiguration = Q_NULLPTR;
    TScreen                  *MyScreen;
    int                       screen_state = 0;
    void                      cleanUp();
    void                      deleteRegionsAndLayouts();
    void                      loadIndex();
    void                      setRegions(QDomElement head);
    QString                   selectRegion(QString region_name);
protected slots:
    void                      startShowMedia(TMedia *media);
    void                      stopShowMedia(TMedia *media);
};

#endif // MAINWINDOW_H
