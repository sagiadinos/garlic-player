#ifndef MEDIA_MODEL_H
#define MEDIA_MODEL_H

#include <QObject>
#include <QMap>

#include "configuration.h"
#include "network_queue.h"
#include "disc_space.h"
#include "files/wgt.h"

/**
 * @brief The FileModel class handles the file management and use the DiscSpace-class as helper
 *
 * - handles different media files eg extracting of wgt
 * - organize a Map of available Files with their "real" path and statuses
 */
class MediaModel : public QObject
{
    Q_OBJECT
    public:
        const     int        _no_exist   = 0;
        const     int        _exist      = 1;
        const     int        _reloadable = 2;
        const     int        _uncachable = 3;

        explicit MediaModel();
        ~MediaModel();
        void     clearQueue();
        QString  findLocalBySrcPath(QString src_file_path);
        int      findStatusBySrcPath(QString src_file_path);
        void     insertAvaibleLink(QString src_file_path);
        void     insertAvaibleFile(QString src_file_path, QString local_file_path);

        //Getter/Setter
        QMap<QString, QPair<QString, int> >           getAvailableMediaList() const {return available_media_list;}
        void                                          setAvailableMediaList(const QMap<QString, QPair<QString, int> > &value) {available_media_list = value;}
        QString                                       determineHashedFilePath(QString src_file_path);

    protected:
        QMap<QString, QPair<QString, int>>            available_media_list;
        QMap<QString, QPair<QString, int>>::iterator  i_available_media_list;
        QString                                       determinePathByMedia(QString src_file_path, QString local_file_path);

};

#endif // MEDIA_MODEL_H
