#ifndef BASE_MEDIA_H
#define BASE_MEDIA_H

#include <QQuickItem> // interates QQmlComponent
#include <QScopedPointer>
#include "smilparser/media/base_media.h"
#include "tools/logger.h"
#include "../player-common/launcher.h"

class PlayerBaseMedia : public QObject
{
        Q_OBJECT
    public:
        explicit PlayerBaseMedia(QQmlComponent *mc, QString r_id, Launcher *lc, QObject *parent = nullptr);
        virtual void          init(BaseMedia *media)   = 0;
        virtual void          deinit()   = 0;
        virtual void          setParentItem(QQuickItem *parent)   = 0;
        BaseMedia            *getSmilMedia();
        QString               getRegionId() const;
        void                  setStartTime();
        QString               createPlayLogXml();
        void                  setRegionId(const QString &value);

    protected:
        Logger&               MyLogger = Logger::getInstance();
        Launcher             *MyLauncher;
        BaseMedia            *SmilMedia;
        QString               start_time = "";
        QString               region_id  = "";
        QQuickItem           *createMediaItem(QQmlComponent *mc, QString str);
        bool                  load(QQuickItem *item);
        bool                  isFileExists(QString path);
};
#endif // BASE_MEDIA_H
