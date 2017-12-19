#include "inventory_table.h"

DB::InventoryTable::InventoryTable(QObject *parent) : QObject(parent)
{
}

bool DB::InventoryTable::init(QString path)
{
    db_file.setFileName(path+"garlic.db");

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
    QString   sql = "REPLACE INTO inventory (resource_uri, cache_name, content_type, content_length, last_update, expires, state ) \
               VALUES( \
                   '" + dataset.resource_uri + "', \
                   '" + dataset.cache_name + "', \
                   '" + dataset.content_type + "', \
                   " + QString::number(dataset.content_length) + ", \
                   '" + dataset.last_update.toString() + "', \
                   '" + dataset.expires.toString() + "', \
                   " + QString::number(dataset.state) + " \
                )";
   if (!query.exec(sql))
       qCritical(Database) << "replace/insert failed" << sql << query.lastError().text();
}

DB::InventoryDataset DB::InventoryTable::getByResourceURI(QString resource_uri)
{
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM inventory WHERE resource_uri ='" +resource_uri+"'"))
        qCritical(Database) << "select failed" << query.lastError().text();
    if (!query.first())
        return InventoryDataset();

    return collectResult(&query);
}

void DB::InventoryTable::updateFileStatus(QString resource_uri, int status)
{
    QSqlQuery query(db);
    if (!query.exec("UPDATE inventory SET status = " + QString::number(status) + " WHERE resource_uri ='" +resource_uri+"'"))
        qCritical(Database) << "delete failed" << query.lastError().text();
}

void DB::InventoryTable::deleteByResourceURI(QString resource_uri)
{
    QSqlQuery query(db);
    if (!query.exec("DELETE FROM inventory WHERE resource_uri ='" +resource_uri+"'"))
        qCritical(Database) << "delete failed" << query.lastError().text();
}

void DB::InventoryTable::deleteByCacheName(QString cache_name)
{
    QSqlQuery query(db);
    if (!query.exec("DELETE FROM inventory WHERE cache_name ='" +cache_name+"'"))
        qCritical(Database) << "delete failed" << query.lastError().text();
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
        qCritical(Database) << "database file" << db_file.fileName() << "could not created";
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
        qCritical(Database) << "Inventory table could not be created" << query.lastError().text();
        return false;
    }
    if (!query.exec("CREATE INDEX cache_name ON inventory (cache_name collate nocase)"))
    {
        qCritical(Database) << "Index cache_name could not be created on table inventory" << query.lastError().text();
        return false;
    }
    return true;
}

bool DB::InventoryTable::createDBFile()
{
    if (!db_file.open(QIODevice::WriteOnly))
    {
        qCritical(Database) << "Inventory table could not be created";
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
