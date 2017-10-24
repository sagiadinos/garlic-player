#ifndef MEDIADECODERWRAPPER_H
#define MEDIADECODERWRAPPER_H

#ifdef SUPPORT_QTAV
    #include <QtAV>
    using namespace QtAV;
#else
    #include <qmediaplayer.h>
    #include <QtGui/QMovie>
#endif

#include <QObject>
#include "mediaviewwrapper.h"
#include "logging_categories.h"

class MediaDecoderWrapper : public QObject
{
    Q_OBJECT
public:
    explicit MediaDecoderWrapper(QObject *parent = Q_NULLPTR);
    ~MediaDecoderWrapper();
    void                setVideoOutput(MediaViewWrapper *renderer);
    void                removeVideoOutput(MediaViewWrapper *renderer);
    bool                load(QString file_path);
    void                unload();

signals:
    void                finished();
public slots:
    void                play();

protected:
    QString             actual_media_path;
#ifdef SUPPORT_QTAV
   QScopedPointer<QtAV::AVPlayer>  MediaDecoder;
#else
    QScopedPointer<QMediaPlayer>   MediaDecoder;
#endif

protected slots:
#ifdef SUPPORT_QTAV
    void onMediaStatusChanged(QtAV::MediaStatus status);
    void displayErrorMessage(const AVError &e);
#else
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void displayErrorMessage();
#endif


};

#endif // MEDIADECODERWRAPPER_H
