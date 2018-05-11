#ifndef MEDIAVIEWWRAPPER_H
#define MEDIAVIEWWRAPPER_H

#include <QWidget>

#ifdef SUPPORT_QTAV
    #include <QtAV>
    #include <QtAVWidgets>
#else
    #include <qvideowidget.h>
#endif


class MediaViewWrapper : public QWidget
{
    Q_OBJECT
public:
    explicit MediaViewWrapper(QWidget *parent = Q_NULLPTR);
    ~MediaViewWrapper();
#ifdef SUPPORT_QTAV
    QtAV::GLWidgetRenderer2 *getVideoWidget();
#else
    QVideoWidget *getVideoWidget();
#endif
    void                  setAspectRatioFill();
    void                  setAspectRatioMeet();
    void                  setAspectRatioMeetBest();
signals:
protected:
#ifdef SUPPORT_QTAV
    QScopedPointer<QtAV::GLWidgetRenderer2> VideoWidget;  // a deleteLater leads to a crash on playlistchange!
#else
    QScopedPointer<QVideoWidget>         VideoWidget;
#endif
};

#endif // MEDIAVIEWWRAPPER_H
