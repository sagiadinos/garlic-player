#ifndef FILES_H
#define FILES_H

#include "base_controller.h"
#include "db/inventory_table.h"
#include <QObject>

namespace RestApi
{
    namespace V2
    {
        class Files : public BaseController
        {
                Q_OBJECT
            public:
                explicit Files(QObject *parent = nullptr);
                void     setInventoryTable(DB::InventoryTable *it);
                QString  responseFind(int max_results, int begin);
            private:
                DB::InventoryTable *MyInventoryTable;
                void        createJsonFromList(QList<DB::InventoryDataset> results);
                QJsonObject createObject(DB::InventoryDataset dataset);
                QString     getObjectId(QString file_name);
                QString     getState(int state);
                qint64      determineTransferLength(QString file_name, int state);
        };
    }
}
#endif // FILES_H
