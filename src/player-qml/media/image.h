#ifndef MPIMAGE_H
#define MPIMAGE_H

#include "base_media.h"
#include "smilparser/image.h"

    class Image : public BaseMedia
    {
            Q_OBJECT
        public:
            explicit      Image(TMedia *media, QObject *parent = nullptr);
            ~Image();
            void          init(QQmlComponent *mc);
            void          setParentItem(QQuickItem *parent);
        protected:
            TImage        *ParserImage;
            QQuickItem    *image_item = Q_NULLPTR;
    };


#endif // MPIMAGE_H
