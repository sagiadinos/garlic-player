#include "base_testsuite.hpp"
#include <QtTest/QtTest>
#include <gmock/gmock.h>
#include "system_infos/disc_space.h"

//void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){Q_UNUSED(type); Q_UNUSED(context); Q_UNUSED(msg)}

using namespace SystemInfos;

class MockWrapperStorageInfo : public IStorageInfo
{
    public:
        MOCK_METHOD(quint64, bytesAvailable, (), (const, override));
        MOCK_METHOD(quint64, bytesTotal, (), (const, override));
        MOCK_METHOD(void, refresh, (), (override));
        MOCK_METHOD(void, setPath, (const QString &), (override));
};

class TestDiscSpace : public BaseTestSuite
{
    Q_OBJECT
    public:
        TestDiscSpace(){}
        QDir cache_dir;

    private Q_SLOTS:
        void testInit();
        void testRecalculate();
    private:
        DiscSpace *discSpace;
        MockWrapperStorageInfo mockStorageInfo;
};
