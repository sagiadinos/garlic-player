#include <QString>
#include <QtTest>
#include "network_queue.h"

class I_NetworkQueue : public NetworkQueue
{
    Q_OBJECT
public:
    explicit I_NetworkQueue(QByteArray ua) : NetworkQueue(ua){}
    void test_doSucceed(QObject *network){doSucceed(network);}
    void test_doNotCacheable(QObject *network){doNotCacheable(network);}
    void test_doNotModified(QObject *network){doNotModified(network);}
    void test_doFailed(QObject *network){doFailed(network);}

};
void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){Q_UNUSED(type); Q_UNUSED(context); Q_UNUSED(msg)}

class TestNetworkQueue : public QObject
{
        Q_OBJECT

    public:
    private Q_SLOTS:
        void initTestCase(){qInstallMessageHandler(noMessageOutput);}
        void testInsertQueue();
        void testDoSuccess();
        void testDoNotCacheable();
        void testDoNotModified();
        void testDoFailed();
};


void TestNetworkQueue::testInsertQueue()
{
    QByteArray agent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    NetworkQueue *MyNetworkQueue = new NetworkQueue(agent);

    MyNetworkQueue->insertQueue("http://notexisting1.ddd", "http://notexisting1.ddd");
    MyNetworkQueue->insertQueue("http://notexisting2.ddd", "http://notexisting2.ddd");
    MyNetworkQueue->insertQueue("http://notexisting3.ddd", "http://notexisting3.ddd");
    MyNetworkQueue->insertQueue("http://notexisting4.ddd", "http://notexisting4.ddd");
    MyNetworkQueue->insertQueue("http://notexisting5.ddd", "http://notexisting5.ddd");
    MyNetworkQueue->insertQueue("http://notexisting6.ddd", "http://notexisting6.ddd");
    MyNetworkQueue->insertQueue("http://notexisting7.ddd", "http://notexisting7.ddd");
    MyNetworkQueue->insertQueue("http://notexisting8.ddd", "http://notexisting8.ddd");
    MyNetworkQueue->insertQueue("http://notexisting9.ddd", "http://notexisting9.ddd");
    MyNetworkQueue->insertQueue("http://notexisting10.ddd", "http://notexisting10.ddd");
    MyNetworkQueue->insertQueue("http://notexisting11.ddd", "http://notexisting11.ddd");
    MyNetworkQueue->insertQueue("http://notexisting12.ddd", "http://notexisting12.ddd");
    MyNetworkQueue->insertQueue("http://notexisting13.ddd", "http://notexisting13.ddd");
    MyNetworkQueue->insertQueue("http://notexisting14.ddd", "http://notexisting14.ddd");
    MyNetworkQueue->insertQueue("http://notexisting15.ddd", "http://notexisting15.ddd");
    MyNetworkQueue->insertQueue("http://notexisting16.ddd", "http://notexisting16.ddd");
    MyNetworkQueue->insertQueue("http://notexisting17.ddd", "http://notexisting17.ddd");
    MyNetworkQueue->insertQueue("http://notexisting18.ddd", "http://notexisting18.ddd");
    MyNetworkQueue->insertQueue("http://notexisting19.ddd", "http://notexisting19.ddd");
    MyNetworkQueue->insertQueue("http://notexisting20.ddd", "http://notexisting20.ddd");
    QVERIFY(MyNetworkQueue->getDownloadSlots().size() < 6);
    QVERIFY(MyNetworkQueue->getMediaQueue().size() > 10 );
}

void TestNetworkQueue::testDoSuccess()
{
    QByteArray agent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    I_NetworkQueue *MyNetworkQueue = new I_NetworkQueue(agent);
    Network *MyNetwork = new Network(agent);
    QFileInfo fi(":/test.png");
    MyNetwork->setLocalFileInfo(fi);
    QUrl url("http://ihoabsverstan.dn");
    MyNetwork->setRemoteFileUrl(url);
    qRegisterMetaType<QString>();
    QSignalSpy spy(MyNetworkQueue, SIGNAL(succeed(QString, QString)));
    QVERIFY(spy.isValid());
    MyNetworkQueue->test_doSucceed(MyNetwork);
    int i = 0;
    while (spy.count() == 0 && i < 1000)
    {
        QTest::qWait(200);
        i +=200;
    }
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qvariant_cast<QString>(spy.at(0).at(0)), url.toString());
    QCOMPARE(qvariant_cast<QString>(spy.at(0).at(1)), fi.absoluteFilePath());
}

void TestNetworkQueue::testDoNotCacheable()
{
    QByteArray agent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    I_NetworkQueue *MyNetworkQueue = new I_NetworkQueue(agent);
    Network *MyNetwork = new Network(agent);
    MyNetwork->setLocalFileInfo(QFileInfo(":/test.png"));
    QUrl url("http://notunderstandi.ng");
    MyNetwork->setRemoteFileUrl(url);
    qRegisterMetaType<QString>();
    QSignalSpy spy(MyNetworkQueue, SIGNAL(notcacheable(QString)));
    QVERIFY(spy.isValid());
    MyNetworkQueue->test_doNotCacheable(MyNetwork);
    int i = 0;
    while (spy.count() == 0 && i < 1000)
    {
        QTest::qWait(200);
        i +=200;
    }
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qvariant_cast<QString>(spy.at(0).at(0)), url.toString());
}

void TestNetworkQueue::testDoNotModified()
{
    QByteArray agent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    I_NetworkQueue *MyNetworkQueue = new I_NetworkQueue(agent);
    Network *MyNetwork = new Network(agent);
    MyNetwork->setLocalFileInfo(QFileInfo(":/test.png"));
    QUrl url("http://denkatalave.no");
    MyNetwork->setRemoteFileUrl(url);
    qRegisterMetaType<QString>();
    QSignalSpy spy(MyNetworkQueue, SIGNAL(notmodified(QString)));
    QVERIFY(spy.isValid());
    MyNetworkQueue->test_doNotModified(MyNetwork);
    int i = 0;
    while (spy.count() == 0 && i < 1000)
    {
        QTest::qWait(200);
        i +=200;
    }
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qvariant_cast<QString>(spy.at(0).at(0)), url.toString());
}

void TestNetworkQueue::testDoFailed()
{
    QByteArray agent("GAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:TestTDownload) xxxxxx-xx/x.x.x (MODEL:GARLIC)");
    I_NetworkQueue *MyNetworkQueue = new I_NetworkQueue(agent);
    Network *MyNetwork = new Network(agent);
    MyNetwork->setLocalFileInfo(QFileInfo(":/test.png"));
    QUrl url("http://nixverste.hn");
    MyNetwork->setRemoteFileUrl(url);
    qRegisterMetaType<QString>();
    QSignalSpy spy(MyNetworkQueue, SIGNAL(failed(QString)));
    QVERIFY(spy.isValid());
    MyNetworkQueue->test_doFailed(MyNetwork);
    int i = 0;
    while (spy.count() == 0 && i < 1000)
    {
        QTest::qWait(200);
        i +=200;
    }
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qvariant_cast<QString>(spy.at(0).at(0)), url.toString());
}

QTEST_MAIN(TestNetworkQueue)

#include "tst_network_queue.moc"
