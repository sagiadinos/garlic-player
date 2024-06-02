#include "base_testsuite.hpp"
#include <QtTest/QtTest>
#include <gtest/gtest.h>

/**
 * To create a testsuite like normal TestsUnits we use the approach of
 * https://alexhuszagh.github.io/2016/using-qttest-effectively/
 *
 * with some changes to use cmake
 * furthermore we do not need to include all files mmanually in cmake
 *
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
    int status = 0;
    auto runTest = [&status, argc, argv](QObject* obj)
    {
        status |= QTest::qExec(obj, argc, argv);
    };

    // run suite
    auto &suite = BaseTestSuite::suite();
    for (auto it = suite.begin(); it != suite.end(); ++it)
    {
        runTest(*it);
    }

    return status;

}
