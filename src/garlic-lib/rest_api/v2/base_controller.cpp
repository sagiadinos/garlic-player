#include "base_controller.h"

RestApi::V2::BaseController::BaseController(QObject *parent) : QObject(parent)
{

}

void RestApi::V2::BaseController::setLibFacade(LibFacade *lf)
{
    MyLibfacade = lf;
}

bool RestApi::V2::BaseController::validateToken(QString access_token)
{
    QString debug = MyLibfacade->getConfiguration()->getUserConfigByKey("api_access_token");

    if (debug != access_token)
        return false;

    return true;
}
