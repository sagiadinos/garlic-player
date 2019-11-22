#ifdef SUPPORT_LIBVLC

#include "vlc_decoder.h"

VlcDecoder::VlcDecoder(QObject *parent) : QObject(parent)
{
    vlcInstance = libvlc_new(0, Q_NULLPTR);
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
    if (vlcMedia == nullptr)
        return false;

    vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);
    libvlc_media_release(vlcMedia);

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
    int vol = 0;
    if (percent.endsWith('%'))
        vol = percent.mid(0, percent.length()-1).toInt();
    libvlc_audio_set_volume(vlcPlayer,vol);
}

void VlcDecoder::unload()
{
}

void VlcDecoder::play()
{
    libvlc_media_player_play(vlcPlayer);

    /* A timer to update the sliders */
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePosition()));
    timer->start(100);

}

void VlcDecoder::stop()
{
    libvlc_media_player_stop(vlcPlayer);
    libvlc_media_player_release(vlcPlayer);

}

void VlcDecoder::updatePosition()
{
    if (vlcPlayer == nullptr)
        return;

    if (libvlc_media_player_get_state(vlcPlayer) == libvlc_Ended)
        emit finished();

}

void VlcDecoder::displayErrorMessage()
{

}
#endif
