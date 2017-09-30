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

QString Wgt::handleRealPath()
{
    QString real_file_path = "";
    if (!extract())
        return "";
    #if defined  Q_OS_WIN
       real_file_path = "file:/"+local_file_path.mid(0, local_file_path.length()-4)+"/index.html"; // Windows needs file:// for opening absolute local_file_path in WebEngine
    #else
        real_file_path = "file://"+local_file_path.mid(0, local_file_path.length()-4)+"/index.html"; // Linux needs file:/// for opening absolute local_file_path in WebEngine
    #endif
    return real_file_path;
}

qint64 Wgt::calculateUncompressedSize()
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
    DiscSpace MyDiscSpace(fi.absolutePath());
    qint64 calc = MyDiscSpace.calculateNeededDiscSpaceToFree(calculateUncompressedSize());
    if (calc > 0 && !MyDiscSpace.freeDiscSpace(calc))
    {
        qCritical(ContentManager) << local_file_path << " Widget was not extracted. Not enough space could be freeed";
        return "";
    }

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
