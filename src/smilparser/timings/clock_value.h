#ifndef TCLOCKVALUE_H
#define TCLOCKVALUE_H
#include <QObject>

/**
 * @brief The TClockValue class calculate timings for dur, min and max attributes.
 *        inherited by TTiming for begin and end attributes
 */
class TClockValue : public QObject
{
    Q_OBJECT
public:
             TClockValue(QObject *parent = 0);
    void     parse(QString s_value);
    QString  getStatus();
    qint64   getMilliseconds();
protected:
    QString   status      = "";
    qint64    ms          = 0;
    qint64    calculateMilliseconds(QString duration);
signals:

public slots:
};

#endif // TCLOCKVALUE_H
