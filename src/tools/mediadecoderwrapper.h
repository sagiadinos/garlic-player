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

class MediaDecoderWrapper : public QObject
{
    Q_OBJECT
public:
    explicit MediaDecoderWrapper();
    ~MediaDecoderWrapper();
    void                setVideoOutput(MediaViewWrapper *renderer);
    bool                load(QString file_path);

signals:
    void                finished();
public slots:
    void                play();
    void                pause();
    void                stop();

protected:
#ifdef SUPPORT_QTAV
   QtAV::AVPlayer      MediaDecoder;
#else
    QMediaPlayer       MediaDecoder;
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
