#ifndef INVENTORY_REPORT_H
#define INVENTORY_REPORT_H

#include "base_reports.h"

namespace Reporting
{
    class CreateInventoryReport : public Reporting::CreateBase
    {
            Q_OBJECT
        public:
            CreateInventoryReport(TConfiguration *config, QObject *parent = nullptr);
    };
}

#endif // INVENTORY_REPORT_H
