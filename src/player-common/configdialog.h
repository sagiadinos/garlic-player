#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "tools/configuration.h"
#include <QDialog>

namespace Ui
{
    class ConfigDialog;
}

#include "ui_configdialog.h"

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent, TConfiguration *Config);
    ~ConfigDialog();

private:
    Ui::ConfigDialog *ui;
    TConfiguration   *MyConfiguration = NULL;

private slots:
    void accept();

};

#endif // CONFIGDIALOG_H
