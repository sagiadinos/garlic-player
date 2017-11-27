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
    connect(MyLibFacade, SIGNAL(newIndexLoaded()), this, SLOT(prepareParsing()));
}

MainWindow::~MainWindow()
{
    delete MyLibFacade;
}

void MainWindow::openDebugInfos()
{
    if (MyDebugInfos->exec() == QDialog::Rejected)
        exit(0);
}

// =================== protected slots ====================================

void MainWindow::deleteRegions()
{
    MyLibFacade->prepareNewLoadedIndex();
}


void MainWindow::createRegions()
{
    // dummy
}

// =================== protected slots ====================================


void MainWindow::prepareParsing()
{
    deleteRegions(); // Must be done first to be clear that no media is loaded or played anymore
    MyLibFacade->prepareNewLoadedIndex();
    createRegions();
    MyLibFacade->beginSmilBodyParsing(); // begin parse not before Layout ist build to prevent crash in MainWindow::startShowMedia

    // Must opened here after parsing, otherwise it crashes with media
    // but had to prevent from open second debug dialog after new index
    if (MyDebugInfos == Q_NULLPTR)
    {
        MyDebugInfos = new DebugInfos(MyLibFacade);
        openDebugInfos();
    }
}



void MainWindow::startShowMedia(TMedia *media)
{
    start_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    MyDebugInfos->setCurrentFilePlayed(media);
    if (media->objectName() == "TVideo" || media->objectName() == "TAudio")
    {
        MyMedia = media;
        QTimer::singleShot(2000, this, SLOT(finishMedia()));
    }
}

void MainWindow::stopShowMedia(TMedia *media)
{
    // Log everything in Playlog
    qInfo(PlayLog).noquote() << MyLogger.createPlayLogEntry(start_time, media->getSrc());
    start_time = ""; // set times to Zero

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

