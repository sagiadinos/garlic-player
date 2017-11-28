#include "event_logs_manager.h"

Reporting::EventLogsManager::EventLogsManager(TConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration      = config;
    MyWebDav             = new WebDav(MyConfiguration->getUserAgent().toUtf8(), this);

}

void Reporting::EventLogsManager::init(SubScription *subscription)
{
    MySubscription = subscription;
    sendEvenLogs();
    timer_id = startTimer(MySubscription->getRefreshInterval()*1000);
}


void Reporting::EventLogsManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer_id)
        sendEvenLogs();
}


void Reporting::EventLogsManager::sendEvenLogs()
{
/*    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    Reporting::CreateEventLogs *MyCreateEventLogs = new Reporting::CreateEventLogs(MyConfiguration, this);
    MyCreateEventLogs->process();
    MyWebDav->processPutData(MySubscription->getAction(), MyCreateEventLogs->asXMLString().toUtf8());
    qDebug(Develop) << "end" << Q_FUNC_INFO;
*/}

