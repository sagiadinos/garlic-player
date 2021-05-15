#ifndef SYNCBASE_H
#define SYNCBASE_H

#include <QObject>
#include "clock_value.h"

class SyncBase : public QObject
{
        Q_OBJECT
    public:

        explicit SyncBase(QObject *parent = nullptr);
        void      parse(QString sync_value);
        bool      hasExternTrigger();
        qint64    getTimeTrigger();
        QString   getSourceId();
        QString   getSymbol();

        void setActive(const bool &value);
        bool isActive() const;

    protected:
        ClockValue *MyClockValue = Q_NULLPTR;
        bool        is_active;
        QChar       operant;
        QString     clock_value = "0";
        QString     source_id = "";
        bool        has_extern_trigger = false;
        QString     splitPossibleClockValue(QString sync_value);
        QString     splitClockValue(QString sync_value);

    private:
        QString symbol = "none";

};

#endif // SYNCBASE_H
