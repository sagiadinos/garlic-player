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
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player"));
    QDir dir(".");
    MyConfiguration->determineBasePath(dir.absolutePath()); // Run in terminal cause absolute path returns user homedirectory in QtCreator
    MyConfiguration->determineUserAgent();
    MyConfiguration->createDirectories();

    QApplication app(argc, argv);

    QApplication::setApplicationName(MyConfiguration->getAppName());
    QApplication::setApplicationVersion(MyConfiguration->getVersion());
    QApplication::setApplicationDisplayName(MyConfiguration->getAppName());
    QCommandLineParser parser;

    parser.setApplicationDescription(MyConfiguration->getDescription());
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
        MyConfiguration->determineIndexUri(args.at(0));
    else
        MyConfiguration->determineIndexUri("");

    w.setInitialSmilIndex(MyConfiguration);

    if (parser.isSet("f"))
        w.showFullScreen();
    else if (parser.isSet("w"))
    {
        w.setMinimumSize(980, 540);
        w.show();
    }
    else
    {
        w.setMinimumSize(980, 540);
        w.show();
    }

//        parser.showHelp(0);

    return app.exec();
}
