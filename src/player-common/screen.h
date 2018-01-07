#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QMainWindow>
#include <QDesktopWidget>
#include "tools/logging_categories.h"

class TScreen : public QObject
{
    Q_OBJECT
public:
    explicit TScreen(QDesktopWidget *dw, QObject *parent = Q_NULLPTR);
    QPoint   getWholeStartPoint();
    QSize    getWholeSize();
    QPoint   getStartPointFromCurrentScreen();
    QSize    getSizeFromCurrentScreen();
    void     determineCurrentScreenId(const QWidget *value);
    void     setScreenId(const int id);
    QRect    getQRectFromScreen();
public slots:

protected:
    QDesktopWidget *desktop_widget;
    QRect           whole_display_geometry;
    int             current_screen_id = 0;
    int             getCurrentScreenId() const;
    void            calculateWholeDisplayGeometrie();
    int             validateScreenId(int screen_id);
};

#endif // SCREEN_H
