#include <QString>
#include <QtTest>
#include <smilparser/tools/downloader.h>

class TestTDownloader : public QObject
{
    Q_OBJECT

public:
    TestTDownloader(){}
    ~TestTDownloader();
private Q_SLOTS:
    void testAgentString();
    void testWithoutAgentString();
    void testDownloadSmil();
    void testDownloadMedia();
};


TestTDownloader::~TestTDownloader()
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
}

void TestTDownloader::testAgentString()
{
    TDownloader *MyDownloader = new TDownloader();
    QString user_agent = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    QString file_name  = "./agent.txt";
    MyDownloader->setUserAgent(user_agent);
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=show_agent_string");
    QSignalSpy spy(MyDownloader, SIGNAL(downloadSucceed()));
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
    TDownloader *MyDownloader = new TDownloader();
    QString user_agent = "";
    QString file_name  = "./agent.txt";
    MyDownloader->setUserAgent(user_agent);
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=show_agent_string");

    QFile file(file_name);
    QVERIFY(!file.exists());
}

void TestTDownloader::testDownloadSmil()
{
    TDownloader *MyDownloader = new TDownloader();
    QString user_agent = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    QString file_name  = "./test.smil";
    MyDownloader->setUserAgent(user_agent);
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_new");
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed()));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
    QFileInfo fi(file);
    QDateTime last_modified = fi.lastModified();
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_new");
    QSignalSpy spy2(MyDownloader, SIGNAL(downloadCanceled()));
    while (spy2.count() == 0)
        QTest::qWait(200);
    QFileInfo fi2(file);
    QCOMPARE(last_modified, fi2.lastModified());

    QTest::qWait(500); // to write file with new date

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_cached");
    QSignalSpy spy3(MyDownloader, SIGNAL(downloadCanceled()));
    while (spy3.count() == 0)
        QTest::qWait(200);
    QFileInfo fi3(file);
    QCOMPARE(last_modified, fi3.lastModified());


    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_smil_updated");
    QSignalSpy spy4(MyDownloader, SIGNAL(downloadSucceed()));
    while (spy4.count() == 0)
        QTest::qWait(200);
    QFileInfo fi4(file);
    QVERIFY(last_modified < fi4.lastModified());
    file.remove();
}

void TestTDownloader::testDownloadMedia()
{
    TDownloader *MyDownloader = new TDownloader();
    QString user_agent = "GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)";
    QString file_name  = "./test.jpg";
    MyDownloader->setUserAgent(user_agent);
    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_new");
    QSignalSpy spy1(MyDownloader, SIGNAL(downloadSucceed()));
    while (spy1.count() == 0)
        QTest::qWait(200);
    QFile file(file_name);
    QVERIFY(file.exists());
    QFileInfo fi(file);
    QDateTime last_modified = fi.lastModified();

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_new");
    QSignalSpy spy2(MyDownloader, SIGNAL(downloadCanceled()));
    while (spy2.count() == 0)
        QTest::qWait(200);
    QFileInfo fi2(file);
    QCOMPARE(last_modified, fi2.lastModified());
    QTest::qWait(500); // to write file with new date

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_cached");
    QSignalSpy spy3(MyDownloader, SIGNAL(downloadCanceled()));
    while (spy3.count() == 0)
        QTest::qWait(200);
    QFileInfo fi3(file);
    QCOMPARE(last_modified, fi3.lastModified());

    MyDownloader->checkFiles(file_name, "http://smil-admin.com/garlic/test.php?action=get_media_updated");
    QSignalSpy spy4(MyDownloader, SIGNAL(downloadSucceed()));
    while (spy4.count() == 0)
        QTest::qWait(200);
    QFileInfo fi4(file);
    QVERIFY(last_modified < fi4.lastModified());
    file.remove();
}


QTEST_MAIN(TestTDownloader)

#include "tst_downloader.moc"
