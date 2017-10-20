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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TConfiguration *MyConfiguration   = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player"));
    QApplication::setApplicationName(MyConfiguration->getAppName());
    QApplication::setApplicationVersion(MyConfiguration->getVersion());
    QApplication::setApplicationDisplayName(MyConfiguration->getAppName());

    QDir dir(".");
    MyConfiguration->determineBasePath(dir.absolutePath()); // Run in terminal could cause absolute path returns user homedirectory in QtCreator
    MyConfiguration->determineUserAgent();
    MyConfiguration->createDirectories();

    TCmdParser MyParser(MyConfiguration);
    MyParser.addOptions();
    MyParser.parse(&app);

    LibFacade      *MyLibFacade     = new LibFacade(MyConfiguration);

    MainWindow w(MyLibFacade);
    w.show();

    MyLibFacade->initIndex();
    MyLibFacade->checkForNewSmilIndex();

    return app.exec();
}

