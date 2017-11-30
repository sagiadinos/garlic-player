#include "system_report_manager.h"

Reporting::SystemReportManager::SystemReportManager(TConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration      = config;
    MyWebDav             = new WebDav(MyConfiguration->getUserAgent().toUtf8(), this);
    MyCreateSystemReport.reset(new Reporting::CreateSystemReport(MyConfiguration, this));
}

Reporting::SystemReportManager::~SystemReportManager()
{
    if (timer_id != 0)
        killTimer(timer_id);
    MyWebDav->deleteLater(); // maybe a send process was established
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
    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    MyCreateSystemReport.data()->process();
    MyWebDav->processPutData(MySubscription->getAction(), MyCreateSystemReport.data()->asXMLString().toUtf8());
    qDebug(Develop) << "end" << Q_FUNC_INFO;
}

