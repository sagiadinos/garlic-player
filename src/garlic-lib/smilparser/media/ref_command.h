#ifndef TREFCOMMAND_H
#define TREFCOMMAND_H

#include "base_media.h"

// Todo inherit from TWeb, cause that is the same functionality

class TRefCommand : public BaseMedia
{
    Q_OBJECT
public:
    explicit TRefCommand(TContainer *pc, Files::MediaManager *mm, MainConfiguration *config, QObject *parent = Q_NULLPTR);
    ~TRefCommand();
public slots:
    void     prepareDurationTimerBeforePlay();
protected:
    void     setAttributes();
};

#endif // TREFCOMMAND_H
