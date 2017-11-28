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
#include "media_model.h"

MediaModel::MediaModel(QObject *parent) :QObject(parent)
{
    clearQueue();
}

MediaModel::~MediaModel()
{
    clearQueue();
}

void MediaModel::clearQueue()
{
    available_media_list.clear();
}


QString MediaModel::findLocalBySrcPath(QString src_file_path)
{
    QString ret ="";
    i_available_media_list = available_media_list.find(src_file_path);
    if (i_available_media_list != available_media_list.end())
        ret = i_available_media_list.value().first;

    return ret;
}

int MediaModel::findStatusBySrcPath(QString src_file_path)
{
    int ret = 0;
    i_available_media_list = available_media_list.find(src_file_path);
    if (i_available_media_list != available_media_list.end())
        ret = i_available_media_list.value().second;

    return ret;
}

void MediaModel::insertAvaibleLink(QString src_file_path)
{
    if (findLocalBySrcPath(src_file_path) == "") // reloadable not neccessary cause caching is part of WebEngine
        available_media_list.insert(src_file_path, qMakePair(src_file_path, MEDIAMODEL_EXISTS));
}


void MediaModel::insertAvaibleFile(QString src_file_path, QString local_file_path)
{
    QString path = determinePathByMedia(src_file_path, local_file_path);
    if (path == "")
        return;

    if (findLocalBySrcPath(src_file_path) == "")
        available_media_list.insert(src_file_path, qMakePair(path, MEDIAMODEL_EXISTS));
    else
        available_media_list[src_file_path] = qMakePair(path, MEDIAMODEL_MODIFIED);
}

QString MediaModel::determineHashedFilePath(QString src_file_path)
{
    QFileInfo fi(src_file_path);
    return QString(QCryptographicHash::hash((src_file_path.toUtf8()), QCryptographicHash::Md5).toHex())+ "."+fi.suffix();
}

// =========================== protected methods ============================

QString MediaModel::determinePathByMedia(QString src_file_path, QString local_file_path)
{
    QFileInfo fi(local_file_path);
    QString path = fi.absoluteFilePath();
    QString real_file_path("");
    // Todo Maybe later a factory when ahref will be implemented
    if (fi.suffix() == "wgt")
    {
        Wgt MyWgt(local_file_path);
        real_file_path = MyWgt.handleRealPath();
        if (real_file_path == "")
        {
            qCritical(Develop) << "UNZIP_FAILED resourceURI: " << src_file_path << " caused an error during processing";
            return "";
        }
    }
    else
        real_file_path = path;

    return real_file_path;
}

