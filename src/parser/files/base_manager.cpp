#include "base_manager.h"

BaseManager::BaseManager()
{

}


// ==================  protected methods =======================================

bool BaseManager::isRemote(QString src)
{
    if (src.mid(0, 4) == "http" || src.mid(0,3) == "ftp")
        return true;
    else
        return false;
}
