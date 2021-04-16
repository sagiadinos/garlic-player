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

#ifndef TBASE_H
#define TBASE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtXml>
#include "tools/logger.h"

enum class  SMIL_TAG{seq, par, excl, img, audio, video, text, ref, prefetch, unknown};

/**
 * @brief The abstract TBase class should inherited for all smil elements in body section
 *        Here comes is the init of all common attributes:
 *        id/xml:id, title, class and xml:lang
 *
 */
class TBase : public QObject
{
    Q_OBJECT
public:
    explicit TBase(QObject *parent = Q_NULLPTR);
    virtual void          preloadParse(QDomElement element) = 0;

            QString       getID(){return id;}
            QString       getTitle(){return title;}
            QString       getLang(){return lang;}
            QString       getClass(){return a_class;}
            QDomElement   getRootElement(){return root_element;}
            void          setParentTag(QString tag);
            SMIL_TAG      getParentTag();
    static  QString       parseID(QDomElement element);
protected:
            SMIL_TAG      parent_tag;
            QString       id             = "";
            QString       title          = "";
            QString       lang           = "";
            QString       a_class        = ""; // cause class is reserved word. a_class means attribut_class
            QDomElement   root_element;
            void          setBaseAttributes();
            QString       getAttributeFromRootElement(const QString attribute_name);
};

#endif // TBASE_H
