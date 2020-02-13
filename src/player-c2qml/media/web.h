#ifndef WEB_H
#define WEB_H

#include "smilparser/media/web.h"
#include "player_base_media.h"

class Web : public PlayerBaseMedia
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
    private:
        QString sanitizeUri(QString uri);
};

#endif // WEB_H
