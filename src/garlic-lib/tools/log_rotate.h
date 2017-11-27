#ifndef LOGROTATE_H
#define LOGROTATE_H

#include <QObject>

class LogRotate : public QObject
{
    Q_OBJECT
public:
    explicit LogRotate(QObject *parent = nullptr);

signals:

public slots:
};

#endif // LOGROTATE_H