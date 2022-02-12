#include "base_controller.h"

RestApi::V2::BaseController::BaseController(QObject *parent) : QObject(parent)
{

}

void RestApi::V2::BaseController::setMainConfiguration(MainConfiguration *mc)
{
    MyConfiguration = mc;
}

bool RestApi::V2::BaseController::validateToken(QString access_token)
{
    QString debug = MyConfiguration->getUserConfigByKey("api_access_token");

    if (debug != access_token)
        return false;

    return true;
}
