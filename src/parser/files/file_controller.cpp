#include "file_controller.h"

FileController::FileController(TConfiguration *config)
{
    MyConfiguration = config;
    //MyFileModel     = new FileModel(MyConfiguration);
}

FileController::~FileController()
{
}

void FileController::registerFile(QString src)
{
    //MyFileModel->registerFile(src);
}

int FileController::checkCacheStatus(QString src)
{
}

QString FileController::requestLoadablePath(QString src)
{
//    MyFileModel->re
}

// ==================  protected methods =======================================

