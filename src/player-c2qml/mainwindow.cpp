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
    connect(MyLibFacade, SIGNAL(newIndexLoaded()), this, SLOT(prepareParsing()));

#ifdef SUPPORT_QTAV
    setSource(QUrl(QStringLiteral("qrc:/root_qtav.qml")));
#else
    setSource(QUrl(QStringLiteral("qrc:/root_qtm.qml")));
#endif
    setMainWindowSize(QSize(980, 540)); // set default
}

MainWindow::~MainWindow()
{
    prepareParsing(); // region must be deleted at last, cause media pointer had to be deleted
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


bool MainWindow::event(QEvent *event)
{
    event->accept();
    if(event->type() == QEvent::TouchBegin)
    {
//        QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
        num_touched++;
        if (num_touched > 4)
        {
            openDebugInfos();
            num_touched = 0;
        }
    }
    return QQuickView::event(event);
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
    if (regions_list.size() > 0)
    {
        Q_UNUSED(event);
        QMap<QString, TRegion *>::iterator i;
        for (i = regions_list.begin(); i != regions_list.end(); ++i)
            regions_list[i.key()]->setRootSize(width(), height());
    }
}

void MainWindow::createRegions()
{
    QList<Region> *region_list = MyLibFacade->getHead()->getLayout();
    setColor(MyLibFacade->getHead()->getRootBackgroundColor());

    QMap<QString, TRegion *>::iterator j;
    for (int i = 0; i < region_list->length(); i++)
    {
        j = regions_list.insert(region_list->at(i).regionName, new TRegion(rootObject()));
        regions_list[j.key()]->setRegion(region_list->at(i), engine());
        regions_list[j.key()]->setRootSize(width(), height());
    }
    qDebug() << regions_list.size() << " regions are created ";
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
    if (regions_list.size() == 0)
        return;

    QString region_name = selectRegion(media->getRegion());

    regions_list[region_name]->startShowMedia(media);
    regions_list[region_name]->setRootSize(width(), height());
    return;
}

void MainWindow::stopShowMedia(TMedia *media)
{
    if (regions_list.size() == 0)
        return;

    QString region_name = selectRegion(media->getRegion());
    regions_list[region_name]->stopShowMedia();
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
            MyLibFacade->initIndex();
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

