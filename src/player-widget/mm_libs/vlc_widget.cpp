#ifdef SUPPORT_LIBVLC
#include "vlc_widget.h"

VlcWidget::VlcWidget(QWidget *parent) : QWidget(parent)
{
    VideoWidget.reset(new QWidget);
}

QWidget *VlcWidget::getVideoWidget()
{
    return VideoWidget.data();
}

QWidget *VlcWidget::getVideoRenderer()
{
    return VideoWidget.data();
}

void VlcWidget::ignoreAspectRatio()
{

}

void VlcWidget::keepAspectRatio()
{

}

void VlcWidget::keepAspectRatioByExpanding()
{

}
#endif
