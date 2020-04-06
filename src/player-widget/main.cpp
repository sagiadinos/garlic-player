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
#include "../player-common/player_configuration.h"

void handleMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Logger& MyLogger = Logger::getInstance();
    MyLogger.dispatchMessages(type, context, msg);
}

int main(int argc, char *argv[])
{
  //  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    LibFacade      *MyLibFacade     = new LibFacade();
    PlayerConfiguration  *MyPlayerConfiguration = new PlayerConfiguration(MyLibFacade->getConfiguration());
    MyPlayerConfiguration->determineInitConfigValues();

#ifdef QT_DEBUG
 //   QLoggingCategory::setFilterRules("*.debug=true\nqt.*=false");
    // QtAV::setLogLevel(QtAV::LogAll); // use with caution! produces crashes in debug.log
#else
    QLoggingCategory::setFilterRules("*.debug=false");
#endif
    qInstallMessageHandler(handleMessages);

    TCmdParser MyParser(MyLibFacade);
    MyParser.addOptions();
    MyParser.parse(&app);

    TScreen    MyScreen(Q_NULLPTR);

    MyScreen.selectCurrentScreen(MyParser.getScreenSelect());
    MainWindow w(&MyScreen, MyLibFacade);

    // do not start without an index uri
    if (!MyPlayerConfiguration->hasLauncher() && MyLibFacade->getConfiguration()->getIndexUri() == "" && w.openConfigDialog() == QDialog::Rejected)
    {
        return 1;
    }

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

    MyLibFacade->initParser();

    return app.exec();
}
