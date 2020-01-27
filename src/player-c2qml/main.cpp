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
#include "Java2Cpp.h"
#include "mainwindow.h"

#if defined  Q_OS_ANDROID
    #include <QtWebView>
#else
    #include <qtwebengineglobal.h>
#endif

void handleMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Logger& MyLogger = Logger::getInstance();
    MyLogger.dispatchMessages(type, context, msg);
}

int main(int argc, char *argv[])
{

#if QT_VERSION < 0x059300
        qputenv("QML_DISABLE_DISK_CACHE", "true"); // due to https://bugreports.qt.io/browse/QTBUG-56935
#endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts); // Raspberry and POT needs this http://thebugfreeblog.blogspot.de/2018/01/pot-570-with-qt-5100-built-for-armv8.html
    QApplication app(argc, argv);

#if defined  Q_OS_ANDROID
    QtWebView::initialize();
    QtAndroid::androidActivity().callMethod<void>("registerBroadcastReceiver");
#endif
    qmlRegisterType<LibFacade>("com.garlic.LibFacade", 1, 0, "LibFacade");
    qmlRegisterType<ResourceMonitor>("com.garlic.ResourceMonitor", 1, 0, "ResourceMonitor");
    LibFacade      *MyLibFacade     = new LibFacade();
    setGlobalLibFaceForJava(MyLibFacade);
    QApplication::setApplicationName(MyLibFacade->getConfiguration()->getAppName());
    QApplication::setApplicationVersion(MyLibFacade->getConfiguration()->getVersion());
    QApplication::setApplicationDisplayName(MyLibFacade->getConfiguration()->getAppName());

    QDir dir(".");
    MyLibFacade->getConfiguration()->determineBasePath(dir.absolutePath()); // When run in terminal absolute path returns user homedirectory in QtCreator
    MyLibFacade->getConfiguration()->createDirectories();

    qInstallMessageHandler(handleMessages);

    TCmdParser MyParser(MyLibFacade);
    MyParser.addOptions();

   if (!MyParser.parse(&app))
        return 1;

    QLoggingCategory::setFilterRules("*.debug=true\nqt.*=false");

    TScreen    MyScreen(Q_NULLPTR);
    MainWindow w(&MyScreen, MyLibFacade);

    QQmlEngine::setObjectOwnership(&w, QQmlEngine::CppOwnership);

    if (MyLibFacade->getConfiguration()->getIndexUri() == "" && w.openConfigDialog() == QDialog::Rejected)
        return 0;

    w.init();

#if defined  Q_OS_ANDROID

    // preserve android screensaver https://stackoverflow.com/questions/44100627/how-to-disable-screensaver-on-qt-android-app
    // https://forum.qt.io/topic/57625/solved-keep-android-5-screen-on

    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (activity.isValid())
    {
        QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
        if (window.isValid())
        {
            const int FLAG_KEEP_SCREEN_ON = 128;
            window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
    }
    // not to crash in Android > 5.x Clear any possible pending exceptions.
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
    }

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
