#include "screen.h"

TScreen::TScreen(QDesktopWidget *dw, QObject *parent) : QObject(parent)
{
     desktop_widget   = dw;
     setActualScreenId(desktop_widget->primaryScreen());
     calculateWholeDisplayGeometrie();
}

QPoint TScreen::getWholeStartPoint()
{
    return QPoint(whole_display_geometry.x(), whole_display_geometry.y());

}

QSize TScreen::getWholeSize()
{
    return QSize(whole_display_geometry.width(), whole_display_geometry.height());
}

QPoint TScreen::getStartPointFromScreen()
{
    QRect rect = desktop_widget->screenGeometry(getActualScreenId());
    return rect.topLeft();
}

QRect TScreen::getQRectFromScreen()
{
    return desktop_widget->screenGeometry(getActualScreenId());
}


QSize TScreen::getSizeFromScreen()
{
    QRect rect = desktop_widget->screenGeometry(getActualScreenId());
    return rect.size();
}

int TScreen::getActualScreenId() const
{
    return actual_screen_id;
}

void TScreen::setActualScreenId(int value)
{
    validateScreenId(value);
}

void TScreen::calculateWholeDisplayGeometrie()
{
    for (int i = 0; i < desktop_widget->screenCount(); i++)
    {
        qInfo(Screen) << "Monitor: " << i << " Geometry:" << desktop_widget->screenGeometry(i);
        QRect screenRect = desktop_widget->screen(i)->geometry();
        whole_display_geometry = whole_display_geometry.united(screenRect); //union
    }
    qInfo(Screen) << " DisplayGeometrie x:" << whole_display_geometry.x() << "y:" << whole_display_geometry.y() << "width:" << whole_display_geometry.width() << "height:" << whole_display_geometry.height();
}

void TScreen::validateScreenId(int screen_id)
{
    if (screen_id > -1 && screen_id < desktop_widget->screenCount())
        actual_screen_id = screen_id;
    else
        actual_screen_id = -1;
}
