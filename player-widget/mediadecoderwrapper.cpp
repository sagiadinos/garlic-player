#include "mediadecoderwrapper.h"

MediaDecoderWrapper::MediaDecoderWrapper()
{
#ifdef SUPPORT_QTAV
   connect(&MediaDecoder, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this, SLOT(onMediaStatusChanged(QtAV::MediaStatus)));
//    connect(&MediaDecoder, SIGNAL(error(QtAV::AVError)), this, SLOT(displayErrorMessage(QtAV::AVError)));
#else
    connect(&MediaDecoder, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(&MediaDecoder, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));
#endif
}

MediaDecoderWrapper::~MediaDecoderWrapper()
{
#ifdef SUPPORT_QTAV
    MediaDecoder.clearVideoRenderers();
#endif
    qDebug() << "wat nen schiet";
}

void MediaDecoderWrapper::setVideoOutput(MediaViewWrapper *renderer)
{
#ifdef SUPPORT_QTAV
    MediaDecoder.setRenderer(renderer->getVideoWidget());
#else
    MediaDecoder.setVideoOutput(renderer->getVideoWidget());
#endif
}

void MediaDecoderWrapper::removeVideoOutput(MediaViewWrapper *renderer)
{
#ifdef SUPPORT_QTAV
    MediaDecoder.removeVideoRenderer(renderer->getVideoWidget());
#endif
}


bool MediaDecoderWrapper::load(QString file_path)
{
#ifdef SUPPORT_QTAV
    MediaDecoder.setAsyncLoad(false);
    MediaDecoder.setFile(file_path);
    return MediaDecoder.load();
#else
    MediaDecoder.setMedia(QUrl::fromLocalFile(file_path));
    return (MediaDecoder.mediaStatus() != QMediaPlayer::NoMedia && MediaDecoder.mediaStatus() != QMediaPlayer::InvalidMedia);
#endif

}

#ifdef SUPPORT_QTAV
void MediaDecoderWrapper::onMediaStatusChanged(QtAV::MediaStatus status)
{
    switch (status)
    {
        case UnknownMediaStatus:
            break;
        case QtAV::NoMedia:
            break;
        case QtAV::LoadedMedia:
            break;
        case QtAV::BufferedMedia:
            break;
        case QtAV::BufferingMedia:
            break;
        case QtAV::LoadingMedia:
            break;
        case QtAV::StalledMedia:
            break;
        case QtAV::EndOfMedia:
            emit finished();
            break;
        case QtAV::InvalidMedia:
            break;
    }
}

void MediaDecoderWrapper::displayErrorMessage(const AVError &e)
{
    qDebug() << e.string();
    return;
}

#else

void MediaDecoderWrapper::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status)
    {
        case QMediaPlayer::UnknownMediaStatus:
            break;
        case QMediaPlayer::NoMedia:
            break;
        case QMediaPlayer::LoadedMedia:
            break;
        case QMediaPlayer::BufferedMedia:
            break;
        case QMediaPlayer::BufferingMedia:
            break;
        case QMediaPlayer::LoadingMedia:
            break;
        case QMediaPlayer::StalledMedia:
            break;
        case QMediaPlayer::EndOfMedia:
            emit finished();
            break;
        case QMediaPlayer::InvalidMedia:
            break;
    }
}

void MediaDecoderWrapper::displayErrorMessage()
{
    qDebug() << MediaDecoder.errorString();
}

#endif


void MediaDecoderWrapper::play()
{
    MediaDecoder.play();
}

void MediaDecoderWrapper::pause()
{
    MediaDecoder.pause();
}

void MediaDecoderWrapper::stop()
{
    MediaDecoder.stop();
}
