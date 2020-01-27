/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/

#ifndef MEDIA_H
#define MEDIA_H

#include "files/media_manager.h"
#include "container.h"

class BaseMedia : public BaseTimings
{
        Q_OBJECT
    public:
        explicit BaseMedia(QObject *parent = Q_NULLPTR);
        QString              getRegion(){return region;}
        QString              getFit(){return fit;}
        QString              getSrc() {return src;}
        QString              getLogContentId() {return log_content_id;}
        QString              getFileName() {return filename;}
        QString              getCacheControl() {return cache_control;}
        QString              getBaseType() {return "media";}
        bool                 parse(QDomElement element);
        void                 resume(){play();}
        void                 registerFile(Files::MediaManager *mm);
        bool                 hasPlayingChilds(){return false;}
        BaseTimings         *getChildElementFromList(){return this;}
        TContainer          *getParentContainer(){return parent_container;}
        static  QString      parseSrc(QDomElement element);
        bool                 isDownloaded();
        bool                 isMedia(){return is_media;}
        QString              getLoadablePath();
        void                 pause();
        void                 stop();
        void                 play();
        void                 emitPreLoad();
    public slots:
        void                 emitfinished();
    protected:
        Files::MediaManager *MyMediaManager;
        TContainer          *parent_container;
        QString              region = "";
        QString              src  = "";
        QString              exec  = "";
        QString              type   = "";
        QString              fit = "";
        QString              filename  = "";
        QString              cache_control  = "";
        QString              log_content_id  = "";
        bool                 is_media = false;
        void                 parseBaseMediaAttributes();
        void                 parseBaseParameters();
        virtual void         setAttributes() = 0;
    private:
    signals:
        void                 startedMedia(TContainer *parent , BaseTimings *element);
        void                 finishedMedia(TContainer *parent , BaseTimings *element);
        void                 preloadElement(TContainer *parent, QDomElement);

};

#endif // MEDIA_H
