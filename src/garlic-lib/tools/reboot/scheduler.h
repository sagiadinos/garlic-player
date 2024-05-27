#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include "main_configuration.h"
#include "weekdayconverter.h"

class Scheduler : public QObject
{
        Q_OBJECT
    public:
        explicit Scheduler(MainConfiguration *config, WeekdayConverter *converter, QObject *parent = nullptr);
        void determineNextReboot();
        QDateTime getNextDatetime() const;

    private:
        MainConfiguration *MyMainConfiguration;
        WeekdayConverter  *MyWeekdayConverter;

        QDateTime          next_datetime = QDateTime::fromMSecsSinceEpoch(0);
        QStringList        reboot_days;
        QTime              reboot_time;
        QDate              today;
        QTime              current_time;

        void calculateNextReboot();
        int calculateDaysToAdd(int day_of_week) const;
        QDateTime findEarliestReboot() const;
        QList<QDateTime> computePotentialRebootTimes() const;

};

#endif // SCHEDULER_H
