#ifndef BASE_MEDIA_H
#define BASE_MEDIA_H

#include <QWidget>
#include <QScopedPointer>
#include "smilparser/media.h"
#include "tools/logger.h"

    class BaseMedia : public QObject
    {
            Q_OBJECT
        public:
            explicit BaseMedia(QObject *parent = nullptr);
            virtual void          init(TMedia *media)   = 0;
            virtual void          deinit() = 0;
            virtual QWidget      *getView()= 0;
            virtual void          changeSize(int w, int h) = 0;
        protected:
                    Logger&      MyLogger = Logger::getInstance();
                    TMedia       *MyMedia = Q_NULLPTR;
                    QString      start_time = "";
                    bool         exists = false;
                    void         setStartTime();
                    bool         isFileExists(QString path);
                    QString      createPlayLogXml();

    };
#endif // BASE_MEDIA_H
