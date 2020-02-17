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

#include "smilparser/base.h"

class InhertitedTBase : public TBase
{
    Q_OBJECT
public:
    InhertitedTBase(TBase * parent = 0){Q_UNUSED(parent);setObjectName("testBasePlaylist");}
    QString   getType(){return "test base";}
    void      preloadParse(QDomElement element){Q_UNUSED(element);} // virtual
    void      test_setBaseAttributes(){setBaseAttributes();}
    void      setActiveElement(QDomElement element){root_element = element;}

    QString   getBaseType(){return  "test base";}   // virtual
};



class TestTBase : public QObject
{
    Q_OBJECT

public:
    TestTBase(){}

private Q_SLOTS:
    void test_setBaseAttributes();
};

void TestTBase::test_setBaseAttributes()
{
    // test with default
    InhertitedTBase MyPlaylist;
    QCOMPARE(MyPlaylist.getID(), QString(""));
    QCOMPARE(MyPlaylist.getTitle(), QString(""));
    QCOMPARE(MyPlaylist.getClass(), QString(""));
    QCOMPARE(MyPlaylist.getLang(), QString(""));

    QDomDocument document;
    // test with old SMIL 2.0 id
    document.setContent(QString("<par id=\"old id\" title=\"this is a title\" class=\"this is a class\" xml:lang=\"el\" />"));
    QDomElement element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("par"));
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_setBaseAttributes();
    QCOMPARE(MyPlaylist.getID(), QString("old id"));
    QCOMPARE(MyPlaylist.getTitle(), QString("this is a title"));
    QCOMPARE(MyPlaylist.getClass(), QString("this is a class"));
    QCOMPARE(MyPlaylist.getLang(), QString("el"));

    // test with new SMIL 3.0 xml:id and rest of supporteed values
    document.setContent(QString("<seq id=\"old id\" xml:id=\"xml id\" title=\"test seq\" />"));
    element = document.firstChild().toElement();
    QCOMPARE(element.tagName(), QString("seq"));
    MyPlaylist.setActiveElement(element);
    MyPlaylist.test_setBaseAttributes();
    QCOMPARE(MyPlaylist.getID(), QString("xml id"));
    QCOMPARE(MyPlaylist.getTitle(), QString("test seq"));
    return;
}



QTEST_APPLESS_MAIN(TestTBase)

#include "tst_base.moc"
