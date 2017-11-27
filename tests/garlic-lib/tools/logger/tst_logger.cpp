#include <QString>
#include <QtTest>
#include "tools/logger.h"

class TestLogger : public QObject
{
    Q_OBJECT

public:
    TestLogger();

private Q_SLOTS:
    void cleanup();
    void testRotateFileLowerThanMaxNUM();
    void testRotateFileGreaterThanMaxNUM();
private:
    QString file_name = "test.log";
    void createLogFiles(int num_files);
};

TestLogger::TestLogger()
{
}

void TestLogger::cleanup()
{
    QDir dir(".");
    QFile   file;
    // find all a files with this name
   foreach (const QString &match,  dir.entryList(QStringList(file_name+"*"), QDir::Files, QDir::Time | QDir::Reversed))
   {
        file.setFileName(match);
        file.remove();
   }
}

void TestLogger::testRotateFileLowerThanMaxNUM()
{
    Logger& MyLogger = Logger::getInstance();
    QFile   file;
    int num_files = 5;

    createLogFiles(num_files);
    QDir dir(".");
    QStringList list = dir.entryList(QStringList(file_name+"*"), QDir::Files, QDir::Time);
    QCOMPARE(list.size(), num_files+1);

    QFile test_file(file_name);
    MyLogger.rotateFile(&test_file);
    for (int i = num_files+1; i > -1; i--)
    {
        if (i != 0)
            file.setFileName(file_name+"."+QString::number(i));
        else
            file.setFileName(file_name);

        QVERIFY(file.exists());
        if (file.open(QIODevice::ReadOnly) )
        {
            QByteArray result = "";
            QByteArray expected = "";
            if (i != 0)
            {
                result = file.read(3);
                expected = QByteArray::number(i-1)+"\n";
            }
            else
            {
                result = file.read(3);
                expected = "";
            }
            QVERIFY(result == expected);
            file.close();
        }
    }
    list = dir.entryList(QStringList(file_name+"*"), QDir::Files, QDir::Time);
    QCOMPARE(list.size(), num_files+2);

 }


void TestLogger::testRotateFileGreaterThanMaxNUM()
{
    Logger& MyLogger = Logger::getInstance();
    QFile   file;
    int num_files = MyLogger.MAX_LOG_FILE_NUMBER; // Should create extact MAX_LOG..!

    createLogFiles(num_files);
    QDir dir(".");
    QStringList list = dir.entryList(QStringList(file_name+"*"), QDir::Files, QDir::Time);
    QCOMPARE(list.size(), num_files+1);

    QFile test_file(file_name);
    MyLogger.rotateFile(&test_file);

    for (int i = num_files+1; i > -1; i--)
    {
        if (i != 0)
            file.setFileName(file_name+"."+QString::number(i));
        else
            file.setFileName(file_name);

        if (i == num_files+1)
            QVERIFY(!file.exists());
        else
        {
            QVERIFY(file.exists());
            if (file.open(QIODevice::ReadOnly) )
            {
                QByteArray result = "";
                QByteArray expected = "";
                if (i != 0)
                {
                    result = file.read(3);
                    expected = QByteArray::number(i-1)+"\n";
                }
                else
                {
                    result = file.read(3);
                    expected = "";
                }
                QVERIFY(result == expected);
                file.close();
            }
        }
    }
    list = dir.entryList(QStringList(file_name+"*"), QDir::Files, QDir::Time);
    QCOMPARE(list.size(), num_files+1);
}


void TestLogger::createLogFiles(int num_files)
{
    QFile file;
    for (int i = num_files; i > -1; i--)
    {
        if (i > 0)
            file.setFileName(file_name+ "."+ QString::number(i));
        else
           file.setFileName(file_name);

        if (file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream(&file);
            stream << QString::number(i) << endl;
            file.close();
        }
        QTest::qWait(10); // only 1o msec to get sure that files are not created same time
    }

}



QTEST_MAIN(TestLogger)

#include "tst_logger.moc"
