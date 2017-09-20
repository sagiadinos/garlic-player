#include "wgt.h"

Wgt::Wgt(QString file_path)
{
    setFilePath(file_path);
}

Wgt::~Wgt()
{
    zip.close();;
}

void Wgt::setFilePath(QString file_path)
{
    local_file_path = file_path;
    zip.setZipName(local_file_path);
    if (!zip.open(QuaZip::mdUnzip))
    {
        qDebug(ContentManager) <<  file_path << " Widget can not be opend. Zip error code: " << zip.getZipError();
    }
}


bool Wgt::isOpen()
{
    return zip.isOpen();
}


qint64 Wgt::calculateSize()
{
    if (!isOpen())
        return 0;

    quint32 size = 0;
    QList<QuaZipFileInfo> zlist = zip.getFileInfoList();

    QList<QuaZipFileInfo>::iterator i;
    for (i = zlist.begin(); i != zlist.end(); ++i)
    {
        QuaZipFileInfo fi = *i;
        size = size + fi.uncompressedSize;
    }

    return (qint64) size;
}

/**
 * @brief FileModel::extractWgt unzip the widget in the local_file_info.absolutePath()
 * which can be the cache directory or the local directory on usb, hardisc etc
 * @return
 */
bool Wgt::extract()
{
    if (!isOpen())
        return false;

    QFileInfo fi(local_file_path);
    QString folder_path = fi.absolutePath()+"/"+fi.baseName();
    QDir dir(folder_path);
    if (dir.exists() && !dir.removeRecursively())
    {
        qDebug(ContentManager) << local_file_path << " Widget was not extracted. Previuos directory cannot be deleted";
        return false;
    }
    if (!dir.mkdir(folder_path))
    {
        qDebug(ContentManager) << local_file_path << " Widget was not extracted. Directory cannot be created";
        return false;
    }

    JlCompress::extractDir(local_file_path, folder_path+"/");

    return true;
}
