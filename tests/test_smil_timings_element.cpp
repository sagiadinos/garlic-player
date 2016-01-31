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
#include "test_smil_timings_element.h"


void Test_SmilTimingsElement::test_TimingsOnly()
{
    QDomDocument document;

    document.setContent(QString("<img id=\"test1\" src=\"\" />"));
    QDomElement element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("img"));
    // start, then pause and look on the remaining times
    testTimingsElement *MyTest1 = new testTimingsElement(NULL);
    MyTest1->parse(element);
    MyTest1->preparePlay();
    QCOMPARE(MyTest1->getStatus(),  MyTest1->_stopped);
    MyTest1->preparePause();
    QCOMPARE(MyTest1->getID(), QString("test1"));
    QCOMPARE(MyTest1->getRemainingBegin(), int(0));
    QCOMPARE(MyTest1->getRemainingEnd(), int(0));
    QCOMPARE(MyTest1->getRemainingDur(), int(0));

    document.setContent(QString("<img xml:id =\"test2\"src=\"data/images/0001.jpg\" begin=\"1s\" dur=\"1s\" end=\"3s\"/>"));
    element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("img"));
    // start, then pause and look on the remaining times
    testTimingsElement *MyTest2 = new testTimingsElement(NULL);
    MyTest2->parse(element);
    MyTest2->preparePlay();
    QCOMPARE(MyTest2->getStatus(),  MyTest2->_waiting);
    MyTest2->preparePause();
    QCOMPARE(MyTest2->getID(), QString("test2"));
    QCOMPARE(MyTest2->getStatus(),  MyTest2->_paused);
    QVERIFY(MyTest2->getRemainingBegin() > 900); // Test with tolerances
    QVERIFY(MyTest2->getRemainingBegin() < 1100); // Test with tolerances
    QVERIFY(MyTest2->getRemainingEnd() > 2800);
    QVERIFY(MyTest2->getRemainingEnd() < 3200);
    QCOMPARE(MyTest2->getRemainingDur(), int(0));

    document.setContent(QString("<img xml:id =\"test3\"src=\"data/images/0001.jpg\" dur=\"2s\" end=\"4s\"/>"));
    element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("img"));
    // start, then pause and look on the remaining times
    testTimingsElement *MyTest3 = new testTimingsElement(NULL);
    MyTest3->parse(element);
    MyTest3->preparePlay();
    QCOMPARE(MyTest3->getID(), QString("test3"));
    QCOMPARE(MyTest3->getRemainingBegin(), int(0));
    QCOMPARE(MyTest3->getStatus(),  MyTest3->_playing);
    MyTest3->preparePause();
    QCOMPARE(MyTest2->getStatus(),  MyTest3->_paused);
    QVERIFY(MyTest3->getRemainingEnd() > 3800);
    QVERIFY(MyTest3->getRemainingEnd() < 4200);
    qDebug() << MyTest3->getRemainingDur();
    QVERIFY(MyTest3->getRemainingDur() > 1800);
    QVERIFY(MyTest3->getRemainingDur() < 2200);


}

//      document.setContent(QString("<img src=\"data/images/0001.jpg\" min=\"2s\" begin=\"0s\" dur=\"1s\" end=\"3s\"/>"));
//      document.setContent(QString("<img src=\"data/images/0001.jpg\" max=\"2s\" begin=\"0s\" dur=\"1s\" end=\"3s\"/>"));
