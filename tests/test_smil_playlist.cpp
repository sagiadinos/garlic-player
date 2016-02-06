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

#include "test_smil_playlist.h"

void Test_SmilPlaylist::test_reactByTag()
{
    testPlaylist MyPlaylist;
    QDomDocument document;
    QDomElement  element = document.createElement(QString("img"));

    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("img"));

    element.setTagName("video");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("video"));

    element.setTagName("audio");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("audio"));

    element.setTagName("text");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("web"));

    element.setTagName("seq");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("seq"));

    element.setTagName("par");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("par"));

    element.setTagName("excl");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("excl"));
    return;
}

void Test_SmilPlaylist::test_reactByTagForRef()
{
    testPlaylist MyPlaylist;
    QDomDocument document;
    QDomElement element = document.createElement(QString("ref"));
    QCOMPARE(element.tagName(), QString("ref"));

    element.setAttribute("type", "image/png");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("img"));

    element.setAttribute("type", "video/mp4");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("video"));

    element.setAttribute("type", "audio/wav");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("audio"));

    element.setAttribute("type", "text/html");
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_reactByTag();
    QCOMPARE(MyPlaylist.test_getFoundActiveTagName(), QString("web"));
    return;
}
