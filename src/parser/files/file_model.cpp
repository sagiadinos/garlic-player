#include "file_model.h"

FileModel::FileModel(TConfiguration *config)
{
    MyConfiguration   = config;
    MyDownloadQueue = new DownloadQueue(MyConfiguration->getUserAgent().toUtf8());
    setCachePath(MyConfiguration->getPaths("cache"));
    connect(MyDownloadQueue, SIGNAL(succeed(QString, QString, QIODevice *)), SLOT(doSucceed(QString, QString, QIODevice *)));
    connect(MyDownloadQueue, SIGNAL(notcacheable(QString)), SLOT(doNotCacheable(QString)));
}

bool FileModel::isRemote(QString src)
{
    if (src.mid(0, 4) == "http" || src.mid(0,3) == "ftp")
        return true;
    else
        return false;
}

/**
 * @brief FileModel::registerFile needs the full path to File
 * This method only differ between an smil-index and a media file, cause media files must have an unique name.
 *
 * @param src
 */
void FileModel::registerFile(QString src_file_path)
{
    MyDiscSpace.init(getCachePath());
    if (isRemote(src_file_path))
        handleRemoteFile(src_file_path);
    else
        handleExistingLocalFile(src_file_path, src_file_path); // src and local path and src are identically if src path not remote == local
}

QString FileModel::findBySrcMediaPath(QString src_file_path)
{
    i_available_media_list = available_media_list.find(src_file_path);
    if (i_available_media_list != available_media_list.end())
        return i_available_media_list.value().first;

    return "";
}


// =========================== protected methods ============================

void FileModel::handleRemoteFile(QString src_file_path)
{
    QFileInfo fi(src_file_path);
    if (fi.suffix() != "smil")
        src_file_path = determineHashedFilePath(src_file_path);
    MyDownloadQueue->insertQueue(src_file_path, cache_path+"/"+src_file_path);
}

void FileModel::handleExistingLocalFile(QString src_file_path, QString local_file_path)
{
    QString path = determineCachePathByMediaExtension(src_file_path, local_file_path);
    if (path == "")
        return;

    if (findBySrcMediaPath(src_file_path) == "")
        available_media_list.insert(src_file_path, qMakePair(path, _exist));
    else
        available_media_list[src_file_path] = qMakePair(path, _reloadable);
}

QString FileModel::determineCachePathByMediaExtension(QString src_file_path, QString local_file_path)
{
    QFileInfo fi(local_file_path);
    QString path = fi.absoluteFilePath();
    QString real_file_path("");
    if (fi.suffix() == "wgt")
    {
        Wgt MyWgt(local_file_path);
        qint64 i = MyWgt.calculateSize();
        if (i <= 0 || !MyDiscSpace.freeDiscSpace(i))
        {
            qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " could not be saved cause no Space could be freed";
            return "";
        }

        if (!MyWgt.extract())
        {
            qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " could not be saved cause wgt cannot extracted";
            return "";
        }
        #if defined  Q_OS_WIN
           real_file_path = "file:/"+path.mid(0, path.length()-4)+"/index.html"; // Windows needs file:// for opening absolute paths in WebEngine
        #else
            real_file_path = "file://"+path.mid(0, path.length()-4)+"/index.html"; // Linux needs file:/// for opening absolute paths in WebEngine
        #endif
    }
    else
        real_file_path = path;

    qInfo(ContentManager) << "OBJECT_UPDATED resourceURI:" << src_file_path << " contentLenght: " << fi.size();
    return real_file_path;
}

bool FileModel::saveToDisk(QString src_file_path, QString local_file_path, QIODevice *data)
{
    QFile file(local_file_path);
    if (!file.open(QIODevice::ReadWrite))
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " could not be saved " << file.errorString();
       // emit failed(src_file_path);
        return false;
    }
    qint64 calc = MyDiscSpace.calculateNeededDiscSpace(data->size());
    if (calc > 0 && !MyDiscSpace.freeDiscSpace(calc))
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << src_file_path << " could not be saved " << file.errorString() << "cause space cannot be freed";
        // emit failed(src_file_path);
        return false;
    }

    file.write(data->readAll()); // should overwrites automatic existing files
    file.close();
    return true;
}

QString FileModel::determineHashedFilePath(QString src_file_path)
{
    // ToDo Optimization: check if it is wise to get a md5-hash from complete file
    // pro:
    //      prevent multiple identically files on player
    // cons:
    //      files had to be downloaded in any case
    //      md5 of a 1GB videofile can be extremly time/resource consuming on weak devices like rpi zero
    QFileInfo fi(src_file_path);
    return QString(QCryptographicHash::hash((src_file_path.toUtf8()), QCryptographicHash::Md5).toHex())+ "."+fi.suffix();
}

// ================ slots ===========================

void FileModel::doSucceed(QString src_file_path, QString local_file_path, QIODevice *data)
{
    if (!saveToDisk(src_file_path, local_file_path, data))
        return;
    handleExistingLocalFile(src_file_path, local_file_path);
}

void FileModel::doNotCacheable(QString src_file_path)
{
    if (findBySrcMediaPath(src_file_path) == "")
        available_media_list.insert(src_file_path, qMakePair(src_file_path, _exist));
}
