#include "mediaviewwrapper.h"

MediaViewWrapper::MediaViewWrapper()
{
}

MediaViewWrapper::~MediaViewWrapper()
{
}

#ifdef SUPPORT_QTAV
QtAV::WidgetRenderer *MediaViewWrapper::getVideoWidget()
{
    return &VideoWidget;
}
#else
QVideoWidget *MediaViewWrapper::getVideoWidget()
{
    return &VideoWidget;
}
#endif


void MediaViewWrapper::setAspectRatioFill()
{
#ifdef SUPPORT_QTAV
    VideoWidget.setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
#else
    VideoWidget.setAspectRatioMode(Qt::IgnoreAspectRatio);
#endif
}

void MediaViewWrapper::setAspectRatioMeet()
{
#ifdef SUPPORT_QTAV
    VideoWidget.setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
#else
    VideoWidget.setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
#endif
}

void MediaViewWrapper::setAspectRatioMeetBest()
{
#ifdef SUPPORT_QTAV
    VideoWidget.setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
#else
    VideoWidget.setAspectRatioMode(Qt::KeepAspectRatio);
#endif
}

void MediaViewWrapper::setParentWidget(QWidget *parent)
{
    VideoWidget.setParent(parent);
}
