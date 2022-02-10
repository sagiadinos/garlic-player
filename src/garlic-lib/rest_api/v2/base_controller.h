#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include "tools/main_configuration.h"
#include "json_model.h"

class BaseController : public QObject
{
        Q_OBJECT
    public:
        explicit BaseController(QObject *parent = nullptr);
        void     setMainConfiguration(MainConfiguration *mc);
        bool     validateToken(QString access_token);

    protected:
        MainConfiguration *MyConfiguration;
        JsonModel JsonRequest, JsonResponse;

};

#endif // BASECONTROLLER_H
