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

#include "smilparser.h"

TSmil::TSmil(TConfiguration *config, QObject *parent)
{
    Q_UNUSED(parent)
    MyConfiguration = config;
}

TSmil::~TSmil()
{
    ar_elements.clear();
}

void TSmil::init(QString smil_index_path)
{
    if (smil_index_path.mid(0, 4) == "http" || smil_index_path.mid(0, 3) == "ftp")
    {
        QUrl url(smil_index_path);
        index_path = url.adjusted(QUrl::RemoveFilename).toString();
    }
    else
    {
        QFileInfo fi(smil_index_path);
        index_path = fi.path();
    }
    if (index_path.mid(index_path.length()-1, 1) != "/")
        index_path += "/";
    return;
}

void TSmil::beginSmilParsing(QDomElement body)
{
    MyBody = new TBody();
    connect(MyBody, SIGNAL(foundElement(TContainer *, QString, QDomElement )), this, SLOT(foundElement(TContainer *, QString, QDomElement )));
    connect(MyBody, SIGNAL(finishedContainer(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    if (MyBody->parse(body))
        MyBody->prepareTimerBeforePlaying();
    return;
}

/**
 * @brief TSmil::foundElement slot catches the signal from playlist, prepare elements for plaing and create a hash map
 * @param parent
 * @param found_tag
 */
void TSmil::foundElement(TContainer *parent, QString type, QDomElement dom_element)
{
    TBaseTiming                              *MyBase;
    bool                                     playable             = false;
    QHash<QString, TBaseTiming *>::iterator  ar_elements_iterator = ar_elements.find(TBase::parseID(dom_element));
    QString                                  base_type            = "";
    if (ar_elements_iterator == ar_elements.end())
    {
        MyBase     = TFactory::createBase(type, parent);
        base_type  = MyBase->getBaseType();
        MyBase->parse(dom_element);
        if (MyBase != NULL)
        {
            ar_elements_iterator = insertIntoObjectContainer(parent, MyBase);
            if (base_type == "media")
                connectMediaSlots(qobject_cast<TMedia *> (MyBase));
            else if (base_type == "container")
                connectContainerSlots(MyBase);
        }
    }
    else
    {
        MyBase     = *ar_elements_iterator;
        base_type  = MyBase->getBaseType();
    }
    qDebug() << MyBase->getID() << QTime::currentTime().toString()  << " foundElement";

    playable    = MyBase->isPlayable();

    if (playable)
    {
         MyBase->prepareTimerBeforePlaying();
    }

    QString object_name = parent->objectName();
    if (object_name == "TExcl" && playable)  // increment active child to determine end of a excl
    {
        TExcl   *MyExclParent   = qobject_cast<TExcl *> (parent);
        MyExclParent->childStarted(*ar_elements_iterator);
    }
    return;
}

/**
 * @brief TSmil::startedElement slot catch the start signal from a element and check if it is allowed to play.
 *        If the element is a media a signal will emitted to an output interface which can show the media e.g. player-widget
 *
 * @param parent
 * @param element
 */
void TSmil::startElement(TContainer *parent, TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << " startElement";
    bool         playable      = true;
    if (parent->getBaseType() == "container")
    {

        TContainer *MyContainer = qobject_cast<TContainer *> (parent);
        playable                = MyContainer->isChildPlayable(element);
        if (playable)
            MyContainer->setPlayedElement(element);
    }

    if (playable)
    {
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<TMedia *> (element));
        else
            element->play();

    }
    return;
}

/**
 * @brief TSmil::finishedElement slot get called when emit finishedMedia finishedContainer active duration (finish playing)
 *        finishElement should kill the timer (TSmil::stopElement(TBase *element)) of his child elements not his own
 *        except in the case is in an excl container and stpped by a peer
 * @param parent
 * @param element
 */
void TSmil::finishElement(TContainer *parent, TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << " finishElement";
    if (element->objectName() != "TBody") // when TBody ends there is no parent and nothing todo anymore
    {
        qDebug()<< element->getID() << QTime::currentTime().toString() << "Not kill Timer";
        stopElement(element);
        parent->next(element);
    }
    else
    {
        ar_elements.clear();
        delete MyBody;
    }
    return;
}


/**
 * @brief TSmil::handlePause prepares elements for pause, pause them and recurses their childs
 * @param parent
 * @param element
 */
void TSmil::pausePlaylingElement(TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << "pause Element";
    element->prepareTimerBeforePausing();
    if (element->hasPlayingChilds())
    {
        TContainer  *MyContainer      = qobject_cast<TContainer *> (element);
        pausePlaylingElement(MyContainer->getChildElementFromList());
    }
    element->pause();
    if (element->getBaseType() == "media")
        emitStopShowMedia(qobject_cast<TMedia *> (element));
    return;
}

void TSmil::resumeQueuedElement(TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << "resume Element";
    element->prepareTimerBeforeResume();
    if (element->getStatus() == element->_paused)
    {
        if (element->hasPlayingChilds())
            resumeQueuedElement(element->getChildElementFromList());
        element->resume();
        if (element->getBaseType() == "media")
            emitStartShowMedia(qobject_cast<TMedia *> (element));
    }
    return;
}

/**
 * @brief TSmil::handleStop checks when an element stopped or finished and recurses to stop all active child elements e.g. timers, too.
 *        If the element is a media a signal will emitted to an output interface which can stop the show the media e.g. player-widget
 * @param parent
 * @param element
 */
void TSmil::stopPlaylingElement(TBaseTiming *element)
{
    qDebug()<< element->getID() << QTime::currentTime().toString() << "Kill Timer";
    killTimer(element);
    stopElement(element);
    return;
}

// ============================== private methods =======================================

void TSmil::stopElement(TBaseTiming *element)
{
    if (element->getStatus() != element->_stopped)
    {
        if (element->hasPlayingChilds())
        {
            qDebug()<< element->getID() << QTime::currentTime().toString() << "has Childs";
            stopPlaylingElement(element->getChildElementFromList()); // recurse with stopPlaylingElement to kill timer of childs
        }
        element->stop();
        if (element->getBaseType() == "media")
            emitStopShowMedia(qobject_cast<TMedia *> (element));
    }
    return;
}

void TSmil::killTimer(TBaseTiming *element)
{
    qDebug() << element->getID() << QTime::currentTime().toString() << "kill Timer";
    element->prepareTimerBeforeStop();
    return;
}

QHash<QString, TBaseTiming *>::iterator TSmil::insertIntoObjectContainer(TBaseTiming *parent, TBaseTiming *child)
{
    QString type = parent->objectName();
    QString id   = child->getID();
    if (parent->getBaseType() == "container")
    {
        TContainer *MyPlaylist  = qobject_cast<TContainer *> (parent);
        MyPlaylist->insertContainerObject(id, child);
    }
    return ar_elements.insert(id, child);;
}

void TSmil::connectContainerSlots(TBaseTiming *element)
{
    TContainer *MyPlaylist = qobject_cast<TContainer *> (element);
    if (MyPlaylist->isPlayable())
    {
        connect(MyPlaylist, SIGNAL(foundElement(TContainer *, QString, QDomElement )), this, SLOT(foundElement(TContainer *, QString, QDomElement )));
        connect(MyPlaylist, SIGNAL(startedContainer(TContainer *, TBaseTiming *)), this, SLOT(startElement(TContainer *, TBaseTiming *)));
        connect(MyPlaylist, SIGNAL(finishedContainer(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
        if (element->objectName() == "TExcl")
        {
            connect(MyPlaylist, SIGNAL(resumeElement(TBaseTiming *)), this, SLOT(resumeQueuedElement(TBaseTiming *)));
            connect(MyPlaylist, SIGNAL(pauseElement(TBaseTiming *)), this, SLOT(pausePlaylingElement(TBaseTiming *)));
            connect(MyPlaylist, SIGNAL(stopElement(TBaseTiming *)), this, SLOT(stopPlaylingElement(TBaseTiming *)));
        }
    }
    return;
}

void TSmil::connectMediaSlots(TMedia *MyMedia)
{
    if (MyMedia->isPlayable())
    {
        MyMedia->prepareLoad(index_path, MyConfiguration); // load media
        connect(MyMedia, SIGNAL(startedMedia(TContainer *, TBaseTiming *)), this, SLOT(startElement(TContainer *, TBaseTiming *)));
        connect(MyMedia, SIGNAL(finishedMedia(TContainer *, TBaseTiming *)), this, SLOT(finishElement(TContainer *, TBaseTiming *)));
    }
    return;
}

void TSmil::emitStartShowMedia(TMedia *MyMedia)
{
    if (MyMedia->isLoaded())
    {
        MyMedia->play();
        qDebug() << MyMedia->getID() <<QTime::currentTime().toString() << "startShowMedia";
        ar_played_media.insert(MyMedia);
        emit startShowMedia(MyMedia);
    }
    return;
}

void TSmil::emitStopShowMedia(TMedia *MyMedia)
{
    if (ar_played_media.find(MyMedia) != ar_played_media.end())
    {
        ar_played_media.remove(MyMedia);
        qDebug() << MyMedia->getID() <<QTime::currentTime().toString() << "stopShowMedia";
        emit stopShowMedia(MyMedia);
    }
    return;
}
