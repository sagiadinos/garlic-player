/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2024 Nikolaos Saghiadinos <ns@smil-control.com>
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
#include "tst_main_configuration.hpp"

void TestMainConfiguration::cleanup()
{
    QDir             dir;
    dir.setPath(QDir::homePath()+"/.qttest");
    dir.removeRecursively(); // clean up
    QSettings *Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    QFile file(Settings->fileName());
    if (file.exists())
        file.remove();
}

void TestMainConfiguration::test_determineIndexUriWhenParameter()
{
    MainConfiguration *MyConfig = initTestClass();

    MyConfig->determineIndexUri("http://domain.tld/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("http://domain.tld/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("http://domain.tld/path_to/"));

    MyConfig->determineIndexUri("ftp://ftpdomain.tld/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("ftp://ftpdomain.tld/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("ftp://ftpdomain.tld/path_to/"));

    MyConfig->setBasePath("/BasePath/");
    MyConfig->determineIndexUri("/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("/path_to/"));
}

void TestMainConfiguration::test_determineIndexUriWhenIni()
{
    QSettings *Settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    MainConfiguration *MyConfig = new MainConfiguration(Settings, "Test", "Uri");
    Settings->setValue("index_uri", "https://smil-index-via-ini-file.tld/a_path/second/path/index.smil");

    MyConfig->determineIndexUri("");
    QCOMPARE(MyConfig->getIndexUri(), QString("https://smil-index-via-ini-file.tld/a_path/second/path/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("https://smil-index-via-ini-file.tld/a_path/second/path/"));

    MyConfig->determineIndexUri("ftps://ftpdomain.tld/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("ftps://ftpdomain.tld/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("ftps://ftpdomain.tld/path_to/"));

}


void TestMainConfiguration::test_determineUserAgent()
{
   MainConfiguration *MyConfig = initTestClass();
   MyConfig->setUuid("the-uuid-is-this");
   MyConfig->setPlayerName("the playername");
   MyConfig->determineUserAgent();

#if defined Q_OS_LINUX
   QVERIFY(MyConfig->getUserAgent().contains("GAPI/1.0 (UUID:the-uuid-is-this; NAME:the playername) garlic-linux"));
#elif defined Q_OS_OSX
   QVERIFY(MyConfig->getUserAgent().contains("GAPI/1.0 (UUID:the-uuid-is-this; NAME:the playername) garlic-macOS"));
#elif defined Q_OS_WIN32
   QVERIFY(MyConfig->getUserAgent().contains("GAPI/1.0 (UUID:the-uuid-is-this; NAME:the playername) garlic-windows"));
#else
   QVERIFY(MyConfig->getUserAgent().contains("GAPI/1.0 (UUID:the-uuid-is-this; NAME:the playername) garlic-unknown"));
#endif

   return;
}

MainConfiguration *TestMainConfiguration::initTestClass()
{
    QSettings *Settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    return new MainConfiguration(Settings, "TestCompany", "https://indexes.domain.tld" );
}

static TestMainConfiguration TEST_MAINCONFIGURATION;
