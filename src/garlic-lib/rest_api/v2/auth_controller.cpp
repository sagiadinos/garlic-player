#include "auth_controller.h"

AuthController::AuthController(QObject *parent): BaseController(parent)
{

}


QString AuthController::determineJsonResponse(QString body)
{
    if (checkJsonRequest(body))
    {
        createJsonResponse();
    }

    return JsonResponse.asString(false).toUtf8();
}

void AuthController::createJsonResponse()
{
    JsonResponse.insertStringValuePair("access_token", MyConfiguration->determineApiAccessToken(username, password));
    JsonResponse.insertStringValuePair("token_type","Bearer");
    JsonResponse.insertStringValuePair("expires_in", MyConfiguration->getApiAccessTokenExpire());
}

bool AuthController::checkJsonRequest(QString json_string)
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



