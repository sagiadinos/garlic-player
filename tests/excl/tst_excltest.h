#ifndef TST_EXCLTEST_H
#define TST_EXCLTEST_H

#include <QString>
#include <QtTest>
#include "../smilparser/excl.h"

class TestTExcl : public TExcl
{
    Q_OBJECT
public:
    TestTExcl(TBaseTiming * parent = 0){Q_UNUSED(parent);setObjectName("TestTExcl");}
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


class ExclTest : public QObject
{
    Q_OBJECT

public:
    ExclTest(){}

private Q_SLOTS:
    void testcreatePriorityClasses();
    void testfindInPriorityClasses();
private:
    void        investigateTestList(TestTExcl *MyExcl, QDomElement child);
    QDomElement getTestSmilFile();
};


#endif // TST_EXCLTEST_H
