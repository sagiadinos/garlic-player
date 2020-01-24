#ifndef CMDPARSER_H
#define CMDPARSER_H

#include <QObject>
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include "tools/main_configuration.h"
class TCmdParser : public QObject

{
    Q_OBJECT
public:
    TCmdParser(MainConfiguration *config);
    QString getWindowMode() const;
    void setWindowMode(const QString &value);

    QSize getWindowSize();
    void setWindowSize(QSize value);

    int getScreenSelect();
    void setScreenSelect(int value);

    QString getIndexUrl() const;
    void setIndexUrl(const QString &value);

    void addOptions();
    bool parse(QApplication *app);
protected:
    QString              window_mode   = "windowed";
    QSize                window_size   = QSize(980,540);
    int                  screen_select = 0;
    QString              index_url     = "";
    QCommandLineParser   parser;
    MainConfiguration       *MyConfiguration;

};

#endif // CMDPARSER_H
