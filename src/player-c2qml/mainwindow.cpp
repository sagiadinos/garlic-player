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

MainWindow::MainWindow(TScreen *screen, LibFacade *lib_facade, PlayerConfiguration *pc)
{
    MyScreen               = screen;
    MyLibFacade            = lib_facade;
    MyPlayerConfiguration  = pc;
    MyInteractions         = new Interactions(lib_facade, this);
}

MainWindow::~MainWindow()
{
    deleteRegions(); // region must be deleted at last, cause media pointer had to be deleted
}

void MainWindow::init()
{
    connect (this, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(doStatusChanged(QQuickView::Status)));
    connect(MyLibFacade, SIGNAL(startShowMedia(BaseMedia *)), this, SLOT(startShowMedia(BaseMedia *)));
    connect(MyLibFacade, SIGNAL(stopShowMedia(BaseMedia *)), this, SLOT(stopShowMedia(BaseMedia *)));
    connect(MyLibFacade, SIGNAL(readyForPlaying()), this, SLOT(prepareParsing()));

    connect(MyLibFacade, SIGNAL(newConfig()), this, SLOT(sendConfig()));
    connect(MyLibFacade, SIGNAL(rebootOS()), this, SLOT(rebootOS()));
    connect(MyLibFacade, SIGNAL(installSoftware(QString)), this, SLOT(installSoftware(QString)));
    connect(engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit())); // to connect quit signal from QML

#ifdef SUPPORT_QTAV
    setSource(QUrl(QStringLiteral("qrc:/root_qtav.qml")));
#else
    setSource(QUrl(QStringLiteral("qrc:/root_qtm.qml")));
#endif
    setMainWindowSize(QSize(980, 540)); // set default
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
    {
        MyInteractions->handleKeyPress(ke);
    }
    else
    {
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
            case Qt::Key_S:  // Ctrl-C will not work during qwebengineview
                openConfigDialog();
            break;

            case Qt::Key_Q: // quit app normal
                MyLibFacade->shutDownParsing();
                sendClosePlayerCorrect();
            //    delete MyLibFacade;
                QApplication::quit();
            break;
            case Qt::Key_C: // quit app not normal e.g. to simulate a crash
                QApplication::quit();
            break;
        }
    }
    return;
}

bool MainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchUpdate || event->type() == QEvent::TouchEnd)
    {
        event->accept();
        if(event->type() == QEvent::TouchBegin)
        {
            start_touch_time.start();
        }
        else if (static_cast<QTouchEvent*>(event)->touchPoints().count() == 1 && event->type() == QEvent::TouchUpdate)
        {
            int ms = start_touch_time.elapsed();
            if (ms > 10000)
            {
                openDebugInfos();
            }
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
    int ret = QDialog::Rejected;
    if (MyPlayerConfiguration->hasLauncher())
    {
        return ret;
    }
    ConfigDialog MyConfigDialog(0, MyLibFacade->getConfiguration());
    setCursor(Qt::ArrowCursor);
    if (MyConfigDialog.exec() == QDialog::Accepted)
    {
        MyLibFacade->initParser();
        ret = QDialog::Accepted;
    }
    setCursor(Qt::BlankCursor);
    return ret;
}

void MainWindow::resizeAsNormalFullScreen()
{
    screen_state = FULLSCREEN;
    setCursor(Qt::BlankCursor);
    showFullScreen();
}

void MainWindow::resizeAsBigFullScreen()
{
    screen_state = BIGFULLSCREEN;
    setFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setCursor(Qt::BlankCursor);
    showNormal();
    setPosition(0, 0);
    resize(MyScreen->getWholeSize());
}

void MainWindow::resizeAsWindow()
{
    screen_state = WINDOWED;
    setFlags(Qt::Window);
    setCursor(Qt::ArrowCursor);
    showNormal();
    MyScreen->determineCurrentScreenId(this->geometry().topLeft());
    setPosition(MyScreen->getStartPointFromCurrentScreen());
    resize(QSize(980, 540));
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
    QQuickView::resizeEvent(event);
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

    MyLibFacade->beginSmilPlaying(); // begin playing not before Layout ist build to prevent crash in MainWindow::startShowMedia
}

void MainWindow::startShowMedia(BaseMedia *media)
{
    if (regions_list.size() == 0)
        return;

    QString region_name = selectRegion(media->getRegion());

    regions_list[region_name]->startShowMedia(media);
    regions_list[region_name]->setRootSize(width(), height());
    return;
}

void MainWindow::stopShowMedia(BaseMedia *media)
{
    if (regions_list.size() == 0)
        return;

    QString region_name = selectRegion(media->getRegion());
    regions_list[region_name]->stopShowMedia(media);
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
            MyLibFacade->initParser();
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

void MainWindow::sendConfig()
{
#if defined  Q_OS_ANDROID
    QAndroidJniObject java_file_path = QAndroidJniObject::fromString(MyLibFacade->getConfiguration()->getPaths("cache")+"configuration.xml");
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity",
                                              "applyConfig",
                                              "(Ljava/lang/String;)V",
                                              java_file_path.object<jstring>());
#endif

}

void MainWindow::rebootOS()
{
#if defined  Q_OS_ANDROID
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity", "rebootOS");
#endif
}

void MainWindow::installSoftware(QString file_path)
{
#if defined  Q_OS_ANDROID
   QAndroidJniObject java_file_path = QAndroidJniObject::fromString(file_path);
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity",
                                              "installSoftware",
                                              "(Ljava/lang/String;)V",
                                              java_file_path.object<jstring>());
#endif
}

void MainWindow::sendClosePlayerCorrect()
{
#if defined  Q_OS_ANDROID
    QAndroidJniObject::callStaticMethod<void>("com/sagiadinos/garlic/player/java/GarlicActivity", "closePlayerCorrect");
#endif
    qDebug(MediaPlayer) << "send ClosePlayerCorrect" ;
}
