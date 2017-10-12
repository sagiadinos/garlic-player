#include "index_manager.h"

IndexManager::IndexManager(IndexModel *im, TConfiguration *config, Network *nw, QObject *parent) : BaseManager(parent)
{
    MyConfiguration = config;
    MyNetwork       = nw;
    MyIndexModel    = im;
    connect(MyNetwork, SIGNAL(succeed(QObject *)), SLOT(doSucceed(QObject *)));

}

void IndexManager::init(QString src)
{
    src_index_path =src;
    loadLocal(MyConfiguration->getLastPlayedIndexPath()); // load last Index to be sure that the player can contunue working if Network failed
}

void IndexManager::lookUpForIndex()
{
    // if nothing is loaded look first at cache if there is an Index
    if (src_index_path == "")
    {
        qDebug(ContentManager) << "set index path first";
        return;
    }
    if (isRemote(src_index_path))
        MyNetwork->processFile(src_index_path, MyConfiguration->getPaths("cache")+"index.smil");
    else
        loadLocal(src_index_path); // src and local path are identically if src path local
}

QDomElement IndexManager::getHead()
{
    return MyIndexModel->getHead();
}

QDomElement IndexManager::getBody()
{
    return MyIndexModel->getBody();
}

// ==================  protected methods =======================================

void IndexManager::loadLocal(QString local_path)
{
    if (!MyIndexModel->loadDocument(local_path))
        return;
    MyConfiguration->setLastPlayedIndexPath(local_path);
    emit availableIndex();
}

// ==================  protected slots =======================================

void IndexManager::doSucceed(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    loadLocal(MyNetwork->getLocalFileInfo().absoluteFilePath());
}
