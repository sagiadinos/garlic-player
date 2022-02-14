#include "files.h"

RestApi::V2::Files::Files(QObject *parent) : BaseController{parent}
{

}

void RestApi::V2::Files::setInventoryTable(DB::InventoryTable *it)
{
    MyInventoryTable = it;
}

QString RestApi::V2::Files::remove(QString body, FreeDiscSpace *fds)
{
    QString id                  = determineIDFromJson(body);
    DB::InventoryDataset result = MyInventoryTable->findByCacheBaseName(id);
   JsonResponse.reset();

   if (!result.resource_uri.isEmpty())
   {
       QString cache_path = MyConfiguration->getPaths("cache") + result.cache_name;
       if (fds->deleteFile(cache_path))
       {
           MyInventoryTable->deleteByCacheName(result.cache_name);
           JsonResponse.insertStringValuePair("success", id + " deleted");
       }
       else
           JsonResponse.insertStringValuePair("error", "delete failed: " + id);
   }
   else
   {
       JsonResponse.insertStringValuePair("error", "Record not found: " + id);
   }

   return JsonResponse.asString(false).toUtf8();
}

QString RestApi::V2::Files::determineID(QString id)
{
   DB::InventoryDataset result = MyInventoryTable->findByCacheBaseName(id);

   JsonResponse.reset();
   if (!result.resource_uri.isEmpty())
   {
       JsonResponse.setJsonObject(createObject(result));
   }
   else
   {
       JsonResponse.insertStringValuePair("error", "Record not found: " + id);
   }

   return JsonResponse.asString(false).toUtf8();
}

QString RestApi::V2::Files::responseFind(int max_results, int begin)
{
    QList<DB::InventoryDataset> results;

    JsonResponse.reset();
    if (max_results > 0)
        results = MyInventoryTable->findPaginated(max_results, begin);
    else
       results = MyInventoryTable->findAll();

    createJsonFromList(results);
    JsonResponse.insertIntegerValuePair("nextPageToken", (max_results + begin));

    return JsonResponse.asString(false).toUtf8();
}

void RestApi::V2::Files::createJsonFromList(QList<DB::InventoryDataset> results)
{
    QJsonArray json_array;
    for (QList<DB::InventoryDataset>::iterator i = results.begin(); i != results.end(); i++)
    {
        json_array.append(createObject(*i));
    }
    JsonResponse.insertArrayValuePair("items", json_array);

}

QJsonObject RestApi::V2::Files::createObject(DB::InventoryDataset dataset)
{
    QJsonObject json;
    json.insert("id", getObjectId(dataset.cache_name));
    json.insert("etag", "");
    json.insert("mimeType", dataset.content_type);
    json.insert("createdDate", "");
    json.insert("modifiedDate", dataset.last_update.toString(Qt::ISODate));
    json.insert("downloadPath", MyConfiguration->getPaths("cache") + dataset.cache_name);
    json.insert("fileSize", QString::number(dataset.content_length));
    json.insert("transferredSize", QString::number(determineTransferLength(dataset.cache_name, dataset.state)));
    json.insert("completed", (dataset.state == DB::COMPLETE));
    return json;
}

QString RestApi::V2::Files::getObjectId(QString file_name)
{
    QFileInfo fi(file_name);
    return fi.baseName();
}

QString RestApi::V2::Files::getState(int state)
{
    switch (state)
    {
        case DB::TRANSFER:
            return "COMPLETE";
        case DB::STREAMING:
            return "STREAMING";
        case DB::CHECKSUM:
            return "CHECKSUM";
        case DB::CHECKSUM_ERROR:
            return "CHECKSUM_ERROR";
        case DB::COMPLETE:
            return "COMPLETE";
        default:
            return "ERROR";
     }
}
qint64 RestApi::V2::Files::determineTransferLength(QString file_name, int state)
{
    QFileInfo fi(MyConfiguration->getPaths("cache") + file_name);
    switch (state)
    {
        case DB::COMPLETE:
            return fi.size();
        case DB::TRANSFER:
             fi.setFile(fi.absoluteFilePath());
             if (fi.exists())
                 return fi.size();
             else
                 return 0;
        default:
            return 0;

    }
}

QString RestApi::V2::Files::determineIDFromJson(QString json_string)
{
    if (!JsonRequest.readFromString(json_string))
        return "";

    // Todo implement real username password
    QString id = JsonRequest.getStringValueByKey("id");
    if (id == "error")
        return "";

    return id;
}
