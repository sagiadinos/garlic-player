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
    MyInteractions         = new Interactions(lib_facade, this);
    MyScreen               = screen;
    MyLibFacade            = lib_facade;
    connect(MyLibFacade, SIGNAL(startShowMedia(BaseMedia *)), this, SLOT(startShowMedia(BaseMedia *)));
    connect(MyLibFacade, SIGNAL(stopShowMedia(BaseMedia *)), this, SLOT(stopShowMedia(BaseMedia *)));
    connect(MyLibFacade, SIGNAL(newIndexLoaded()), this, SLOT(prepareParsing()));
    setCursor(Qt::BlankCursor);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    deleteRegions();
    delete centralWidget;
}


void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    if (!ke->modifiers().testFlag(Qt::ControlModifier))
    {
      MyInteractions->handleKeyPress(ke);
      return;
    }
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
            openCommandline();
            setCursor(Qt::BlankCursor);
            break;
        case Qt::Key_S:
            setCursor(Qt::ArrowCursor);
            if (openConfigDialog() == QDialog::Accepted)
            {
                MyLibFacade->initParser();
            }
            setCursor(Qt::BlankCursor);
            break;
#ifdef SUPPORT_EMBEDDED
        case Qt::Key_N:
            setCursor(Qt::ArrowCursor);
            if (openNetworkDialog() == QDialog::Accepted)
                MyLibFacade->checkForNewSmilIndex();
            setCursor(Qt::BlankCursor);
            break;
#endif
        case Qt::Key_Q:
             QApplication::quit();

        break;
    }
}

void MainWindow::openDebugInfos()
{
    DebugInfos MyDebugInfos(MyLibFacade);
    MyDebugInfos.exec();
}

void MainWindow::openCommandline()
{
    QProcess process;
    process.start("/bin/bash", QStringList() << "mc");
    process.waitForFinished(-1); // will wait forever until finished
}

#ifdef SUPPORT_EMBEDDED
int MainWindow::openNetworkDialog()
{
    NetworkDialog MyNetworkDialog(0, MyLibFacade->getConfiguration());
    return MyNetworkDialog.exec();
}
#endif

int MainWindow::openConfigDialog()
{
    ConfigDialog MyConfigDialog(0, MyLibFacade->getConfiguration());
    return MyConfigDialog.exec();
}

void MainWindow::resizeAsNormalFullScreen()
{
    setCursor(Qt::BlankCursor);
    showFullScreen();
    screen_state = FULLSCREEN;
}

void MainWindow::resizeAsBigFullScreen()
{
    // To work correct a taskbar had to set for overlap or hidden
    move(0,0);
    setWindowFlags(Qt::CustomizeWindowHint  | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setCursor(Qt::BlankCursor);
    showNormal();

    resize(MyScreen->getWholeSize());
    screen_state = BIGFULLSCREEN;
}

void MainWindow::resizeAsWindow()
{
    MyScreen->determineCurrentScreenId(this);
    move(MyScreen->getStartPointFromCurrentScreen());
    setWindowFlags(Qt::Window);
    setCursor(Qt::ArrowCursor);

    // This nonsense workaround prevent that MainWindow showed maximized after full- or bigscreen.
    showNormal();
    setWindowState(Qt::WindowMaximized);
    showNormal();
    resize(QSize(980, 540));

    screen_state = WINDOWED;
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
        QMap<QString, TRegion *>::iterator i;
        for (i = regions_list.begin(); i != regions_list.end(); ++i)
            regions_list[i.key()]->setRootSize(event->size().width(), event->size().height());
    }
    QMainWindow::resizeEvent(event);
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
    if (size == 0) // prevent to call functionx of deleted or not existing regions
    {
        return;
    }
    qDeleteAll(regions_list);
    regions_list.clear();
    qDebug() << size << " region(s) deleted";
}

// =================== protected slots ====================================


void MainWindow::prepareParsing()
{
    deleteRegions(); // Must be done first to be clear that no media is loaded or played anymore

    createRegions();
    MyLibFacade->beginSmilBodyParsing(); // begin parse not before Layout ist build to prevent crash in MainWindow::startShowMedia
}


void MainWindow::startShowMedia(BaseMedia *media)
{
    if (regions_list.size() > 0) // prevent to call functionx of deleted or not existing regions
    {
        QString region_name = selectRegion(media->getRegion());
        regions_list[region_name]->startShowMedia(media);
        regions_list[region_name]->setRootSize(width(), height());
    }
}

void MainWindow::stopShowMedia(BaseMedia *media)
{
    if (regions_list.size() > 0)// prevent to call function of deleted or nonexisting regions
        regions_list[selectRegion(media->getRegion())]->stopShowMedia();
}
