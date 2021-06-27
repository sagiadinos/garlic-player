#ifndef ENDTIMER_H
#define ENDTIMER_H

#include "enhanced_timer.h"
#include <QObject>

namespace Timings
{
    class EndTimer : public Timings::EnhancedTimer
    {
            Q_OBJECT
        public:
            EndTimer(QObject *parent = nullptr);
            void setActiveBeginTimeTrigger(qint64 value);
        protected:
            bool fireImmediately();
        private:
            qint64     active_begin_time_trigger = 0;

    };
}
#endif // ENDTIMER_H
