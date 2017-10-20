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
    connect (this, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(doStatusChanged(QQuickView::Status)));
    MyScreen               = screen;
    MyLibFacade            = lib_facade;
    connect(MyLibFacade, SIGNAL(startShowMedia(TMedia *)), this, SLOT(startShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(stopShowMedia(TMedia *)));
    connect(MyLibFacade, SIGNAL(newIndex(QList<Region> *)), this, SLOT(setRegions(QList<Region> *)));

#ifdef SUPPORT_QTAV
    setSource(QUrl(QStringLiteral("qrc:/root_qtav.qml")));
#else
    setSource(QUrl(QStringLiteral("qrc:/root_qtm.qml")));
#endif
    setMainWindowSize(QSize(980, 540)); // set default
}

MainWindow::~MainWindow()
{
    deleteRegionsAndLayouts(); // region must be deleted at last, cause media pointer had to be deleted
}

void MainWindow::deleteRegionsAndLayouts()
{
    qDeleteAll(ar_regions);
    ar_regions.clear();
}


void MainWindow::setRegions(QList<Region> *region_list)
{
    deleteRegionsAndLayouts();
//    setStyleSheet("background-color:"+MyLibFacade->getHead()->getRootBackgroundColor()+";");
    QMap<QString, TRegion *>::iterator j;
    for (int i = 0; i < region_list->length(); i++)
    {
        j = ar_regions.insert(region_list->at(i).regionName, new TRegion(rootObject()));
        ar_regions[j.key()]->setRegion(region_list->at(i), engine());
        ar_regions[j.key()]->setRootSize(width(), height());
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
            MyLibFacade->checkForNewSmilIndex(); // load index when QML comiled complete
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
                qDebug(MediaPlayer) << err.messageType() << err.description() << err.line();
            }
            break;
    }

}

