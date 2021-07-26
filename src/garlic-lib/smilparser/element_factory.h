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
#ifndef ElementFactory_H
#define ElementFactory_H
#include "smilparser/media/image.h"
#include "smilparser/media/audio.h"
#include "smilparser/media/brush.h"
#include "smilparser/media/video.h"
#include "smilparser/media/web.h"
#include "smilparser/media/prefetch.h"
#include "smilparser/media/unknown.h"
#include "smilparser/media/widget.h"
#include "smilparser/media/ref_command.h"
#include "smilparser/container/body.h"
#include "smilparser/container/seq.h"
#include "smilparser/container/par.h"
#include "smilparser/container/excl.h"

class ElementFactory : public QObject
{
    Q_OBJECT
public:
    explicit ElementFactory(Files::MediaManager *mm, MainConfiguration *config, SmilHead::PlaceHolder *ph, QObject *parent = Q_NULLPTR);
    BaseTimings    *createBase(QDomElement dom_element, QObject *parent);
protected:
    Files::MediaManager   *MyMediaManager;
    SmilHead::PlaceHolder *MyPlaceHolder;
    MainConfiguration     *MyMainConfiguration;


};

#endif // ElementFactory_H
