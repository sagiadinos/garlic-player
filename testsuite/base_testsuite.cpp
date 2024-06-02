#include "base_testsuite.hpp"

BaseTestSuite::BaseTestSuite(QObject *parent) : QObject{parent}
{
    suite().push_back(this);
}

std::vector<QObject *> &BaseTestSuite::suite()
{
    static std::vector<QObject*> objects;
    return objects;
}
