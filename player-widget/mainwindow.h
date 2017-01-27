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

#include <QWidget>
#include <QMap>
#include <QLayout>
#include "region.h"
#include "smilparser/index_file.h"
#include "smilparser/head.h"
#include "files/downloader.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void                      setInitialSmilIndex(TConfiguration *Configuration);
public slots:
    void                      setSmilIndex();
    void                      checkForNewSmilIndex();
    void                      startShowMedia(TMedia *media);
    void                      stopShowMedia(TMedia *media);
    void                      keyPressEvent(QKeyEvent *ke);
protected:
    QMap<QString, TRegion *>  ar_regions;
    TSmil                    *MySmil          = NULL;
    THead                    *MyHead          = NULL;
    TIndexFile               *MyIndexFile     = NULL;
    TConfiguration           *MyConfiguration = NULL;
    QHBoxLayout              *layout;
    QString                   smil_index_path;
    void                      deleteRegionsAndLayouts();
    void                      loadIndex();
    void                      setRegions(QDomElement head);
    QString                   selectRegion(QString region_name);
    void                      resizeEvent(QResizeEvent * event);
    void                      playImage(TImage *MyImage);
    void                      playVideo(TVideo *MyVideo);
    void                      playAudio(TAudio *MyAudio);
    void                      playWeb(TWeb *MyWeb);
    void                      removeImage(TImage *MyImage);
    void                      removeVideo(TVideo *MyVideo);
    void                      removeWeb(TWeb *MyWeb);
};

#endif // MAINWINDOW_H
