#ifndef FILES_H
#define FILES_H

#include "base_controller.h"
#include "db/inventory_table.h"
#include "files/free_disc_space.h"
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
                QString  determineID(QString id);
                QString  responseFind(int max_results, int begin);
                QString  remove(QString body, FreeDiscSpace *fds);
            private:
                DB::InventoryTable *MyInventoryTable;
                void        createJsonFromList(QList<DB::InventoryDataset> results);
                QJsonObject createObject(DB::InventoryDataset dataset);
                QString     getObjectId(QString file_name);
                QString     getState(int state);
                qint64      determineTransferLength(QString file_name, int state);
                QString     determineIDFromJson(QString json_string);
        };
    }
}
#endif // FILES_H
