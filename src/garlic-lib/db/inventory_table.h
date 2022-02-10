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
    enum state
    {
        TRANSFER          = 1,
        STREAMING         = 2,
        CHECKSUM          = 3,
        CHECKSUM_ERROR    = 4,
        COMPLETE          = 5
    };

    class InventoryTable : public QObject
    {
            Q_OBJECT
        public:
            explicit InventoryTable(QObject *parent = nullptr);
            ~InventoryTable();
            bool init(QString path);
            void replace(InventoryDataset dataset);
            InventoryDataset getByResourceURI(QString resource_uri);
            void updateFileStatus(QString resource_uri, int state);
            void deleteByResourceURI(QString resource_uri);
            void deleteByCacheName(QString cache_name);
            QList<DB::InventoryDataset> findPaginated(int max_results, int begin = 0);
            QList<InventoryDataset> findAll();
            void setDbPath(QString path);

        protected:
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
            QFile db_file;
            bool createDbFile();
            bool openDbFile();
            bool createTable();
            InventoryDataset collectResult(QSqlQuery *query);
    };
}
#endif // INVENTORYTABLE_H
