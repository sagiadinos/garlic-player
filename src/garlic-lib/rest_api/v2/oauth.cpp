#include "oauth.h"

RestApi::V2::OAuth::OAuth(QObject *parent): BaseController(parent)
{

}


QString RestApi::V2::OAuth::determineJsonResponse(QString body)
{
    if (checkJsonRequest(body))
    {
        createJsonResponse();
    }

    return JsonResponse.asString(false).toUtf8();
}

void RestApi::V2::OAuth::createJsonResponse()
{
    JsonResponse.insertStringValuePair("access_token", MyLibfacade->getConfiguration()->determineApiAccessToken(username, password));
    JsonResponse.insertStringValuePair("token_type","Bearer");
    JsonResponse.insertStringValuePair("expires_in", MyLibfacade->getConfiguration()->getApiAccessTokenExpire());
}

bool RestApi::V2::OAuth::checkJsonRequest(QString json_string)
{
    if (!JsonRequest.readFromString(json_string))
        return false;

    if (JsonRequest.getStringValueByKey("grant_type") != "password")
        return false;

    // Todo implement real username password
    username = JsonRequest.getStringValueByKey("username");
    if (username != "admin")
        return false;

    password = JsonRequest.getStringValueByKey("password");
    if (password != "")
        return false;

    return true;
}



