#ifndef MPIMAGE_H
#define MPIMAGE_H

#include "base_media.h"
#include "smilparser/image.h"

    class Image : public BaseMedia
    {
            Q_OBJECT
        public:
            explicit      Image(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
            ~Image();
            void          init(TMedia *media);
            void          deinit();
            void          setParentItem(QQuickItem *parent);
        protected:
            TImage     *MyImage;
            QScopedPointer<QQuickItem>  image_item;
    };


#endif // MPIMAGE_H
