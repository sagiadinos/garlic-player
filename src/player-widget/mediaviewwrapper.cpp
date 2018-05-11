#include "mediaviewwrapper.h"

MediaViewWrapper::MediaViewWrapper(QWidget *parent) :QWidget(parent)
{
#ifdef SUPPORT_QTAV
    VideoWidget.reset(new QtAV::GLWidgetRenderer2(this));
#else
    VideoWidget.reset(new  QVideoWidget);
#endif
}

MediaViewWrapper::~MediaViewWrapper()
{
}

#ifdef SUPPORT_QTAV
QtAV::GLWidgetRenderer2 *MediaViewWrapper::getVideoWidget()
{
    return VideoWidget.data();
}
#else
QVideoWidget *MediaViewWrapper::getVideoWidget()
{
    return VideoWidget.data();
}
#endif


void MediaViewWrapper::setAspectRatioFill()
{
#ifdef SUPPORT_QTAV
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
#else
    VideoWidget.data()->setAspectRatioMode(Qt::IgnoreAspectRatio);
#endif
}

void MediaViewWrapper::setAspectRatioMeet()
{
#ifdef SUPPORT_QTAV
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
#else
    VideoWidget.data()->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
#endif
}

void MediaViewWrapper::setAspectRatioMeetBest()
{
#ifdef SUPPORT_QTAV
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
#else
    VideoWidget.data()->setAspectRatioMode(Qt::KeepAspectRatio);
#endif
}

