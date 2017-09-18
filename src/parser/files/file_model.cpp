#include "file_model.h"

FileModel::FileModel(Network *network, QString cache)
{
    MyNetwork   = network;
    setCachePath(cache);
    connect(MyNetwork, SIGNAL(downloadPossible(QIODevice *)), SLOT(saveToDisk(QIODevice *)));
    connect(MyNetwork, SIGNAL(downloadInpossible()), SLOT(emitPaths()));
}

/**
 * @brief FileModel::registerFile needs the full path to File
 * This class should do not differ between an smil.index and a medie file, cause it is possible to link to an index.
 *
 * @param src
 */
void FileModel::registerFile(QString src)
{
    setSrcFilePath(src);
    MyDiscSpace.init(getCachePath());
    if (isRemote(src_file_path))
    {
        local_file_info.setFile(cache_path+"/"+determineHashedFilePath());
        storage.setPath(cache_path);
        MyNetwork->processFile(QUrl(src_file_path), local_file_info); // leads to method saveToDisk
    }
    else
    {
        local_file_info.setFile(src_file_path);
    }
    checkForExtension();
    emitPaths();
}

bool FileModel::isRemote(QString src)
{
    if (src.mid(0, 4) == "http" || src.mid(0,3) == "ftp")
        return true;
    else
        return false;
}

qint64 FileModel::calculateNeededDiscSpace(qint64 size_new_file)
{
    return -1*(MyDiscSpace.getBytesAvailable() - size_new_file - 10);
}

bool FileModel::freeDiscSpace(qint64 size_to_free)
{
    QDir dir(cache_path);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); // do not work, when used in entryInfoList overloaded params directly so set hiere first
    dir.setSorting(QDir::Time | QDir::Reversed);
    QFileInfoList dirList = dir.entryInfoList();
    for(int i = 0; i < dirList.size(); i++)
    {
        if (dirList.at(i).isFile())
        {
            if (!deleteFile(dirList.at(i).absoluteFilePath()))
                return false;
        }
        if (dirList.at(i).isDir())
        {
            if (!deleteDirectory(dirList.at(i).absoluteFilePath()))
                return false;

            // delete corresponding wgt here to prevent inconsistencies e.g. deleted dir, but not wgt - or vice versa
            if (!deleteFile(dirList.at(i).absoluteFilePath()+".wgt"))
                return false;
        }
        if (size_to_free < MyDiscSpace.getBytesDeleted())
            break;
    }
    qInfo(ContentManager) << "INFO " << MyDiscSpace.getBytesDeleted() << " Bytes of old content were deleted";
    return true;
}

void FileModel::checkForExtension()
{
    QString path = local_file_info.absoluteFilePath();
    if (local_file_info.suffix() == "wgt")
    {
        if (!extractWgt())
        {
            emit failed(src_file_path);
            return;
        }
        #if defined  Q_OS_WIN
            setLocalFilePath("file:/"+path.mid(0, path.length()-4)+"/index.html"); // Windows needs file:// for opening absolute paths in WebEngine
        #else
            setLocalFilePath("file://"+path.mid(0, path.length()-4)+"/index.html"); // Linux needs file:/// for opening absolute paths in WebEngine
        #endif
    }
    else
        setLocalFilePath(path);

}

void FileModel::saveToDisk(QIODevice *data)
{
    QFile file(local_file_info.absoluteFilePath());
    if (!file.open(QIODevice::ReadWrite))
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " could not be saved " << file.errorString();
        emit failed(src_file_path);
        return;
    }
    qint64 calc = calculateNeededDiscSpace(data->size());
    if (calc > 0 && !freeDiscSpace(calc))
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " could not be saved " << file.errorString() << "cause space cannot be freed";
        emit failed(src_file_path);
        return;
    }

    file.write(data->readAll());
    file.close();
    return;
}

/**
 * @brief FileModel::extractWgt unzip the widget in the local_file_info.absolutePath()
 * which can be the cache directory or the local directory on usb, hardisc etc
 * @return
 */
bool FileModel::extractWgt()
{
    QuaZip zip(local_file_info.absoluteFilePath());

    if (!zip.open(QuaZip::mdUnzip))
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " Widget can not be opend. Zip error code: " << zip.getZipError();
        return false;
    }

    QString folder_path = local_file_info.absolutePath()+"/"+local_file_info.baseName();
    QDir dir(folder_path);
    if (dir.exists() && !dir.removeRecursively())
    {
        qWarning(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " Widget was not extracted. Directory cannot be deleted";
        return false;
    }
    if (!dir.mkdir(folder_path))
    {
        qWarning(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " Widget was not extracted. Directory cannot be created";
        return false;
    }

    JlCompress::extractDir(local_file_info.absoluteFilePath(), folder_path+"/");
    zip.close();

    return true;
}

QString FileModel::determineHashedFilePath()
{
    QFileInfo fi(src_file_path);
    return QString(QCryptographicHash::hash((src_file_path.toUtf8()), QCryptographicHash::Md5).toHex())+ "."+fi.suffix();
}

bool FileModel::deleteFile(QString file_path)
{
    QFile del_file(file_path);

    if (!del_file.exists()) // not an error. to prevent eventually inconsistencies e.g. wgt <=> wgt directory
        return true;

    qint64 del_size = del_file.size();
    if (!del_file.remove())
    {
        qCritical(ContentManager) << file_path << " cannot be removed " << del_file.errorString();
        return false;
    }
    MyDiscSpace.freedSpace(del_size);
    return true;
}

bool FileModel::deleteDirectory(QString dir_path)
{
    QDir del_dir(dir_path);

    if (!del_dir.exists()) // not an error. to prevent eventually inconsistencies e.g wgt <=> wgt directory
        return true;

    quint64 dir_size = calculateDirectorySize(dir_path);
    if (!del_dir.removeRecursively())
    {
        qCritical(ContentManager) << dir_path << " cannot be removed";
        return false;
    }
    MyDiscSpace.freedSpace(dir_size);
    return true;
}

qint64 FileModel::calculateDirectorySize(QString dir_path)
{
    QDirIterator it(dir_path, QDir::Files, QDirIterator::Subdirectories);
    QFileInfo fi;
    qint64 dir_size = 0;
    while (it.hasNext())
    {
        fi.setFile(it.next());
        dir_size += fi.size();
    }
    return dir_size;
}

void FileModel::emitPaths()
{
    qInfo(ContentManager) << "OBJECT_UPDATED resourceURI:" << src_file_path << " contentLenght: " << local_file_info.size();
    emit succeed(src_file_path, local_file_path);
}

