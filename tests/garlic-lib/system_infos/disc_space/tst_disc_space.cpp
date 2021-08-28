#include <QString>
#include <QtTest>
#include "system_infos/disc_space.h"

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
        void testInit();
        void testRefresh();
    private:
        void createFileInKiloByte(int size);
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

void TestDiscSpace::testInit()
{
    SystemInfos::DiscSpace MyDiscSpace;
    QStorageInfo   storage;

    MyDiscSpace.init(cache_dir.path());
    storage.setPath(cache_dir.path());
    quint64 free  =  storage.bytesAvailable();
    quint64 total =  storage.bytesTotal();

    quint64 free1 =  MyDiscSpace.getBytesFree();
    quint64 total1 = MyDiscSpace.getBytesTotal();

    QVERIFY(free > free1);
    QVERIFY(total > total1);

    QCOMPARE(total1, total - MyDiscSpace.getBytesLocked());

}

void TestDiscSpace::testRefresh()
{
    SystemInfos::DiscSpace MyDiscSpace;
    QStorageInfo   storage;

    MyDiscSpace.init(cache_dir.path());

    quint64 total1 = MyDiscSpace.getBytesTotal();
    quint64 free1 =  MyDiscSpace.getBytesFree();

    createFileInKiloByte(1024); // create 1 MB file

    quint64 total2 = MyDiscSpace.getBytesTotal();
    quint64 free2 =  MyDiscSpace.getBytesFree();

    QCOMPARE(total1, total2);
    QCOMPARE(free1, free2);

    MyDiscSpace.recalculate();

    total2 = MyDiscSpace.getBytesTotal();
    free2 =  MyDiscSpace.getBytesFree();

    QCOMPARE(free1, free2 + 1048576);
    QCOMPARE(total1, total2);

}


void TestDiscSpace::createFileInKiloByte(int size)
{
    QString filename=cache_dir.path()+"/data.txt";
    QFile file( filename );
    QString s = "s";
    for (int i = 0; i < 1024; i++)
    {
        file.write("s", 1);
    }
    if (file.open(QIODevice::ReadWrite))
    {
        for (int i = 0; i < size; i++)
        {
            file.write(s.toLatin1(), 1024);
        }
    }
    file.close();
}

QTEST_APPLESS_MAIN(TestDiscSpace)

#include "tst_disc_space.moc"
