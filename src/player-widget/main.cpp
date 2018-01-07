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

#include "tools/logger.h"
#include "mainwindow.h"
#include "../player-common/cmdparser.h"
#include "../player-common/screen.h"


void handleMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Logger& MyLogger = Logger::getInstance();
    MyLogger.dispatchMessages(type, context, msg);
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

#ifdef QT_DEBUG
    QLoggingCategory::setFilterRules("*.debug=true\nqt.*=false");
#else
    QLoggingCategory::setFilterRules("*.debug=false");
#endif

    qInstallMessageHandler(handleMessages);

    TCmdParser MyParser(MyLibFacade->getConfiguration());
    MyParser.addOptions();
    MyParser.parse(&app);

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
        MyLibFacade->init();
        MyLibFacade->loadIndex();
        MyLibFacade->checkForNewSmilIndex();

        ret = app.exec();
    }
    return ret;
}
