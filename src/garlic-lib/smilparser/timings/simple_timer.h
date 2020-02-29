#ifndef TSIMPLETIMER_H
#define TSIMPLETIMER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

#include "clock_value.h"

class SimpleTimer : public QObject
{
    Q_OBJECT
public:
    const     int        TYPE_CLOCKVALUE     = 1;
    const     int        TYPE_INDEFINITE     = 2;
    const     int        TYPE_MEDIA          = 3;

    explicit SimpleTimer(QObject *parent = nullptr);
    ~SimpleTimer();

    void         parse(QString s_value);
    bool         isActive(){return MyTimer->isActive();}
    void         start();
    void         pause();
    void         resume();
    void         stop();
    int          getType(){return type;}
    int          getRemaining() const;

protected:
    QTimer      *MyTimer = Q_NULLPTR;
    ClockValue   MyClockValue;
    qint64       pause_start;
    int          remaining = 0;
    int          type = 3;
    void         initTimer();
    void         deleteTimer();
protected slots:
    void         emitTimeout();

signals:
    void timeout();
};

#endif // TTIMER_H
