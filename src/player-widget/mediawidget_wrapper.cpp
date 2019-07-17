#include "mediawidget_wrapper.h"
#include "mediawidget_wrapper.h"

MediaWidgetWrapper::MediaWidgetWrapper(QWidget *parent) :QWidget(parent)
{
#if defined SUPPORT_QTAV
#elif defined SUPPORT_LIBVLC
#else
#endif
}

MediaWidgetWrapper::~MediaWidgetWrapper()
{
}

QWidget *MediaWidgetWrapper::getVideoWidget()
{
   return VideoWidget.getVideoWidget();
}

void MediaWidgetWrapper::setAspectRatioFill()
{
    VideoWidget.setAspectRatioFill();
}

void MediaWidgetWrapper::setAspectRatioMeet()
{
    VideoWidget.setAspectRatioMeet();
}

void MediaWidgetWrapper::setAspectRatioMeetBest()
{
    VideoWidget.setAspectRatioMeetBest();
}

