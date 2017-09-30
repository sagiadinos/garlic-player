#ifndef BASE_MANAGER_H
#define BASE_MANAGER_H

#include <QObject>

class BaseManager : public QObject
{
        Q_OBJECT
    public:
        BaseManager();

    protected:
        bool     isRemote(QString src);


};

#endif // BASE_MANAGER_H
