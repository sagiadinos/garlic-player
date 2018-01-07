#include "screen.h"

TScreen::TScreen(QDesktopWidget *dw, QObject *parent) : QObject(parent)
{
     desktop_widget   = dw;
     current_screen_id = desktop_widget->primaryScreen();
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

QPoint TScreen::getStartPointFromCurrentScreen()
{
    return desktop_widget->screenGeometry(getCurrentScreenId()).topLeft();
}

QRect TScreen::getQRectFromScreen()
{
    return desktop_widget->screenGeometry(getCurrentScreenId());
}


QSize TScreen::getSizeFromCurrentScreen()
{
    QRect rect = desktop_widget->screenGeometry(getCurrentScreenId());
    return rect.size();
}

int TScreen::getCurrentScreenId() const
{
    return current_screen_id;
}

void TScreen::determineCurrentScreenId(const QWidget *value)
{
    current_screen_id = desktop_widget->screenNumber(value);
}

void TScreen::setScreenId(const int id)
{
    current_screen_id = validateScreenId(id);
}

void TScreen::calculateWholeDisplayGeometrie()
{
    for (int i = 0; i < desktop_widget->screenCount(); i++)
    {
        qInfo(Screen) << "Monitor: " << i << " Geometry:" << desktop_widget->screenGeometry(i);
        QRect screenRect = desktop_widget->screen(i)->geometry();
        whole_display_geometry = whole_display_geometry.united(screenRect); //union
    }
    qInfo(Develop) << " DisplayGeometrie x:" << whole_display_geometry.x() << "y:" << whole_display_geometry.y() << "width:" << whole_display_geometry.width() << "height:" << whole_display_geometry.height();
}

int TScreen::validateScreenId(int screen_id)
{
    if (screen_id > -1 && screen_id < desktop_widget->screenCount())
        return  screen_id;

    return desktop_widget->primaryScreen();
}
