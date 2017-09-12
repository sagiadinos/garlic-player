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
#include <files/downloader.h>
#include <tools/configuration.h>

class TestTDownloader : public QObject
{
    Q_OBJECT

public:
    TestTDownloader(){}
private Q_SLOTS:
    void cleanup();
    void testAgentString();
    void testWithoutAgentString();
    void testDownloadSmil();
    void testDownloadMedia();
    void testDownloadMediaDirect();
    void testDownloadWebSite();

};

void TestTDownloader::cleanup()
{
    // to secure all files will be deleted even after a failed test
    QFile file1("./agent.txt");
    if (file1.exists())
        file1.remove();
    QFile file2("./test.smil");
    if (file2.exists())
        file2.remove();
    QFile file3("./test.jpg");
    if (file3.exists())
        file3.remove();
    QFile file4("./server.jpg");
    if (file4.exists())
        file4.remove();
}

void TestTDownloader::testAgentString()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    MyConfiguration->setUserAgent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    TDownloader *MyDownloader = new TDownloader(MyConfiguration);
    QString file_name         = "./agent.txt";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=show_agent_string", file_name);
    QSignalSpy spy(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy.count() == 0)
        QTest::qWait(200);

    QFile file(file_name);
    QVERIFY(file.exists());
    file.open(QIODevice::ReadOnly);
    QTextStream instream(&file);
  //  QCOMPARE(MyConfiguration->getUserAgent(), instream.readLine());
    file.close();
    file.remove();
}

void TestTDownloader::testWithoutAgentString()
{
    TDownloader *MyDownloader = new TDownloader(new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test")));
    QString file_name         = "./agent.txt";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=show_agent_string", file_name);

    QFile file(file_name);
    QVERIFY(!file.exists());
}


void TestTDownloader::testDownloadSmil()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    MyConfiguration->setUserAgent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    TDownloader *MyDownloader = new TDownloader(MyConfiguration);

    QString file_name         = "./test.smil";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_new", file_name);
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
    QFileInfo fi(file);
    QDateTime last_modified = fi.lastModified();
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_new", file_name);
    QSignalSpy spy2(MyDownloader, SIGNAL(noModified(QString)));
    while (spy2.count() == 0)
        QTest::qWait(200);
    QFileInfo fi2(file);
    QCOMPARE(last_modified, fi2.lastModified());

    QTest::qWait(500); // to write file with new date

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_cached", file_name);
    QSignalSpy spy3(MyDownloader, SIGNAL(noModified(QString)));
    while (spy3.count() == 0)
        QTest::qWait(200);
    QFileInfo fi3(file);
    QCOMPARE(last_modified, fi3.lastModified());


    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_updated", file_name);
    QSignalSpy spy4(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy4.count() == 0)
        QTest::qWait(200);
    QFileInfo fi4(file);
    QVERIFY(last_modified < fi4.lastModified());
    file.remove();
}


void TestTDownloader::testDownloadMedia()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    MyConfiguration->setUserAgent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    TDownloader *MyDownloader = new TDownloader(MyConfiguration);
    QString file_name         = "./server.jpg";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_new", file_name);
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed(QString)));
    int i = 0;
    while (spy1.count() == 0 && i < 2000)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
    QFileInfo fi(file);
    QDateTime last_modified = fi.lastModified();

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_new", file_name);
    QSignalSpy spy2(MyDownloader, SIGNAL(noModified(QString)));
    i = 0;
    while (spy2.count() == 0 && i < 2000)
        QTest::qWait(200);
    QFileInfo fi2(file);
    QCOMPARE(last_modified, fi2.lastModified());
    QTest::qWait(500); // to write file with new date

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_cached", file_name);
    QSignalSpy spy3(MyDownloader, SIGNAL(noModified(QString)));
    i = 0;
    while (spy3.count() == 0 && i < 2000)
        QTest::qWait(200);
    QFileInfo fi3(file);
    QCOMPARE(last_modified, fi3.lastModified());

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_updated", file_name);
    QSignalSpy spy4(MyDownloader, SIGNAL(downloadSucceed(QString)));
    i = 0;
    while (spy4.count() == 0 && i < 2000)
    {
        i += 200;
        QTest::qWait(200);
    }
    QFileInfo fi4(file);
    QCOMPARE(1, spy4.count());
//    QVERIFY(last_modified < fi4.lastModified());
    file.remove();
}

void TestTDownloader::testDownloadMediaDirect()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    MyConfiguration->setUserAgent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    TDownloader *MyDownloader = new TDownloader(MyConfiguration);
    QString file_name         = "./server.jpg";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/server.jpg", file_name);
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
}

void TestTDownloader::testDownloadWebSite()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player-test"));
    MyConfiguration->setUserAgent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    TDownloader *MyDownloader = new TDownloader(MyConfiguration);
    QString file_name         = "./websuite.html";
    MyDownloader->checkFiles(file_name, "http://www.tagesschau.de/infoscreen/", file_name);
    QSignalSpy spy1(MyDownloader, SIGNAL(uncachable(QString)));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(!file.exists());
}



QTEST_MAIN(TestTDownloader)

#include "tst_downloader.moc"
