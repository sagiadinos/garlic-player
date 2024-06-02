#ifndef BASE_TESTSUITE_HPP
#define BASE_TESTSUITE_HPP

#include <QObject>
#include <vector>

class BaseTestSuite : public QObject
{
        Q_OBJECT
    public:
         BaseTestSuite(QObject *parent = nullptr);
         static std::vector<QObject*> & suite();
};

#endif // BASE_TESTSUITE_HPP
