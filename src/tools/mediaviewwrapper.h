#ifndef MEDIAVIEWWRAPPER_H
#define MEDIAVIEWWRAPPER_H

#include <QWidget>

#ifdef SUPPORT_QTAV
    #include <QtAVWidgets>
#else
    #include <qvideowidget.h>
#endif


class MediaViewWrapper : public QWidget
{
    Q_OBJECT
public:
    explicit MediaViewWrapper(QWidget *parent);
    ~MediaViewWrapper();
#ifdef SUPPORT_QTAV
    QtAV::WidgetRenderer *getVideoWidget();
#else
    QVideoWidget *getVideoWidget();
#endif
    void                  setAspectRatioFill();
    void                  setAspectRatioMeet();
    void                  setAspectRatioMeetBest();
    void                  setParentWidget(QWidget *parent);
signals:
protected:
#ifdef SUPPORT_QTAV
    QtAV::WidgetRenderer VideoWidget;
#else
    QVideoWidget         VideoWidget;
#endif
};

#endif // MEDIAVIEWWRAPPER_H
