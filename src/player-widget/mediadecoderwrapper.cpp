#include "mediadecoderwrapper.h"

MediaDecoderWrapper::MediaDecoderWrapper(QObject *parent) : QObject(parent)
{
#ifdef SUPPORT_QTAV
    MediaDecoder = new QtAV::AVPlayer(this);
   connect(MediaDecoder, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this, SLOT(onMediaStatusChanged(QtAV::MediaStatus)));
//    connect(MediaDecoder, SIGNAL(error(QtAV::AVError)), this, SLOT(displayErrorMessage(QtAV::AVError)));
#else
    MediaDecoder = new QMediaPlayer(this);
    connect(MediaDecoder, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
//    connect(MediaDecoder, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));
#endif
}

MediaDecoderWrapper::~MediaDecoderWrapper()
{
#ifdef SUPPORT_QTAV
    MediaDecoder->clearVideoRenderers();
#endif
    delete MediaDecoder;
}

void MediaDecoderWrapper::setVideoOutput(MediaViewWrapper *renderer)
{
#ifdef SUPPORT_QTAV
    MediaDecoder->setRenderer(renderer->getVideoWidget());
#else
    MediaDecoder->setVideoOutput(renderer->getVideoWidget());
#endif
}

void MediaDecoderWrapper::removeVideoOutput(MediaViewWrapper *renderer)
{
#ifdef SUPPORT_QTAV
    MediaDecoder->removeVideoRenderer(renderer->getVideoWidget());
#else
    Q_UNUSED(renderer);
#endif
}


bool MediaDecoderWrapper::load(QString file_path)
{
    actual_media_path = file_path;
#ifdef SUPPORT_QTAV
    MediaDecoder->setAsyncLoad(false);
    MediaDecoder->setFile(actual_media_path);
    return MediaDecoder->load();
#else
    MediaDecoder->setMedia(QMediaContent(QUrl::fromLocalFile(actual_media_path)));
    return (MediaDecoder->mediaStatus() != QMediaPlayer::NoMedia && MediaDecoder->mediaStatus() != QMediaPlayer::InvalidMedia);
#endif
}

void MediaDecoderWrapper::unload()
{
    actual_media_path = "";
#ifdef SUPPORT_QTAV
    MediaDecoder->setFile("");
#else
    MediaDecoder->setMedia(QMediaContent());
#endif
}


#ifdef SUPPORT_QTAV
void MediaDecoderWrapper::onMediaStatusChanged(QtAV::MediaStatus status)
{
     switch (status)
    {
        case UnknownMediaStatus:
            qWarning(MediaPlayer) << "Unknown media status! Media:" << actual_media_path;
            break;
        case QtAV::NoMedia:
            qWarning(MediaPlayer) << "No Media " << actual_media_path;
            break;
        case QtAV::LoadedMedia:
            qInfo(MediaPlayer) << "Loaded media " << actual_media_path;
            break;
        case QtAV::BufferedMedia:
            qInfo(MediaPlayer) << "Buffered media " << actual_media_path;
            break;
        case QtAV::BufferingMedia:
            qInfo(MediaPlayer) << "Buffering media " << actual_media_path;
            break;
        case QtAV::LoadingMedia:
            qInfo(MediaPlayer) << "Loading media " << actual_media_path;
            break;
        case QtAV::StalledMedia:
            qWarning(MediaPlayer) << "Stalled media " << actual_media_path;
            break;
        case QtAV::EndOfMedia:
            emit finished();
            break;
        case QtAV::InvalidMedia:
            qWarning(MediaPlayer) << "invalid Media " << actual_media_path;
            break;
    }
}

void MediaDecoderWrapper::displayErrorMessage(const AVError &e)
{
    qWarning(MediaPlayer) << e.string();
    return;
}

#else

void MediaDecoderWrapper::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status)
    {
        case QMediaPlayer::UnknownMediaStatus:
            qWarning(MediaPlayer) << "Unknown media status! Media:" << actual_media_path;
            break;
        case QMediaPlayer::NoMedia:
            qWarning(MediaPlayer) << "No Media " << actual_media_path;
            break;
        case QMediaPlayer::LoadedMedia:
            qInfo(MediaPlayer) << "Loaded media " << actual_media_path;
            break;
        case QMediaPlayer::BufferedMedia:
            qInfo(MediaPlayer) << "Buffered media " << actual_media_path;
            break;
        case QMediaPlayer::BufferingMedia:
            qInfo(MediaPlayer) << "Buffering media " << actual_media_path;
            break;
        case QMediaPlayer::LoadingMedia:
            qInfo(MediaPlayer) << "Loading media " << actual_media_path;
            break;
        case QMediaPlayer::StalledMedia:
            qWarning(MediaPlayer) << "Stalled media " << actual_media_path;
            break;
        case QMediaPlayer::EndOfMedia:
            qInfo(MediaPlayer) << "End of media " << actual_media_path;
            emit finished();
            break;
        case QMediaPlayer::InvalidMedia:
            qWarning(MediaPlayer) << "invalid Media " << actual_media_path;
            break;
    }
}

void MediaDecoderWrapper::displayErrorMessage()
{
   qWarning(MediaPlayer) << MediaDecoder->errorString();
}

#endif


void MediaDecoderWrapper::play()
{
    MediaDecoder->play();
}
