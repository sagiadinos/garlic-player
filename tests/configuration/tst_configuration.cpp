#include <QString>
#include <QtTest>

#include "configuration.h"

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
    MyConfig.setBaseDirectory(dir.absolutePath()+"/");
    MyConfig.test_createDirectories();
    dir.setPath("var");
    QCOMPARE(dir.exists(), true);
    dir.setPath("var/media");
    QCOMPARE(dir.exists(), true);
    dir.setPath("configuration");
    QCOMPARE(dir.exists(), true);

    dir.setPath("var");
    dir.removeRecursively();
    dir.setPath("configuration");
    dir.removeRecursively();
}

void TestTConfiguration::test_getConfigXML()
{
    // First test without a config file
    InheritedTConfiguration MyConfig_no_xml;
    QCOMPARE(MyConfig_no_xml.getIndexServer(), QString("http://indexes.smil-admin.com"));

    // now test with a config
    QDir dir;
    InheritedTConfiguration MyConfig_xml;
    MyConfig_xml.setBaseDirectory(dir.absolutePath()+"/");
    dir.mkdir("configuration");
    QFile::copy("../data/config.xml", "configuration/config.xml");
    QCOMPARE(MyConfig_xml.getIndexServer(), QString("http://indexes.of-a-smil-server.com"));

    // clean up
    dir.setPath("configuration");
    dir.removeRecursively();
    return;
}

void TestTConfiguration::test_getUserAgent()
{
    InheritedTConfiguration MyConfig;
    QCOMPARE(MyConfig.getUserAgent(), QString("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:5c338e54bcae) linux/0.1.0 (MODEL:Garlic)"));
}


QTEST_APPLESS_MAIN(TestTConfiguration)

#include "tst_configuration.moc"
