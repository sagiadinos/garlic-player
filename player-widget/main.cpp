/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-.control.com>
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
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QApplication::setApplicationName(TConfiguration::getAppName());
    QApplication::setApplicationVersion(TConfiguration::getVersion());
    QApplication::setApplicationDisplayName(TConfiguration::getAppName());
    QCommandLineParser parser;

    parser.setApplicationDescription(TConfiguration::getDescription());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("SMIL_INDEX", QCoreApplication::translate("main", "Path to SMIL index"));

    parser.addOptions(
    {
        {{"f", "fullscreen"}, "Starts in fullscreen mode"},
         {{"w", "window"}, "Starts in windows mode"}
    });
    MainWindow w;
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.size() > 0)
        w.setSmilIndex(args.at(0));
    else
        w.setSmilIndex("");

    if (parser.isSet("f"))
        w.showFullScreen();
    else if (parser.isSet("w"))
        w.show();
    else
        w.show();
//        parser.showHelp(0);

    return app.exec();
}
