#ifndef BASETRIGGER_H
#define BASETRIGGER_H

#include <QObject>
#include "clock_value.h"

namespace Trigger
{
    class BaseTrigger : public QObject
    {
            Q_OBJECT
        public:
            explicit BaseTrigger(QObject *parent = nullptr);

            bool      hasExternTrigger();
            qint64    getTimeTrigger();
            QString   getSourceId();
            void      setActive(const bool &value);
            bool      isActive() const;

        protected:
            ClockValue *MyClockValue = Q_NULLPTR;
            bool        is_active;
            QChar       operant;
            QString     clock_value = "0";
            QString     source_id = "";
            bool        has_extern_trigger = false;
            QString     splitPossibleClockValue(QString sync_value);
            QString     splitClockValue(QString sync_value);
    };
}
#endif // BASETRIGGER_H
