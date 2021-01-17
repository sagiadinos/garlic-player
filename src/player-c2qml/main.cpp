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
#include <QQmlApplicationEngine>

#include "tools/logger.h"
#include "tools/resource_monitor.h"
#include "../player-common/cmdparser.h"
#include "../player-common/screen.h"
#include "../player-common/player_configuration.h"

#if defined  Q_OS_ANDROID
    #include "Java2Cpp.h"
    #include "android_manager.h"
#endif

#include <QtWebView>
#include "mainwindow.h"

void handleMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Logger& MyLogger = Logger::getInstance();
    MyLogger.dispatchMessages(type, context, msg);
}

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts); // Raspberry and POT needs this http://thebugfreeblog.blogspot.de/2018/01/pot-570-with-qt-5100-built-for-armv8.html

    QApplication app(argc, argv);
    MainConfiguration    *MyMainConfiguration   = new MainConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player"));
    PlayerConfiguration  *MyPlayerConfiguration = new PlayerConfiguration(MyMainConfiguration);

    LibFacade  *MyLibFacade = new LibFacade();
    MyLibFacade->init(MyMainConfiguration);

    QtWebView::initialize();

#if defined Q_OS_ANDROID
    AndroidManager *MyAndroidManager = new AndroidManager();
    if (!MyAndroidManager->checkPermissiones())
    {
        MyAndroidManager->sendCloseCorrect();
        QApplication::quit();
    }

    if (MyAndroidManager->hasLauncher())
    {
        MyLibFacade->toggleLauncher(MyAndroidManager->hasLauncher());
        MyPlayerConfiguration->setHasLauncher(MyAndroidManager->hasLauncher());
        MyPlayerConfiguration->setUuidFromLauncher(MyAndroidManager->getUUIDFromLauncher());
        MyPlayerConfiguration->setSmilIndexUriFromLauncher(MyAndroidManager->getSmilIndexFromLauncher());
    }

    setGlobalLibFaceForJava(MyLibFacade);
#endif

    qmlRegisterType<LibFacade>("com.garlic.LibFacade", 1, 0, "LibFacade");
    qmlRegisterType<ResourceMonitor>("com.garlic.ResourceMonitor", 1, 0, "ResourceMonitor");

    MyPlayerConfiguration->determineInitConfigValues();


    qInstallMessageHandler(handleMessages);

    TCmdParser MyParser(MyMainConfiguration);
    MyParser.addOptions();

    if (!MyParser.parse(MyLibFacade))
        return 1;

    QLoggingCategory::setFilterRules("*.debug=true\nqt.*=false");

    TScreen    MyScreen(Q_NULLPTR);
    MyScreen.selectCurrentScreen(MyParser.getScreenSelect());
    MainWindow w(&MyScreen, MyLibFacade, MyPlayerConfiguration);

    QQmlEngine::setObjectOwnership(&w, QQmlEngine::CppOwnership);

    if (MyMainConfiguration->getIndexUri().isEmpty() && w.openConfigDialog() == QDialog::Rejected)
    {
        return 0;
    }
    w.init();

#if defined  Q_OS_ANDROID

    MyAndroidManager->disableScreenSaver();
    w.showFullScreen();
#else

    w.show();

    QString val = MyParser.getWindowMode();
    if (val == "fullscreen")
        w.resizeAsNormalFullScreen();
    else if (val == "bigscreen")
        w.resizeAsBigFullScreen();
    else if (val == "windowed")
    {
        w.setMainWindowSize(MyParser.getWindowSize());
        w.resizeAsWindow();
    }
#endif

    return app.exec();
}
