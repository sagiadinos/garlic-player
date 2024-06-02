#include <QtTest/QtTest>
#include <gmock/gmock.h>
#include <QStorageInfo>
#include "disc_space.h"

//void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){Q_UNUSED(type); Q_UNUSED(context); Q_UNUSED(msg)}

using namespace SystemInfos;

class MockQStorageInfo : public QStorageInfo
{
    public:
        MOCK_METHOD0(bytesTotal, qint64());
/*        MOCK_METHOD0(bytesAvailable, qint64());
        MOCK_METHOD0(refresh, void());
        MOCK_METHOD1(setPath, void(const QString &));
*/};

class TestDiscSpace : public QObject
{
    Q_OBJECT
    public:
        TestDiscSpace(){}
        QDir cache_dir;

    private Q_SLOTS:
        void init();
        // void testInit();
    private:
        DiscSpace *discSpace;
        MockQStorageInfo *mockStorageInfo;
};

void TestDiscSpace::init()
{
    mockStorageInfo = new MockQStorageInfo();
    discSpace = new DiscSpace(mockStorageInfo, nullptr);
}
/*
void TestDiscSpace::testInit()
{
    QString testPath = "/test/path";
    qint64 totalBytes = 1000000;
    qint64 availableBytes = 500000;

    EXPECT_CALL(*mockStorageInfo, setPath(testPath)).Times(1);
    EXPECT_CALL(*mockStorageInfo, bytesTotal()).WillOnce(testing::Return(totalBytes));
    EXPECT_CALL(*mockStorageInfo, bytesAvailable()).WillOnce(testing::Return(availableBytes));

    discSpace->init(testPath);

    QCOMPARE(discSpace->getBytesTotal(), static_cast<quint64>(totalBytes * 0.8));
    QCOMPARE(discSpace->getBytesLocked(), static_cast<quint64>(totalBytes * 0.2));
    QCOMPARE(discSpace->getBytesFree(), static_cast<quint64>(availableBytes - (totalBytes * 0.2)));

}
*/

QTEST_MAIN(TestDiscSpace)

#include "tst_disc_space.moc"
