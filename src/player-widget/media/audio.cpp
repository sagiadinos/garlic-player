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
#include "audio.h"

Audio::Audio(TMedia *media, QObject *parent) : BaseMedia(parent)
{
    MyAudio  = qobject_cast<TAudio *>(media);
    connect(&MediaDecoder, SIGNAL(finished()), this, SLOT(finished()));
}

Audio::~Audio()
{
    MediaDecoder.unload();
}

void Audio::init()
{
    MediaDecoder.load(MyAudio->getLoadablePath());
    MediaDecoder.play();
}


void Audio::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
}


QWidget *Audio::getView()
{
    return Q_NULLPTR;
}

void Audio::finished()
{
   MyAudio->finishedSimpleDuration();
}
