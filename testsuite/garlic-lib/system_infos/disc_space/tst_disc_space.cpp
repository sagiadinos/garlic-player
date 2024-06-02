#include "tst_disc_space.hpp"

void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){Q_UNUSED(type); Q_UNUSED(context); Q_UNUSED(msg)}

using ::testing::Exactly;
using ::testing::Return;


void TestDiscSpace::testInit()
{
    QString test_path      = "/test/path";
    qint64 total_bytes     = 100; // => 96 => 4 lockked
    qint64 available_bytes = 50;

    discSpace = new DiscSpace(&mockStorageInfo, nullptr);

    EXPECT_CALL(mockStorageInfo, setPath(test_path)).Times(Exactly(1));
    EXPECT_CALL(mockStorageInfo, bytesTotal()).Times(Exactly(2)).WillRepeatedly(Return(total_bytes));
    EXPECT_CALL(mockStorageInfo, bytesAvailable()).Times(Exactly(1)).WillOnce(Return(available_bytes));

    discSpace->init(test_path);

    QCOMPARE(discSpace->getBytesTotal(), 96);
    QCOMPARE(discSpace->getBytesLocked(), 4);
    QCOMPARE(discSpace->getBytesFree(), 46);
}

void TestDiscSpace::testRecalculate()
{
    qint64 available_bytes = 50;

    discSpace = new DiscSpace(&mockStorageInfo, nullptr);

    EXPECT_CALL(mockStorageInfo, bytesAvailable()).Times(Exactly(1)).WillOnce(Return(available_bytes));
    EXPECT_CALL(mockStorageInfo, refresh()).Times(Exactly(1));

    discSpace->recalculate();

    QCOMPARE(discSpace->getBytesTotal(), 0);
    QCOMPARE(discSpace->getBytesLocked(), 0);
    QCOMPARE(discSpace->getBytesFree(), 50);
}



static TestDiscSpace TEST_DISKSPACE;
