#include "event_logs.h"

Reporting::CreateEventLogs::CreateEventLogs(TConfiguration *config,
                                            QObject *parent) : Reporting::CreateBase(config, parent)
{

}


void Reporting::CreateEventLogs::process()
{
    init();
}
