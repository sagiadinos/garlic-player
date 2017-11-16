#ifndef MPIMAGE_H
#define MPIMAGE_H

#include "base_media.h"
#include "smilparser/image.h"

    class Image : public BaseMedia
    {
            Q_OBJECT
        public:
            const int STRETCH            = 0;
            const int PRESERVEASPECTFIT  = 1;
            const int PRESERVEASPECTCROP = 2;

            explicit      Image(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
            ~Image();
            void          init(TMedia *media);
            void          deinit();
            void          setParentItem(QQuickItem *parent);
        protected:
            TImage                     *MyImage;
            QScopedPointer<QQuickItem>  image_item;
            int                         determineFillMode(QString smil_fit);
    };


#endif // MPIMAGE_H
