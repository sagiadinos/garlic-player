#ifdef SUPPORT_QTAV
#include "qtav_decoder.h"

QtAVDecoder::QtAVDecoder(QObject *parent) : QObject(parent)
{
    MediaDecoder.reset(new QtAV::AVPlayer(this));

#ifdef SUPPORT_RPI
    // ensure that Raspberry uses MMAL encoder
    MediaDecoder.data()->setVideoDecoderPriority(QStringList() << "MMAL" << "FFmpeg");
#endif

    MediaDecoder.data()->audio()->setBufferSamples(512); // prevent stuttering
    connect(MediaDecoder.data(), SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this, SLOT(onMediaStatusChanged(QtAV::MediaStatus)));
    connect(MediaDecoder.data(), SIGNAL(error(QtAV::AVError)), this, SLOT(displayErrorMessage(QtAV::AVError)));
}

QtAVDecoder::~QtAVDecoder()
{
    MediaDecoder.data()->clearVideoRenderers();
}

void QtAVDecoder::setVideoOutput(MediaWidgetWrapper *renderer)
{
    MediaDecoder.data()->setRenderer(renderer->getVideoRenderer());
}

void QtAVDecoder::removeVideoOutput(MediaWidgetWrapper *renderer)
{
    MediaDecoder.data()->removeVideoRenderer(renderer->getVideoRenderer());
}

bool QtAVDecoder::load(QString file_path)
{
    current_media_path = file_path;
    MediaDecoder.data()->setAsyncLoad(false);
    MediaDecoder.data()->setFile(current_media_path);
    return MediaDecoder.data()->load();
}

void QtAVDecoder::setVolume(QString percent)
{
    qreal vol = 0;
    if (percent.endsWith('%'))
        vol = (percent.mid(0, percent.length()-1).toDouble()) / 100;
    // sometimes result messed up with 0.10000000000000000001, but that should be no problem
    // a more significant problem could be, that e.g. 10% Volume is in QtMultimedia louder than in QtAV.
    MediaDecoder.data()->audio()->setVolume(vol);
}

void QtAVDecoder::unload()
{
    current_media_path = "";
    MediaDecoder.data()->setFile("");
}

void QtAVDecoder::play()
{
    MediaDecoder.data()->play();
}

void QtAVDecoder::stop()
{
    MediaDecoder.data()->stop();
}

void QtAVDecoder::onMediaStatusChanged(QtAV::MediaStatus status)
{
    switch (status)
    {
        case QtAV::UnknownMediaStatus:
            qWarning(Develop) << "Unknown media status! Media:" << current_media_path;
            break;
        case QtAV::NoMedia:
            qWarning(Develop) << "No Media " << current_media_path;
            break;
        case QtAV::LoadedMedia:
            qInfo(Develop) << "Loaded media " << current_media_path;
            break;
        case QtAV::BufferedMedia:
            qInfo(Develop) << "Buffered media " << current_media_path;
            break;
        case QtAV::BufferingMedia:
            qInfo(Develop) << "Buffering media " << current_media_path;
            break;
        case QtAV::LoadingMedia:
            qInfo(Develop) << "Loading media " << current_media_path;
            break;
        case QtAV::StalledMedia:
            qWarning(Develop) << "Stalled media " << current_media_path;
            break;
        case QtAV::EndOfMedia:
            qInfo(Develop) << "End of media " << current_media_path;
            emit finished();
            break;
        case QtAV::InvalidMedia:
            qWarning(Develop) << "invalid Media " << current_media_path;
            break;
    }
    return;
}


void QtAVDecoder::displayErrorMessage(QtAV::AVError e)
{
    qWarning(Develop) << e.string();
    return;
}

#endif
