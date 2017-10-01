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
#ifndef TFACTORY_H
#define TFACTORY_H
#include "smilparser/image.h"
#include "smilparser/audio.h"
#include "smilparser/video.h"
#include "smilparser/web.h"
#include "smilparser/prefetch.h"

#include "smilparser/body.h"
#include "smilparser/seq.h"
#include "smilparser/par.h"
#include "smilparser/excl.h"

class TFactory : public QObject
{
    Q_OBJECT
public:
    explicit TFactory(TBaseTiming *parent = 0);
    static TBaseTiming    *createBase(QString type, TContainer *parent);
};

#endif // TFACTORY_H
