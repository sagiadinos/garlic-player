#include "system_report_manager.h"

Reporting::SystemReportManager::SystemReportManager(TConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration      = config;
    MyWebDav             = new WebDav(MyConfiguration->getUserAgent().toUtf8(), this);
}

Reporting::SystemReportManager::~SystemReportManager()
{
    if (timer_id != 0)
        killTimer(timer_id);
    MyWebDav->deleteLater(); // maybe a sendprocess is established
}

void Reporting::SystemReportManager::init(SubScription *subscription)
{
    MySubscription = subscription;
    sendSystemReport();
    timer_id = startTimer(MySubscription->getRefreshInterval()*1000);
}

void Reporting::SystemReportManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer_id)
        sendSystemReport();
}

void Reporting::SystemReportManager::sendSystemReport()
{
    qDebug(Develop) << "begin send system report" << Q_FUNC_INFO;
    Reporting::CreateSystemReport *MyCreateSystemReport = new Reporting::CreateSystemReport(new DiscSpace(MyConfiguration->getPaths("cache"), this),
                                                                           new SystemInfos::Memory(this),
                                                                           new SystemInfos::Network(this),
                                                                           MyConfiguration,
                                                                           this);
    MyCreateSystemReport->process();
    MyWebDav->processPutData(MySubscription->getAction(), MyCreateSystemReport->asXMLString().toUtf8());
    delete MyCreateSystemReport;
    qDebug(Develop) << "end send system report" << Q_FUNC_INFO;
}

