#include "event_logs_manager.h"

Reporting::EventLogsManager::EventLogsManager(TConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration      = config;
    MyWebDav             = new WebDav(MyConfiguration->getUserAgent().toUtf8(), this);
    MyCreateEventLogs.reset(new Reporting::CreateEventLogs(MyConfiguration, this));
    log_dir.setPath(MyConfiguration->getPaths("logs"));
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
    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    foreach (const QString &match,  log_dir.entryList(QStringList("event_log*"), QDir::Files, QDir::Time | QDir::Reversed))
    {
        if (match.mid(match.length()-1, 1) == "l")
            Logger::getInstance().rotateLog("event");
        MyCreateEventLogs.data()->process(match);
        QString action = MySubscription->getAction()+"/event-"+MyConfiguration->createUuid().mid(0,23)+MyConfiguration->getUuid().mid(23)+".xml";
        MyWebDav->processPutData(QUrl(action), MyCreateEventLogs.data()->asXMLString().toUtf8());
    }
    qDebug(Develop) << "end" << Q_FUNC_INFO;
}

