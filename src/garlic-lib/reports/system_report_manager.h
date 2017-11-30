#ifndef SYSTEM_REPORT_MANAGER_H
#define SYSTEM_REPORT_MANAGER_H

#include <QObject>
#include <QTimerEvent>
#include <QTimer>
#include "files/webdav.h"
#include "create/system_report.h"
#include "smilparser/head/subscription.h"
#include "tools/logger.h"
namespace Reporting
{
    class SystemReportManager : public QObject
    {
            Q_OBJECT
        public:
            explicit SystemReportManager(TConfiguration *config, QObject *parent = nullptr);
            ~SystemReportManager();
            void             init(SubScription *subscription);
        protected:
            int              timer_id = 0;
            WebDav          *MyWebDav = Q_NULLPTR;
            SubScription    *MySubscription;
            TConfiguration  *MyConfiguration;
            QScopedPointer<Reporting::CreateSystemReport> MyCreateSystemReport;
            void             timerEvent(QTimerEvent *event);
            void             sendSystemReport();
        signals:

        public slots:
    };
}
#endif // SYSTEM_REPORT_MANAGER_H
