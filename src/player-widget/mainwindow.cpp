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

MainWindow::MainWindow(TScreen *screen, LibFacade *lib_facade)
{
    centralWidget          = new QWidget(this); // had to be there to get fullscreen simulation over multiple monitors
    MyScreen               = screen;
    MyLibFacade            = lib_facade;
    connect(MyLibFacade, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(newIndexLoaded()), this, SLOT(prepareParsing()));
    setCursor(Qt::BlankCursor);
    setCentralWidget(centralWidget);
//    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setMainWindowSize(QSize(980, 540)); // set default
}

MainWindow::~MainWindow()
{
    deleteRegions();
    delete centralWidget;
}


void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    if (!ke->modifiers().testFlag(Qt::ControlModifier))
        return;
    switch (ke->key())
    {
        case Qt::Key_F:
        if (screen_state != FULLSCREEN)
                resizeAsNormalFullScreen();
            else
                resizeAsWindow();
        break;
        case Qt::Key_B:
            if (screen_state != BIGFULLSCREEN)
                 resizeAsBigFullScreen();
            else
                resizeAsWindow();
        break;
        case Qt::Key_D:
            setCursor(Qt::ArrowCursor);
            openDebugInfos();
            setCursor(Qt::BlankCursor);
            break;
        case Qt::Key_C:
            setCursor(Qt::ArrowCursor);
            if (openConfigDialog() == QDialog::Accepted)
                MyLibFacade->checkForNewSmilIndex();
            setCursor(Qt::BlankCursor);
            break;
        case Qt::Key_Q:
             exit(0);
        break;
    }
}

void MainWindow::openDebugInfos()
{
    DebugInfos MyDebugInfos(MyLibFacade);
    MyDebugInfos.exec();
}

int MainWindow::openConfigDialog()
{
    ConfigDialog MyConfigDialog(0, MyLibFacade->getConfiguration());
    return MyConfigDialog.exec();
}

void MainWindow::resizeAsNormalFullScreen()
{
    setCursor(Qt::BlankCursor);
    screen_state = FULLSCREEN;
    move(MyScreen->getStartPointFromScreen());
    resize(MyScreen->getSizeFromScreen());
}

void MainWindow::resizeAsBigFullScreen()
{
    setCursor(Qt::BlankCursor);
    screen_state = BIGFULLSCREEN;
    move(0, 0);
    resize(MyScreen->getWholeSize());
}

void MainWindow::resizeAsWindow()
{
    setCursor(Qt::BlankCursor);
    screen_state = WINDOWED;
    move(MyScreen->getStartPointFromScreen());
    resize(getMainWindowSize());
}

void MainWindow::setMainWindowSize(QSize size)
{
    mainwindow_size = size;
}

QSize MainWindow::getMainWindowSize()
{
    return mainwindow_size;
}

// =================== protected methods ====================================


void MainWindow::resizeEvent(QResizeEvent * event)
{
    if (regions_list.size() > 0)
    {
        Q_UNUSED(event);
        QMap<QString, TRegion *>::iterator i;
        for (i = regions_list.begin(); i != regions_list.end(); ++i)
            regions_list[i.key()]->setRootSize(width(), height());
    }
}

QString MainWindow::selectRegion(QString region_name)
{
    QMap<QString, TRegion *>::iterator i;
    if (!regions_list.contains(region_name))
        i = regions_list.begin();
    else
       i = regions_list.find(region_name);
    return i.key();
}

void MainWindow::createRegions()
{
    QList<Region> *region_list = MyLibFacade->getHead()->getLayout();
    setStyleSheet("background-color:"+MyLibFacade->getHead()->getRootBackgroundColor()+";");
    QMap<QString, TRegion *>::iterator j;
    for (int i = 0; i < region_list->length(); i++)
    {
        j = regions_list.insert(region_list->at(i).regionName, new TRegion(this));
        regions_list[j.key()]->setRegion(region_list->at(i));
        regions_list[j.key()]->setRootSize(width(), height());
        regions_list[j.key()]->show();
    }
    qDebug() << regions_list.size() << " region(s) created";
}

void MainWindow::deleteRegions()
{
    int size = regions_list.size();
    qDeleteAll(regions_list);
    regions_list.clear();
    qDebug() << size << " region(s) deleted";
}

// =================== protected slots ====================================


void MainWindow::prepareParsing()
{
    deleteRegions(); // Must be done first to be clear that no media is loaded or played anymore
    MyLibFacade->prepareNewLoadedIndex();
    createRegions();
    MyLibFacade->beginSmilBodyParsing(); // begin parse not before Layout ist build to prevent crash in MainWindow::startShowMedia
}


void MainWindow::startShowMedia(TMedia *media)
{
    if (regions_list.size() > 0) // prevent to call functionx of deleted or not existing regions
    {
        QString region_name = selectRegion(media->getRegion());
        regions_list[region_name]->startShowMedia(media);
        regions_list[region_name]->setRootSize(width(), height());
    }
}

void MainWindow::stopShowMedia(TMedia *media)
{
    if (regions_list.size() > 0)// prevent to call function of deleted or nonexisting regions
        regions_list[selectRegion(media->getRegion())]->stopShowMedia();
}
