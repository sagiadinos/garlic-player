#include "task.h"

RestApi::V2::Task::Task(QObject *parent) : BaseController{parent}
{

}

QString RestApi::V2::Task::fireNotifyViaJson(QString json_string)
{
    if (!JsonRequest.readFromString(json_string))
    {
        JsonResponse.reset();
        JsonResponse.insertStringValuePair("error", " No JSON found");
        return JsonResponse.asString(false).toUtf8();
    }

   return fireNotify(JsonRequest.getStringValueByKey("smilEvent"));
}

QString RestApi::V2::Task::fireNotify(QString smil_event)
{
    JsonResponse.reset();
    if (!smil_event.isEmpty())
        MyLibfacade->transferNotify(smil_event);
    else
        JsonResponse.insertStringValuePair("error", " No SmilEvent exists");

    return JsonResponse.asString(false).toUtf8();
}

void RestApi::V2::Task::takeScreenShot(QString file_path)
{
    JsonResponse.reset();
    MyLibfacade->takeScreenshot(file_path);
}

void RestApi::V2::Task::reboot()
{
    MyLibfacade->reboot("httpd: " + QDateTime::currentDateTime().toString(Qt::ISODate));
}
