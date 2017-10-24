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
#include "../player-common/cmdparser.h"
#include "../player-common/screen.h"

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
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    LibFacade      *MyLibFacade     = new LibFacade();
    QApplication::setApplicationName(MyLibFacade->getConfiguration()->getAppName());
    QApplication::setApplicationVersion(MyLibFacade->getConfiguration()->getVersion());
    QApplication::setApplicationDisplayName(MyLibFacade->getConfiguration()->getAppName());

    QDir dir(".");
    MyLibFacade->getConfiguration()->determineBasePath(dir.absolutePath()); // Run in terminal could cause absolute path returns user homedirectory in QtCreator
    MyLibFacade->getConfiguration()->createDirectories();

    TCmdParser MyParser(MyLibFacade->getConfiguration());
    MyParser.addOptions();
    MyParser.parse(&app);


    // Set the logging file
    event_log.reset(new QFile(MyLibFacade->getConfiguration()->getPaths("logs") + "events.log"));
    event_log.data()->open(QFile::Append | QFile::Text);
    QLoggingCategory::setFilterRules("*.debug=true\n");
   // qInstallMessageHandler(myMessageHandler);

    bool is_index = true;
    TScreen    MyScreen(QApplication::desktop());
    MainWindow w(&MyScreen, MyLibFacade);
    if (MyLibFacade->getConfiguration()->getIndexUri() == "")
    {
        if (w.openConfigDialog() == QDialog::Rejected)
            is_index = false;
    }

    int ret = 0;
    if (is_index)
    {
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
        MyLibFacade->initIndex();
        MyLibFacade->checkForNewSmilIndex();
        ret = app.exec();
    }
    return ret;
}
