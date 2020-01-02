#ifndef MPIMAGE_H
#define MPIMAGE_H

#include "base_media.h"
#include "smilparser/media/image.h"
#include <QQuickImageProvider>
#include <QQuickImageResponse>
class Image : public BaseMedia
{
        Q_OBJECT
    public:
        const int STRETCH            = 0;
        const int PRESERVEASPECTFIT  = 1;
        const int PRESERVEASPECTCROP = 2;

        explicit      Image(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
        ~Image();
        void          init(BaseMedia *media);
        void          deinit();
        void          setParentItem(QQuickItem *parent);
    protected:
        QScopedPointer<QQuickItem>  image_item;
        int                         determineFillMode(QString smil_fit);
   protected slots:
        void doStateChanged(QString state);
};


#endif // MPIMAGE_H
