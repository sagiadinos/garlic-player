#include "app.h"

RestApi::V2::App::App(QObject *parent) : BaseController{parent}
{

}

QString RestApi::V2::App::startJson(QString json_string)
{
    if (!JsonRequest.readFromString(json_string))
        return respondJSONError("No JSON found");

    return start(JsonRequest.getStringValueByKey("uri"),
                 JsonRequest.getStringValueByKey("packageName"),
                 JsonRequest.getStringValueByKey("className"),
                 JsonRequest.getStringValueByKey("Action"),
                 JsonRequest.getStringValueByKey("Type")
                );
}

QString RestApi::V2::App::start(QString uri, QString package_name, QString class_name, QString action, QString type)
{
    Q_UNUSED(action);Q_UNUSED(type);
    if (!MyLibfacade->getConfiguration()->validateContentUrl(uri))
        return respondJSONError(MyLibfacade->getConfiguration()->getErrorText());

    MyLibfacade->getConfiguration()->determineIndexUri(uri);
    MyLibfacade->initParser();

    JsonResponse.reset();
    JsonResponse.insertStringValuePair("packageName", package_name);
    JsonResponse.insertStringValuePair("className", class_name);
    JsonResponse.insertStringValuePair("data", uri);
    return JsonResponse.asString(false).toUtf8();
}

QString RestApi::V2::App::execJson(QString json_string)
{
    if (!JsonRequest.readFromString(json_string))
        return respondJSONError("No JSON found");

    return exec(JsonRequest.getStringValueByKey("uri"),
                 JsonRequest.getStringValueByKey("packageName"),
                 JsonRequest.getStringValueByKey("className"),
                 JsonRequest.getStringValueByKey("Action"),
                 JsonRequest.getStringValueByKey("Type")
                );
}

QString RestApi::V2::App::exec(QString uri, QString package_name, QString class_name, QString action, QString type)
{
    Q_UNUSED(action);Q_UNUSED(type);
    if (!MyLibfacade->getConfiguration()->validateContentUrl(uri))
        return respondJSONError(MyLibfacade->getConfiguration()->getErrorText());

    MyLibfacade->initParserWithTemporaryFile(uri);

    JsonResponse.reset();
    JsonResponse.insertStringValuePair("packageName", package_name);
    JsonResponse.insertStringValuePair("className", class_name);
    JsonResponse.insertStringValuePair("data", uri);
    return JsonResponse.asString(false).toUtf8();
}

QString RestApi::V2::App::switchJson(QString json_string)
{
    if (!JsonRequest.readFromString(json_string))
        return respondJSONError("No JSON found");

    return switchByMode(JsonRequest.getStringValueByKey("mode"));
}

QString RestApi::V2::App::switchByMode(QString mode)
{
    if (mode != "start")
        return respondJSONError("wrong mode");

    MyLibfacade->initParser();

    JsonResponse.reset();
    JsonResponse.insertStringValuePair("packageName", "");
    JsonResponse.insertStringValuePair("className", "");
    JsonResponse.insertStringValuePair("data", MyLibfacade->getConfiguration()->getIndexUri());
    return JsonResponse.asString(false).toUtf8();
}


