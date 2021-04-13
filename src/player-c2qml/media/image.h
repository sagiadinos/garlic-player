#ifndef MPIMAGE_H
#define MPIMAGE_H

#include "player_base_media.h"
#include "smilparser/media/image.h"
#include <QQuickImageProvider>
#include <QQuickImageResponse>
class Image : public PlayerBaseMedia
{
        Q_OBJECT
    public:
        const int STRETCH            = 0;
        const int PRESERVEASPECTFIT  = 1;
        const int PRESERVEASPECTCROP = 2;
        const int TILE               = 4;
        const int TILE_V             = 5;
        const int NONE               = 6;

        explicit      Image(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent = nullptr);
        ~Image();
        void          init(BaseMedia *media);
        void          deinit();
        void          setParentItem(QQuickItem *parent);
    protected:
        QScopedPointer<QQuickItem>  image_item;
        int                         determineFillMode(QString smil_fit);

};


#endif // MPIMAGE_H
