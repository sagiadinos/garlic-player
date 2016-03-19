#include <QString>
#include <QtTest>

#include "tools/configuration.h"

class InheritedTConfiguration : public TConfiguration
{
public:
    void test_createDirectories(){createDirectories();}

};



class TestTConfiguration : public QObject
{
    Q_OBJECT

public:
    TestTConfiguration(){}

private Q_SLOTS:
    void test_createDirectories();
    void test_getConfigXML();
    void test_getUserAgent();
};

void TestTConfiguration::test_createDirectories()
{
    InheritedTConfiguration MyConfig;
    QDir             dir;
    MyConfig.test_createDirectories();
    dir.setPath("var");
    QCOMPARE(dir.exists(), true);
    dir.removeRecursively(); // clean up

    dir.setPath("configuration");
    QCOMPARE(dir.exists(), true);
    dir.removeRecursively(); // clean up
}

void TestTConfiguration::test_getConfigXML()
{
    // First test without a config file
    InheritedTConfiguration MyConfig_no_xml;
    QCOMPARE(MyConfig_no_xml.getIndexPath(), QString("http://indexes.smil-admin.com/"));

    // copy config
    QFile::copy("../tests/data/config.xml", "configuration/config.xml");
    // now test with a config
    QDir dir;
    dir.mkdir("configuration");
    InheritedTConfiguration MyConfig_xml;
    QCOMPARE(MyConfig_xml.getIndexPath(), QString("http://indexes.of-a-smil-server.com/"));
    dir.setPath("configuration");
    dir.removeRecursively();// clean up
    return;
}

void TestTConfiguration::test_getUserAgent()
{
    InheritedTConfiguration MyConfig;
    QCOMPARE(MyConfig.getUserAgent(), QString("GAPI/1.0 (UUID:e15dbc25-a550-447a-b4a7-97a22e377397; NAME:97a22e377397) garlic-linux/0.1.0 (MODEL:Garlic)"));
}


QTEST_APPLESS_MAIN(TestTConfiguration)

#include "tst_configuration.moc"
