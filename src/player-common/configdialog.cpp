#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent, TConfiguration *Config) :  QDialog(parent), ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    MyConfiguration = Config;
    ui->lineEditPlayerName->setText(MyConfiguration->getPlayerName());
    ui->lineEditContentUrl->setText(MyConfiguration->getIndexUri());
}


ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::accept()
{
    MyConfiguration->setPlayerName(ui->lineEditPlayerName->text());
    MyConfiguration->determineIndexUri(ui->lineEditContentUrl->text());
    QDialog::accept();
}
