#include "base_testsuite.hpp"
#include <QtTest/QtTest>
#include "tools/log_file.h"

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(LogFile *lf)
    {
        MyLogFile = lf;
    }
    qint64 getFileSize(){return file_size;}
protected:
    LogFile *MyLogFile;
    qint64   file_size = 0;
    void run()
    {
        QString str = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz";
        for (int i = 1; i < 16001; i++)
        {
            QString write = QString::number(i) + str;
            MyLogFile->write(write);
            file_size += write.length()+1; // cause of the endl
        }

    }
};

class TestLogFile : public BaseTestSuite
{
        Q_OBJECT
    public:
        TestLogFile();

    private Q_SLOTS:
        void cleanup();
        void testWriteFile();
        void testRotateFileLowerThanMaxNUM();
        void testRotateFileGreaterThanMaxNUM();
      //  void testWriteFileDuringInterupted();
    private:
        QString file_name = "test.log";
        void createLogFiles(int num_files);
        QStringList  readFileContents(QFile *file);
};
