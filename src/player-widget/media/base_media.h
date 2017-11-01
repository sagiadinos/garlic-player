#ifndef BASE_MEDIA_H
#define BASE_MEDIA_H

#include <QObject>
#include <QWidget>
#include "smilparser/media.h"

    class BaseMedia : public QObject
    {
            Q_OBJECT
        public:
            explicit BaseMedia(QObject *parent = nullptr);
            virtual void          init(TMedia *media)   = 0;
            virtual void          deinit() = 0;
            virtual QWidget      *getView()= 0;
            virtual void          changeSize(int w, int h) = 0;

    };
#endif // BASE_MEDIA_H
