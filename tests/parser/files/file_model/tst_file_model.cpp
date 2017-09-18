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
#include <files/file_model.h>

class I_FileModel : public FileModel
{
    Q_OBJECT
public:
    I_FileModel(Network *network, QString cache) : FileModel(network, cache)
    {
        Q_UNUSED(network);setCachePath(cache);
    }
    void    test_extractWgt(){extractWgt();}
    void    test_checkForExtension(){checkForExtension();}
    bool    test_freeDiskSpace(qint64 size_to_delete){return freeDiscSpace(size_to_delete);}
    QString test_determineHashedFilePath(){return determineHashedFilePath();}
    void    test_deleteFile(QString file_path){deleteFile(file_path);}
    void    test_deleteDirectory(QString dir_path){deleteDirectory(dir_path);}
    void    setBytesAvailable(qint64 ba){bytes_available = ba;}
    qint64  getDeletedByteSize(){return MyDiscSpace.getBytesDeleted();}

protected:
    qint64  bytes_available;
    qint64  getBytesAvailable(){return bytes_available;}
};


void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){Q_UNUSED(type); Q_UNUSED(context); Q_UNUSED(msg)}
class TestFileModel : public QObject
{
    Q_OBJECT

public:
    TestFileModel(){}
    QDir cache_dir;

private Q_SLOTS:
    void init();
    void cleanup();
    void registerRemoteFile();
    void registerLocalFile();
    void testIsRemote();
    void testFreeDiscSpacePartly();
    void testFreeDiscSpaceAll();
    void testCheckForExtensionIfWgt();
    void testCheckForExtensionIfOther();
    void testExtractWgt();
    void testDetermineHashedFilePath();
    void testDeleteFile();
    void testDeleteDirectory();
    void createFilesForDelete();
};

void TestFileModel::init()
{
   qInstallMessageHandler(noMessageOutput);
    cache_dir.setPath("./");
    if (cache_dir.mkdir("cache"))
        cache_dir.setPath("./cache");
}

void TestFileModel::cleanup()
{
   cache_dir.setPath("./cache"); // for cases whem test breaks and did nor cleanup correct
   cache_dir.removeRecursively();
}

void TestFileModel::registerRemoteFile()
{
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QSignalSpy spy(MyFileModel, SIGNAL(succeed(QString, QString)));
    QVERIFY(spy.isValid());

    MyFileModel->registerFile("http://smil-admin.com/garlic/server.jpg");

    int i = 0;
    while (spy.count() == 0 && i < 2000)
        QTest::qWait(200);
    QTest::qWait(1000);

    QCOMPARE(spy.count(), 1);

    QFileInfo fi;
    fi.setFile("./cache/d78fbabf91cc808710186ab04ae110a1.jpg");
    QVERIFY(fi.exists());

    QString src_file_path = qvariant_cast<QString>(spy.at(0).at(0));
    QString local_file_path = qvariant_cast<QString>(spy.at(0).at(1));
    QCOMPARE(src_file_path, QString("http://smil-admin.com/garlic/server.jpg"));

    QVERIFY(QString("file://") != local_file_path.mid(0, 7)); // make sure that non wgt do not start with file://

    QVERIFY(local_file_path.contains("cache/d78fbabf91cc808710186ab04ae110a1.jpg"));
}


void TestFileModel::registerLocalFile()
{
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QFile file(":/test.wgt");
    file.copy("./cache/test.wgt");
    QSignalSpy spy(MyFileModel, SIGNAL(succeed(QString, QString)));
    QVERIFY(spy.isValid());

    MyFileModel->registerFile("./cache/test.wgt");

    int i = 0;
    while (spy.count() == 0 && i < 2000)
        QTest::qWait(200);
    QCOMPARE(spy.count(), 1);

    QFileInfo fi;
    fi.setFile("./cache/test/index.html");
    QVERIFY(fi.exists());
    fi.setFile("./cache/test/green-bullet.png");
    QVERIFY(fi.exists());
    fi.setFile("./cache/test/test.js");
    QVERIFY(fi.exists());

    QFile js("./cache/test/test.js");
    QVERIFY(js.open(QIODevice::ReadWrite));
    QCOMPARE(js.readAll(), QByteArray("function test{}{return;}\r\n"));
    file.close();
    QString src_file_path = qvariant_cast<QString>(spy.at(0).at(0));
    QString local_file_path = qvariant_cast<QString>(spy.at(0).at(1));
    QCOMPARE(src_file_path, QString("./cache/test.wgt"));

    QVERIFY(local_file_path.contains("file://"));
    QVERIFY(local_file_path.contains("/cache/test/index.html"));
}

void TestFileModel::testIsRemote()
{
    FileModel *MyFileModel = new FileModel(new Network("This-is-my-agent"), cache_dir.path());
    QVERIFY(MyFileModel->isRemote("http://google.de"));
    QVERIFY(MyFileModel->isRemote("https://google.de"));
    QVERIFY(MyFileModel->isRemote("ftps://google.de"));
    QVERIFY(MyFileModel->isRemote("ftp://google.de"));
    QVERIFY(!MyFileModel->isRemote("/google.de"));
    QVERIFY(!MyFileModel->isRemote("google.de"));
}

void TestFileModel::testFreeDiscSpacePartly()
{
    createFilesForDelete();
    QFile png1("./cache/1.png");
    QFile png2("./cache/2.png");
    QFile png3("./cache/3.png");
    QFile png4("./cache/4.png");
    QFile png5("./cache/5.png");
    QFile wgt1("./cache/1.wgt");
    QFile wgt2("./cache/2.wgt");


    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());

    MyFileModel->test_freeDiskSpace(1000);

    QVERIFY(wgt1.exists());
    QVERIFY(wgt2.exists());
    QVERIFY(png1.exists());
    QVERIFY(png2.exists());
    QVERIFY(png3.exists());
    QVERIFY(!png4.exists());
    QVERIFY(!png5.exists());
    QCOMPARE(MyFileModel->getDeletedByteSize(), 1258);
}

