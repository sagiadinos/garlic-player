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

#ifndef TEST_SMIL_PLAYLIST_H
#define TEST_SMIL_PLAYLIST_H

#include "AutoTest.h"
#include "smilparser/playlist.h"

class testPlaylist : public TPlaylist
{
    Q_OBJECT
public:
    testPlaylist(TBase * parent = 0){Q_UNUSED(parent);setObjectName("testBasePlaylist");}
    QString   getType(){return "test base playlist";}
    bool      parse(QDomElement element){Q_UNUSED(element);return true;}    // virtual
    void      test_reactByTag(){reactByTag();}
    void      setActiveElement(QDomElement element){active_element = element;}
    QString   test_getFoundActiveTagName(){return found_tag;}
    void      childStarted(TBase *element){ Q_UNUSED(element);}             // virtual
    void      childEnded(TBase *element){ Q_UNUSED(element);}               // virtual
    void      play(){}                                                      // virtual
    void      pause(){}                                                     // virtual
    void      stop(){}                                                      // virtual
public slots:
    void      emitfinished(){}                                              // virtual
protected:
    void      setDurationTimerBeforePlay(){}                                // virtual
};


class Test_SmilPlaylist : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(){}
    void test_reactByTag();
    void test_reactByTagForRef();
    void cleanupTestCase(){}
};

DECLARE_TEST(Test_SmilPlaylist)

#endif // TEST_SMIL_PLAYLIST_H
