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

void QtMMWidget::setAspectRatioFill()
{
    VideoWidget.data()->setAspectRatioMode(Qt::IgnoreAspectRatio);
}

void QtMMWidget::setAspectRatioMeet()
{
    VideoWidget.data()->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

void QtMMWidget::setAspectRatioMeetBest()
{
    VideoWidget.data()->setAspectRatioMode(Qt::KeepAspectRatio);
}
#endif
