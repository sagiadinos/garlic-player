#ifdef SUPPORT_QTAV

#include "qtav_widget.h"
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

void QtAVWidget::ignoreAspectRatio()
{
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
}

void QtAVWidget::keepAspectRatio()
{
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
}

void QtAVWidget::keepAspectRatioByExpanding()
{
    VideoWidget.data()->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
}
#endif
