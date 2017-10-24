#ifndef BASE_MEDIA_H
#define BASE_MEDIA_H

#include <QQuickItem> // interates QQmlComponent
#include <QScopedPointer>
#include "smilparser/media.h"

#include <unistd.h>

    class BaseMedia : public QObject
    {
            Q_OBJECT
        public:
            explicit BaseMedia(QObject *parent = nullptr);
            virtual void          init(QQmlComponent *mc)   = 0;
            virtual void          setParentItem(QQuickItem *parent)   = 0;
        protected:
            QQuickItem           *createMediaItem(QQmlComponent *mc, QString str);
            QString               determineQmlFillMode(QString fill_mode);
            qint64                getCurrentRSS();
    };
#endif // BASE_MEDIA_H
