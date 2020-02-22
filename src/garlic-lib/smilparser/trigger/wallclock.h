#ifndef WALLCLOCK_H
#define WALLCLOCK_H

#include <QObject>
#include <QDateTime>
#include <QRegularExpression>

struct IsoPeriod
{
    int    years   = 0;
    int    months  = 0;
    qint64 days    = 0;
    qint64 hours   = 0;
    qint64 minutes = 0;
    qint64 seconds = 0;
};

class WallClock : public QObject
{
    Q_OBJECT
public:
    explicit  WallClock(QObject *parent = nullptr);
    void      parse(QString iso_date);
    IsoPeriod getPeriod(){return period;}
    bool      isActive(){return active;}
    int       getRepeats(){return repeats;}
    QDateTime getDateTime(){return datetime;}
    bool      remainingRepeats();
    qint64    getNextTimerTrigger(QDateTime current = QDateTime::currentDateTime());
protected:
    bool      active  = false;
    int       repeats = 0;
    QDateTime datetime;
    IsoPeriod period;
    int       remaining_repeats;
    qint64    getNextTrigger(QDateTime current);
    int       analyseRepeats(QString r_value);
    qint64    analyseRemainingRepeats(QDateTime actual);
    IsoPeriod analysePeriods(QString p_value);
    QDateTime analyseDate(QString date);
    QDateTime addWallClockInterval(QDateTime calculated);
    QDateTime addWallClockIntervalOptimized(QDateTime actual, QDateTime calculated);
signals:

};

#endif // WALLCLOCK_H
