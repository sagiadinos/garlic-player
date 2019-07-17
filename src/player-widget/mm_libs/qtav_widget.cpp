#include "qtav_widget.h"
#ifdef SUPPORT_QTAV
QtAVWidget::QtAVWidget(QWidget *parent) :QWidget(parent)
{
    VideoWidget.reset(new QtAV::VideoOutput(this));
}

QWidget *QtAVWidget::getVideoWidget()
{
    return VideoWidget.data()->widget();
}

QtAV::VideoRenderer *QtAVWidget::getVideoRenderer()
{
    return VideoWidget.data();
}

void QtAVWidget::setAspectRatioFill()
{
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
}

void QtAVWidget::setAspectRatioMeet()
{
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
}

void QtAVWidget::setAspectRatioMeetBest()
{
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
}
#endif
