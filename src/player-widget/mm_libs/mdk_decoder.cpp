#ifdef SUPPORT_MDK

#include "mdk_decoder.h"

MdkDecoder::MdkDecoder(QObject *parent) : QObject(parent)
{
    MediaDecoder.reset(new QMDKPlayer(this));
    MediaDecoder->setDecoders({"VideoToolbox", "VAAPI", "D3D11", "DXVA2", "MMAL", "MediaCodec", "FFmpeg"});

   // connect(MediaDecoder.data(), SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

void MdkDecoder::setVideoOutput(MediaWidgetWrapper *renderer)
{

}

void MdkDecoder::removeVideoOutput(MediaWidgetWrapper *renderer)
{

}

bool MdkDecoder::load(QString file_path)
{
    MediaDecoder.data()->setMedia(file_path.toUtf8().constData());
}

void MdkDecoder::setVolume(QString percent)
{
    float vol = 0;
    if (percent.endsWith('%'))
        vol = (percent.mid(0, percent.length()-1).toFloat()) / 100;
    MediaDecoder.data()->setVolume(vol);

}

void MdkDecoder::play()
{
    MediaDecoder.data()->play();

    /* A timer to update the sliders, position or get end of the vid */
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePosition()));
    timer->start(100);

}

void MdkDecoder::stop()
{
    MediaDecoder.data()->stop();
}


void MdkDecoder::updatePosition()
{
    if (libvlc_media_player_get_state(vlcPlayer) == libvlc_Ended)
        emit finished();

}

void MdkDecoder::displayErrorMessage()
{

}

#endif
