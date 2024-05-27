#ifndef TIMER_H
#define TIMER_H

#include <QObject>

#include <QObject>
#include <QTimer>
#include <QDateTime>

class Timer : public QObject
{
        Q_OBJECT
    public:
        explicit Timer(QObject *parent = nullptr);
        void setRebootTime(const QDateTime &rebootDateTime);

    signals:
        void rebootTimeReached();

    private slots:
        void checkRebootTime();

    private:
        QTimer timer;
        QDateTime rebootDateTime;
};

#endif // TIMER_H
