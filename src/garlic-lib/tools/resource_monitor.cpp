#include "resource_monitor.h"

ResourceMonitor::ResourceMonitor(QObject *parent) : QObject(parent)
{

}

void ResourceMonitor::refresh()
{
    MyMemoryInfos.refresh();
    determineMemorySystem();
    determineMemoryApp();
    determineThreads();
}

QString ResourceMonitor::getTotalMemorySystem() const
{
    return total_memory_system;
}

QString ResourceMonitor::getFreeMemorySystem() const
{
    return free_memory_system;
}

QString ResourceMonitor::getMemoryAppUse() const
{
    return memory_apps_use;
}

QString ResourceMonitor::getMaxMemoryAppUsed() const
{
    return max_memory_app_used;
}

QString ResourceMonitor::getThreadsNumber() const
{
    return threads_number;
}

QString ResourceMonitor::getMaxThreadsNumber() const
{
    return max_threads_number;
}

void ResourceMonitor::determineMemorySystem()
{
    qint64  total_mem = MyMemoryInfos.getTotal();
    qint64  free_mem = MyMemoryInfos.getFree();
    double d_total = (double)total_mem / (double)1048576;
    double d_free = (double)free_mem / (double)1048576;
    total_memory_system = QString("Total Memory System: %1" ).arg(d_total, 0, 'f', 2) + " MiB";
    free_memory_system  = QString("Free Memory System: %1" ).arg(d_free, 0, 'f', 2) + " MiB";
}

void ResourceMonitor::determineMemoryApp()
{
    qint64  current_rss = MyMemoryInfos.getRSS();
    if (current_rss > max_memory_used)
    {
        max_memory_used = current_rss;
        max_memory_time = QDateTime::currentDateTime().toString();
    }
    double d_current = (double)current_rss / (double)1048576;
    double d_max = (double)max_memory_used / (double)1048576;
    memory_apps_use     = QString("App Memory use: %1" ).arg(d_current, 0, 'f', 2) + " MiB";
    max_memory_app_used = QString("Max Memory App used: %1" ).arg(d_max, 0, 'f', 2) + " MiB at " + max_memory_time;
}

void ResourceMonitor::determineThreads()
{
    qint64  current_threads = MyGeneralInfos.countThreads();
    if (current_threads > max_threads_used)
        max_threads_used = current_threads;
    threads_number     = "Threads: " + QString::number(current_threads);
    max_threads_number = "Max Threads: " + QString::number(max_threads_used);
}
