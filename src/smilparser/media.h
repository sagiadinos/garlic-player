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
#include "smilparser/container.h"

class TMedia : public TBase
{
    Q_OBJECT
public:
    TMedia(TBase * parent = 0);
    void        setIndexPath(QString path);
    QString     getRegion();
    QString     getSrc();
    QString     getFit();
    QString     getExec();
    QString     getFileName();
    QString     getCacheControl();
    QString     getLogContentId();
    bool        parse(QDomElement element);
    virtual bool load(QString index_path) = 0;
    void        resume(){play();}
    QString     getBaseType() {return "media";}
    bool        hasPlayingChilds(){return false;}
    TBase      *getPlayedElement(){return this;}
public slots:
    void        emitfinished();
protected:
    TContainer *parent_container;
    QString     region, src, exec;
    QString     filename, cache_control, log_content_id  = "";
    void        setBaseMediaAttributes();
    void        setBaseParameters();
    virtual void setAttributes() = 0;
signals:
    void        startedMedia(TContainer *parent , TBase *element);
    void        finishedMedia(TContainer *parent , TBase *element);

};

#endif // MEDIA_H
