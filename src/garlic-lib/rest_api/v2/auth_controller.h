#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include "base_controller.h"

class AuthController : public BaseController
{
        Q_OBJECT
    public:
        AuthController(QObject* parent = Q_NULLPTR);
        QString determineJsonResponse(QString body);
    private:
        bool checkJsonRequest(QString json_string);
        void createJsonResponse();
        QString username, password;
};

#endif // AUTHCONTROLLER_H
