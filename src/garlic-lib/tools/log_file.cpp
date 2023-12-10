#include "log_file.h"

LogFile::LogFile(QString file_path, QObject *parent) : QObject(parent)
{
    log_file.reset(new QFile(file_path));
    log_file_info.reset(new QFileInfo(file_path));
    log_file.data()->open(QFile::Append | QFile::Text);
    out.setDevice(log_file.data());

}

void LogFile::setMaxSize(qint64 size)
{
    max_size = size;
}

void LogFile::setMaxNumber(qint64 number)
{
    max_number = number;
}

void LogFile::write(QString data)
{
    out << data << Qt::endl;
    if (isSizeExeeds() && !is_rotated)
    {
        rotateFile();
    }
    else
        out.flush();
}

bool LogFile::isSizeExeeds()
{
    return (log_file.data()->size() >= max_size);
}

void LogFile::rotateFile()
{
    out.setString(&buffer); // to buffer during rotating calls automatic flush

    is_rotated = true;
    QString file_path = log_file_info.data()->absoluteFilePath();
    log_file->close();
    QDir dir(log_file_info.data()->absoluteDir());

    // find all a files with this name
   foreach (const QString &match,  dir.entryList(QStringList(log_file_info.data()->baseName()+"*"), QDir::Files, QDir::Time | QDir::Reversed))
   {
        QStringList list = match.split(".");
        int num = list.at(list.size()-1).toInt();

        if (num == 0)
            QFile::rename(file_path, file_path+".1");
        else if (num < max_number)
            QFile::rename(file_path+"."+QString::number(num), file_path+"."+QString::number(num+1));
        else
            QFile::remove(file_path+"."+QString::number(num));
   }

   log_file.reset(new QFile(file_path));
   log_file_info.reset(new QFileInfo(file_path));
   log_file.data()->open(QFile::Append | QFile::Text);
   out.setDevice(log_file.data());
   is_rotated = false;

   // Write buffer at the end of old file
   QFile log_file_old(file_path+".1");
   log_file_old.open(QFile::Append | QFile::Text);
   log_file_old.write(buffer.toUtf8());
   log_file_old.close();
}

