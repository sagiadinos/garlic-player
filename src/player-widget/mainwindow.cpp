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
    connect(MyLibFacade, SIGNAL(newIndex(QList<Region> *)), this, SLOT(setRegions(QList<Region> *)));
    setCursor(Qt::BlankCursor);
    setCentralWidget(centralWidget);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setMainWindowSize(QSize(980, 540)); // set default
}

MainWindow::~MainWindow()
{
    deleteRegionsAndLayouts();
    delete centralWidget;
}


void MainWindow::deleteRegionsAndLayouts()
{
    qDeleteAll(ar_regions);
    ar_regions.clear();
}

void MainWindow::setRegions(QList<Region> *region_list)
{
    deleteRegionsAndLayouts();
    setStyleSheet("background-color:"+MyLibFacade->getHead()->getRootBackgroundColor()+";");
    QMap<QString, TRegion *>::iterator j;
    for (int i = 0; i < region_list->length(); i++)
    {
        j = ar_regions.insert(region_list->at(i).regionName, new TRegion(this));
        ar_regions[j.key()]->setRegion(region_list->at(i));
        ar_regions[j.key()]->setRootSize(width(), height());
        ar_regions[j.key()]->show();
    }
    qDebug() << ar_regions.size() << " regions are created ";
    MyLibFacade->beginSmilParsing(); // parse not before Layout ist build to prevent crash in MainWindow::startShowMedia
}

QString MainWindow::selectRegion(QString region_name)
{
    QMap<QString, TRegion *>::iterator i;
    if (!ar_regions.contains(region_name))
        i = ar_regions.begin();
    else
       i = ar_regions.find(region_name);
    return i.key();
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

void MainWindow::resizeEvent(QResizeEvent * event)
{
    if (ar_regions.size() > 0)
    {
        Q_UNUSED(event);
        QMap<QString, TRegion *>::iterator i;
        for (i = ar_regions.begin(); i != ar_regions.end(); ++i)
            ar_regions[i.key()]->setRootSize(width(), height());
    }
}

// =================== protected slots ====================================

void MainWindow::startShowMedia(TMedia *media)
{
    QString region_name = selectRegion(media->getRegion());
    ar_regions[region_name]->startShowMedia(media);
    ar_regions[region_name]->setRootSize(width(), height());
    return;
}

void MainWindow::stopShowMedia(TMedia *media)
{
    QString region_name = selectRegion(media->getRegion());
    ar_regions[region_name]->stopShowMedia();
    return;
}
