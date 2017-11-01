#include "mediadecoderwrapper.h"

MediaDecoderWrapper::MediaDecoderWrapper(QObject *parent) : QObject(parent)
{
#ifdef SUPPORT_QTAV
    MediaDecoder.reset(new QtAV::AVPlayer(this));
    MediaDecoder.data()->audio()->setBufferSamples(64); // prevent stuttering
    connect(MediaDecoder.data(), SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this, SLOT(onMediaStatusChanged(QtAV::MediaStatus)));
    connect(MediaDecoder.data(), SIGNAL(error(QtAV::AVError)), this, SLOT(displayErrorMessage(QtAV::AVError)));
#else
    MediaDecoder.reset(new QMediaPlayer(this));
    connect(MediaDecoder.data(), SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
//    connect(MediaDecoder.data(), SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));
#endif
}

MediaDecoderWrapper::~MediaDecoderWrapper()
{
#ifdef SUPPORT_QTAV
    MediaDecoder.data()->clearVideoRenderers();
#endif
}

void MediaDecoderWrapper::setVideoOutput(MediaViewWrapper *renderer)
{
#ifdef SUPPORT_QTAV
    MediaDecoder.data()->setRenderer(renderer->getVideoWidget());
#else
    MediaDecoder.data()->setVideoOutput(renderer->getVideoWidget());
#endif
}

void MediaDecoderWrapper::removeVideoOutput(MediaViewWrapper *renderer)
{
#ifdef SUPPORT_QTAV
    MediaDecoder.data()->removeVideoRenderer(renderer->getVideoWidget());
#else
    Q_UNUSED(renderer);
#endif
}


bool MediaDecoderWrapper::load(QString file_path)
{
    current_media_path = file_path;
#ifdef SUPPORT_QTAV
    MediaDecoder.data()->setAsyncLoad(false);
    MediaDecoder.data()->setFile(current_media_path);
    return MediaDecoder.data()->load();
#else
    MediaDecoder.data()->setMedia(QMediaContent(QUrl::fromLocalFile(current_media_path)));
    return (MediaDecoder.data()->mediaStatus() != QMediaPlayer::NoMedia && MediaDecoder.data()->mediaStatus() != QMediaPlayer::InvalidMedia);
#endif
}

void MediaDecoderWrapper::unload()
{
    current_media_path = "";
#ifdef SUPPORT_QTAV
    MediaDecoder.data()->setFile("");
#else
    MediaDecoder.data()->setMedia(QMediaContent());
#endif
}


#ifdef SUPPORT_QTAV
void MediaDecoderWrapper::onMediaStatusChanged(QtAV::MediaStatus status)
{
     switch (status)
    {
        case UnknownMediaStatus:
            qWarning(MediaPlayer) << "Unknown media status! Media:" << current_media_path;
            break;
        case QtAV::NoMedia:
            qWarning(MediaPlayer) << "No Media " << current_media_path;
            break;
        case QtAV::LoadedMedia:
            qInfo(MediaPlayer) << "Loaded media " << current_media_path;
            break;
        case QtAV::BufferedMedia:
            qInfo(MediaPlayer) << "Buffered media " << current_media_path;
            break;
        case QtAV::BufferingMedia:
            qInfo(MediaPlayer) << "Buffering media " << current_media_path;
            break;
        case QtAV::LoadingMedia:
            qInfo(MediaPlayer) << "Loading media " << current_media_path;
            break;
        case QtAV::StalledMedia:
            qWarning(MediaPlayer) << "Stalled media " << current_media_path;
            break;
        case QtAV::EndOfMedia:
             qInfo(MediaPlayer) << "End of media " << current_media_path;
            emit finished();
            break;
        case QtAV::InvalidMedia:
            qWarning(MediaPlayer) << "invalid Media " << current_media_path;
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
            qWarning(MediaPlayer) << "Unknown media status! Media:" << current_media_path;
            break;
        case QMediaPlayer::NoMedia:
            qWarning(MediaPlayer) << "No Media " << current_media_path;
            break;
        case QMediaPlayer::LoadedMedia:
            qInfo(MediaPlayer) << "Loaded media " << current_media_path;
            break;
        case QMediaPlayer::BufferedMedia:
            qInfo(MediaPlayer) << "Buffered media " << current_media_path;
            break;
        case QMediaPlayer::BufferingMedia:
            qInfo(MediaPlayer) << "Buffering media " << current_media_path;
            break;
        case QMediaPlayer::LoadingMedia:
            qInfo(MediaPlayer) << "Loading media " << current_media_path;
            break;
        case QMediaPlayer::StalledMedia:
            qWarning(MediaPlayer) << "Stalled media " << current_media_path;
            break;
        case QMediaPlayer::EndOfMedia:
            qInfo(MediaPlayer) << "End of media " << current_media_path;
            emit finished();
            break;
        case QMediaPlayer::InvalidMedia:
            qWarning(MediaPlayer) << "invalid Media " << current_media_path;
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
    MediaDecoder.data()->play();
}

void MediaDecoderWrapper::stop()
{
    MediaDecoder.data()->stop();
}
