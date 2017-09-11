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

#ifndef TREGION_H
#define TREGION_H

#include <QQuickItem> // interates QQmlComponent
#include <head.h>
#include <smilparser.h>
#include <file_manager.h>


class TRegion : public QQuickItem
{
    Q_OBJECT
public:
    explicit TRegion(QObject *parent);
    ~TRegion();
    void                setRootSize(int w, int h);
    void                setRegion(Region r, QQmlEngine *e);
    void                playImage(TMedia *Media);
    void                playVideo(TMedia *Media);
    void                playAudio(TMedia *Media);
    void                playWeb(TMedia *Media);
    void                removeMedia();

protected:
    QQmlComponent        *media_component, *rectangle;
    QQuickItem           *root_item, *rectangle_item, *media_item;
    QQmlEngine           *engine;
    TMedia               *MyMedia;
    qreal                 root_width_px, root_height_px = 0;
    Region                region;
    void                  resizeGeometry();
    void                  handleMediaItem(QString str);
    QString               determineQmlFillMode(QString fill_mode);
protected slots:
    void                  finished();
};

#endif // TREGION_H
