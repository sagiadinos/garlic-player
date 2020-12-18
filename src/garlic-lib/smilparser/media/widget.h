#ifndef TWIDGET_H
#define TWIDGET_H

#include "base_media.h"

class TWidget : public BaseMedia
{
    Q_OBJECT
public:
    explicit TWidget(TContainer *pc, Files::MediaManager *mm, MainConfiguration *config, QObject *parent = Q_NULLPTR);
    ~TWidget();
public slots:
    void     prepareDurationTimerBeforePlay();
protected:
    void     setAttributes();
};
#endif // TWIDGET_H
