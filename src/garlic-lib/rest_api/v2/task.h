#ifndef TASK_H
#define TASK_H

#include "base_controller.h"

namespace RestApi
{
    namespace V2
    {
        class Task : public BaseController
        {
                Q_OBJECT
            public:
                explicit Task(QObject *parent = nullptr);
                QString fireNotifyViaJson(QString json_string);
                QString fireNotify(QString smil_event);
                void   reboot();
            private:

         };
    }
}

#endif // TASK_H
