#ifndef EVENT_LOGS_H
#define EVENT_LOGS_H

#include "base_reports.h"

namespace Reporting
{
    class CreateEventLogs : public Reporting::CreateBase
    {
        public:
            CreateEventLogs(TConfiguration *config, QObject *parent);
            void process(QString file_path);
        protected:
            QDomElement player_event_log;
            void mergeXml(QString file_path);

    };
}
#endif // EVENT_LOGS_H
