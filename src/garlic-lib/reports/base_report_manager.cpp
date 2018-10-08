#include "base_report_manager.h"

Reporting::BaseReportManager::BaseReportManager(TConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration      = config;
    MyWebDav.reset(new WebDav(MyConfiguration));
    connect(MyWebDav.data(), SIGNAL(succeed(TNetworkAccess *)), SLOT(doSucceed(TNetworkAccess *)));
    connect(MyWebDav.data(), SIGNAL(failed(TNetworkAccess *)), SLOT(doFailed(TNetworkAccess *)));
}

Reporting::BaseReportManager::~BaseReportManager()
{
    if (timer_id != 0)
        killTimer(timer_id);
}

void Reporting::BaseReportManager::init(QString action, int refresh)
{
    action_url = action;
    handleSend();
    timer_id = startTimer(refresh * 1000);
}

void Reporting::BaseReportManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer_id)
        handleSend();
}
