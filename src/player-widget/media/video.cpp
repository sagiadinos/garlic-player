#include "video.h"

Video::Video(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyVideo  = qobject_cast<TVideo *>(media);
    connect(&MediaDecoder, SIGNAL(finished()), this, SLOT(finished()));
}

Video::~Video()
{
    MediaDecoder.unload();
    delete VideoWidget;
}

void Video::init()
{
   VideoWidget    = new MediaViewWrapper;
   MediaDecoder.setVideoOutput(VideoWidget);
   MediaDecoder.load(MyVideo->getLoadablePath());
   MediaDecoder.play();
}

void Video::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
    if (MyVideo->getFit() == "fill")
        VideoWidget->setAspectRatioFill();
    else if (MyVideo->getFit() == "meet")
        VideoWidget->setAspectRatioMeet();
    else if (MyVideo->getFit() == "meetbest")
        VideoWidget->setAspectRatioMeetBest();
}

QWidget *Video::getView()
{
    return VideoWidget->getVideoWidget();
}

void Video::finished()
{
   MyVideo->finishedSimpleDuration();
}
