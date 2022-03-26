#include "request_mapper.h"

RestApi::RequestMapper::RequestMapper(LibFacade *lf, QObject* parent) : HttpRequestHandler(parent)
{
    MyLibFacade      = lf;
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
    if (sl.at(2) == "oauth2")
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
    else if (sl.at(2) == "app")
    {
        queryApp(request, response, sl);
    }
    else if (sl.at(2) == "task")
    {
        queryTask(request, response, sl);
    }
    else
        responseNotFound(response);

}

void RestApi::RequestMapper::queryOauth(HttpRequest &request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    if (path ==  "/v2/oauth2/token")
    {
        MyAuthController.setLibFacade(MyLibFacade);
        QString res = MyAuthController.determineJsonResponse(request.getBody());
        respond(response, res);
    }
    else
        responseNotFound(response);
}

void RestApi::RequestMapper::querySystem(HttpRequest& request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    MySIController.setLibFacade(MyLibFacade);
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
    MyFilesController.setLibFacade(MyLibFacade);
    if (!MyFilesController.validateToken(request.getParameter("access_token")))
    {
        responseAccessViolation(response);
        return;
    }

    QString json_response;
    if (path.at(3)  ==  "new")
    {
        json_response = MyFilesController.createNew(request.getUploadedFile("data"),
                                                    request.getParameter("fileSize").toLong(),
                                                    request.getParameter("downloadPath"),
                                                    request.getParameter("etag"),
                                                    request.getParameter("mimeType"),
                                                    request.getParameter("modifiedDate")
                                                    );
    }
    else if (path.at(3)  ==  "find")
    {
        json_response = MyFilesController.findPaginated(request.getParameter("maxResults").toInt(), request.getParameter("pageToken").toInt());
    }
    else if (path.at(3)  ==  "delete")
    {
        json_response = MyFilesController.remove(request.getBody());
    }
    else
    {
        if (request.getMethod() == "GET")
            json_response = MyFilesController.findInfoByID(path.at(3));
        else if (request.getMethod() == "POST")
            json_response = MyFilesController.modifyByID(path.at(3),
                                                       request.getParameter("seek").toLong(),
                                                       request.getUploadedFile("data"),
                                                       request.getParameter("fileSize").toLong(),
                                                       request.getParameter("downloadPath"),
                                                       request.getParameter("etag"),
                                                       request.getParameter("mimeType"),
                                                       request.getParameter("modifiedDate"));
    }
    respond(response, json_response);
}

void RestApi::RequestMapper::queryApp(HttpRequest &request, HttpResponse &response, QStringList path)
{
/*    if (path.at(3)  ==  "exec")
    {

    }
    else if (path.at(3)  ==  "start")
    {

    }
    else if (path.at(3)  ==  "switch")
    {

    }
    else
*/        responseNotFound(response);

}

void RestApi::RequestMapper::queryTask(HttpRequest &request, HttpResponse &response,  QStringList path)
{
    MyTaskController.setLibFacade(MyLibFacade);
    QString json_response;
    if (path.at(3)  ==  "notify")
    {
        if (request.getHeader("content-type") == "application/json")
          json_response = MyTaskController.fireNotifyViaJson(request.getBody());
        else
          json_response = MyTaskController.fireNotify(request.getParameter("smilEvent"));

        respond(response, json_response);
    }
    else if (path.at(3)  ==  "reboot")
    {
        MyTaskController.reboot();
        respond(response, "");
    }
    else if (path.at(3)  ==  "screenshot")
    {
        QString file_path = MyLibFacade->getConfiguration()->getPaths("logs") + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") +".jpg";
        MyTaskController.takeScreenShot(file_path);
        respondImage(response, file_path);
    }
 /*   else if (path.at(3)  ==  "commitConfiguration")
    {

    }
    else if (path.at(3)  ==  "exportConfiguration")
    {

    }
*/    else
        responseNotFound(response);
}

void RestApi::RequestMapper::respond(HttpResponse& response, QString json)
{
    response.setHeader("Cache-Control", "no-cache,no-store");
    response.setHeader("Pragma", "no-cache");
    response.setHeader("Content-Type", "application/json; charset=utf-8");
    response.write(json.toUtf8(), true);
}

void RestApi::RequestMapper::respondImage(HttpResponse& response, QString file_path)
{
    // the quick and Dirty solution as Image is saved in another thread and we have to wait.
    // Alternative send Signals.
    QTime dieTime = QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    QFile f(file_path);
    if (!f.exists())
    {
        response.write("file not created.",true);
        return;
    }
    if (!f.open(QFile::ReadOnly))
    {
        response.write("file not readable.",true);
        return;
    }
    response.setHeader("Content-Type", "image/jpeg");
    response.setHeader("Content-Length", f.size());
    response.write(f.readAll(), true);
    f.close();
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
