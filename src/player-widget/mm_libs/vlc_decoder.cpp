#ifdef SUPPORT_LIBVLC

#include "vlc_decoder.h"

VlcDecoder::VlcDecoder(QObject *parent) : QObject(parent)
{
    vlcInstance = libvlc_new(0, Q_NULLPTR);
}

VlcDecoder::~VlcDecoder()
{
    libvlc_release(vlcInstance);
}

void VlcDecoder::setVideoOutput(MediaWidgetWrapper *renderer)
{
    Renderer = renderer;
}

void VlcDecoder::removeVideoOutput(MediaWidgetWrapper *renderer)
{
    Q_UNUSED(renderer);
}

bool VlcDecoder::load(QString file_path)
{
    current_media_path = file_path;
    libvlc_media_t *vlcMedia = libvlc_media_new_path(vlcInstance, current_media_path.toStdString().c_str());
    if (vlcMedia == NULL)
        return false;

    vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);

    libvlc_media_release(vlcMedia);

    if (vlcPlayer == NULL)
        return false;

    /* Integrate the video in the interface */
#if defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(vlcPlayer, (void *)Renderer->getVideoWidget()->winId());
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(vlcPlayer, Renderer->getVideoRenderer()->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(vlcPlayer, Renderer->getVideoRenderer()->winId());
#endif

    return true;
}

void VlcDecoder::setVolume(QString percent)
{
    if (vlcPlayer == NULL)
        return;

    int vol = 0;
    if (percent.endsWith('%'))
        vol = percent.mid(0, percent.length()-1).toInt();
    libvlc_audio_set_volume(vlcPlayer,vol);
}

void VlcDecoder::unload()
{
    libvlc_media_player_release(vlcPlayer);
}

void VlcDecoder::play()
{
    if (vlcPlayer == NULL)
        return;

    libvlc_media_player_play(vlcPlayer);

    /* A timer to update the sliders */
    PositionTimer.reset(new QTimer(this));
    connect(PositionTimer.data(), SIGNAL(timeout()), this, SLOT(updatePosition()));
    PositionTimer.data()->start(100);

}

void VlcDecoder::stop()
{
    if (vlcPlayer == NULL)
        return;

    libvlc_media_player_stop(vlcPlayer);
    libvlc_media_player_release(vlcPlayer);
}

void VlcDecoder::updatePosition()
{
    if (vlcPlayer == NULL)
        return;


    libvlc_state_t status = libvlc_media_player_get_state(vlcPlayer);

    switch (status)
    {
        case libvlc_Opening:
            qInfo(Develop) << "Loaded media " << current_media_path;
            break;
        case libvlc_Buffering:
            qInfo(Develop) << "Buffered media " << current_media_path;
            break;
        case libvlc_Playing:
         //   qInfo(Develop) << "Playing media " << current_media_path;
            break;
        case libvlc_Paused:
            qInfo(Develop) << "paused media " << current_media_path;
            break;
        case libvlc_Stopped:
            qWarning(Develop) << "stopped media " << current_media_path;
            PositionTimer.data()->stop();
            break;
        case libvlc_Ended:
            qInfo(Develop) << "End of media " << current_media_path;
            PositionTimer.data()->stop();
            emit finished();
            break;
        case libvlc_Error:
            qWarning(Develop) << "Error" << current_media_path;
            break;

        case libvlc_NothingSpecial:
        default:
            break;

    }
}

void VlcDecoder::displayErrorMessage()
{

}
#endif
