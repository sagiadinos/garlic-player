#include "web.h"

Web::Web(QQmlComponent *mc, QString r_id, QObject *parent) : BaseMedia(mc, r_id, parent)
{
    setRegionId(r_id);
    QString str("import QtQuick 2.7\n \
                 import QtWebView 1.1\n \
                    WebView {\n \
                        id: "+getRegionId()+"_web;\n \
                        anchors.fill: parent;\n \
                   }\n"
    );
    web_item.reset(createMediaItem(mc, str));
}

Web::~Web()
{
    web_item.reset();
}

void Web::init(TMedia *media)
{
    MyMedia = media;
    web_item.data()->setProperty("url", MyMedia->getLoadablePath());
}

void Web::deinit()
{
    web_item.data()->setProperty("url", "");
}

void Web::setParentItem(QQuickItem *parent)
{
    web_item.data()->setParentItem(parent);
}
