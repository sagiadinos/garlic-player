#include "audio.h"

Audio::Audio(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyAudio  = qobject_cast<TAudio *>(media);
    connect(&MediaDecoder, SIGNAL(finished()), this, SLOT(finished()));
}

Audio::~Audio()
{
    MediaDecoder.unload();
}

void Audio::init()
{
    MediaDecoder.load(MyAudio->getLoadablePath());
    MediaDecoder.play();
}


void Audio::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
}


QWidget *Audio::getView()
{
    return Q_NULLPTR;
}

void Audio::finished()
{
   MyAudio->finishedSimpleDuration();
}
