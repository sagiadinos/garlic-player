#ifndef BASE_MEDIA_H
#define BASE_MEDIA_H

#include <QWidget>
#include <QScopedPointer>
#include "smilparser/media/base_media.h"
#include "tools/logger.h"

using namespace MediaParser;

    class PlayerBaseMedia : public QObject
    {
            Q_OBJECT
        public:
            explicit PlayerBaseMedia(MainConfiguration *config, QObject *parent = nullptr);
            virtual void          loadMedia(BaseMedia *media, Region *reg)   = 0;
            virtual void          play() = 0;
            virtual void          restart() = 0;
            virtual void          stop() = 0;
            virtual void          resume() = 0;
            virtual void          pause() = 0;
            virtual QWidget      *getView()= 0;
            virtual void          changeSize(int w, int h) = 0;
            BaseMedia            *getSmilMedia();
        protected:
            Logger&      MyLogger = Logger::getInstance();
            BaseMedia   *SmilMedia = Q_NULLPTR;
            MainConfiguration *MyMainConfiguration;
            QString      start_time = "";
            Region       *region;
            bool         exists = false;
            void         setStartTime();
            bool         isFileExists(QString path);
            QString      createPlayLogXml();

    };
#endif // BASE_MEDIA_H
