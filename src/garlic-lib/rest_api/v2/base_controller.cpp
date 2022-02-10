#include "base_controller.h"

BaseController::BaseController(QObject *parent) : QObject(parent)
{

}

void BaseController::setMainConfiguration(MainConfiguration *mc)
{
    MyConfiguration = mc;
}

bool BaseController::validateToken(QString access_token)
{
    QString debug = MyConfiguration->getUserConfigByKey("api_access_token");

    if (debug != access_token)
        return false;

    return true;
}
