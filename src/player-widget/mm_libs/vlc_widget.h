#ifndef VLC_WIDGET_H
#define VLC_WIDGET_H

#include <QWidget>

class VlcWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VlcWidget(QWidget *parent = nullptr);
    QWidget              *getVideoWidget();
    QWidget              *getVideoRenderer();

    void                  setAspectRatioFill();
    void                  setAspectRatioMeet();
    void                  setAspectRatioMeetBest();

signals:
protected:
    QScopedPointer<QWidget> VideoWidget;
public slots:
};

#endif // VLC_WIDGET_H
