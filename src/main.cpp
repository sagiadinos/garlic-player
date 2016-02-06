#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Configuration conf;
    qDebug() << conf.getUserAgent();
    return a.exec();
}

