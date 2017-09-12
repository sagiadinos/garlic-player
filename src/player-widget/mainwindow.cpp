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

MainWindow::MainWindow(TFileManager *FileManager, TScreen *screen)
{
    QWidget *centralWidget = new QWidget(this); // had to be there to get fullscreen simulation over multiple monitors
    MyScreen        = screen;

    MyFileManager   = FileManager;
    MyIndexFile     = new TIndexManager(MyFileManager->getConfiguration());
    connect(MyIndexFile, SIGNAL(isLoaded()), this, SLOT(setSmilIndex()));
    MyHead          = new THead();
    connect(MyHead, SIGNAL(checkForNewIndex()), this, SLOT(checkForNewSmilIndex()));
    setCursor(Qt::BlankCursor);
    setCentralWidget(centralWidget);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setMainWindowSize(QSize(980, 540)); // set default
}

MainWindow::~MainWindow()
{
    delete MySmil;
    deleteRegionsAndLayouts(); // region must be deletetd at last, cause media pointer had to be deleted
}

/**
 * @briegetIndexPathsetSmilIndex is a slot which is activated, when an indexfile is ready on local hard disc for reading
 */
void MainWindow::setSmilIndex()
{
    qDebug(SmilParser) << "clear MySmil";
    if (ar_regions.size() > 0)
    {
        delete MySmil;
        deleteRegionsAndLayouts();
    }
    MySmil = new TSmil(MyFileManager);
    connect(MySmil, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MySmil, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    MySmil->init();
    setRegions(MyIndexFile->getHead());
    setGeometry(0,0, width(), height());
    MySmil->beginSmilParsing(MyIndexFile->getBody());
}

/**
 * @brief MainWindow::refreshSmilIndex is a slot which is called when THead send a signal to check for a new Smil-Index (refresh)
 */
void MainWindow::checkForNewSmilIndex()
{
    MyIndexFile->load();
}


void MainWindow::deleteRegionsAndLayouts()
{
    qDeleteAll(ar_regions);
    ar_regions.clear();
}

void MainWindow::setRegions(QDomElement head)
{
    MyHead->parse(head);
    setStyleSheet("background-color:"+MyHead->getRootBackgroundColor()+";");
    QList<Region>  region_list = MyHead->getLayout();
    QMap<QString, TRegion *>::iterator j;
    for (int i = 0; i < region_list.length(); i++)
    {
        j = ar_regions.insert(region_list.at(i).regionName, new TRegion(this));
        ar_regions[j.key()]->setRegion(region_list.at(i));
        ar_regions[j.key()]->setRootSize(width(), height());
        ar_regions[j.key()]->show();
    }
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

void MainWindow::startShowMedia(TMedia *media)
{
    QString type   = media->objectName();

   if (type == "TImage")
        playImage(qobject_cast<TImage *>(media));
    else if (type == "TVideo")
        playVideo(qobject_cast<TVideo *>(media));
    else if (type == "TAudio")
        playAudio(qobject_cast<TAudio *>(media));
    else if (type == "TWeb")
        playWeb(qobject_cast<TWeb *>(media));
    return;
}

void MainWindow::stopShowMedia(TMedia *media)
{
    QString type        = media->objectName();
    if (type == "TImage")
        removeImage(qobject_cast<TImage *>(media));
    else if (type == "TVideo")
        removeVideo(qobject_cast<TVideo *>(media));
    else if (type == "TWeb")
        removeWeb(qobject_cast<TWeb *>(media));
    return;
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
    case Qt::Key_C:
        setCursor(Qt::ArrowCursor);
        if (openConfigDialog() == QDialog::Accepted)
            checkForNewSmilIndex();
        setCursor(Qt::BlankCursor);
        break;
        case Qt::Key_Q:
            exit(0);
        break;
    }
}

int MainWindow::openConfigDialog()
{
    ConfigDialog MyConfigDialog(0, MyFileManager->getConfiguration());
    return MyConfigDialog.exec();
}

void MainWindow::resizeAsNormalFullScreen()
{
    screen_state = FULLSCREEN;
    move(MyScreen->getStartPointFromScreen());
    resize(MyScreen->getSizeFromScreen());
}

void MainWindow::resizeAsBigFullScreen()
{
    screen_state = BIGFULLSCREEN;
    move(0, 0);
    resize(MyScreen->getWholeSize());
}

void MainWindow::resizeAsWindow()
{
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

void MainWindow::playImage(TImage *MyImage)
{
    QString region_name = selectRegion(MyImage->getRegion());
    ar_regions[region_name]->playImage(MyImage);
    ar_regions[region_name]->setRootSize(width(), height());
}

void MainWindow::playVideo(TVideo *MyVideo)
{
    QString region_name = selectRegion(MyVideo->getRegion());
    ar_regions[region_name]->playVideo(MyVideo);
    ar_regions[region_name]->setRootSize(width(), height());
}

void MainWindow::playAudio(TAudio *MyAudio)
{
    ar_regions[selectRegion(MyAudio->getRegion())]->playAudio(MyAudio);
}

void MainWindow::playWeb(TWeb *MyWeb)
{
    QString region_name = selectRegion(MyWeb->getRegion());
    ar_regions[region_name]->playWeb(MyWeb);
    ar_regions[region_name]->setRootSize(width(), height());
}

void MainWindow::removeImage(TImage *MyImage)
{
    ar_regions[selectRegion(MyImage->getRegion())]->removeImage();
}

void MainWindow::removeVideo(TVideo *MyVideo)
{
    ar_regions[selectRegion(MyVideo->getRegion())]->removeVideo();
}

void MainWindow::removeWeb(TWeb *MyWeb)
{
    ar_regions[selectRegion(MyWeb->getRegion())]->removeWeb();
}

