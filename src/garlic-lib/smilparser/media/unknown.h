#ifndef UNKNOWN_H
#define UNKNOWN_H

#include "base_media.h"
/**
 * @brief The Unknown class
 * This class will used, when a unknown tag found
 * It will be treated as media and will be ignored
 */
class Unknown : public BaseMedia
{
    Q_OBJECT
public:
    explicit Unknown(TContainer *pc, QObject *parent = Q_NULLPTR);
    void     registerInMediaManager(Files::MediaManager *mm);
public slots:
    void     prepareDurationTimerBeforePlay();
protected:
    void     setAttributes();

};

#endif // UNKNOWN_H
