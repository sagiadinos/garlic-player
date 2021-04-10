#ifdef SUPPORT_QTMM
#include "qtmm_widget.h"

QtMMWidget::QtMMWidget(QWidget *parent) :QWidget(parent)
{
    VideoWidget.reset(new QVideoWidget);
}

QWidget  *QtMMWidget::getVideoWidget()
{
    return VideoWidget.data();
}

QVideoWidget *QtMMWidget::getVideoRenderer()
{
    return VideoWidget.data();
}

void QtMMWidget::ignoreAspectRatio()
{
    VideoWidget.data()->setAspectRatioMode(Qt::IgnoreAspectRatio);
}

void QtMMWidget::keepAspectRatio()
{
    VideoWidget.data()->setAspectRatioMode(Qt::KeepAspectRatio);
}

void QtMMWidget::keepAspectRatioByExpanding()
{
    VideoWidget.data()->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

#endif
