#include "media/image.h"

Image::Image(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    // should stay a raw pointer cause it belongs to Parser
    MyImage = qobject_cast<TImage *>(media);
}

Image::~Image()
{
}

void Image::init(QQmlComponent *mc)
{
    QString fill_mode = determineQmlFillMode(MyImage->getFit());
    QString anchors   = "";
    if (fill_mode != "Pad")
        anchors = "anchors.fill: parent;\n";
    QString str(
                "import QtQuick 2.7\n \
                    Image {\n " +
                            anchors+
                        "fillMode: Image."+fill_mode+";\n \
                        source: \"file:"+MyImage->getLoadablePath()+"\";\n \
                    }\n"
    );
    image_item.reset(createMediaItem(mc, str));
}

void Image::setParentItem(QQuickItem *parent)
{
    image_item.data()->setParentItem(parent);
}
