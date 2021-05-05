#ifndef BEGINTIMER_H
#define BEGINTIMER_H

#include "enhanced_timer.h"
#include <QObject>

namespace Timings
{
    class BeginTimer : public EnhancedTimer
    {
            Q_OBJECT
        public:
            BeginTimer(SimpleTimer *dt, QObject *parent  = nullptr);
            ~BeginTimer();
            void        start();
        protected:
            SimpleTimer *DurTimer;
    };
}
#endif // BEGINTIMER_H
