#ifndef WEB_H
#define WEB_H

#include "smilparser/media/web.h"
#include "base_media.h"

class Web : public BaseMedia
{
        Q_OBJECT
    public:
        Web(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
        ~Web();
        void          init(BaseMedia *media);
        void          deinit();
        void          setParentItem(QQuickItem *parent);
    protected:
        QScopedPointer<QQuickItem>   web_item;
        QQmlComponent                *media_component;
};

#endif // WEB_H
