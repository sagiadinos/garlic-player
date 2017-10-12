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
    connect (this, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(doStatusChanged(QQuickView::Status)));
    MyScreen               = screen;
    MyConfiguration        = config;
    MyNetwork              = new Network(MyConfiguration->getUserAgent().toUtf8());
    MyIndexManager         = new IndexManager(new IndexModel(this), MyConfiguration, MyNetwork, this);
    connect(MyIndexManager, SIGNAL(availableIndex()), this, SLOT(setSmilIndex()));

    MyIndexManager->init(MyConfiguration->getIndexUri());

#ifdef SUPPORT_QTAV
    setSource(QUrl(QStringLiteral("qrc:/root_qtav.qml")));
#else
    setSource(QUrl(QStringLiteral("qrc:/root_qtm.qml")));
#endif
    setMainWindowSize(QSize(980, 540)); // set default
}

MainWindow::~MainWindow()
{
    delete MySmil;
    deleteRegionsAndLayouts(); // region must be deleted at last, cause media pointer had to be deleted
}

void MainWindow::setSmilIndex()
{
    qDebug(SmilParser) << "clear MySmil";
    if (ar_regions.size() > 0)
        cleanUp();

    MyMediaModel    = new MediaModel(this);
    MyNetworkQueue  = new NetworkQueue(MyConfiguration->getUserAgent().toUtf8(), this);
    MyMediaManager  = new MediaManager(MyMediaModel, MyConfiguration, MyNetworkQueue , this);

    MyHead          = new THead(this);
    connect(MyHead, SIGNAL(checkForNewIndex()), this, SLOT(checkForNewSmilIndex()));

    MySmil = new TSmil(MyMediaManager, this);
    connect(MySmil, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MySmil, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    MySmil->init();

    setRegions(MyIndexManager->getHead());
    setGeometry(0,0, width(), height());
    MySmil->beginSmilParsing(MyIndexManager->getBody());
}

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
    //setStyleSheet("background-color:"+MyHead->getRootBackgroundColor()+";");
    QList<Region>  region_list = MyHead->getLayout();
    QMap<QString, TRegion *>::iterator j;
    for (int i = 0; i < region_list.length(); i++)
    {
        j = ar_regions.insert(region_list.at(i).regionName, new TRegion(rootObject()));
        ar_regions[j.key()]->setRegion(region_list.at(i), engine());
        ar_regions[j.key()]->setRootSize(width(), height());
    }
    qDebug() << ar_regions.size() << " QML regions are created ";
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
            {
                checkForNewSmilIndex();
            }
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
    screen_state = FULLSCREEN;
    setPosition(MyScreen->getStartPointFromScreen());
    resize(MyScreen->getSizeFromScreen());
    setFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setCursor(Qt::BlankCursor);
}

void MainWindow::resizeAsBigFullScreen()
{
    screen_state = BIGFULLSCREEN;
    setPosition(0, 0);
    resize(MyScreen->getWholeSize());
    setFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setCursor(Qt::BlankCursor);
}

void MainWindow::resizeAsWindow()
{
    screen_state = WINDOWED;
    setPosition(MyScreen->getStartPointFromScreen());
    resize(getMainWindowSize());
    setCursor(Qt::ArrowCursor);
    setFlags(0);
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

void MainWindow::cleanUp()
{
    delete MySmil;
    delete MyMediaManager;
    delete MyMediaModel;
    delete MyNetworkQueue;
    delete MyHead;
    deleteRegionsAndLayouts(); // region should be deleted last
}

void MainWindow::startShowMedia(TMedia *media)
{
    if (ar_regions.size() == 0)
        return;

    QString region_name = selectRegion(media->getRegion());

    ar_regions[region_name]->startShowMedia(media);
    ar_regions[region_name]->setRootSize(width(), height());
    return;
}

void MainWindow::stopShowMedia(TMedia *media)
{
    if (ar_regions.size() == 0)
        return;

    QString region_name = selectRegion(media->getRegion());
    ar_regions[region_name]->stopShowMedia();
    return;
}


void MainWindow::doStatusChanged(QQuickView::Status status)
{
    switch (status)
    {
        case QQuickView::Null:
            qDebug(MediaPlayer) << "No QML source set";
            break;
        case QQuickView::Ready:
            MyIndexManager->init(MyConfiguration->getIndexUri());
            break;
        case QQuickView::Loading:
            qDebug(MediaPlayer) << "QML loaded/compiled... ";
            break;
        case QQuickView::Error:
            QList<QQmlError> qml_errors = errors();
            QList<QQmlError> ::iterator i;
            for (i = qml_errors.begin(); i != qml_errors.end(); ++i)
            {
                QQmlError err = *i;
                //qDebug(MediaPlayer) << err.messageType() << err.description() << err.line();
            }
            break;
    }

}

