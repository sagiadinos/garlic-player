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
            explicit BaseMedia(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
            virtual void          init(TMedia *media)   = 0;
            virtual void          deinit()   = 0;
            virtual void          setParentItem(QQuickItem *parent)   = 0;
            QString               getRegionId() const;
            void                  setRegionId(const QString &value);

    protected:
        QString               region_id;
        QQuickItem           *createMediaItem(QQmlComponent *mc, QString str);
            QString               determineQmlFillMode(QString fill_mode);
    };
#endif // BASE_MEDIA_H
