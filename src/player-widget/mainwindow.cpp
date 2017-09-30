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

MainWindow::MainWindow(TConfiguration *config, TScreen *screen)
{
    QWidget *centralWidget = new QWidget(this); // had to be there to get fullscreen simulation over multiple monitors
    MyScreen               = screen;
    MyConfiguration        = config;
    QByteArray user_agent(MyConfiguration->getUserAgent().toUtf8());
    MyIndexManager         = new IndexManager(new IndexModel, MyConfiguration, new Network(user_agent));
    connect(MyIndexManager, SIGNAL(availableIndex()), this, SLOT(setSmilIndex()));
    MyMediaManager         = new MediaManager(new MediaModel, MyConfiguration, new NetworkQueue(user_agent));

    MyIndexManager->init(MyConfiguration->getIndexUri());
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
    MyHead          = new THead();
    connect(MyHead, SIGNAL(checkForNewIndex()), this, SLOT(checkForNewSmilIndex()));
    MySmil = new TSmil(MyMediaManager);
    connect(MySmil, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MySmil, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    MySmil->init();
    setRegions(MyIndexManager->getHead());
    setGeometry(0,0, width(), height());
    MySmil->beginSmilParsing(MyIndexManager->getBody());
}

/**
 * @brief MainWindow::refreshSmilIndex is a slot which is called when THead send a signal to check for a new Smil-Index (refresh)
 */
void MainWindow::checkForNewSmilIndex()
{
    MyIndexManager->lookUpForIndex();
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
    ConfigDialog MyConfigDialog(0, MyConfiguration);
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
    QString type   = media->objectName();
    QString region_name = selectRegion(media->getRegion());

    // ToDo change with something more elegant

   if (type == "TImage")
       ar_regions[region_name]->playImage(qobject_cast<TImage *>(media));
    else if (type == "TVideo")
       ar_regions[region_name]->playVideo(qobject_cast<TVideo *>(media));
    else if (type == "TAudio")
       ar_regions[region_name]->playAudio(qobject_cast<TAudio *>(media));
    else if (type == "TWeb")
       ar_regions[region_name]->playWeb(qobject_cast<TWeb *>(media));

  if (type != "TAudio")
      ar_regions[region_name]->setRootSize(width(), height());
   return;
}

void MainWindow::stopShowMedia(TMedia *media)
{
    QString type        = media->objectName();
    QString region_name = selectRegion(media->getRegion());
    if (type == "TImage")
        ar_regions[region_name]->removeImage();
    else if (type == "TVideo")
        ar_regions[region_name]->removeVideo();
    else if (type == "TWeb")
        ar_regions[region_name]->removeWeb();
    return;
}
