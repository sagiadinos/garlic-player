#include "brush.h"

Brush::Brush(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent) : PlayerBaseMedia(mc, r_id, lc, parent)
{
    setRegionId(r_id);
    QString str(
                "import QtQuick 2.12\n \
                    Rectangle {  \
                        id: "+getRegionId()+"_image; \
                        anchors.fill: parent; \n \
                }\n"
    );
    brush_item.reset(createMediaItem(mc, str));

}

Brush::~Brush()
{
    brush_item.reset();
}

void Brush::init(BaseMedia *media)
{
    SmilMedia = media;
    MyBrush = qobject_cast<TBrush *> (media);
    brush_item.data()->setProperty("color", MyBrush->getColor());
    if (SmilMedia->getLogContentId() != "")
        setStartTime();
}

void Brush::deinit()
{
    if (!SmilMedia->getLogContentId().isEmpty())
        qInfo(PlayLog).noquote() << createPlayLogXml();
    brush_item.data()->setProperty("source","");
}

void Brush::setParentItem(QQuickItem *parent)
{
    brush_item.data()->setParentItem(parent);
}
