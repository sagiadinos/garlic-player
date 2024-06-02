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

#include "smilparser/timings/simple_timer.h"

/**
 * @brief To save a test file this will test TTiming and TClockvalue which is inherited from TTiming
 */
class InhertitedTTiming : public TTiming
{
    Q_OBJECT
public:
    InhertitedTTiming(QObject * parent = 0){Q_UNUSED(parent);}
    qint64    test_calculateDuration(QString dur){return calculateMilliseconds(dur);}
    WallClock test_parseWallclock(QString iso_date){return parseWallclock(iso_date);}
};


class TestSimpleTimer : public QObject
{
    Q_OBJECT
public:
    TestSimpleTimer(){}

private Q_SLOTS:
    void test_parse();
};

void TestSimpleTimer::test_parse()
{
    TClockValue *MyClock = new TClockValue;
    MyClock->parse("indefinite");
    QCOMPARE(MyClock->getStatus(), QString("indefinite"));
    delete MyClock;

    MyClock = new TClockValue;
    MyClock->parse("dradinettum");
    QCOMPARE(MyClock->getStatus(), QString("ms"));
    delete MyClock;

    MyClock = new TClockValue;
    MyClock->parse("media");
    QCOMPARE(MyClock->getStatus(), QString("media"));
    delete MyClock;

    MyClock = new TClockValue;
    MyClock->parse("13:23");
    QCOMPARE(MyClock->getStatus(), QString("ms"));
    delete MyClock;

    MyClock = new TClockValue;
    MyClock->parse("13.2s");
    QCOMPARE(MyClock->getStatus(), QString("ms"));
    delete MyClock;

    InhertitedTTiming *MyTiming = new InhertitedTTiming();
    MyTiming->parse("indefinite");
    QCOMPARE(MyTiming->getStatus(), QString("indefinite"));
    delete MyTiming;
    MyTiming = new InhertitedTTiming();
    MyTiming->parse("media"); // media shouild not be returned in TTiming cause there coudl only be begin or end attribute
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new InhertitedTTiming();
    MyTiming->parse("tamtararara tam tam");
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new InhertitedTTiming();
    MyTiming->parse("1.2min");
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new InhertitedTTiming();
    MyTiming->parse("-10");
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new InhertitedTTiming();
    MyTiming->parse("wallclock(zeneoidd)");
    QCOMPARE(MyTiming->getStatus(), QString("wallclock"));

    delete MyTiming;
    MyTiming = new InhertitedTTiming();
    MyTiming->parse("0;15");  // no support for value-list @moment
    QCOMPARE(MyTiming->getStatus(), QString("ms"));


}


QTEST_APPLESS_MAIN(TestSimpleTimer)

#include "tst_simple_timer.moc"
