#include "inventory_report_manager.h"

Reporting::InventoryReportManager::InventoryReportManager(MainConfiguration *config, DB::InventoryTable *inv_table, QObject *parent) : Reporting::BaseReportManager(config, parent)
{
    MyCreateInventoryReport.reset(new Reporting::CreateInventoryReport(MyConfiguration, this));
    MyInventoryTable = inv_table;

}


void Reporting::InventoryReportManager::handleSend()
{
    MyCreateInventoryReport.data()->process(MyInventoryTable->getAll());
    MyWebDav.data()->processPutData(action_url, MyCreateInventoryReport.data()->asXMLString().toUtf8());
}

void Reporting::InventoryReportManager::doSucceed(TNetworkAccess *uploader)
{
    qDebug(Develop) << "upload succeed" << uploader->getRemoteFileUrl().toString();
}

void Reporting::InventoryReportManager::doFailed(TNetworkAccess *uploader)
{
    qDebug(Develop) << "upload failed" << uploader->getRemoteFileUrl().toString();
}
