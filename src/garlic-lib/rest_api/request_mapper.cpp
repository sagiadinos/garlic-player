#include "request_mapper.h"

RequestMapper::RequestMapper(MainConfiguration *mc, QObject* parent) : HttpRequestHandler(parent)
{
    MyConfiguration = mc;
}

void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QStringList sl  = QString(request.getPath()).split('/');
    QByteArray path = request.getPath();
    qDebug() << sl;

    if (sl.size() == 0 || sl.at(1) != "v2")
    {
        responseNotFound(response);
        return;
    }

    if (path ==  "/v2/oauth2/token")
    {
        MyAuthController.setMyConfiguration(MyConfiguration);
        QString res = MyAuthController.determineJsonResponse(request.getBody());
        respond(response, res);
    }
    else if (path ==  "/v2/system/firmwareInfo")
    {
        MySIController.setMyConfiguration(MyConfiguration);
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
        MySIController.setMyConfiguration(MyConfiguration);
        if (!MySIController.validateToken(request.getParameter("access_token")))
        {
            responseAccessViolation(response);
            return;
        }
        QString res = MySIController.responsemodelInfo();
        respond(response, res);
    }
    else
        responseNotFound(response);

    qDebug() << "RequestMapper: finished request";
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
