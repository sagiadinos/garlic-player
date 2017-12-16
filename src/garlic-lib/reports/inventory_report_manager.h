#ifndef INVENTORY_REPORT_MANAGER_H
#define INVENTORY_REPORT_MANAGER_H

#include "create/event_logs.h"
#include "base_report_manager.h"

namespace Reporting
{
    class InventoryReportManager : public Reporting::BaseReportManager
    {
            Q_OBJECT
        public:
            InventoryReportManager(TConfiguration *config, QObject *parent = nullptr);
        protected:
            void             handleSend();
         protected slots:
            void               doSucceed(TNetworkAccess *uploader);
            void               doFailed(TNetworkAccess *uploader);
    };
}

#endif // INVENTORY_REPORT_MANAGER_H
