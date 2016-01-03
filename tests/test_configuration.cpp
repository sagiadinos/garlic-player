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

#include "test_configuration.h"
#include <QDebug>


void Test_Configuration::test_createDirectories()
{
    pw_Configuration MyConfig;
    QDir             dir;
    MyConfig.setBaseDirectory(dir.absolutePath()+"/");
    MyConfig.test_createDirectories();
    dir.setPath("var");
    QCOMPARE(dir.exists(), true);
    dir.setPath("var/media");
    QCOMPARE(dir.exists(), true);
    dir.setPath("configuration");
    QCOMPARE(dir.exists(), true);

    dir.setPath("var");
    dir.removeRecursively();
    dir.setPath("configuration");
    dir.removeRecursively();
}

void Test_Configuration::test_getConfigXML()
{
    // First test without a config file
    pw_Configuration MyConfig_no_xml;
    QCOMPARE(MyConfig_no_xml.getIndexServer(), QString("http://indexes.smil-admin.com"));

    // now test with a config
    QDir dir;
    pw_Configuration MyConfig_xml;
    MyConfig_xml.setBaseDirectory(dir.absolutePath()+"/");
    dir.mkdir("configuration");
    QFile::copy("../tests/data/config.xml", "configuration/config.xml");
    QCOMPARE(MyConfig_xml.getIndexServer(), QString("http://indexes.of-a-smil-server.com"));

    // clean up
    dir.setPath("configuration");
    dir.removeRecursively();
    return;
}

void Test_Configuration::test_getUserAgent()
{
    pw_Configuration MyConfig;
    QCOMPARE(MyConfig.getUserAgent(), QString("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:5c338e54bcae) linux/0.1.0 (MODEL:Garlic)"));
}

