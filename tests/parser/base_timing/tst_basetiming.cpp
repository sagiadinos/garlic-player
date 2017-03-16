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

#include "smilparser/base_timing.h"

class InhertitedTBaseTiming : public TBaseTiming
{
    Q_OBJECT
public:
    InhertitedTBaseTiming(QObject * parent = 0){Q_UNUSED(parent);initTimer();setObjectName("TestBaseTimings");}
    QString       getType(){return "test timings for elements";}
    bool          parse(QDomElement element) {root_element = element;setTimingAttributes();return true;}
    void          emitfinished(){emit signal_end();}
    void          play(){status = _playing;}                // virtual
    void          pause(){status = _paused;}                // virtual
    void          stop(){status = _stopped;}                // virtual
    void          resume(){status = _playing;}              // virtual
    QString       getBaseType(){return  "test base";}       // virtual
    TBaseTiming  *getChildElementFromList(){ return this;}  // virtual
    bool          hasPlayingChilds(){return true;}          // virtual

    // for Test
    int           getRemainingBegin(){return begin_remaining;}
    int           getRemainingEnd(){return end_remaining;}
    int           getRemainingDur(){return dur_remaining;}
    void          test_setBaseAttributes(){setBaseAttributes();}
    bool          test_checkRepeatCountStatus(){return checkRepeatCountStatus();}
    void          setForRepeatCountCheck(int rC, int r_c, bool in){repeatCount=rC;internal_count=r_c;indefinite=in;}
    int           getRepeatCount(){return repeatCount;}
    bool          getIndefinite(){return indefinite;}
protected:
    void       setDurationTimerBeforePlay() // behaves like TImage and TWeb
    {
        if (hasDurAttribute())
        {
            play();
            emit signal_begin();
        }
        else
            emitfinished();
    }
signals:
    void signal_begin();
    void signal_end();
};


class TestTBaseTiming : public QObject
{
    Q_OBJECT

public:
    TestTBaseTiming(){}

private Q_SLOTS:
    void test_checkRepeatCountStatus();
    void test_TimingsOnly();
};

void TestTBaseTiming::test_checkRepeatCountStatus()
{
    // test with default
    InhertitedTBaseTiming MyContainer;
    MyContainer.setForRepeatCountCheck(2, 0, true);
    QCOMPARE(MyContainer.test_checkRepeatCountStatus(), true);
    MyContainer.setForRepeatCountCheck(2, 0, false);
    QCOMPARE(MyContainer.test_checkRepeatCountStatus(), true);
    MyContainer.setForRepeatCountCheck(2, 2, false);
    QCOMPARE(MyContainer.test_checkRepeatCountStatus(), false);
    MyContainer.setForRepeatCountCheck(2, 1, false);
    QCOMPARE(MyContainer.test_checkRepeatCountStatus(), true);
    return;
}

void TestTBaseTiming::test_TimingsOnly()
{
    QDomDocument document;

    document.setContent(QString("<img id=\"test1\" src=\"\" />"));
    QDomElement element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("img"));
    // start, then pause and look on the remaining times
    InhertitedTBaseTiming *MyTest1 = new InhertitedTBaseTiming(NULL);
    MyTest1->parse(element);
    MyTest1->prepareTimerBeforePlaying();
    QCOMPARE(MyTest1->getStatus(),  MyTest1->_stopped);
    MyTest1->prepareTimerBeforePausing();
    QCOMPARE(MyTest1->getID(), QString("test1"));
    QCOMPARE(MyTest1->getRemainingBegin(), int(0));
    QCOMPARE(MyTest1->getRemainingEnd(), int(0));
    QCOMPARE(MyTest1->getRemainingDur(), int(0));

    document.setContent(QString("<img xml:id=\"test2\" begin=\"2s\" dur=\"1s\" end=\"4s\"/>"));
    element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("img"));
    // start, then pause and look on the remaining times
    InhertitedTBaseTiming *MyTest2 = new InhertitedTBaseTiming(NULL);
    MyTest2->parse(element);
    MyTest2->prepareTimerBeforePlaying();
    MyTest2->prepareTimerBeforePausing();
    QCOMPARE(MyTest2->getID(), QString("test2"));
    QVERIFY(MyTest2->getRemainingBegin() > 1950); // Test with tolerances value should be 2000
    QVERIFY(MyTest2->getRemainingBegin() < 2050);
    QVERIFY(MyTest2->getRemainingEnd() > 3950); // Test with tolerances value should be 4000
    QVERIFY(MyTest2->getRemainingEnd() < 4050);
    QCOMPARE(MyTest2->getRemainingDur(), int(0));

    document.setContent(QString("<img xml:id =\"test3\" dur=\"2s\" end=\"4s\"/>"));
    element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("img"));
    // start, then pause and look on the remaining times
    InhertitedTBaseTiming *MyTest3 = new InhertitedTBaseTiming(NULL);
    MyTest3->parse(element);
    MyTest3->prepareTimerBeforePlaying();
    QCOMPARE(MyTest3->getID(), QString("test3"));
    QCOMPARE(MyTest3->getRemainingBegin(), int(0));
    QCOMPARE(MyTest3->getStatus(),  MyTest3->_playing);

    MyTest3->prepareTimerBeforePausing();
    QVERIFY(MyTest3->getRemainingEnd() > 3950); // Test with tolerances value should be 4000
    QVERIFY(MyTest3->getRemainingEnd() < 4050);

    QVERIFY(MyTest3->getRemainingDur() > 1950); // Test with tolerances value should be 2000
    QVERIFY(MyTest3->getRemainingDur() < 2050);
}

//      document.setContent(QString("<img src=\"data/images/0001.jpg\" min=\"2s\" begin=\"0s\" dur=\"1s\" end=\"3s\"/>"));
//      document.setContent(QString("<img src=\"data/images/0001.jpg\" max=\"2s\" begin=\"0s\" dur=\"1s\" end=\"3s\"/>"));


QTEST_MAIN(TestTBaseTiming)

#include "tst_basetiming.moc"
