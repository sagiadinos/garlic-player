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
#include <QString>
#include <QtTest>
#include "tools/configuration.h"

class TestTConfiguration : public QObject
{
    Q_OBJECT

public:
    TestTConfiguration(){}

private Q_SLOTS:
    void cleanup();
    void test_determineIndexUriWhenParameter();
    void test_determineIndexUriWhenIni();
    void test_determineIndexUriWhenConfigXML();
    void test_determineBasePath();
    void test_createDirectories();
    void test_determineUserAgent();
};

void TestTConfiguration::cleanup()
{
    QDir             dir;
    dir.setPath(QDir::homePath()+"/.qttest");
    dir.removeRecursively(); // clean up
    QSettings *Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    QFile file(Settings->fileName());
    if (file.exists())
        file.remove();
}

void TestTConfiguration::test_determineIndexUriWhenParameter()
{
    QSettings *Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    TConfiguration *MyConfig = new TConfiguration(Settings);

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

    MyConfig->setBasePath("/BasePath/");
    MyConfig->determineIndexUri("path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("/BasePath/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("/BasePath/path_to/"));
}

void TestTConfiguration::test_determineIndexUriWhenIni()
{
    QSettings *Settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    TConfiguration *MyConfig = new TConfiguration(Settings);
    Settings->setValue("index_uri", "https://smil-index-via-ini-file.tld/a_path/second/path/index.smil");

    MyConfig->determineIndexUri("");
    QCOMPARE(MyConfig->getIndexUri(), QString("https://smil-index-via-ini-file.tld/a_path/second/path/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("https://smil-index-via-ini-file.tld/a_path/second/path/"));

    MyConfig->determineIndexUri("ftps://ftpdomain.tld/path_to/index.smil");
    QCOMPARE(MyConfig->getIndexUri(), QString("ftps://ftpdomain.tld/path_to/index.smil"));
    QCOMPARE(MyConfig->getIndexPath(), QString("ftps://ftpdomain.tld/path_to/"));

}

void TestTConfiguration::test_determineIndexUriWhenConfigXML()
{
    QSettings *Settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    TConfiguration *MyConfig = new TConfiguration(Settings);
    QDir dir(".");

    MyConfig->determineBasePath(dir.absolutePath());

    QFile::copy(":/config.xml", "./config.xml");

    MyConfig->determineIndexUri("");
    QCOMPARE(MyConfig->getIndexUri(), QString("http://indexes.of-a-smil-server.com/path_to_a/index.php?site=player_get_index&owner_id=2"));
    QCOMPARE(MyConfig->getIndexPath(), QString("http://indexes.of-a-smil-server.com/path_to_a/"));
    QFile file("./config_readed.xml");
    file.remove();
}

void TestTConfiguration::test_determineBasePath()
{
    QSettings *Settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
    TConfiguration *MyConfig = new TConfiguration(Settings);

    MyConfig->determineBasePath("/home/user/garlic/bin");
    QCOMPARE(MyConfig->getBasePath(), QString("/home/user/garlic/"));

    MyConfig->determineBasePath("/home/user2/garlic2/");
    QCOMPARE(MyConfig->getBasePath(), QString("/home/user2/garlic2/"));

    MyConfig->determineBasePath("/home/user3/garlic3");
    QCOMPARE(MyConfig->getBasePath(), QString("/home/user3/garlic3/"));
}

void TestTConfiguration::test_createDirectories()
{
    QStandardPaths::setTestModeEnabled(true);
    TConfiguration *MyConfig = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    QCoreApplication::setApplicationName("garlic-player-test"); // to make sure a folder in Standradpath is set
    QDir             dir;
    MyConfig->createDirectories();
    dir.setPath(QDir::homePath()+"/.qttest/cache/garlic-player-test/");
    QVERIFY(dir.exists());

    dir.setPath(QDir::homePath()+"/.qttest/share/garlic-player-test/logs/");
    QCOMPARE(dir.exists(), true);

}


void TestTConfiguration::test_determineUserAgent()
{
   QSettings *Settings      = new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test");
   TConfiguration *MyConfig = new TConfiguration(Settings);
   MyConfig->setUuid("the-uuid-is-this");
   MyConfig->setPlayerName("the playername");
   MyConfig->setOS("hurd");
   MyConfig->determineUserAgent();
   QCOMPARE(MyConfig->getUserAgent(), QString("GAPI/1.0 (UUID:the-uuid-is-this; NAME:the playername) garlic-hurd/0.1.0 (MODEL:Garlic)"));

   return;
}


QTEST_APPLESS_MAIN(TestTConfiguration) // appless do not wor with cachedir

#include "tst_configuration.moc"
