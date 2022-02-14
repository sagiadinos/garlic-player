#include "request_mapper.h"

RestApi::RequestMapper::RequestMapper(MainConfiguration *mc, DB::InventoryTable *it, FreeDiscSpace *fds, QObject* parent) : HttpRequestHandler(parent)
{
    MyConfiguration  = mc;
    MyInventoryTable = it;
    MyFreeDiscSpace  = fds;
}

void RestApi::RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QStringList sl  = QString(request.getPath()).split('/');
    qDebug() << sl;
    if (sl.size() < 4 || sl.at(1) != "v2")
    {
        responseNotFound(response);
        return;
    }

    if (sl.at(2) == "oauth")
    {
        queryOauth(request, response);
    }
    else if (sl.at(2) == "system")
    {
        querySystem(request, response);
    }
    else if (sl.at(2) == "files")
    {
        queryFiles(request, response, sl);
    }
    else
        responseNotFound(response);

}

void RestApi::RequestMapper::queryOauth(HttpRequest &request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    if (path ==  "/v2/oauth2/token")
    {
        MyAuthController.setMainConfiguration(MyConfiguration);
        QString res = MyAuthController.determineJsonResponse(request.getBody());
        respond(response, res);
    }
    else
        responseNotFound(response);
}

void RestApi::RequestMapper::querySystem(HttpRequest& request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    MySIController.setMainConfiguration(MyConfiguration);
    if (!MySIController.validateToken(request.getParameter("access_token")))
    {
        responseAccessViolation(response);
        return;
    }
    if (path ==  "/v2/system/firmwareInfo")
    {
        QString res = MySIController.responseFirmwareInfo();
        respond(response, res);
    }
    else if (path ==  "/v2/system/modelInfo")
    {
        QString res = MySIController.responseModelInfo();
        respond(response, res);
    }
    else
        responseNotFound(response);
}

void RestApi::RequestMapper::queryFiles(HttpRequest &request, HttpResponse &response, QStringList path)
{
    MyFilesController.setMainConfiguration(MyConfiguration);
    MyFilesController.setInventoryTable(MyInventoryTable);
    if (!MyFilesController.validateToken(request.getParameter("access_token")))
    {
        responseAccessViolation(response);
        return;
    }

    if (path.at(3)  ==  "new")
    {
    }
    else if (path.at(3)  ==  "find")
    {
        QString res = MyFilesController.responseFind(request.getParameter("maxResults").toInt(), request.getParameter("pageToken").toInt());
        respond(response, res);
    }
    else if (path.at(3)  ==  "delete")
    {
        QString res = MyFilesController.remove(request.getBody(), MyFreeDiscSpace);
        respond(response, res);
    }
    else
    {
        QString res = MyFilesController.determineID(path.at(3));
        respond(response, res);
    }

}

void RestApi::RequestMapper::respond(HttpResponse& response, QString json)
{
    response.setHeader("Cache-Control", "no-cache,no-store");
    response.setHeader("Pragma", "no-cache");
    response.setHeader("Content-Type", "application/json; charset=utf-8");
    response.write(json.toUtf8(), true);
}

void RestApi::RequestMapper::responseNotFound(HttpResponse& response)
{
    response.setStatus(404,"Not found");
    response.write("The URL is wrong, no such document.",true);
}

void RestApi::RequestMapper::responseAccessViolation(HttpResponse& response)
{
    response.setStatus(403,"Access violation");
    response.write("{'error':'ACCESSVIOLATION'}",true);
}
