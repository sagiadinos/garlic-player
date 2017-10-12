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
#include "factory.h"

TFactory::TFactory(QObject *parent) : QObject(parent)
{
}

TBaseTiming* TFactory::createBase(QString type, TContainer *parent_container, QObject *parent)
{
    if (type == "img")
        return new TImage(parent_container, parent);
    else if (type == "video")
        return new TVideo(parent_container, parent);
    else if (type == "audio")
        return new TAudio(parent_container, parent);
    else if (type == "text")
        return new TWeb(parent_container, parent);
    else if (type == "prefetch")
        return new TPrefetch(parent_container, parent);
    else if (type == "seq")
        return new TSeq(parent_container, parent);
    else if (type == "par")
        return new TPar(parent_container, parent);
    else if (type == "excl")
        return new TExcl(parent_container, parent);
    else if (type == "body")
        return new TBody(parent);
    return NULL;
}
