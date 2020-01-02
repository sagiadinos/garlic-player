#ifndef SAXPARSER_H
#define SAXPARSER_H

#include <QObject>

class SaxParser : public QObject
{
    Q_OBJECT
public:
    explicit SaxParser(QObject *parent = nullptr);

signals:

};

#endif // SAXPARSER_H
