#ifndef INVENTORYTABLE_H
#define INVENTORYTABLE_H

#include <QtSql>
#include <QList>
#include <QtDebug>
#include <QDateTime>
#include <QFile>
#include "logger.h"

namespace DB
{
    struct InventoryDataset
    {
            QString  resource_uri  = "";
            QString cache_name     = "";
            QString content_type   = "";
            qint64  content_length = 0;
            QDateTime last_update  = QDateTime();
            QDateTime expires      = QDateTime();
            int       state        = 0;

    };

    const int TRANSFER          = 1;
    const int STREAMING         = 2;
    const int CHECKSUM          = 3;
    const int CHECKSUM_ERROR    = 4;
    const int COMPLETE          = 5;

    class InventoryTable : public QObject
    {
            Q_OBJECT
        public:
            explicit InventoryTable(QObject *parent = nullptr);
            bool init(QString path);
            void replace(InventoryDataset dataset);
            InventoryDataset getByResourceURI(QString resource_uri);
            void updateFileStatus(QString resource_uri, int status);
            void deleteByResourceURI(QString resource_uri);
            void deleteByCacheName(QString cache_name);
            QList<InventoryDataset> getAll();
        protected:
            QSqlDatabase db;
            QFile   db_file;
            bool openDBFile();
            bool createDBFile();
            bool createTable();
            InventoryDataset collectResult(QSqlQuery *query);
    };
}
#endif // INVENTORYTABLE_H
