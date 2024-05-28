#include <QtTest>
#include "tools/reboot/weekdayconverter.h"

class TestWeekdayConverter : public QObject
{
    Q_OBJECT

public:
    TestWeekdayConverter(){};

private Q_SLOTS:
    void testConvertWeekDay();
};

void TestWeekdayConverter::testConvertWeekDay()
{
    WeekdayConverter TestClass;

   QCOMPARE(TestClass.convertWeekDay("MonDay"), 1);
   QCOMPARE(TestClass.convertWeekDay("tuesday"), 2);
   QCOMPARE(TestClass.convertWeekDay("Wednesday"), 3);
   QCOMPARE(TestClass.convertWeekDay("thursday"), 4);
   QCOMPARE(TestClass.convertWeekDay("Friday"), 5);
   QCOMPARE(TestClass.convertWeekDay("saturday"), 6);
   QCOMPARE(TestClass.convertWeekDay("sunday"), 7);
   QCOMPARE(TestClass.convertWeekDay("notexistsday"), 0);
}

QTEST_MAIN(TestWeekdayConverter)

#include "tst_weekdayconverter.moc"
