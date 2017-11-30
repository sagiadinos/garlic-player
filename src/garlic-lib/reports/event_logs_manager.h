#ifndef EVENT_LOGS_MANAGER_H
#define EVENT_LOGS_MANAGER_H

#include <QTimerEvent>
#include <QTimer>
#include "files/webdav.h"
#include "create/event_logs.h"
#include "smilparser/head/subscription.h"

namespace Reporting
{
   class EventLogsManager : public QObject
    {
            Q_OBJECT
        public:
            explicit EventLogsManager(TConfiguration *config, QObject *parent = nullptr);

       void             init(SubScription *subscription);
   protected:
       int              timer_id = 0;
       QDir             log_dir;
       WebDav          *MyWebDav = Q_NULLPTR;
       SubScription    *MySubscription;
       TConfiguration  *MyConfiguration;
       QScopedPointer<Reporting::CreateEventLogs> MyCreateEventLogs;

       void             timerEvent(QTimerEvent *event);
       void             sendEvenLogs();
     };
}
#endif // EVENT_LOGS_MANAGER_H
