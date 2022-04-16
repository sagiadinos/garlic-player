#include "brush.h"

PlayerBrush::PlayerBrush(QObject *parent) : PlayerBaseMedia(parent)
{
    BrushWidget.reset(new QLabel);
}

PlayerBrush::~PlayerBrush()
{

}

void PlayerBrush::init(BaseMedia *media, Region *reg)
{
    SmilMedia = media; // other wise it cannote stopped in  TRegion::stopShowMedia
    region    = reg;
    MyBrush   = qobject_cast<TBrush *> (media);
    BrushWidget.data()->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    BrushWidget.data()->setAutoFillBackground(true);
    BrushWidget.data()->setStyleSheet("background-color:" + MyBrush->getColor());
    if (MyBrush->getLogContentId() != "")
        setStartTime();
}

void PlayerBrush::deinit()
{
    BrushWidget.data()->setStyleSheet("background-color:green");
    if (MyBrush->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void PlayerBrush::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
}

QWidget *PlayerBrush::getView()
{
    return BrushWidget.data();
}
