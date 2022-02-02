#ifndef BASE_MEDIA_H
#define BASE_MEDIA_H

#include <QWidget>
#include <QScopedPointer>
#include "smilparser/media/base_media.h"
#include "tools/logger.h"

    class PlayerBaseMedia : public QObject
    {
            Q_OBJECT
        public:
            explicit PlayerBaseMedia(QObject *parent = nullptr);
            virtual void          init(BaseMedia *media, Region *reg)   = 0;
            BaseMedia            *getSmilMedia();
            virtual void          deinit() = 0;
            virtual QWidget      *getView()= 0;
            virtual void          changeSize(int w, int h) = 0;
        protected:
            Logger&      MyLogger = Logger::getInstance();
            BaseMedia   *SmilMedia = Q_NULLPTR;
            QString      start_time = "";
            Region       *region;
            bool         exists = false;
            void         setStartTime();
            bool         isFileExists(QString path);
            QString      createPlayLogXml();

    };
#endif // BASE_MEDIA_H
