#include "system_report_manager.h"

Reporting::SystemReportManager::SystemReportManager(MainConfiguration *config, SystemInfos::DiscSpace *ds, QObject *parent) : Reporting::BaseReportManager(config, ds, parent)
{
    MyCreateSystemReport.reset(new Reporting::CreateSystemReport(MyConfiguration, this));
    MyCreateSystemReport.data()->setDiscSpace(ds);
}

void Reporting::SystemReportManager::handleSend()
{
    MyCreateSystemReport.data()->process();
    MyWebDav.data()->processPutData(action_url, MyCreateSystemReport.data()->asXMLString().toUtf8());
}

void Reporting::SystemReportManager::doSucceed(TNetworkAccess *uploader)
{
    qInfo(Develop) << "upload succeed" << uploader->getRemoteFileUrl().toString();
}

void Reporting::SystemReportManager::doFailed(TNetworkAccess *uploader)
{
    qWarning(Develop) << "upload failed" << uploader->getRemoteFileUrl().toString();
}

