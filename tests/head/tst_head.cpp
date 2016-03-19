#include <QString>
#include <QtTest>

#include "smilparser/head.h"
class InhertitedTHead : public THead
{
    public:
    // for not waiting for godot in test_refreshSmilIndex, cause of the hammering preventing function in void THead::setRefreshTimer()
    void resetRefreshTimer()
    {
        refresh_timer->start(500);
    }


};

class TestTHead : public QObject
{
    Q_OBJECT

public:
    TestTHead(){}

private Q_SLOTS:
    void test_getDefaultValues();
    void test_getHeadValuesFromSimpleSmil();
    void test_getHeadValuesFromComplexSmil();
    void test_refreshSmilIndex();
};

void TestTHead::test_getDefaultValues()
{
    InhertitedTHead MySmilHead;
    QCOMPARE(MySmilHead.getRootBackgroundColor(), QString("transparent"));
    QCOMPARE(MySmilHead.getTitle(), QString("No Title"));
    QList<Region> region_list = MySmilHead.getLayout();
    QCOMPARE(region_list.count(), int(1));
    QCOMPARE(region_list.at(0).regionName, QString("screen"));
    QCOMPARE(region_list.at(0).width, qreal(100));
    QCOMPARE(region_list.at(0).height, qreal(100));
    QCOMPARE(region_list.at(0).top, qreal(0));
    QCOMPARE(region_list.at(0).left, qreal(0));
    QCOMPARE(region_list.at(0).z_index, int(0));
    QCOMPARE(region_list.at(0).backgroundColor, QString("transparent"));
}

void TestTHead::test_getHeadValuesFromSimpleSmil()
{
    QFile file("../tests/data/smil/head_simple.smil");
    QDomDocument document;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    document.setContent(&file);
    file.close();
    QDomElement head = document.firstChildElement().firstChildElement();
    InhertitedTHead MySmilHead;
    MySmilHead.parse(head);
    QCOMPARE(MySmilHead.getTitle(), QString("Simple SMIL header for testing"));
    QCOMPARE(MySmilHead.getRootBackgroundColor(), QString("transparent"));
    QList<Region> region_list = MySmilHead.getLayout();
    QCOMPARE(region_list.count(), int(1));

    QCOMPARE(region_list.at(0).regionName, QString("test_screen"));
    QCOMPARE(region_list.at(0).width, qreal(1));
    QCOMPARE(region_list.at(0).height, qreal(1));
    QCOMPARE(region_list.at(0).top, qreal(0));
    QCOMPARE(region_list.at(0).left, qreal(0));
    QCOMPARE(region_list.at(0).z_index, int(1));
    QCOMPARE(region_list.at(0).backgroundColor, QString("#000"));

}

void TestTHead::test_getHeadValuesFromComplexSmil()
{
    QFile file("../tests/data/smil/head_complex.smil");
    QDomDocument document;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    document.setContent(&file);
    file.close();
    QDomElement head = document.firstChildElement().firstChildElement();
    InhertitedTHead MySmilHead;
    MySmilHead.parse(head);

    QCOMPARE(MySmilHead.getRefreshTime(), int(30)); // in file stands 2 seconds for refresh
    QCOMPARE(MySmilHead.getTitle(), QString("Complex SMIL header for testing"));
    QList<Region> region_list = MySmilHead.getLayout();
    QCOMPARE(region_list.count(), int(5));

    QCOMPARE(region_list.at(0).regionName, QString("screen1"));
    QCOMPARE(region_list.at(0).width, qreal(1));
    QCOMPARE(region_list.at(0).height, qreal(1));
    QCOMPARE(region_list.at(0).top, qreal(0));
    QCOMPARE(region_list.at(0).left, qreal(0));
    QCOMPARE(region_list.at(0).z_index, int(0));
    QCOMPARE(region_list.at(0).backgroundColor, QString("red"));

    QCOMPARE(region_list.at(1).regionName, QString("screen2"));
    QCOMPARE(region_list.at(1).width, qreal(0.25));
    QCOMPARE(region_list.at(1).height, qreal(0.25));
    QCOMPARE(region_list.at(1).top, qreal(0));
    QCOMPARE(region_list.at(1).left, qreal(0.50));
    QCOMPARE(region_list.at(1).z_index, int(1));
    QCOMPARE(region_list.at(1).backgroundColor, QString("green"));

    QCOMPARE(region_list.at(2).regionName, QString("screen3"));
    QCOMPARE(region_list.at(2).width, qreal(0.50));
    QCOMPARE(region_list.at(2).height, qreal(0.50));
    QCOMPARE(region_list.at(2).top, qreal(0.50));
    QCOMPARE(region_list.at(2).left, qreal(0));
    QCOMPARE(region_list.at(2).z_index, int(1));
    QCOMPARE(region_list.at(2).backgroundColor, QString("blue"));

    QCOMPARE(region_list.at(3).regionName, QString("screen4"));
    QCOMPARE(region_list.at(3).width, qreal(0.25));
    QCOMPARE(region_list.at(3).height, qreal(0.50));
    QCOMPARE(region_list.at(3).top, qreal(0.50));
    QCOMPARE(region_list.at(3).left, qreal(0.50));
    QCOMPARE(region_list.at(3).z_index, int(1));
    QCOMPARE(region_list.at(3).backgroundColor, QString("orange"));

    QCOMPARE(region_list.at(4).regionName, QString("screen5"));
    QCOMPARE(region_list.at(4).width, qreal(0.25));
    QCOMPARE(region_list.at(4).height, qreal(0.50));
    QCOMPARE(region_list.at(4).top, qreal(0.50));
    QCOMPARE(region_list.at(4).left, qreal(0.75));
    QCOMPARE(region_list.at(4).z_index, int(1));
    QCOMPARE(region_list.at(4).backgroundColor, QString("transparent"));
}

void TestTHead::test_refreshSmilIndex()
{
    QFile file("../tests/data/smil/head_complex.smil");
    QDomDocument document;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    document.setContent(&file);
    file.close();
    QDomElement head = document.firstChildElement().firstChildElement();
    InhertitedTHead *MyHead = new InhertitedTHead();

    MyHead->parse(head);
    MyHead->resetRefreshTimer();
    QSignalSpy spy1(MyHead, SIGNAL(checkForNewIndex()));
    int i = 0;
    while (spy1.count() == 0)
    {
        QTest::qWait(100);
        i += 100;
    }
    QVERIFY(i > 499);
    QVERIFY(i < 601);
    QSignalSpy spy2(MyHead, SIGNAL(checkForNewIndex()));
    while (spy2.count() == 0)
    {
        QTest::qWait(100);
        i += 100;
    }
    QVERIFY(i > 999);
    QVERIFY(i < 1101);
}

QTEST_MAIN(TestTHead)

#include "tst_head.moc"
