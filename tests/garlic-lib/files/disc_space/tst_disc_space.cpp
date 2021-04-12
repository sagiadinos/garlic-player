#include <QString>
#include <QtTest>
#include <files/disc_space.h>

class I_DiscSpace : public DiscSpace
{
    Q_OBJECT
public:
    I_DiscSpace(QString path)
    {
        Q_UNUSED(path);
    }
    bool    test_freeDiskSpace(qint64 size_to_delete){return freeDiscSpace(size_to_delete);}
    bool    test_deleteFile(QString file_path){return deleteFile(file_path);}
    bool    test_deleteDirectory(QString dir_path){return deleteDirectory(dir_path);}
    qint64  test_calculateDirectorySize(QString dir_path){return calculateDirectorySize(dir_path);}
    qint64  getDeletedByteSize(){return getBytesDeleted();}
};

void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){Q_UNUSED(type); Q_UNUSED(context); Q_UNUSED(msg)}
class TestDiscSpace : public QObject
{
    Q_OBJECT

public:
    TestDiscSpace(){}
    QDir cache_dir;

private Q_SLOTS:
    void init();
    void cleanup();
    void testCalculateNeededDiscSpaceToFree();
    void testCalculateDirectorySize();
    void testDeleteFile();
    void testDeleteDirectory();
    void testFreedSpace();
    void testFreeDiscSpacePartly();
    void testFreeDiscSpaceAll();
    void testFreeDiscSpaceToMuch();
    void createFilesForDelete();
    void openCloseForLastRead(QString source, QString copy);
};

void TestDiscSpace::init()
{
    qInstallMessageHandler(noMessageOutput);
    cache_dir.setPath("./");
    if (cache_dir.mkpath("cache/test_dir"))
        cache_dir.setPath("./cache");
}

void TestDiscSpace::cleanup()
{
   cache_dir.setPath("./cache"); // for cases whem test breaks and did nor cleanup correct
   cache_dir.removeRecursively();
}

void TestDiscSpace::testCalculateNeededDiscSpaceToFree()
{
    I_DiscSpace *MyDiscSpace = new I_DiscSpace(cache_dir.path());
    MyDiscSpace->setBytesAvailable(1000);
    QCOMPARE(MyDiscSpace->calculateNeededDiscSpaceToFree(1100), 110);

    MyDiscSpace->setBytesAvailable(10000);
    QCOMPARE(MyDiscSpace->calculateNeededDiscSpaceToFree(1100), 0);

    QCOMPARE(MyDiscSpace->calculateNeededDiscSpaceToFree(9568), 0);

}

void TestDiscSpace::testDeleteFile()
{
    createFilesForDelete();
    I_DiscSpace *MyDiscSpace = new I_DiscSpace(cache_dir.path());
    QFile png("./cache/1.png");
    QFileInfo fipng("./cache/1.png");
    QVERIFY(png.exists());

    QVERIFY(MyDiscSpace->test_deleteFile(fipng.absoluteFilePath()));
    QCOMPARE(MyDiscSpace->getDeletedByteSize(), 629);
    QVERIFY(!png.exists());

    // try to get a true cause missing file is not an error
    QVERIFY(MyDiscSpace->test_deleteFile(fipng.absoluteFilePath()));
    QCOMPARE(MyDiscSpace->getDeletedByteSize(), 629);


    // point to a direcory to get an error
    fipng.setFile("./cache/test_dir");
    QVERIFY(!MyDiscSpace->test_deleteFile(fipng.absoluteFilePath()));

}

void TestDiscSpace::testDeleteDirectory()
{
    createFilesForDelete();
    I_DiscSpace *MyDiscSpace = new I_DiscSpace(cache_dir.path());

    QFile wgt("./cache/test_dir/1.wgt");
    QFile png("./cache/test_dir/1.png");
    QDir dir("./cache/test_dir");
    QVERIFY(wgt.exists());
    QVERIFY(png.exists());
    QVERIFY(dir.exists());

    QVERIFY(MyDiscSpace->test_deleteDirectory(dir.absolutePath()));
    QVERIFY(!wgt.exists());
    QVERIFY(!png.exists());
    QVERIFY(!dir.exists());
    QCOMPARE(MyDiscSpace->getDeletedByteSize(), 1747);

    // try again, cause a not existing directory should not return an error
    QVERIFY(MyDiscSpace->test_deleteDirectory(dir.absolutePath()));

}

void TestDiscSpace::testCalculateDirectorySize()
{
    createFilesForDelete();
    I_DiscSpace *MyDiscSpace = new I_DiscSpace(cache_dir.path());
    QCOMPARE(MyDiscSpace->test_calculateDirectorySize(cache_dir.absolutePath()), 7128);
}

void TestDiscSpace::testFreedSpace()
{
    DiscSpace MyDiscSpace;
    MyDiscSpace.freedSpace(159);
    QCOMPARE(MyDiscSpace.getBytesDeleted(), 159);
    MyDiscSpace.freedSpace(1);
    QCOMPARE(MyDiscSpace.getBytesDeleted(), 160);
    MyDiscSpace.freedSpace(1000);
    QCOMPARE(MyDiscSpace.getBytesDeleted(), 1160);
    MyDiscSpace.freedSpace(40);
    QCOMPARE(MyDiscSpace.getBytesDeleted(), 1200);
    MyDiscSpace.freedSpace(800);
    QCOMPARE(MyDiscSpace.getBytesDeleted(), 2000);
}

