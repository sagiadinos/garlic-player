#ifndef TEST_SMIL_IMAGE_H
#define TEST_SMIL_IMAGE_H


class Test_SmilImage : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(){}
    void test_beginValues();
    void cleanupTestCase(){}
};

DECLARE_TEST(test_SmilImage);


#endif // TEST_SMIL_IMAGE_H
