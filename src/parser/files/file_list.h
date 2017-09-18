#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <QObject>

class file_list : public QObject
{
    Q_OBJECT
public:
    explicit file_list(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FILE_LIST_H