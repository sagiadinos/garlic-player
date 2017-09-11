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
#include "cmdparser.h"
#if defined  Q_OS_ANDROID
    #include <QtAndroidExtras/QAndroidJniEnvironment>
    #include <QtAndroidExtras/QtAndroidExtras>
#endif
QScopedPointer<QFile>   event_log;

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(event_log.data());

    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    switch (type)
    {
        case QtInfoMsg:
            out << "INFO ";
            break;
        case QtDebugMsg:
            out << "DEBUG ";
            break;
        case QtWarningMsg:
            out << "WARNING ";
            break;
        case QtCriticalMsg:
            out << "CRITICAL ";
            break;
        case QtFatalMsg:
            out << "FATAL ";
            abort();
            break;
    }
    out << context.category << " " << msg << endl;
    out.flush();    // Clear the buffered data
}

int main(int argc, char *argv[])
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player"));
    QDir dir(".");
    int ret = 0;
    MyConfiguration->determineBasePath(dir.absolutePath()); // Run in terminal cause absolute path returns user homedirectory in QtCreator
    MyConfiguration->determineUserAgent();
    MyConfiguration->createDirectories();

    QApplication app(argc, argv);

    // Set the logging file
    event_log.reset(new QFile(MyConfiguration->getPaths("logs") + "events.log"));
    event_log.data()->open(QFile::Append | QFile::Text);
   // QLoggingCategory::setFilterRules("*.debug=false\n");
    qInstallMessageHandler(myMessageHandler);

    QApplication::setApplicationName(MyConfiguration->getAppName());
    QApplication::setApplicationVersion(MyConfiguration->getVersion());
    QApplication::setApplicationDisplayName(MyConfiguration->getAppName());

    TCmdParser MyParser(MyConfiguration);
    MyParser.addOptions();
    MyParser.parse(&app);

    bool is_index = true;
    TScreen    MyScreen(0, QApplication::desktop());
    MyConfiguration->determineIndexUri("http://indexes.smil-admin.com");

    MainWindow w(new TFileManager(MyConfiguration), &MyScreen);

/*    if (MyConfiguration->getIndexUri() == "")
    {
        if (w.openConfigDialog() == QDialog::Rejected)
            is_index = false;
    }
*/
    if (is_index)
    {

#if defined  Q_OS_ANDROID
        // preserve android screensaver https://stackoverflow.com/questions/44100627/how-to-disable-screensaver-on-qt-android-app
        // https://forum.qt.io/topic/57625/solved-keep-android-5-screen-on
        QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
        if (activity.isValid()) {
            QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
            if (window.isValid()) {
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
        MyScreen.setActualScreenId(MyParser.getScreenSelect());
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

        w.checkForNewSmilIndex();
        ret = app.exec();
    }
    return ret;
}
