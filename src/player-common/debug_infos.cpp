#include "debug_infos.h"
#include "ui_debug_infos.h"

DebugInfos::DebugInfos(LibFacade *lib_facade, QWidget *parent) :  QDialog(parent), ui(new Ui::DebugInfos)
{
    setLibFacade(lib_facade);
    ui->setupUi(this);
    timer_id = startTimer(500);
    setWindowOpacity(0.9);
#if !defined  Q_OS_ANDROID
    // cause in Android it shows fullscreen and not as dialog
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Popup	);
#endif
}

void DebugInfos::setCurrentFilePlayed(TMedia *media)
{
    ui->CurrentFileInUse->setText(preparePlayedMediaText(media));
}

void DebugInfos::setLibFacade(LibFacade *lib_facade)
{
    MyLibFacade = lib_facade;
}


DebugInfos::~DebugInfos()
{
    delete ui;
    killTimer(timer_id);
}

void DebugInfos::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    outputResourcesUsage();
}

QString DebugInfos::preparePlayedMediaText(TMedia *media)
{
    QString key = media->getRegion();
    QString value = media->getTitle();

    if (value == "")
        value = media->getSrc();

    QMap<QString, QString>::iterator i = played_media.find(key);
    if (i != played_media.end())
        played_media[i.key()] = value;
    else
        played_media.insert(key, value);

    QString output = "";
    for (i = played_media.begin(); i != played_media.end(); i++)
    {
        output += i.key()+": " + i.value()+"\n";
    }
    return output;
}

void DebugInfos::outputResourcesUsage()
{
    MyMemoryInfos.refresh();

    qint64  total_mem = MyMemoryInfos.getTotal();
    qint64  free_mem = MyMemoryInfos.getFree();
    double d_total = (double)total_mem / (double)1048576;
    double d_free = (double)free_mem / (double)1048576;
    ui->TotalSystemMemory->setText(QString("Total Memory System:: %1" ).arg(d_total, 0, 'f', 2) + " MiB");
    ui->FreeSystemMemory->setText(QString("Free Memory System:: %1" ).arg(d_free, 0, 'f', 2) + " MiB");

    qint64  current_rss = MyMemoryInfos.getRSS();
    double d_current = (double)current_rss / (double)1048576;
    ui->OutputMemoryUse->setText(QString("App Memory use: <b>%1" ).arg(d_current, 0, 'f', 2) + " MiB</b>");
    if (current_rss > max_memory_used)
    {
        max_memory_used = current_rss;
        double d_max = (double)max_memory_used / (double)1048576;
        ui->MaxMemoryUsed->setText(QString("Max Memory App used: %1" ).arg(d_max, 0, 'f', 2) + " MiB (" + QTime::currentTime().toString() +")");
    }

    qint64  current_threads = MyGeneralInfos.countThreads();
    ui->ThreadsNumber->setText("Threads: " + QString::number(current_threads));
    if (current_threads > max_threads_used)
    {
        max_threads_used = current_threads;
        ui->MaxThreadsNumber->setText("Max Threads: " + QString::number(max_threads_used));
    }
    ui->PlaylistTitle->setText(MyLibFacade->getHead()->getTitle());

}
