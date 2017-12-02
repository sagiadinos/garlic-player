#include "system_report_manager.h"

Reporting::SystemReportManager::SystemReportManager(TConfiguration *config, QObject *parent) : Reporting::BaseReportManager(config, parent)
{
    MyCreateSystemReport.reset(new Reporting::CreateSystemReport(MyConfiguration, this));
}


void Reporting::SystemReportManager::handleSend()
{
    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    MyCreateSystemReport.data()->process();
    MyWebDav.data()->processPutData(action_url, MyCreateSystemReport.data()->asXMLString().toUtf8());
    qDebug(Develop) << "end" << Q_FUNC_INFO;
}

void Reporting::SystemReportManager::doSucceed(TNetworkAccess *uploader)
{
    qDebug(Develop) << "upload succeed" << uploader->getRemoteFileUrl().toString();
}

void Reporting::SystemReportManager::doFailed(TNetworkAccess *uploader)
{
    qDebug(Develop) << "upload failed" << uploader->getRemoteFileUrl().toString();
}

