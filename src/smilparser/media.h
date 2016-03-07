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

#include <QFile>
#include <QCryptographicHash>
#include "smilparser/container.h"
#include "smilparser/tools/downloader.h"
#include "configuration.h"

class TMedia : public TBaseTiming
{
    Q_OBJECT
public:
    TMedia(TBaseTiming * parent = 0);
    void            setIndexPath(QString path);
    QString         getRegion();
    bool            parse(QDomElement element);
    void            resume(){play();}
    bool            isLoaded(QString index_path, TConfiguration *config);
    QString         getBaseType() {return "media";}
    bool            hasPlayingChilds(){return false;}
    TBaseTiming    *getChildElementFromList(){return this;}
public slots:
    void            emitfinished();
protected:
    TContainer     *parent_container;
    QString         region, src, exec;
    QString         filename, cache_control, log_content_id  = "";
    TDownloader    *MyDownloader;
    TConfiguration *MyConfiguration;
    bool            downloaded;
    bool            loaded;
    void            setBaseMediaAttributes();
    void            setBaseParameters();
    virtual void    setAttributes() = 0;
    virtual bool    load(QString file_path) = 0;
    bool            isDownloaded(QString cached_file_path, QString file_path);
protected slots:
    void            downloadSucceed(QString local_path);
private:
    QString         local_file_path;
    QString         getFilePath(QString index_path);
signals:
    void            startedMedia(TContainer *parent , TBaseTiming *element);
    void            finishedMedia(TContainer *parent , TBaseTiming *element);

};

#endif // MEDIA_H
