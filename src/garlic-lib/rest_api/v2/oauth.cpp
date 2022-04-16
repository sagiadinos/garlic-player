#include "oauth.h"

RestApi::V2::OAuth::OAuth(QObject *parent): BaseController(parent)
{

}


QString RestApi::V2::OAuth::determineJsonResponse(QString json_string)
{
    if (!JsonRequest.readFromString(json_string))
        return respondJSONError("No JSON found");

    return determineResponse(JsonRequest.getStringValueByKey("grant_type"),
                                 JsonRequest.getStringValueByKey("username"),
                                 JsonRequest.getStringValueByKey("password")
    );
}

QString RestApi::V2::OAuth::determineResponse(QString grant_type, QString username, QString password)
{
    if (grant_type != "password")
        return respondJSONError("wrong grant_type");

    // Todo implement real username password
    if (username != "admin")
        return respondJSONError("wrong username");

    if (password != "")
        return respondJSONError("wrong password");

    return createJsonResponse();
}

QString RestApi::V2::OAuth::createJsonResponse()
{
    JsonResponse.reset();
    JsonResponse.insertStringValuePair("access_token", MyLibfacade->getConfiguration()->determineApiAccessToken(username, password));
    JsonResponse.insertStringValuePair("token_type","Bearer");
    JsonResponse.insertStringValuePair("expires_in", MyLibfacade->getConfiguration()->getApiAccessTokenExpire());
    return JsonResponse.asString(false).toUtf8();
}


