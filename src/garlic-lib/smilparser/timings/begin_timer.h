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
        protected:
            bool fireImmediately();
     };
}
#endif // BEGINTIMER_H
