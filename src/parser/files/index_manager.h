#ifndef INDEX_MANAGER_H
#define INDEX_MANAGER_H

#include <QObject>
#include "base_manager.h"
#include "index_model.h"
#include "configuration.h"
#include "network.h"

class IndexManager : public BaseManager
{
        Q_OBJECT
    public:
        explicit IndexManager(IndexModel *im, TConfiguration *config,  Network *nw);
        void            init(QString src);
        void            lookUpForIndex();
        QDomElement     getHead();
        QDomElement     getBody();
    protected:
        TConfiguration *MyConfiguration;
        IndexModel     *MyIndexModel;
        Network        *MyNetwork;
        QString         src_index_path;
        void            loadLocal(QString local_path);

    protected slots:
        void doSucceed(QObject *network);
    signals:
        void availableIndex();

};

#endif // INDEX_MANAGER_H
