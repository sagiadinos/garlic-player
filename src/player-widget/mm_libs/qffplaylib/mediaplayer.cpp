#include "mediaplayer.h"

QtFFmpegPlayer::MediaPlayer::MediaPlayer(QObject *parent)
    : QObject{parent}
{
    MyDecoder.init();
    connect(&MyDecoder, SIGNAL(endOfFile()), this, SLOT(doEndOfFile()));
}

QtFFmpegPlayer::MediaPlayer::~MediaPlayer()
{
    MyDecoder.deinit();
}

void QtFFmpegPlayer::MediaPlayer::setRenderer(VideoOut *r)
{
    MyDecoder.setRenderer(r);
}

bool QtFFmpegPlayer::MediaPlayer::load(QString file_path)
{
 //   stop();
    emit mediaStatusChanged(LoadingMedia);

    if (!MyDecoder.load(file_path) || !MyDecoder.findStreams() || !MyDecoder.prepareDecode())
    {
        MyDecoder.deinit();
        emit mediaStatusChanged(InvalidMedia);
        return false;
    }
    emit mediaStatusChanged(LoadedMedia);
    return true;
}

void QtFFmpegPlayer::MediaPlayer::play()
{
    MyDecoder.start();
    emit mediaStatusChanged(BufferedMedia);
}

void QtFFmpegPlayer::MediaPlayer::stop()
{
    MyDecoder.terminate();
    MyDecoder.deinit();
}

void QtFFmpegPlayer::MediaPlayer::pause()
{
    MyDecoder.terminate();
}

void QtFFmpegPlayer::MediaPlayer::resume()
{
    MyDecoder.start();
}

void QtFFmpegPlayer::MediaPlayer::seek(quint64 position)
{
    Q_UNUSED(position);
}

QString QtFFmpegPlayer::MediaPlayer::getErrorText()
{
    return MyDecoder.getErrorText();
}

void QtFFmpegPlayer::MediaPlayer::doEndOfFile()
{
   stop();
   emit mediaStatusChanged(EndOfMedia);
}

