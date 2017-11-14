#include <QString>
#include <QtTest>
#include "files/base_manager.h"

class I_BaseManager : public BaseManager
{
        Q_OBJECT
    public:
        bool   test_isRemote(QString src){return isRemote(src);}
        bool   test_isRelative(QString src){return isRelative(src);}
};


class TestBaseManager : public QObject
{
        Q_OBJECT
    public:
        TestBaseManager(){}

    private Q_SLOTS:
        void testIsRemote();
        void testIsRelative();
};


void TestBaseManager::testIsRemote()
{
    I_BaseManager   *MyBaseManager  = new I_BaseManager();
    QVERIFY(MyBaseManager->test_isRemote("http://google.de"));
    QVERIFY(MyBaseManager->test_isRemote("https://google.de"));
    QVERIFY(MyBaseManager->test_isRemote("ftps://google.de"));
    QVERIFY(MyBaseManager->test_isRemote("ftp://google.de"));
    QVERIFY(!MyBaseManager->test_isRemote("/google.de"));
    QVERIFY(!MyBaseManager->test_isRemote("google.de"));
}

void TestBaseManager::testIsRelative()
{
    I_BaseManager   *MyBaseManager  = new I_BaseManager();
    QVERIFY(MyBaseManager->test_isRelative("path_to/tralala"));
    QVERIFY(MyBaseManager->test_isRelative("./path_to/tralala"));
    QVERIFY(!MyBaseManager->test_isRelative("/path_to/tralala"));
    QVERIFY(!MyBaseManager->test_isRelative("//path_to/tralala"));
}


QTEST_APPLESS_MAIN(TestBaseManager)

#include "tst_base_manager.moc"
