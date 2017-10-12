#include "base_manager.h"

BaseManager::BaseManager(QObject *parent) : QObject(parent)
{
    setParent(parent);
}


// ==================  protected methods =======================================

bool BaseManager::isRemote(QString src)
{
    if (src.mid(0, 4) == "http" || src.mid(0,3) == "ftp")
        return true;
    else
        return false;
}
