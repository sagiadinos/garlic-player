#include "AutoTest.h"
#include <QString>
#include <QtTest>

class TesttestTest : public QObject
{
    Q_OBJECT

public:
    TesttestTest();

private Q_SLOTS:
    void testCase1();
};

TesttestTest::TesttestTest()
{
}

void TesttestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TesttestTest)

#include "tst_testtesttest.moc"
