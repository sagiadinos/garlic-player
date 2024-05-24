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
#ifndef TAUDIO_H
#define TAUDIO_H

#include "base_media.h"
namespace MediaParser
{
    class TAudio : public BaseMedia
    {
        Q_OBJECT
    public:
        explicit TAudio(Files::MediaManager *mm, MainConfiguration *mc, SmilHead::PlaceHolder *ph, QObject *parent);
        ~TAudio();
        QString                 getSoundLevel(){return soundLevel;}
    protected:
        QString                 soundLevel = "100%";
        void                    setAttributes();
        bool                    loadMedia();
        void                    prepareDurationTimersForRepeat();
    protected slots:
        void                    prepareDurationTimers();
    };
}
#endif // TAUDIO_H
