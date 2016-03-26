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

class InheritedTConfiguration : public TConfiguration
{
public:
    void test_createDirectories(){createDirectories();}

};



class TestTConfiguration : public QObject
{
    Q_OBJECT

public:
    TestTConfiguration(){}

private Q_SLOTS:
    void test_createDirectories();
    void test_getConfigXML();
    void test_getUserAgent();
};

void TestTConfiguration::test_createDirectories()
{
    InheritedTConfiguration MyConfig;
    QDir             dir;
    MyConfig.test_createDirectories();
    dir.setPath("var");
    QCOMPARE(dir.exists(), true);
    dir.removeRecursively(); // clean up

    dir.setPath("configuration");
    QCOMPARE(dir.exists(), true);
    dir.removeRecursively(); // clean up
}

void TestTConfiguration::test_getConfigXML()
{
    // First test without a config file
    InheritedTConfiguration MyConfig_no_xml;
    QCOMPARE(MyConfig_no_xml.getIndexPath(), QString("http://indexes.smil-admin.com/"));

    // copy config
    QFile::copy("../tests/data/config.xml", "configuration/config.xml");
    // now test with a config
    QDir dir;
    dir.mkdir("configuration");
    InheritedTConfiguration MyConfig_xml;
    QCOMPARE(MyConfig_xml.getIndexPath(), QString("http://indexes.of-a-smil-server.com/"));
    dir.setPath("configuration");
    dir.removeRecursively();// clean up
    return;
}

void TestTConfiguration::test_getUserAgent()
{
    InheritedTConfiguration MyConfig;
    QCOMPARE(MyConfig.getUserAgent(), QString("GAPI/1.0 (UUID:e15dbc25-a550-447a-b4a7-97a22e377397; NAME:97a22e377397) garlic-linux/0.1.0 (MODEL:Garlic)"));
}


QTEST_APPLESS_MAIN(TestTConfiguration)

#include "tst_configuration.moc"
