#ifdef SUPPORT_QTAV

#ifndef QTAV_H
#define QTAV_H

#include <QObject>
#include "tools/logging_categories.h"
#include <QtAV/AVPlayer.h>
#include "mediawidget_wrapper.h"

class QtAVDecoder  : public QObject
{
    Q_OBJECT
public:
    QtAVDecoder(QObject *parent = Q_NULLPTR);
    ~QtAVDecoder();
    void                setVideoOutput(MediaWidgetWrapper *renderer);
    void                removeVideoOutput(MediaWidgetWrapper *renderer);
    bool                load(QString file_path);
    void                setVolume(QString percent);
    void                unload();

signals:
    void                finished();
public slots:
    void                play();
    void                stop();

protected:
    QString             current_media_path;
    QScopedPointer<QtAV::AVPlayer, QScopedPointerDeleteLater>  MediaDecoder;

protected slots:
    void onMediaStatusChanged(QtAV::MediaStatus status);
    void displayErrorMessage(QtAV::AVError e);

};
#endif

#endif // QTAV_H
