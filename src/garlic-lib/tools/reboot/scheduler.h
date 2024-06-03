/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2024 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/
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
