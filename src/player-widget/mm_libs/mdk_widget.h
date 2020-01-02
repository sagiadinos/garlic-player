#ifdef SUPPORT_MDK

#ifndef MDK_WIDGET_H
#define MDK_WIDGET_H

#include <QWidget>
#include "mdk_classes/QMDKRenderer.h"

class MdkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MdkWidget(QWidget *parent = nullptr);

    QWidget              *getVideoWidget();
    QWidget              *getVideoRenderer();

    void                  setAspectRatioFill();
    void                  setAspectRatioMeet();
    void                  setAspectRatioMeetBest();

protected:
    QScopedPointer<QMDKWidgetRenderer> VideoWidget;  // a deleteLater leads to a crash on playlistchange!
public slots:
};

#endif // MDK_WIDGET_H

#endif
