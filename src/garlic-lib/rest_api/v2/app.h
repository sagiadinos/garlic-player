#ifndef APP_H
#define APP_H

#include "base_controller.h"
namespace RestApi
{
    namespace V2
    {
        class App : public BaseController
        {
                Q_OBJECT
            public:
                explicit App(QObject *parent = nullptr);

            signals:

        };
    }
}

#endif // APP_H
