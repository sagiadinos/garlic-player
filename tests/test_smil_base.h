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

#ifndef TESTSMILBASEPLAYLIST_H
#define TESTSMILBASEPLAYLIST_H

#include "AutoTest.h"
#include "smilparser/base.h"

class testBase : public TBase
{
    Q_OBJECT
public:
    testBase(QObject * parent = 0){Q_UNUSED(parent);setObjectName("testBasePlaylist");}
    QString   getType(){return "test base";}
    bool      parse(QDomElement element){Q_UNUSED(element);return true;}
    void      beginPlay(){}
    void      test_setBaseAttributes(){setBaseAttributes();}
    bool      test_checkRepeatCountStatus(){return checkRepeatCountStatus();}

    void      setActiveElement(QDomElement element){root_element = element;}

    void      setForRepeatCountCheck(int rC, int r_c, bool in){repeatCount=rC;internal_count=r_c;indefinite=in;}
    int       getRepeatCount(){return repeatCount;}
    bool      getIndefinite(){return indefinite;}
public slots:
    void       emitfinished(){}
protected:
    void       play(){}
    void       pause(){}
    void       stop(){}

protected slots:
    void       setDurationTimerBeforePlay(){}
};


class Test_SmilBase : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(){}
    void test_setBaseAttributes();
    void test_checkRepeatCountStatus();
    void cleanupTestCase(){}
};

DECLARE_TEST(Test_SmilBase)

#endif // TESTSMILBASEPLAYLIST_H



