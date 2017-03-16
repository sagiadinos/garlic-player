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
#include "smilparser/excl.h"

class InhertitedTExcl : public TExcl
{
    Q_OBJECT
public:
    InhertitedTExcl(TContainer * parent = 0)
    {
        Q_UNUSED(parent);
        ActivePriorityClass    = NULL;
        NewActivePriorityClass = NULL;
        setObjectName("TExcl");
    }
    int                 test_countPriorityClassList(){return ar_priorities.size();}
    TPriorityClass     *test_findPriorityClass(QDomElement dom_element){return findPriorityClass(dom_element);}
    QList<QDomElement>  test_getPriorityChilds()
    {
        QList<QDomElement> list;
        TPriorityClass *MyPriorityClass;
        for (QHash<int, TPriorityClass *>::iterator i = ar_priorities.begin(); i != ar_priorities.end(); i++)
        {
            MyPriorityClass = *i;
            list.append(MyPriorityClass->getChildList());
        }
        return list;
    }
};


class TestTExcl : public QObject
{
    Q_OBJECT

public:
    TestTExcl(){}

private Q_SLOTS:
    void test_createPriorityClasses();
    void test_findInPriorityClasses();
private:
    void        investigateTestList(InhertitedTExcl *MyExcl, QDomElement child);
    QDomElement getTestSmilFile();
};

void TestTExcl::test_createPriorityClasses()
{
    QDomElement  body          = getTestSmilFile();
    QDomNodeList dom_node_list = body.elementsByTagName("excl");
    InhertitedTExcl *MyExcl1         = new InhertitedTExcl();
    MyExcl1->parse(dom_node_list.item(0).toElement());
    QCOMPARE(MyExcl1->test_countPriorityClassList(), 4);

    InhertitedTExcl *MyExcl2         = new InhertitedTExcl();
    MyExcl2->parse(dom_node_list.item(1).toElement());
    QCOMPARE(MyExcl2->test_countPriorityClassList(), 1);

    InhertitedTExcl *MyExcl3         = new InhertitedTExcl();
    MyExcl3->parse(dom_node_list.item(2).toElement());
    QCOMPARE(MyExcl3->test_countPriorityClassList(), 2);
}

void TestTExcl::test_findInPriorityClasses()
{
    QDomElement         body          = getTestSmilFile();
    QDomNodeList        dom_node_list = body.elementsByTagName("excl");
    InhertitedTExcl          *MyExcl       = new InhertitedTExcl();
    MyExcl->parse(dom_node_list.item(0).toElement());
    QList<QDomElement>  list          = MyExcl->test_getPriorityChilds();
    QCOMPARE(list.size(), 4);

    investigateTestList(MyExcl, list.at(0));
    investigateTestList(MyExcl, list.at(1));
    investigateTestList(MyExcl, list.at(2));
    investigateTestList(MyExcl, list.at(3));
}

// ===================  private methods ====================================

void TestTExcl::investigateTestList(InhertitedTExcl *MyExcl, QDomElement child)
{
    TPriorityClass     *priority_class;
    if (child.attribute("xml:id") == "imgA0010")
    {
        priority_class = MyExcl->test_findPriorityClass(child);
        QCOMPARE(priority_class->getID(), QString("prioA1"));
    }
    else if (child.attribute("xml:id") == "imgA0009")
    {
        priority_class = MyExcl->test_findPriorityClass(child);
        QCOMPARE(priority_class->getID(), QString("prioA2"));
    }
    else if (child.attribute("xml:id") == "seq02")
    {
        priority_class = MyExcl->test_findPriorityClass(child);
        QCOMPARE(priority_class->getID(), QString("prioA3"));
    }
    else if (child.attribute("xml:id") == "seq01")
    {
        priority_class = MyExcl->test_findPriorityClass(child);
        QCOMPARE(priority_class->getID(), QString("prioA4"));
    }
    else
        QCOMPARE(false, true);
    return;
}

QDomElement TestTExcl::getTestSmilFile()
{
    QFile file("../tests/data/smil/priorityClass.smil");
    QDomDocument document;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "------------------------------ ERROR open file";

    document.setContent(&file);
    file.close();
    return document.firstChildElement();
}


QTEST_APPLESS_MAIN(TestTExcl)

#include "tst_excl.moc"
