#ifndef SYSTEMINFORMATIONCONTROLLER_H
#define SYSTEMINFORMATIONCONTROLLER_H

#include "base_controller.h"
#include <QObject>

class SystemInformationController : public BaseController
{
        Q_OBJECT
    public:
        explicit SystemInformationController(QObject *parent = nullptr);
        QString responseFirmwareInfo();
        QString responseModelInfo();
};

#endif // SYSTEMINFORMATIONCONTROLLER_H
