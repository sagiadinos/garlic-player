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
#ifndef TEST_SMIL_IMAGE_H
#define TEST_SMIL_IMAGE_H

#include "AutoTest.h"
#include "smilparser/base.h"

class testTimingsElement : public TBase
{
    Q_OBJECT
public:
    testTimingsElement(QObject * parent = 0){Q_UNUSED(parent);initTimer();setObjectName("testtestTimingsElement");}
    QString   getType(){return "test timings for elements";}
    bool      parse(QDomElement element) {root_element = element;setBaseAttributes();return true;}
    int       getRemainingBegin(){return begin_remaining;}
    int       getRemainingEnd(){return end_remaining;}
    int       getRemainingDur(){return dur_remaining;}
public slots:
    void       emitfinished(){emit signal_end();}
    void       play(){status = _playing;}            // virtual
    void       pause(){status = _paused;}            // virtual
    void       stop(){status = _stopped;}            // virtual
    QString    getBaseType(){return  "test base";}   // virtual
protected:
    void       setDurationTimerBeforePlay() // like TImage and TWeb
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


class Test_SmilTimingsElement : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(){}
    void test_TimingsOnly();
    void cleanupTestCase(){}
};
DECLARE_TEST(Test_SmilTimingsElement);


#endif // TEST_SMIL_IMAGE_H
