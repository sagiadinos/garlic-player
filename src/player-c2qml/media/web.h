#ifndef WEB_H
#define WEB_H

#include "smilparser/media/web.h"
#include "player_base_media.h"

class Web : public PlayerBaseMedia
{
    Q_OBJECT
public:
        Web(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent = nullptr);
        ~Web();
        void          loadMedia(BaseMedia *media, Region *reg);
        void          play();
        void          stop();
        void          resume();
        void          pause();
        void          setParentItem(QQuickItem *parent);
        void          changeSize(int w, int h);
    protected:
        QScopedPointer<QQuickItem, QScopedPointerDeleteLater>   web_item;
        QQmlComponent                *media_component;
    private:
        QString sanitizeUri(QString uri);
};

#endif // WEB_H
