#ifdef SUPPORT_QTMM
#include "qtmm_widget.h"

QtMMWidget::QtMMWidget(QWidget *parent) :QWidget(parent)
{
    VideoWidget = new QVideoWidget;
}

QtMMWidget::~QtMMWidget()
{
    delete VideoWidget;
}

QWidget  *QtMMWidget::getVideoWidget()
{
    return VideoWidget;
}

QVideoWidget *QtMMWidget::getVideoRenderer()
{
    return VideoWidget;
}

void QtMMWidget::ignoreAspectRatio()
{
    VideoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
}

void QtMMWidget::keepAspectRatio()
{
    VideoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
}

void QtMMWidget::keepAspectRatioByExpanding()
{
    VideoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

#endif
