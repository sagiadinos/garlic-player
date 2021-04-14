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

#include "smilparser/trigger/wallclock.h"


class TestWallClock : public QObject
{
    Q_OBJECT
public:
    TestWallClock(){}

private Q_SLOTS:
    void test_parse1();
    void test_parse2();
    void test_parse3();
    void test_parse4();
    void test_parseDateWithWeekDay();
    void test_getNextTimerTrigger1();
    void test_getNextTimerTrigger2();
    void test_getNextTimerTrigger3();
};

void TestWallClock::test_parse1()
{
    WallClock MyWallClock;
    MyWallClock.parse("R/2001-04-05T12:13:01/P1D");
    IsoPeriod period = MyWallClock.getPeriod();
    QCOMPARE(MyWallClock.isActive(), true);
    QCOMPARE(MyWallClock.getRepeats(), int(-1));
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2001-04-05 12:13:01", "yyyy-MM-dd HH:mm:ss"));
    QCOMPARE(period.years, int(0));
    QCOMPARE(period.months, int(0));
    QCOMPARE(period.days, qint64(1));
    QCOMPARE(period.hours, qint64(0));
    QCOMPARE(period.minutes, qint64(0));
    QCOMPARE(period.seconds, qint64(0));
}

void TestWallClock::test_parse2()
{
    WallClock MyWallClock;
    MyWallClock.parse("R31/2001-04-05/P1Y2W1DT6H2S");
    IsoPeriod period = MyWallClock.getPeriod();
    QCOMPARE(MyWallClock.isActive(), true);
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2001-04-05 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    QCOMPARE(MyWallClock.getRepeats(), int(31));
    QCOMPARE(period.years, int(1));
    QCOMPARE(period.months, int(0));
    QCOMPARE(period.days, qint64(15));
    QCOMPARE(period.hours, qint64(6));
    QCOMPARE(period.minutes, qint64(0));
    QCOMPARE(period.seconds, qint64(2));
}

void TestWallClock::test_parse3()
{
    WallClock MyWallClock;
    MyWallClock.parse("/2001-04-05T12:13:01/guas");
    IsoPeriod period = MyWallClock.getPeriod();
    QCOMPARE(MyWallClock.isActive(), true);
    QCOMPARE(MyWallClock.getRepeats(), int(0));
    QCOMPARE(period.years, int(0));
    QCOMPARE(period.months, int(0));
    QCOMPARE(period.days, qint64(0));
    QCOMPARE(period.hours, qint64(0));
    QCOMPARE(period.minutes, qint64(0));
    QCOMPARE(period.seconds, qint64(0));
}

void TestWallClock::test_parse4()
{
    WallClock MyWallClock;
    MyWallClock.parse("RZZ/2001-04-05T12:13:01/");
    IsoPeriod period = MyWallClock.getPeriod();
    QCOMPARE(MyWallClock.isActive(), true);
    QCOMPARE(MyWallClock.getRepeats(), int(0));
    QCOMPARE(period.years, int(0));
    QCOMPARE(period.months, int(0));
    QCOMPARE(period.days, qint64(0));
    QCOMPARE(period.hours, qint64(0));
    QCOMPARE(period.minutes, qint64(0));
    QCOMPARE(period.seconds, qint64(0));
}

void TestWallClock::test_parseDateWithWeekDay()
{
    WallClock MyWallClock;
    // day of week after
    MyWallClock.parse("2021-04-01+w1T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-04-05 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01+w2T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-04-06 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01+w3T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-04-07 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01+w4T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-04-08 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01+w5T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-04-02 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01+w6T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-04-03 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01+w7T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-04-04 00:00:00", "yyyy-MM-dd HH:mm:ss"));

    // day of week after without +
    MyWallClock.parse("2021-02-01w1T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-02-08 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-02-01w2T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-02-02 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-02-01w3T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-02-03 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-02-01w4T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-02-04 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-02-01w5T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-02-05 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-02-01w6T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-02-06 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-02-01w7T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-02-07 00:00:00", "yyyy-MM-dd HH:mm:ss"));

    // day of week before
    MyWallClock.parse("2021-04-01-w1T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-03-29 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01-w2T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-03-30 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01-w3T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-03-31 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01-w4T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-03-25 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01-w5T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-03-26 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01-w6T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-03-27 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    MyWallClock.parse("2021-04-01-w7T00:00");
    QCOMPARE(MyWallClock.getDateTime(), QDateTime::fromString("2021-03-28 00:00:00", "yyyy-MM-dd HH:mm:ss"));
}

void TestWallClock::test_getNextTimerTrigger1()
{
    WallClock MyWallClock;
    MyWallClock.parse("R5/2016-01-13T01:01:01/P1D");
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2016-01-13 00:00:00", "yyyy-MM-dd HH:mm:ss")), qint64(3661000));
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2016-01-14 00:00:02", "yyyy-MM-dd HH:mm:ss")), qint64(3659000));
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2016-01-18 00:00:02", "yyyy-MM-dd HH:mm:ss")), qint64(3659000));
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2016-01-19 01:00:00", "yyyy-MM-dd HH:mm:ss")), qint64(61000));

    // when Trigger is in past
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2017-01-19 01:00:00", "yyyy-MM-dd HH:mm:ss")), qint64(0));
}

void TestWallClock::test_getNextTimerTrigger2()
{
    WallClock MyWallClock;
    MyWallClock.parse("R/2001-01-13T01:01:01/PT1H");

    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2001-01-13 02:01:01", "yyyy-MM-dd HH:mm:ss")), qint64(0));
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2001-01-13 02:01:00", "yyyy-MM-dd HH:mm:ss")), qint64(1000));
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2017-01-13 02:01:00", "yyyy-MM-dd HH:mm:ss")), qint64(1000));
}

void TestWallClock::test_getNextTimerTrigger3()
{
    WallClock MyWallClock;
    MyWallClock.parse("R/2001-01-13T01:01:01/PT10S");
    QCOMPARE(MyWallClock.getNextTimerTrigger(QDateTime::fromString("2016-01-31 19:20:42", "yyyy-MM-dd HH:mm:ss")), qint64(9000));
}


QTEST_APPLESS_MAIN(TestWallClock)

#include "tst_wallclock.moc"
