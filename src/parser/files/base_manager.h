#ifndef BASE_MANAGER_H
#define BASE_MANAGER_H

#include <QObject>

class BaseManager : public QObject
{
        Q_OBJECT
    public:
        BaseManager(QObject *parent = Q_NULLPTR);

    protected:
        bool     isRemote(QString src);


};

#endif // BASE_MANAGER_H
