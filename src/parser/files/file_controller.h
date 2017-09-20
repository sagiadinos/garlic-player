#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include <QObject>
#include "file_model.h"
#include "configuration.h"

/**
 * @brief The FileController class is the "interface" to media parser
 * - has a list of all playlist files
 * - has a key value list of available files
 * - get from parser a file request and delegates it to FileModel for retrieve when not available
 * - answers to a parser request with a playable filepath
 */
class FileController : public QObject
{
    Q_OBJECT
public:
    explicit FileController(TConfiguration *config);
    ~FileController();
    void                             registerFile(QString src);
    QString                          requestLoadablePath(QString src);
    int                              checkCacheStatus(QString src);

protected:
    TConfiguration                  *MyConfiguration;
    FileModel                       *MyFileModel;
protected slots:

};

#endif // FILE_CONTROLLER_H
