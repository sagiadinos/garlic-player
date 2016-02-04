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

#ifndef SMILPARSER_H
#define SMILPARSER_H

#include <smilparser/file.h>
#include <smilparser/head.h>
#include <smilparser/body.h>
#include <smilparser/seq.h>
#include <smilparser/par.h>
#include <smilparser/excl.h>
#include <smilparser/image.h>
#include <smilparser/audio.h>
#include <smilparser/video.h>
#include <smilparser/web.h>
#include <QSet>

/**
 * @brief   The TSmil class is the interface between playlists,  medias and
 *          the communication with the graphical output in mainwindow
 *          It is emits a signal which informs about region and media
 *          has to be played.
 *
 */
class TSmil : public QObject
{
    Q_OBJECT
public:
    TSmil(QObject *parent = 0);
    ~TSmil();
    void                               init(QString smil_index = "");
    THead                              getHeader();
    void                               beginSmilParsing();
protected:
    QDomElement                        parser;
    QString                            index_path;
    QList<Region>                      region_list;
    TFile                              MyFile;
    THead                              MyHead;
    TBody                             *MyBody;
    QHash<QString, TBase *>            ar_elements;
    showImg                            ImgAttr;
    void                               next();
    void                               next(TSeq *MySeq);
    void                               next(TPar *MyPar);
    void                               next(TExcl *MyExcl, TBase *element);
    void                               handleStop(TBase *element);
    void                               handlePause(TBase *element);
protected slots:
    void                               foundElement(TBase *parent, QDomElement dom_element);
    void                               startElement(TBase *, TBase *);
    void                               finishElement(TBase *, TBase *);
    void                               handleResumeElement(TBase *element);
private:
    QSet<TBase *>                      ar_played_media;
    QHash<QString, TBase *>::iterator  insertIntoObjectContainer(TBase *parent, TBase *child);
    void                               emitStartShowMedia(TBase *media);
    void                               emitStopShowMedia(TBase *media);

signals:
    void                               startShowMedia(TBase *media);
    void                               stopShowMedia(TBase *media);
};

#endif // SMILPARSER_H
