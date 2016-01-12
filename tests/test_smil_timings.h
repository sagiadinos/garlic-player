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

#ifndef TEST_SMILTIMINGS_H
#define TEST_SMILTIMINGS_H

#include "AutoTest.h"
#include "smilparser/timings/timing.h"

/**
 * @brief To save a test file this will test TTiming and TClockvalue which is inherited from TTiming
 */
class testTiming : public TTiming
{
    Q_OBJECT
public:
    testTiming(QObject * parent = 0){Q_UNUSED(parent);}
    qint64    test_calculateDuration(QString dur){return calculateMilliseconds(dur);}
    WallClock test_parseWallclock(QString iso_date){return parseWallclock(iso_date);}
};


class test_SmilTimings : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(){}
    void test_calculateDuration();
    void test_parseWallclock();
    void test_checkStatus();
    void test_getNextTrigger();
    void cleanupTestCase(){}
};

DECLARE_TEST(test_SmilTimings)


#endif // TEST_SMILTIMINGS_H
