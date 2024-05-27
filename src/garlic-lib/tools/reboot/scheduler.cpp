#include "scheduler.h"
#include <QDebug>
#include <QList>

Scheduler::Scheduler(MainConfiguration *config, WeekdayConverter *converter, QObject *parent)
    : QObject{parent}, MyMainConfiguration{config}, MyWeekdayConverter{converter} {}

void Scheduler::determineNextReboot()
{
    reboot_days = MyMainConfiguration->getRebootDays().split(" ");

    if (reboot_days.size() == 0)
        return;


    reboot_time = QTime::fromString(MyMainConfiguration->getRebootTime());
    if (!reboot_time.isValid())
        reboot_time = QTime::fromString("03:00:00");

    QDateTime now = QDateTime::currentDateTime();
    today = now.date();
    current_time = now.time();

    calculateNextReboot();
}

void Scheduler::calculateNextReboot()
{
    next_datetime = findEarliestReboot();

    qDebug() << "Next available appointment is on" << next_datetime.toString("dddd, yyyy-MM-dd HH:mm");
}


QDateTime Scheduler::getNextDatetime() const
{
    return next_datetime;
}

QDateTime Scheduler::findEarliestReboot() const
{
    QDateTime earliest_reboot = QDateTime::fromMSecsSinceEpoch(LLONG_MAX);

    foreach (const QDateTime &reboot_time, computePotentialRebootTimes())
    {
        if (reboot_time < earliest_reboot)
        {
            earliest_reboot = reboot_time;
        }
    }

    return earliest_reboot;
}

QList<QDateTime> Scheduler::computePotentialRebootTimes() const
{
    QList<QDateTime> potential_reboot_times;

    foreach (const QString &day_str, reboot_days)
    {
        int day_of_week = MyWeekdayConverter->convertWeekDay(day_str);
        int days_to_add = calculateDaysToAdd(day_of_week);
        QDateTime potential_datetime(today.addDays(days_to_add), reboot_time);
        potential_reboot_times.append(potential_datetime);
    }

    return potential_reboot_times;
}

int Scheduler::calculateDaysToAdd(int day_of_week) const
{
    int days_to_add = (day_of_week - today.dayOfWeek() + 7) % 7;

    if (days_to_add == 0 && current_time > reboot_time)
    {
        days_to_add = 7; // in case weekday is today but time passed
    }

    return days_to_add;
}

