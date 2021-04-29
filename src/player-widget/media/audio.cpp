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

PlayerAudio::PlayerAudio(QObject *parent) : PlayerBaseMedia(parent)
{
    MediaDecoder.reset(new MediaPlayerWrapper(this));
    connect(MediaDecoder.data(), SIGNAL(finished()), this, SLOT(finished()));
}

PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::init(BaseMedia *media)
{
    SmilMedia  = media;
    QString path = SmilMedia->getLoadablePath();
    if (isFileExists(path))
    {
        MediaDecoder.data()->load(path);
        MediaDecoder.data()->play();
        TAudio  *MyParser = qobject_cast<TAudio *>(media);
        MediaDecoder.data()->setVolume(MyParser->getSoundLevel());
        if (SmilMedia->getLogContentId() != "")
            setStartTime();
    }
    else
    {
         SmilMedia->finishedNotFound();
    }
 }

void PlayerAudio::deinit()
{
    MediaDecoder.data()->stop();
    MediaDecoder.data()->unload();
    if (SmilMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void PlayerAudio::changeSize(int w, int h)
{
    Q_UNUSED(w);Q_UNUSED(h)
}


QWidget *PlayerAudio::getView()
{
    return Q_NULLPTR;
}

void PlayerAudio::finished()
{
   SmilMedia->finishIntrinsicDuration();
}
