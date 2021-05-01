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
#include "conditional/expr.h"
#include "tools/main_configuration.h"

class BaseMedia : public BaseTimings
{
        Q_OBJECT
    public:
    const     int        CACHE_CONTROL_USE_CACHE  = 0;
    const     int        CACHE_CONTROL_ONLY_IF_CACHED  = 1;

        explicit BaseMedia(Files::MediaManager *mm, MainConfiguration *config, QObject *parent = Q_NULLPTR);

        void                 preloadParse(QDomElement element);
        QString              getLoadablePath();
        void                 pause();
        void                 start();
        void                 stop();
        void                 resume();
        void                 interruptByRestart();


        QString              getRegion()       {return region;}
        QString              getFit()          {return fit;}
        QString              getSrc()          {return src;}
        QString              getLogContentId() {return log_content_id;}
        QString              getFileName()     {return filename;}
        int                  getCacheControl() {return cache_control;}
        QString              getBaseType()     {return "media";}
        bool                 isMedia()         {return is_media;}
        void                 registerInMediaManager();
        void                 registerInMediaManagerAsUncachable();

        bool                 hasPlayingChilds(){return false;}
        QString              getParamsAsQuery() const;

        void                 emitPause();
        void                 emitResume();

    public slots:
        void                 emitfinishedActiveDuration();

    protected:
        Files::MediaManager *MyMediaManager;
        MainConfiguration   *MyMainConfiguration;
        QStringList          params_as_query;
        Expr                 MyExpr;
        QString              region          = "";
        QString              src             = "";
        QString              type            = "";
        QString              fit             = "";
        QString              filename        = "";
        int                  cache_control   = 0;
        QString              log_content_id  = "";
        bool                 is_media        = false;
        void                 parseBaseMediaAttributes();
        void                 parseBaseParameters();
        virtual void         setAttributes() = 0;

    private:
        void setAdditionalParameters(QDomElement param);
        int                  determineCacheControl(QString value);

};

#endif // MEDIA_H
