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
    I_FileModel(TConfiguration *config) : FileModel(config){ Q_UNUSED(config);}
    void      test_handleExistingLocalFile(QString src, QString local){handleExistingLocalFile(src, local);}
    QString   test_determineCachePathByMediaExtension(QString src, QString local){return determineCachePathByMediaExtension(src, local);}
    QString   test_determineHashedFilePath(QString src){return determineHashedFilePath(src);}
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
    void testIsRemote();
    void testHandleExistingLocalFile();
    void testDetermineCachePathByMediaExtensionIfWgt();
    void testDetermineCachePathByMediaExtensionIfOther();
    void testDetermineHashedFilePath();
};

void TestFileModel::init()
{
   qInstallMessageHandler(noMessageOutput);
    cache_dir.setPath("./");
    if (cache_dir.mkpath("./cache/test"))
        cache_dir.setPath("./cache");
}

void TestFileModel::cleanup()
{
   cache_dir.setPath("./cache"); // for cases whem test breaks and did nor cleanup correct
   cache_dir.removeRecursively();
}

void TestFileModel::testIsRemote()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings);
    MyConfiguration->setUserAgent("This-is-my-agent");
    FileModel *MyFileModel = new FileModel(MyConfiguration);
    MyFileModel->setCachePath(cache_dir.absoluteFilePath("./cache"));
    QVERIFY(MyFileModel->isRemote("http://google.de"));
    QVERIFY(MyFileModel->isRemote("https://google.de"));
    QVERIFY(MyFileModel->isRemote("ftps://google.de"));
    QVERIFY(MyFileModel->isRemote("ftp://google.de"));
    QVERIFY(!MyFileModel->isRemote("/google.de"));
    QVERIFY(!MyFileModel->isRemote("google.de"));
}


void TestFileModel::testHandleExistingLocalFile()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings);
    MyConfiguration->setUserAgent("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyConfiguration);
    MyFileModel->setCachePath(cache_dir.absoluteFilePath("./cache"));
    QFile file(":/test.wgt");
    file.copy("./cache/test.wgt");
    MyFileModel->test_handleExistingLocalFile("./cache/test.wgt", "./cache/test.wgt");

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
    QString local_file_path(MyFileModel->findBySrcMediaPath("./cache/test.wgt"));

    QVERIFY(local_file_path.contains("file://"));
    QVERIFY(local_file_path.contains("/cache/test/index.html"));
}

void TestFileModel::testDetermineCachePathByMediaExtensionIfWgt()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings);
    MyConfiguration->setUserAgent("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyConfiguration);
    MyFileModel->setCachePath(cache_dir.absoluteFilePath("./cache"));
    QFile file(":/test.wgt");
    file.copy("./cache/test.wgt");

    QString local = MyFileModel->test_determineCachePathByMediaExtension("./cache/test.wgt", "./cache/test.wgt");


#if defined  Q_OS_WIN
    QVERIFY(local.contains("file://")); // Windows needs file:// for open absolute paths in WebEngine
#else
    QVERIFY(local.contains("file:///"));
#endif
    QVERIFY(local.contains("/cache/test/index.html"));
}

void TestFileModel::testDetermineCachePathByMediaExtensionIfOther()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings);
    MyConfiguration->setUserAgent("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyConfiguration);
    MyFileModel->setCachePath(cache_dir.absoluteFilePath("./cache"));
    QFile file(":/test.png");
    file.copy("./uiuiui.png");

    QString local = MyFileModel->test_determineCachePathByMediaExtension("./uiuiui.png", "./uiuiui.png");

    QVERIFY(local.contains("/"));
    QVERIFY(local.contains("/uiuiui.png"));

    file.remove();
}

void TestFileModel::testDetermineHashedFilePath()
{
    TConfiguration *MyConfiguration = new TConfiguration(new QSettings);
    MyConfiguration->setUserAgent("This-is-my-agent");
    I_FileModel *MyFileModel = new I_FileModel(MyConfiguration);
    MyFileModel->setCachePath(cache_dir.absoluteFilePath("./cache"));


    QCOMPARE(QString("ff09d70446a146d3f8426b9136db0a00.wgt"), MyFileModel->test_determineHashedFilePath("https://www.smil-admin.com/uiuiui.wgt"));
}

QTEST_MAIN(TestFileModel)

#include "tst_file_model.moc"
