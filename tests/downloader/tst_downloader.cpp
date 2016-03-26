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

class TestTDownloader : public QObject
{
    Q_OBJECT

public:
    TestTDownloader(){deleteFiles();}
    ~TestTDownloader(){deleteFiles();}
private Q_SLOTS:
    void testAgentString();
    void testWithoutAgentString();
    void testDownloadSmil();
    void testDownloadMedia();
    void testDownloadMediaDirect();
    void testDownloadWebSite();
    void deleteFiles();
};

void TestTDownloader::testAgentString()
{
    QString user_agent        = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    TDownloader *MyDownloader = new TDownloader(user_agent);
    QString file_name         = "./agent.txt";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=show_agent_string");
    QSignalSpy spy(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy.count() == 0)
        QTest::qWait(200);

    QFile file(file_name);
    QVERIFY(file.exists());
    file.open(QIODevice::ReadOnly);
    QTextStream instream(&file);
    QCOMPARE(instream.readLine(), user_agent);
    file.close();
    file.remove();
}

void TestTDownloader::testWithoutAgentString()
{
    QString user_agent        = "";
    TDownloader *MyDownloader = new TDownloader(user_agent);
    QString file_name         = "./agent.txt";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=show_agent_string");

    QFile file(file_name);
    QVERIFY(!file.exists());
}

void TestTDownloader::testDownloadSmil()
{
    QString user_agent        = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    TDownloader *MyDownloader = new TDownloader(user_agent);
    QString file_name         = "./test.smil";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_new");
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
    QFileInfo fi(file);
    QDateTime last_modified = fi.lastModified();
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_new");
    QSignalSpy spy2(MyDownloader, SIGNAL(noModified(QString)));
    while (spy2.count() == 0)
        QTest::qWait(200);
    QFileInfo fi2(file);
    QCOMPARE(last_modified, fi2.lastModified());

    QTest::qWait(500); // to write file with new date

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_cached");
    QSignalSpy spy3(MyDownloader, SIGNAL(noModified(QString)));
    while (spy3.count() == 0)
        QTest::qWait(200);
    QFileInfo fi3(file);
    QCOMPARE(last_modified, fi3.lastModified());


    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_updated");
    QSignalSpy spy4(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy4.count() == 0)
        QTest::qWait(200);
    QFileInfo fi4(file);
    QVERIFY(last_modified < fi4.lastModified());
    file.remove();
}

void TestTDownloader::testDownloadMedia()
{
    QString user_agent        = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    TDownloader *MyDownloader = new TDownloader(user_agent);
    QString file_name         = "./test.jpg";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_new");
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
    QFileInfo fi(file);
    QDateTime last_modified = fi.lastModified();

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_new");
    QSignalSpy spy2(MyDownloader, SIGNAL(noModified(QString)));
    while (spy2.count() == 0)
        QTest::qWait(200);
    QFileInfo fi2(file);
    QCOMPARE(last_modified, fi2.lastModified());
    QTest::qWait(500); // to write file with new date

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_cached");
    QSignalSpy spy3(MyDownloader, SIGNAL(noModified(QString)));
    while (spy3.count() == 0)
        QTest::qWait(200);
    QFileInfo fi3(file);
    QCOMPARE(last_modified, fi3.lastModified());

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_updated");
    QSignalSpy spy4(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy4.count() == 0)
        QTest::qWait(200);
    QFileInfo fi4(file);
    QVERIFY(last_modified < fi4.lastModified());
    file.remove();
}

void TestTDownloader::testDownloadMediaDirect()
{
    QString user_agent        = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    TDownloader *MyDownloader = new TDownloader(user_agent);
    QString file_name         = "./server.jpg";
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/server.jpg");
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed(QString)));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
}

void TestTDownloader::testDownloadWebSite()
{
    QString user_agent        = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    TDownloader *MyDownloader = new TDownloader(user_agent);
    QString file_name         = "./websuite.html";
    MyDownloader->checkFiles(file_name, "http://www.tagesschau.de/infoscreen/");
    QSignalSpy spy1(MyDownloader, SIGNAL(uncachable(QString)));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(!file.exists());
}

// ====================== tool methods ===========================

void TestTDownloader::deleteFiles()
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


QTEST_MAIN(TestTDownloader)

#include "tst_downloader.moc"
