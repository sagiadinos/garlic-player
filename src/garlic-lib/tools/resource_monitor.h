#ifndef RESOURCE_MONITOR_H
#define RESOURCE_MONITOR_H

#include <QObject>
#include <QDateTime>
#include "system_infos/memory.h"
#include "system_infos/general.h"

class ResourceMonitor : public QObject
{
        Q_OBJECT
    public:
        explicit ResourceMonitor(QObject *parent = nullptr);
        Q_INVOKABLE void refresh();
        Q_INVOKABLE QString getTotalMemorySystem() const;
        Q_INVOKABLE QString getFreeMemorySystem() const;
        Q_INVOKABLE QString getMemoryAppUse() const;
        Q_INVOKABLE QString getMaxMemoryAppUsed() const;
        Q_INVOKABLE QString getThreadsNumber() const;
        Q_INVOKABLE QString getMaxThreadsNumber() const;

    protected:
        QString              total_memory_system = "";
        QString              free_memory_system = "";
        QString              memory_apps_use = "";
        QString              max_memory_app_used = "";
        QString              threads_number = "";
        QString              max_threads_number = "";
        qint64               max_memory_used = 0;
        QString              max_memory_time = "";
        qint64               max_threads_used = 0;
        SystemInfos::Memory  MyMemoryInfos;
        SystemInfos::General MyGeneralInfos;
        void determineMemorySystem();
        void determineMemoryApp();
        void determineThreads();

};

#endif // RESOURCE_MONITOR_H
