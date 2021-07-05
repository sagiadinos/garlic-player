#ifndef BEGINTIMER_H
#define BEGINTIMER_H

#include "enhanced_timer.h"
#include <QObject>

namespace Timings
{
    class BeginTimer : public Timings::EnhancedTimer
    {
            Q_OBJECT
        public:
            BeginTimer(QObject *parent = nullptr);
            qint64 getElapsedButActiveTimeTrigger();
            void   setDurTimer(SimpleTimer *dur);
        protected:
            bool fireImmediately();
        private:
            SimpleTimer *DurTimer = Q_NULLPTR;
     };
}
#endif // BEGINTIMER_H
