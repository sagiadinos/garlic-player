#include <QString>
#include <QtTest>
#include <files/disc_space.h>

class TestDiscSpace : public QObject
{
    Q_OBJECT

public:
    TestDiscSpace(){}

private Q_SLOTS:
    void testFreedSpace();
};


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

QTEST_APPLESS_MAIN(TestDiscSpace)

#include "tst_disc_space.moc"
