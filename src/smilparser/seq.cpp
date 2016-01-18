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

#include "seq.h"

TSeq::TSeq(QObject * parent)
{
    parent_playlist = parent;
    initTimer();
    setObjectName("TSeq");
}

bool TSeq::parse(QDomElement element)
{
    root_element   = element;
    setBaseAttributes();
    if (element.hasChildNodes())
    {
        active_element = element.firstChildElement();
        if (active_element.tagName() == "metadata")
            doMetaData();
        setPlaylist();
        iterator       = ar_playlist.begin();
        active_element = *iterator;
    }
    return true;
}

void TSeq::beginPlay()
{
    setTimedStart();
    return;
}

void TSeq::checkBeforePlay()
{
    if (setTimedEnd() || ar_playlist.length() > 0)
    {
        resetInternalRepeatCount();
        play();
        emit startedPlaylist(parent_playlist, this);
    }
    else // when end or duration is not specified or no child elements stop imediately
        emitfinished();
    return;
}

void TSeq::play()
{
    iterator       = ar_playlist.begin();
    active_element = *iterator;
    reactByTag();
    status = _playing;
    return;
}

void TSeq::stop()
{
    status = _stopped;
    return;
}

void TSeq::pause()
{
    status = _paused;
    return;
}

void TSeq::doMetaData()
{
    QDomNodeList childs = root_element.childNodes();
    QDomElement  element;
    int          length = childs.length();
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() == "meta" && element.hasAttribute("name") && element.hasAttribute("content"))
        {
            if (element.attribute("name") == "adapi:pickingAlgorithm")
                pickingAlgorithm = element.attribute("content");
            else if (element.attribute("name") == "adapi:pickingBehavior")
                pickingBehavior = element.attribute("content");
            else if (element.attribute("name") == "adapi:pickNumber")
                pickNumber = element.attribute("content").toInt();
        }
    }
    return;
}

bool TSeq::next()
{
    bool ret = false;
    iterator++; // inc iterator first only when inc result smaller than  .end()
    if (iterator < ar_playlist.end())  // cause .end() pointing to the imaginary item after the last item in the vector
    {
        active_element = *iterator;
        ret            = true;
        reactByTag();
    }
    else // end check if repeat value is indefined or Limnit not reached
    {
        if (checkRepeatCountStatus())
        {
            play();
            ret            = true;
        }
        else
        {
            emitfinished();
        }
    }
    return ret;
}

bool TSeq::previous()
{
    return false;
}

void TSeq::setPlaylist()
{
    QDomNodeList childs = root_element.childNodes();
    int          length = childs.length();
    QDomElement  element;
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "metadata")
            ar_playlist.append(element);
    }
    if (pickingAlgorithm == "shuffle" && pickingBehavior == "pickN" && pickNumber > 0)
    {
        randomizePlaylist();
        random = true;
        internal_pick = pickNumber;
    }
}

void TSeq::randomizePlaylist()
{
    // Knuth shuffle
    int length = ar_playlist.length();
    for (int i = length - 1; i > 0; --i)
    {
        int i2 = qrand() % (i + 1);
        std::swap(ar_playlist[i], ar_playlist[i2]);
    }
    // Let only pickNumber items in List
    for (int i = 0; i < (length-pickNumber); i++)
        ar_playlist.removeLast();
}
