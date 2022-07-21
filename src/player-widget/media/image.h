#ifndef MPIMAGE_H
#define MPIMAGE_H

#include <QLabel>
#include <QImageReader>
#include <QPixmap>
#include "player_base_media.h"
#include "smilparser/media/image.h"

    class PlayerImage : public PlayerBaseMedia
    {
            Q_OBJECT
        public:            explicit      PlayerImage(QObject *parent = nullptr);
            ~PlayerImage();
            void          loadMedia(BaseMedia *media, Region *reg);
            void          play();
            void          stop();
            void          resume();
            void          pause();
            void          changeSize(int w, int h);
            QWidget      *getView();
        protected:
            QPixmap       loaded_image;
            void          setAlignment();
            QScopedPointer<QLabel, QScopedPointerDeleteLater>  ImageWidget;
    };


#endif // MPIMAGE_H
