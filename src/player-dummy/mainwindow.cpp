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

#include "mainwindow.h"

MainWindow::MainWindow(LibFacade *lib_facade, QWidget *parent) :   QMainWindow(parent)
{
    MyLibFacade            = lib_facade;
    connect(MyLibFacade, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(newIndexLoaded()), this, SLOT(deleteRegionsAndLayouts()));
    connect(MyLibFacade, SIGNAL(newIndexPrepared(QList<Region> *)), this, SLOT(setRegions(QList<Region> *)));
}

MainWindow::~MainWindow()
{
    delete MyLibFacade;
}

void MainWindow::openDebugInfos()
{
    DebugInfos MyDebugInfos(MyLibFacade);
    if (MyDebugInfos.exec() == QDialog::Rejected)
        exit(0);
}

// =================== protected slots ====================================

void MainWindow::deleteRegionsAndLayouts()
{
    MyLibFacade->prepareNewLoadedIndex();
}


void MainWindow::setRegions(QList<Region> *region_list)
{
    Q_UNUSED(region_list);
    MyLibFacade->beginSmilBodyParsing(); // parse not before Layout ist build to prevent crash in MainWindow::startShowMedia
}


void MainWindow::startShowMedia(TMedia *media)
{
    if (media->objectName() == "TVideo" || media->objectName() == "TAudio")
    {
        MyMedia = media;
        QTimer::singleShot(2000, this, SLOT(finishMedia()));
    }
}

void MainWindow::stopShowMedia(TMedia *media)
{
    Q_UNUSED(media);
    MyMedia = NULL; // prevent crash look at finish Media
}

void MainWindow::finishMedia()
{
     if (MyMedia != NULL) // prevent crash when playlist changed kills MyMedia-Video, but QTimer::singleShot wants to finish again
     {
        MyMedia->finishedSimpleDuration();
     }
}

