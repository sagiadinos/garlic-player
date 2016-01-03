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

#include "test_smil_head.h"

void Test_SmilHead::test_getDefaultValues()
{
    THead MySmilHead;
    QCOMPARE(MySmilHead.getRefreshTime(), int(900));
    QCOMPARE(MySmilHead.getRootWidth(), int(1366));
    QCOMPARE(MySmilHead.getRootHeight(), int(768));
    QCOMPARE(MySmilHead.getRootBackgroundColor(), QString("transparent"));
    QCOMPARE(MySmilHead.getTitle(), QString("No Title"));
    QList<Region> region_list = MySmilHead.getLayout();
    QCOMPARE(region_list.count(), int(1));
    QCOMPARE(region_list.at(0).regionName, QString("screen"));
    QCOMPARE(region_list.at(0).width, qreal(100));
    QCOMPARE(region_list.at(0).height, qreal(100));
    QCOMPARE(region_list.at(0).top, qreal(0));
    QCOMPARE(region_list.at(0).left, qreal(0));
    QCOMPARE(region_list.at(0).z_index, int(0));
    QCOMPARE(region_list.at(0).backgroundColor, QString("transparent"));
}

void Test_SmilHead::test_getHeadValuesFromSimpleSmil()
{
    QFile file("../tests/data/smil/head_simple.smil");
    QDomDocument document;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    document.setContent(&file);
    file.close();
    QDomElement head = document.firstChildElement().firstChildElement();
    THead MySmilHead;
    MySmilHead.parse(head);
    QCOMPARE(MySmilHead.getRefreshTime(), int(60));
    QCOMPARE(MySmilHead.getRootWidth(), int(1920));
    QCOMPARE(MySmilHead.getRootHeight(), int(1080));
    QCOMPARE(MySmilHead.getTitle(), QString("Simple SMIL header for testing"));
    QCOMPARE(MySmilHead.getRootBackgroundColor(), QString("transparent"));
    QList<Region> region_list = MySmilHead.getLayout();
    QCOMPARE(region_list.count(), int(1));

    QCOMPARE(region_list.at(0).regionName, QString("test_screen"));
    QCOMPARE(region_list.at(0).width, qreal(1));
    QCOMPARE(region_list.at(0).height, qreal(1));
    QCOMPARE(region_list.at(0).top, qreal(0));
    QCOMPARE(region_list.at(0).left, qreal(0));
    QCOMPARE(region_list.at(0).z_index, int(1));
    QCOMPARE(region_list.at(0).backgroundColor, QString("#000"));

}

void Test_SmilHead::test_getHeadValuesFromComplexSmil()
{
    QFile file("../tests/data/smil/head_complex.smil");
    QDomDocument document;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    document.setContent(&file);
    file.close();
    QDomElement head = document.firstChildElement().firstChildElement();
    THead MySmilHead;
    MySmilHead.parse(head);

    QCOMPARE(MySmilHead.getRefreshTime(), int(300));
    QCOMPARE(MySmilHead.getTitle(), QString("Complex SMIL header for testing"));
    QList<Region> region_list = MySmilHead.getLayout();
    QCOMPARE(region_list.count(), int(5));

    QCOMPARE(region_list.at(0).regionName, QString("screen1"));
    QCOMPARE(region_list.at(0).width, qreal(1));
    QCOMPARE(region_list.at(0).height, qreal(1));
    QCOMPARE(region_list.at(0).top, qreal(0));
    QCOMPARE(region_list.at(0).left, qreal(0));
    QCOMPARE(region_list.at(0).z_index, int(0));
    QCOMPARE(region_list.at(0).backgroundColor, QString("red"));

    QCOMPARE(region_list.at(1).regionName, QString("screen2"));
    QCOMPARE(region_list.at(1).width, qreal(0.25));
    QCOMPARE(region_list.at(1).height, qreal(0.25));
    QCOMPARE(region_list.at(1).top, qreal(0));
    QCOMPARE(region_list.at(1).left, qreal(0.50));
    QCOMPARE(region_list.at(1).z_index, int(1));
    QCOMPARE(region_list.at(1).backgroundColor, QString("green"));

    QCOMPARE(region_list.at(2).regionName, QString("screen3"));
    QCOMPARE(region_list.at(2).width, qreal(0.50));
    QCOMPARE(region_list.at(2).height, qreal(0.50));
    QCOMPARE(region_list.at(2).top, qreal(0.50));
    QCOMPARE(region_list.at(2).left, qreal(0));
    QCOMPARE(region_list.at(2).z_index, int(1));
    QCOMPARE(region_list.at(2).backgroundColor, QString("blue"));

    QCOMPARE(region_list.at(3).regionName, QString("screen4"));
    QCOMPARE(region_list.at(3).width, qreal(0.25));
    QCOMPARE(region_list.at(3).height, qreal(0.50));
    QCOMPARE(region_list.at(3).top, qreal(0.50));
    QCOMPARE(region_list.at(3).left, qreal(0.50));
    QCOMPARE(region_list.at(3).z_index, int(1));
    QCOMPARE(region_list.at(3).backgroundColor, QString("orange"));

    QCOMPARE(region_list.at(4).regionName, QString("screen5"));
    QCOMPARE(region_list.at(4).width, qreal(0.25));
    QCOMPARE(region_list.at(4).height, qreal(0.50));
    QCOMPARE(region_list.at(4).top, qreal(0.50));
    QCOMPARE(region_list.at(4).left, qreal(0.75));
    QCOMPARE(region_list.at(4).z_index, int(1));
    QCOMPARE(region_list.at(4).backgroundColor, QString("transparent"));
}
