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

#include "test_smil_base.h"
#include <QDebug>

void Test_SmilBase::test_setBaseAttributes()
{
    // test with default
    testBase MyPlaylist;
    QCOMPARE(MyPlaylist.getID(), QString(""));
    QCOMPARE(MyPlaylist.getBegin(), QString("0"));
    QCOMPARE(MyPlaylist.getEnd(), QString(""));
    QCOMPARE(MyPlaylist.getTitle(), QString(""));
    QCOMPARE(MyPlaylist.getRepeatCount(), int(0));
    QCOMPARE(MyPlaylist.getIndefinite(), false);

    QDomDocument document;

    // test with old SMIL 2.0 id
    document.setContent(QString("<par id=\"old id\" repeatCount=\"25\" dur=\"23s\"/>"));
    QDomElement element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("par"));
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_setBaseAttributes();
    QCOMPARE(MyPlaylist.getID(), QString("old id"));
    QCOMPARE(MyPlaylist.getRepeatCount(), int(25));
    QCOMPARE(MyPlaylist.getIndefinite(), false);
    QCOMPARE(MyPlaylist.getDuration(), qint64(23000));

    // test with new SMIL 3.0 xml:id and rest of supporteed values
    document.setContent(QString("<seq id=\"old id\" xml:id=\"xml id\" begin=\"10s\" end=\"20s\" title=\"test seq\" repeatCount=\"indefinite\" />"));
    element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("seq"));
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_setBaseAttributes();
    QCOMPARE(MyPlaylist.getID(), QString("xml id"));
    QCOMPARE(MyPlaylist.getBegin(), QString("10s"));
    QCOMPARE(MyPlaylist.getEnd(), QString("20s"));
    QCOMPARE(MyPlaylist.getTitle(), QString("test seq"));
    QCOMPARE(MyPlaylist.getIndefinite(), true);
    return;
}

void Test_SmilBase::test_checkRepeatCountStatus()
{
    // test with default
    testBase MyPlaylist;
    MyPlaylist.setForRepeatCountCheck(2, 0, true);
    QCOMPARE(MyPlaylist.test_checkRepeatCountStatus(), true);
    MyPlaylist.setForRepeatCountCheck(2, 0, false);
    QCOMPARE(MyPlaylist.test_checkRepeatCountStatus(), true);
    MyPlaylist.setForRepeatCountCheck(2, 2, false);
    QCOMPARE(MyPlaylist.test_checkRepeatCountStatus(), false);
    MyPlaylist.setForRepeatCountCheck(2, 1, false);
    QCOMPARE(MyPlaylist.test_checkRepeatCountStatus(), true);
    return;
}

void Test_SmilBase::test_calculateDuration()
{
    testBase MyPlaylist;
    // Full clock
    QCOMPARE(MyPlaylist.test_calculateDuration("02:31:05.5"), qint64(9065500));
    QCOMPARE(MyPlaylist.test_calculateDuration("5.02s"), qint64(5020));
    QCOMPARE(MyPlaylist.test_calculateDuration("5.02"), qint64(5020));
    QCOMPARE(MyPlaylist.test_calculateDuration("2h"), qint64(7200000));
    QCOMPARE(MyPlaylist.test_calculateDuration("2.213h"), qint64(7966800));
    QCOMPARE(MyPlaylist.test_calculateDuration("12.7713h"), qint64(45976680));
    QCOMPARE(MyPlaylist.test_calculateDuration("11min"), qint64(660000));
    QCOMPARE(MyPlaylist.test_calculateDuration("4.15min"), qint64(249000));
    return;
}

