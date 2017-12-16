#include "inventory_table.h"

DB::InventoryTable::InventoryTable(QObject *parent) : QObject(parent)
{
}

bool DB::InventoryTable::init(QString path)
{
    db_file.setFileName(path);

    if (!db_file.exists())
    {
        if (!createDBFile() || !openDBFile() || !createTable())
        {
            db_file.remove(); // delete to try again
            return false;
        }
        return true;
    }
    else
        return openDBFile();
}

void DB::InventoryTable::replace(DB::InventoryDataset dataset)
{
    QSqlQuery query(db);
    query.exec("REPLACE INTO inventory \
               VALUES( \
                   resource_uri   = '" + dataset.resource_uri + "' \
                   cache_name     = '" + dataset.cache_name + "' \
                   content_type   = '" + dataset.content_type + "' \
                   content_length = '" + dataset.content_length + "' \
                   last_update    = '" + dataset.last_update.toString() + "' \
                   expires        = '" + dataset.expires.toString() + "' \
                   state          = '" + dataset.state + "' \
                )");
}

DB::InventoryDataset DB::InventoryTable::getByResourceURI(QString resource_uri)
{
    QSqlQuery query(db);
    query.exec("SELECT * FROM inventory WHERE resource_uri ='" +resource_uri+"'");
    if (!query.first())
        return InventoryDataset();

    return collectResult(&query);
}

QList<DB::InventoryDataset> DB::InventoryTable::getAll()
{
    QSqlQuery query(db);
    QList<InventoryDataset> result{DB::InventoryDataset()};
    query.exec("SELECT * FROM inventory ORDER BY UPPER(last_update) DESC");
    if (!query.first())
        return result;
    do
    {
        result.append(collectResult(&query));
    }
    while (query.next());

    return result;

}

bool DB::InventoryTable::openDBFile()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    db.setDatabaseName(db_file.fileName());
    if (!db.open())
    {
        qCritical() << "database file" << db_file.fileName() << "could not created";
        return false;
    }
    return true;
}

bool DB::InventoryTable::createTable()
{
    QSqlQuery query(db);
    QString sql = "CREATE TABLE inventory ( \
                  resource_uri binary(255) PRIMARY KEY, \
                  cache_name varchar(50), \
                  content_type varchar(20), \
                  content_length bigint, \
                  last_update date_time, \
                  expires datetime, \
                  state int \
                  )";

    if (!query.exec(sql))
    {
        qCritical() << "Inventory table could not be created" << query.lastError().text();
        return false;
    }
    return true;
}

bool DB::InventoryTable::createDBFile()
{
    if (!db_file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Inventory table could not be created";
        return false;
    }

    db_file.close();
    return true;
}

DB::InventoryDataset DB::InventoryTable::collectResult(QSqlQuery *query)
{
    DB::InventoryDataset dataset;
    dataset.resource_uri   = query->value("resource_uri").toString();
    dataset.cache_name     = query->value("cache_name").toString();
    dataset.content_type   = query->value("content_type").toString();
    dataset.content_length = query->value("content_length").toLongLong();
    dataset.last_update    = query->value("last_update").toDateTime();
    dataset.expires        = query->value("expires").toDateTime();
    dataset.state          = query->value("state").toInt();
    return dataset;
}
