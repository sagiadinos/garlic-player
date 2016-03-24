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
#include "files/file_manager.h"

class TMedia : public TBaseTiming
{
    Q_OBJECT
public:
    TMedia(TBaseTiming * parent = 0);
    void              setIndexPath(QString path);
    QString           getRegion();
    bool              parse(QDomElement element);
    void              resume(){play();}
    void              prepareLoad(TFileManager *manager);
    bool              isLoaded();
    QString           getSrc() {return "src";}
    QString           getBaseType() {return "media";}
    bool              hasPlayingChilds(){return false;}
    TBaseTiming      *getChildElementFromList(){return this;}
    TContainer       *getParentContainer(){return parent_container;}
    virtual bool      load() = 0;
    static  QString   parseSrc(QDomElement element);

public slots:
    void              emitfinished();
protected:
    TContainer       *parent_container;
    QString           region, src, exec;
    QString           cached_file_path;
    QString           filename, cache_control, log_content_id  = "";
    TFileManager     *MyFileManager;
    bool              loaded;
    void              setBaseMediaAttributes();
    void              setBaseParameters();
    virtual void      setAttributes() = 0;
private:
signals:
    void              startedMedia(TContainer *parent , TBaseTiming *element);
    void              finishedMedia(TContainer *parent , TBaseTiming *element);

};

#endif // MEDIA_H