void TestDiscSpace::testFreeDiscSpacePartly()
{
    createFilesForDelete();
    QFile png1("./cache/1.png");
    QFile png2("./cache/2.png");
    QFile png3("./cache/3.png");
    QFile png4("./cache/4.png");
    QFile png5("./cache/5.png");
    QFile wgt1("./cache/1.wgt");
    QFile wgt2("./cache/2.wgt");
    QFile wgt_t1("./cache/test_dir/1.wgt");
    QFile png_t1("./cache/test_dir/1.png");
    QDir dir("./cache/test_dir");

    QVERIFY(png1.exists());
    QVERIFY(png2.exists());
    QVERIFY(png3.exists());
    QVERIFY(png4.exists());
    QVERIFY(png5.exists());
    QVERIFY(wgt1.exists());
    QVERIFY(wgt2.exists());
    QVERIFY(wgt_t1.exists());
    QVERIFY(png_t1.exists());
    QVERIFY(dir.exists());

    QString cache = cache_dir.path();
    I_DiscSpace *MyDiscSpace = new I_DiscSpace(cache_dir.path());
    MyDiscSpace->init(cache_dir.path());
    MyDiscSpace->test_freeDiskSpace(1000);

    QVERIFY(MyDiscSpace->getDeletedByteSize()> 1000);
}

void TestDiscSpace::testFreeDiscSpaceAll()
{
    createFilesForDelete();
    QFile png1("./cache/1.png");
    QFile png2("./cache/2.png");
    QFile png3("./cache/3.png");
    QFile png4("./cache/4.png");
    QFile png5("./cache/5.png");
    QFile wgt1("./cache/1.wgt");
    QFile wgt2("./cache/2.wgt");
    QFile wgt_t1("./cache/test_dir/1.wgt");
    QFile png_t1("./cache/test_dir/1.png");
    QDir dir("./cache/test_dir");
    QVERIFY(png1.exists());
    QVERIFY(png2.exists());
    QVERIFY(png3.exists());
    QVERIFY(png4.exists());
    QVERIFY(png5.exists());
    QVERIFY(wgt1.exists());
    QVERIFY(wgt2.exists());
    QVERIFY(wgt_t1.exists());
    QVERIFY(png_t1.exists());
    QVERIFY(dir.exists());

    I_DiscSpace *MyDiscSpace = new I_DiscSpace(cache_dir.path());
    MyDiscSpace->init(cache_dir.path());
    // Free all
    MyDiscSpace->test_freeDiskSpace(7128);

    QVERIFY(!png1.exists());
    QVERIFY(!png2.exists());
    QVERIFY(!png3.exists());
    QVERIFY(!png4.exists());
    QVERIFY(!wgt1.exists());
    QVERIFY(!wgt2.exists());
    QVERIFY(!wgt_t1.exists());
    QVERIFY(!png_t1.exists());
    QVERIFY(!dir.exists());

    QCOMPARE(MyDiscSpace->getDeletedByteSize(),  7128);
}

void TestDiscSpace::testFreeDiscSpaceToMuch()
{
    createFilesForDelete();
    QFile png1("./cache/1.png");
    QFile png2("./cache/2.png");
    QFile png3("./cache/3.png");
    QFile png4("./cache/4.png");
    QFile png5("./cache/5.png");
    QFile wgt1("./cache/1.wgt");
    QFile wgt2("./cache/2.wgt");
    QFile wgt_t1("./cache/test_dir/1.wgt");
    QFile png_t1("./cache/test_dir/1.png");
    QDir dir("./cache/test_dir");

    I_DiscSpace *MyDiscSpace = new I_DiscSpace(cache_dir.path());
    MyDiscSpace->init(cache_dir.path());
    // Free more than possible
    QVERIFY(!MyDiscSpace->test_freeDiskSpace(8000));

    QCOMPARE(MyDiscSpace->getDeletedByteSize(),  7128);
}

void TestDiscSpace::createFilesForDelete()
{
    // put two files in the testdir
    openCloseForLastRead(":/test.png", "./cache/test_dir/1.png"); // => 629 Byte
    openCloseForLastRead(":/test.wgt", "./cache/test_dir/1.wgt"); // without directory => 1118

    openCloseForLastRead(":/test.wgt", "./cache/2.wgt"); //  => 2236 Byte

    openCloseForLastRead(":/test.png", "./cache/5.png");  // => 3145 Byte
    openCloseForLastRead(":/test.png", "./cache/4.png");
    openCloseForLastRead(":/test.png", "./cache/3.png");
    openCloseForLastRead(":/test.png", "./cache/2.png");
    openCloseForLastRead(":/test.png", "./cache/1.png");

    // create some deleteable png files and wgt
    openCloseForLastRead(":/test.wgt", "./cache/1.wgt"); // without directory => 1118
}

void TestDiscSpace::openCloseForLastRead(QString source, QString destination)
{
    QFile original(source);
    original.copy(destination); // without directory => 1118
    QTest::qWait(10); // to get different values for lastRead
    QFile copy(destination);
    copy.open(QIODevice::ReadOnly);
    copy.close();
}

QTEST_APPLESS_MAIN(TestDiscSpace)

#include "tst_disc_space.moc"
