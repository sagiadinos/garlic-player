#include "request_mapper.h"

RequestMapper::RequestMapper(MainConfiguration *mc, DB::InventoryTable *it,QObject* parent) : HttpRequestHandler(parent)
{
    MyConfiguration  = mc;
    MyInventoryTable = it;
}

void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QStringList sl  = QString(request.getPath()).split('/');
    qDebug() << sl;
    if (sl.size() < 2 || sl.at(1) != "v2")
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
        queryFiles(request, response);
    }
}

void RequestMapper::queryOauth(HttpRequest &request, HttpResponse &response)
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

void RequestMapper::querySystem(HttpRequest& request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    if (path ==  "/v2/system/firmwareInfo")
    {
        MySIController.setMainConfiguration(MyConfiguration);
        if (!MySIController.validateToken(request.getParameter("access_token")))
        {
            responseAccessViolation(response);
            return;
        }
        QString res = MySIController.responseFirmwareInfo();
        respond(response, res);
    }
    else if (path ==  "/v2/system/modelInfo")
    {
        MySIController.setMainConfiguration(MyConfiguration);
        if (!MySIController.validateToken(request.getParameter("access_token")))
        {
            responseAccessViolation(response);
            return;
        }
        QString res = MySIController.responseModelInfo();
        respond(response, res);
    }
    else
        responseNotFound(response);

}

void RequestMapper::queryFiles(HttpRequest &request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    if (path ==  "/v2/files/new")
    {
    }
    else if (path ==  "/v2/files/id")
    {
        // GET / POST
    }
    else if (path ==  "/v2/files/find")
    {
        MyFilesController.setMainConfiguration(MyConfiguration);
        MyFilesController.setInventoryTable(MyInventoryTable);
        if (!MyFilesController.validateToken(request.getParameter("access_token")))
        {
            responseAccessViolation(response);
            return;
        }
        QString res = MyFilesController.responseFind(request.getParameter("maxResults").toInt(), request.getParameter("pageToken").toInt());
        respond(response, res);
    }
    else if (path ==  "/v2/files/delete")
    {
    }
    else
        responseNotFound(response);
}

void RequestMapper::respond(HttpResponse& response, QString json)
{
    response.setHeader("Cache-Control", "no-cache,no-store");
    response.setHeader("Pragma", "no-cache");
    response.setHeader("Content-Type", "application/json; charset=utf-8");
    response.write(json.toUtf8(), true);
}

void RequestMapper::responseNotFound(HttpResponse& response)
{
    response.setStatus(404,"Not found");
    response.write("The URL is wrong, no such document.",true);
}

void RequestMapper::responseAccessViolation(HttpResponse& response)
{
    response.setStatus(403,"Access violation");
    response.write("{'error':'ACCESSVIOLATION'}",true);
}
