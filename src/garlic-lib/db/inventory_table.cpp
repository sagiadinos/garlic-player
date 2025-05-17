#include "inventory_table.h"

DB::InventoryTable::InventoryTable(QObject *parent) : QObject(parent)
{
}

DB::InventoryTable::~InventoryTable()
{
    db.close();
}

bool DB::InventoryTable::init(QString path)
{
    setDbPath(path);
    if (!db_file.exists())
    {
        if (!createDbFile() || !openDbFile() || !createTable())
        {
            db_file.remove(); // delete to try again
            return false;
        }
        return true;
    }
    else
        return openDbFile();
}

bool DB::InventoryTable::replace(DB::InventoryDataset dataset)
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
    {
        qCritical(Database) << "replace/insert failed" << sql << query.lastError().text();
        return false;
    }
    return true;

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

void DB::InventoryTable::updateFileStatus(QString resource_uri, int state)
{
    QSqlQuery query(db);
    if (!query.exec("UPDATE inventory SET state = " + QString::number(state) + " WHERE resource_uri ='" +resource_uri+"'"))
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

DB::InventoryDataset DB::InventoryTable::findByCacheBaseName(QString base_name)
{
    QSqlQuery query(db);
    InventoryDataset result;
    query.exec("SELECT * FROM inventory WHERE cache_name LIKE '" + base_name +".%' LIMIT 1");
    if (!query.first())
        return result;

    return collectResult(&query);
}

QList<DB::InventoryDataset> DB::InventoryTable::findAll()
{
    QSqlQuery query(db);
    QList<InventoryDataset> result;
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

QList<DB::InventoryDataset> DB::InventoryTable::findPaginated(int max_results, int begin)
{
    QSqlQuery query(db);
    QList<InventoryDataset> result;
    query.exec("SELECT * FROM inventory ORDER BY UPPER(last_update) DESC LIMIT " + QString::number(begin) + ", " +  QString::number(max_results));
    if (!query.first())
        return result;
    do
    {
        result.append(collectResult(&query));
    }
    while (query.next());

    return result;

}


void DB::InventoryTable::setDbPath(QString path)
{
    db_file.setFileName(path+"garlic.db");
}


bool DB::InventoryTable::createTable()
{
    QSqlQuery query(db);
    QString sql = "CREATE TABLE inventory ( \
                  resource_uri TEXT PRIMARY KEY, \
                  cache_name TEXT, \
                  content_type TEXT, \
                  content_length INTEGER, \
                  last_update TEXT, \
                  etag TEXT, \
                  expires TEXT, \
                  state INTEGER \
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

bool DB::InventoryTable::openDbFile()
{
    db.setDatabaseName(db_file.fileName());
    if (!db.open())
    {
        qCritical(Database) << "database file" << db_file.fileName() << "could not be created";
        return false;
    }
    if (tableExists("inventory") && !hasField("inventory", "etag"))
    {
        QSqlQuery query(db);
        qCritical(Database) << "database file" << db_file.fileName() << " etag coould not be created";
        return query.exec("ALTER TABLE inventory ADD COLUMN etag TEXT;");
    }

    return true;
}


bool DB::InventoryTable::checkFields()
{
    // check for etag
    if (!hasField("inventory", "etag"))

    return true;
}

bool DB::InventoryTable::createDbFile()
{
    if (!db_file.open(QIODevice::WriteOnly))
    {
        qCritical(Database) << "DB file could not be created";
        return false;
    }

    db_file.close();
    return true;
}

bool DB::InventoryTable::hasField(const QString &tableName, const QString &fieldName)
{
    QSqlQuery q;
    q.prepare(QStringLiteral("PRAGMA table_info(%1)").arg(tableName));
    if (!q.exec()) return false;
    while (q.next()) {
        if (q.value(1).toString() == fieldName)
            return true;
    }
    return false;
}

bool DB::InventoryTable::tableExists(const QString &tableName)
{

    QSqlQuery query(db);
    if (!query.exec(
            QString("SELECT name FROM sqlite_master "
                    "WHERE type='table' AND name='%1'")
                .arg(tableName)))
        return false;
    return query.next();
}

DB::InventoryDataset DB::InventoryTable::collectResult(QSqlQuery *query)
{
    DB::InventoryDataset dataset;
    dataset.resource_uri   = query->value("resource_uri").toString();
    dataset.cache_name     = query->value("cache_name").toString();
    dataset.content_type   = query->value("content_type").toString();
    dataset.content_length = query->value("content_length").toLongLong();
    dataset.last_update    = QDateTime::fromString(query->value("last_update").toString());
    dataset.etag           = query->value("etag").toString();
    dataset.expires        = QDateTime::fromString(query->value("expires").toString());
    dataset.state          = query->value("state").toInt();
    return dataset;
}
