#ifndef DEBUGINFOS_H
#define DEBUGINFOS_H

#include <QDialog>
#include <QMap>
#include "lib_facade.h"

#include "system_infos/memory.h"
#include "system_infos/general.h"
#include "media.h"

namespace Ui
{
    class DebugInfos;
}

#include "ui_debug_infos.h"

class DebugInfos : public QDialog
{
        Q_OBJECT

    public:
        explicit DebugInfos(LibFacade *lib_facade, QWidget *parent = Q_NULLPTR);
        void     setCurrentFilePlayed(TMedia *media);
        void     setLibFacade(LibFacade *lib_facade);
        ~DebugInfos();
        void outputResourcesUsage();
    protected:
        qint64                 max_memory_used = 0;
        qint64                 max_threads_used = 0;
        QMap<QString, QString> played_media;
        int                    timer_id;
        LibFacade             *MyLibFacade;
        SystemInfos::Memory    MyMemoryInfos;
        SystemInfos::General   MyGeneralInfos;
        void                   timerEvent(QTimerEvent *event);
        QString                preparePlayedMediaText(TMedia *media);
    private:
        Ui::DebugInfos *ui;
};

#endif // DEBUGINFOS_H
