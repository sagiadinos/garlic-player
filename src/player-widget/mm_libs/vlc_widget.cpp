#ifdef SUPPORT_LIBVLC
#include "vlc_widget.h"

VlcWidget::VlcWidget(QWidget *parent) : QWidget(parent)
{
}

QWidget *VlcWidget::getVideoWidget()
{
    return &VideoWidget;
}

QWidget *VlcWidget::getVideoRenderer()
{
    return getVideoWidget();
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
