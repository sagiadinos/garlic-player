#ifndef WEEKDAYCONVERTER_H
#define WEEKDAYCONVERTER_H

#include <QObject>
#include <QMap>


class WeekdayConverter : public QObject
{
        Q_OBJECT
    public:
        explicit WeekdayConverter(QObject *parent = nullptr);
        int convertWeekDay(const QString &weekday_string) const;
};

#endif // WEEKDAYCONVERTER_H
