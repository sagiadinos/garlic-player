#include "inventory_report_manager.h"

Reporting::InventoryReportManager::InventoryReportManager(MainConfiguration *config, DB::InventoryTable *inv_table, SystemInfos::DiscSpace *ds, QObject *parent) : Reporting::BaseReportManager(config, ds, parent)
{
    MyCreateInventoryReport.reset(new Reporting::CreateInventoryReport(MyConfiguration, this));
    MyCreateInventoryReport.data()->setDiscSpace(ds);
    MyInventoryTable = inv_table;

}


void Reporting::InventoryReportManager::handleSend()
{
    MyCreateInventoryReport.data()->process(MyInventoryTable->findAll());
    MyWebDav.data()->processPutData(action_url, MyCreateInventoryReport.data()->asXMLString().toUtf8());
}

void Reporting::InventoryReportManager::doSucceed(TNetworkAccess *uploader)
{
    qInfo(Develop) << "upload succeed" << uploader->getRemoteFileUrl().toString();
}

void Reporting::InventoryReportManager::doFailed(TNetworkAccess *uploader)
{
    qWarning(Develop) << "upload failed" << uploader->getRemoteFileUrl().toString();
}
