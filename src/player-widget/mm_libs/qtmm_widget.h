#ifdef SUPPORT_QTMM
#ifndef QTMM_WIDGET_H
#define QTMM_WIDGET_H

#include <QWidget>
#include <QVideoWidget>

class QtMMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QtMMWidget(QWidget *parent = Q_NULLPTR);
    QWidget              *getVideoWidget();
    QVideoWidget         *getVideoRenderer();

    void                  setAspectRatioFill();
    void                  setAspectRatioMeet();
    void                  setAspectRatioMeetBest();
signals:
protected:
    QScopedPointer<QVideoWidget>         VideoWidget;
public slots:
};
#endif

#endif // QTMM_WIDGET_H
