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
#include <QHash>

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
    void      init(QString smil_index = "");
    THead     getHeader();
    QString   getMediaType();
    void      beginSmilParsing();
    void  next();
    void  next(TSeq *MySeq);
    void  next(TPar *MyPar);
    void  next(TExcl *MyExcl);
public slots:
    void  handleFoundElement(QObject *parent, TFoundTag found_tag);
    void  emitstartedElement(QObject *, QObject *);
    void  emitfinishedElement(QObject *, QObject *);
    void  handlePause(QObject *element);
protected:
    QDomElement     parser;
    QString         index_path;
    QList<Region>   region_list;
    TFile           MyFile;
    THead           MyHead;
    TBody          *MyBody;
    QHash<QString, QObject *> ar_elements;
    QHash<QString, QObject *>::iterator ar_elements_iterator;
    showImg         ImgAttr;
    void            insertIntoObjectContainer(QString id, QObject *parent, QObject *child);
    void  handleStop(QObject *parent, QObject *element);
private:

signals:
    void            playMedia(QObject *media);
    void            finishedMedia(QObject *media);
};

#endif // SMILPARSER_H