void TestFileModel::testFreeDiscSpaceAll()
{
    createFilesForDelete();
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QFile png1("./cache/1.png");
    QFile png2("./cache/2.png");
    QFile png3("./cache/3.png");
    QFile png4("./cache/4.png");
    QFile png5("./cache/5.png");
    QFile wgt1("./cache/1.wgt");
    QFile wgt2("./cache/2.wgt");

    QFileInfo fiwgt("./cache/2.wgt");
    MyFileModel->setSrcFilePath(fiwgt.absoluteFilePath());
    MyFileModel->setLocalFileInfo(fiwgt);
    MyFileModel->test_extractWgt(); // => 4969 Byte
    QDir dir("./cache/2");
    QVERIFY(dir.exists());


    // Free all 10350 Byte
    MyFileModel->test_freeDiskSpace(10300);

    QVERIFY(!png1.exists());
    QVERIFY(!png2.exists());
    QVERIFY(!png3.exists());
    QVERIFY(!png4.exists());
    QVERIFY(!png5.exists());
    QVERIFY(!wgt1.exists());
    QVERIFY(!wgt2.exists());
    QVERIFY(!dir.exists());
}

void TestFileModel::testCheckForExtensionIfWgt()
{
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QFile file(":/test.wgt");
    file.copy("./cache/test.wgt");

    QFileInfo wgt("./cache/test.wgt");
    MyFileModel->setSrcFilePath(wgt.absoluteFilePath());
    MyFileModel->setLocalFileInfo(wgt);

    MyFileModel->test_checkForExtension();
#if defined  Q_OS_WIN
    QVERIFY(MyFileModel->getLocalFilePath().contains("file://")); // Windows needs file:// for open absolute paths in WebEngine
#else
    QVERIFY(MyFileModel->getLocalFilePath().contains("file:///"));
#endif
    QVERIFY(MyFileModel->getLocalFilePath().contains("/cache/test/index.html"));
}

void TestFileModel::testCheckForExtensionIfOther()
{
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QFile file(":/test.png");
    file.copy("./uiuiui.png");

    QFileInfo wgt("./uiuiui.png");
    MyFileModel->setSrcFilePath(wgt.absoluteFilePath());
    MyFileModel->setLocalFileInfo(wgt);

    MyFileModel->test_checkForExtension();
    QVERIFY(MyFileModel->getLocalFilePath().contains("/"));
    QVERIFY(MyFileModel->getLocalFilePath().contains("/uiuiui.png"));

    file.remove();
}

void TestFileModel::testExtractWgt()
{
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QFile file(":/test.wgt");
    file.copy("./cache/test.wgt");

    QFileInfo wgt("./cache/test.wgt");
    MyFileModel->setSrcFilePath(wgt.absoluteFilePath());
    MyFileModel->setLocalFileInfo(wgt);
    MyFileModel->test_extractWgt();
    QFileInfo fi;
    fi.setFile("./cache/test/index.html");
    QVERIFY(fi.exists());
    fi.setFile("./cache/test/green-bullet.png");
    QVERIFY(fi.exists());
    fi.setFile("./cache/test/test.js");
    QVERIFY(fi.exists());

    QFile js("./cache/test/test.js");
    QVERIFY(js.open(QIODevice::ReadWrite));
    QCOMPARE(js.readAll(), QByteArray("function test{}{return;}\r\n"));
    file.close();
}

void TestFileModel::testDetermineHashedFilePath()
{
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());

    MyFileModel->setSrcFilePath("https://www.smil-admin.com/uiuiui.wgt");

    QCOMPARE(QString("ff09d70446a146d3f8426b9136db0a00.wgt"), MyFileModel->test_determineHashedFilePath());
}

void TestFileModel::testDeleteFile()
{
    createFilesForDelete();
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QFile png("./cache/1.png");
    QFileInfo fipng("./cache/1.png");
    MyFileModel->test_deleteFile(fipng.absoluteFilePath());

    QCOMPARE(MyFileModel->getDeletedByteSize(), 629);
}

void TestFileModel::testDeleteDirectory()
{
    Network *MyNetwork = new Network("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyNetwork, cache_dir.path());
    QFile file(":/test.wgt");
    file.copy("./cache/test.wgt");

    QFileInfo wgt("./cache/test.wgt");
    QDir dir("./cache/test");
    MyFileModel->setSrcFilePath(wgt.absoluteFilePath());
    MyFileModel->setLocalFileInfo(wgt);
    MyFileModel->test_extractWgt();
    MyFileModel->test_deleteDirectory(dir.absolutePath());

    QCOMPARE(MyFileModel->getDeletedByteSize(), 864);
}

void TestFileModel::createFilesForDelete()
{
    // create some deleteable png files and wgt
    QFile file_wgt(":/test.wgt");
    file_wgt.copy("./cache/1.wgt"); // without directory => 1118
    QFile file_png(":/test.png");
    file_png.copy("./cache/1.png"); // => 629 Byte
    file_png.copy("./cache/2.png");
    file_png.copy("./cache/3.png");
    file_png.copy("./cache/4.png");
    file_png.copy("./cache/5.png"); // => 3145 Byte
    file_wgt.copy("./cache/2.wgt"); //  => 2236 Byte

}

QTEST_MAIN(TestFileModel)

#include "tst_file_model.moc"
