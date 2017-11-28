#ifndef EVENT_LOGS_H
#define EVENT_LOGS_H

#include "base_reports.h"

namespace Reporting
{
    class CreateEventLogs : public Reporting::CreateBase
    {
        public:
            CreateEventLogs(TConfiguration *config, QObject *parent);
            void process();
        protected:
            QDomElement           system_info, network, interface, hardware_info, configuration;

    };
}
#endif // EVENT_LOGS_H
