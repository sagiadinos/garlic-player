/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-.control.com>
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

#include "test_smil_timings.h"

void test_SmilTimings::test_calculateDuration()
{
    testTiming MyTiming;
    // Full clock
    QCOMPARE(MyTiming.test_calculateDuration("02:31:05.5"), qint64(9065500));
    QCOMPARE(MyTiming.test_calculateDuration("5.02s"), qint64(5020));
    QCOMPARE(MyTiming.test_calculateDuration("5.02"), qint64(5020));
    QCOMPARE(MyTiming.test_calculateDuration("2h"), qint64(7200000));
    QCOMPARE(MyTiming.test_calculateDuration("2.213h"), qint64(7966800));
    QCOMPARE(MyTiming.test_calculateDuration("12.7713h"), qint64(45976680));
    QCOMPARE(MyTiming.test_calculateDuration("11min"), qint64(660000));
    QCOMPARE(MyTiming.test_calculateDuration("4.15min"), qint64(249000));
    QCOMPARE(MyTiming.test_calculateDuration("some waste"), qint64(0));
    return;
}

void test_SmilTimings::test_parseWallclock()
{
    testTiming *MyTiming = new testTiming();
    WallClock wall_clock = MyTiming->test_parseWallclock("R/2001-04-05T12:13:01/P1D");
    QCOMPARE(wall_clock.active, true);
    QCOMPARE(wall_clock.repeats, int(-1));
    QCOMPARE(wall_clock.datetime, QDateTime::fromString("2001-04-05 12:13:01", "yyyy-MM-dd HH:mm:ss"));
    QCOMPARE(wall_clock.period.years, int(0));
    QCOMPARE(wall_clock.period.months, int(0));
    QCOMPARE(wall_clock.period.days, qint64(1));
    QCOMPARE(wall_clock.period.hours, qint64(0));
    QCOMPARE(wall_clock.period.minutes, qint64(0));
    QCOMPARE(wall_clock.period.seconds, qint64(0));

    delete MyTiming;
    MyTiming = new testTiming();
    WallClock wall_clock1 = MyTiming->test_parseWallclock("R31/2001-04-05/P1Y2W1DT6H2S");
    QCOMPARE(wall_clock1.active, true);
    QCOMPARE(wall_clock1.datetime, QDateTime::fromString("2001-04-05 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    QCOMPARE(wall_clock1.repeats, int(31));
    QCOMPARE(wall_clock1.period.years, int(1));
    QCOMPARE(wall_clock1.period.months, int(0));
    QCOMPARE(wall_clock1.period.days, qint64(15));
    QCOMPARE(wall_clock1.period.hours, qint64(6));
    QCOMPARE(wall_clock1.period.minutes, qint64(0));
    QCOMPARE(wall_clock1.period.seconds, qint64(2));

    delete MyTiming;
    MyTiming = new testTiming();
    WallClock wall_clock2 = MyTiming->test_parseWallclock("/2001-04-05T12:13:01/guas");
    QCOMPARE(wall_clock2.active, true);
    QCOMPARE(wall_clock2.repeats, int(0));
    QCOMPARE(wall_clock2.period.years, int(0));
    QCOMPARE(wall_clock2.period.months, int(0));
    QCOMPARE(wall_clock2.period.days, qint64(0));
    QCOMPARE(wall_clock2.period.hours, qint64(0));
    QCOMPARE(wall_clock2.period.minutes, qint64(0));
    QCOMPARE(wall_clock2.period.seconds, qint64(0));

    delete MyTiming;
    MyTiming = new testTiming();
    WallClock wall_clock3 = MyTiming->test_parseWallclock("RZZ/2001-04-05T12:13:01/");
    QCOMPARE(wall_clock3.active, true);
    QCOMPARE(wall_clock3.repeats, int(0));
    QCOMPARE(wall_clock3.period.years, int(0));
    QCOMPARE(wall_clock3.period.months, int(0));
    QCOMPARE(wall_clock3.period.days, qint64(0));
    QCOMPARE(wall_clock3.period.hours, qint64(0));
    QCOMPARE(wall_clock3.period.minutes, qint64(0));
    QCOMPARE(wall_clock3.period.seconds, qint64(0));
    return;
}

void test_SmilTimings::test_checkStatus()
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


    testTiming *MyTiming = new testTiming();
    MyTiming->parse("indefinite");
    QCOMPARE(MyTiming->getStatus(), QString("indefinite"));
    delete MyTiming;
    MyTiming = new testTiming();
    MyTiming->parse("media"); // media shouild not be returned in TTiming cause there coudl only be begin or end attribute
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new testTiming();
    MyTiming->parse("tamtararara tam tam");
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new testTiming();
    MyTiming->parse("1.2min");
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new testTiming();
    MyTiming->parse("-10");
    QCOMPARE(MyTiming->getStatus(), QString("ms"));

    delete MyTiming;
    MyTiming = new testTiming();
    MyTiming->parse("wallclock(zeneoidd)");
    QCOMPARE(MyTiming->getStatus(), QString("wallclock"));


}

void test_SmilTimings::test_getNextTrigger()
{
    testTiming *MyTiming = new testTiming();
    MyTiming->parse("wallclock(R5/2016-01-13T01:01:01/P1D)");
    QCOMPARE(MyTiming->getNextTrigger(QDateTime::fromString("2016-01-13 00:00:00", "yyyy-MM-dd HH:mm:ss")), qint64(3661000));
    QCOMPARE(MyTiming->getNextTrigger(QDateTime::fromString("2016-01-14 00:00:02", "yyyy-MM-dd HH:mm:ss")), qint64(3659000));
    QCOMPARE(MyTiming->getNextTrigger(QDateTime::fromString("2016-01-18 00:00:02", "yyyy-MM-dd HH:mm:ss")), qint64(3659000));
    QCOMPARE(MyTiming->getNextTrigger(QDateTime::fromString("2016-01-19 01:00:00", "yyyy-MM-dd HH:mm:ss")), qint64(61000));
    delete MyTiming;
    MyTiming = new testTiming();
    MyTiming->parse("wallclock(R/2001-01-13T01:01:01/PT1H)");
    QCOMPARE(MyTiming->getNextTrigger(QDateTime::fromString("2001-01-13 02:01:01", "yyyy-MM-dd HH:mm:ss")), qint64(0));
    QCOMPARE(MyTiming->getNextTrigger(QDateTime::fromString("2001-01-13 02:01:00", "yyyy-MM-dd HH:mm:ss")), qint64(1000));
    QCOMPARE(MyTiming->getNextTrigger(QDateTime::fromString("2017-01-13 02:01:00", "yyyy-MM-dd HH:mm:ss")), qint64(1000));

    return;
}

