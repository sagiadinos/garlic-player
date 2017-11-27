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

void handleMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Logger& MyLogger = Logger::getInstance();
    MyLogger.dispatchMessages(type, context, msg);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LibFacade      *MyLibFacade     = new LibFacade();
    QApplication::setApplicationName(MyLibFacade->getConfiguration()->getAppName());
    QApplication::setApplicationVersion(MyLibFacade->getConfiguration()->getVersion());
    QApplication::setApplicationDisplayName(MyLibFacade->getConfiguration()->getAppName());
    MyLibFacade->getConfiguration()->determineIndexUri("http://indexes.smil-admin.com");

    QDir dir(".");
    MyLibFacade->getConfiguration()->determineBasePath(dir.absolutePath()); // Run in terminal could cause absolute path returns user homedirectory in QtCreator
    MyLibFacade->getConfiguration()->determineUserAgent();
    MyLibFacade->getConfiguration()->createDirectories();
    QLoggingCategory::setFilterRules("*.debug=true\nqt.*=false");
    qInstallMessageHandler(handleMessages);

    TCmdParser MyParser(MyLibFacade->getConfiguration());
    MyParser.addOptions();
    MyParser.parse(&app);

    MainWindow w(MyLibFacade);
    MyLibFacade->initIndex();
    MyLibFacade->checkForNewSmilIndex();

    return app.exec();
}

