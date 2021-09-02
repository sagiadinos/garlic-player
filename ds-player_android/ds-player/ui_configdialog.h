/********************************************************************************
** Form generated from reading UI file 'configdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelErrorMessage;
    QLabel *labelPlayerName;
    QLineEdit *lineEditPlayerName;
    QLabel *labelContentUrl;
    QLineEdit *lineEditContentUrl;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QString::fromUtf8("ConfigDialog"));
        ConfigDialog->resize(621, 196);
        gridLayout = new QGridLayout(ConfigDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelErrorMessage = new QLabel(ConfigDialog);
        labelErrorMessage->setObjectName(QString::fromUtf8("labelErrorMessage"));
        labelErrorMessage->setStyleSheet(QString::fromUtf8("color:rgb(255, 0, 0)"));

        verticalLayout->addWidget(labelErrorMessage);

        labelPlayerName = new QLabel(ConfigDialog);
        labelPlayerName->setObjectName(QString::fromUtf8("labelPlayerName"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelPlayerName->sizePolicy().hasHeightForWidth());
        labelPlayerName->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(labelPlayerName);

        lineEditPlayerName = new QLineEdit(ConfigDialog);
        lineEditPlayerName->setObjectName(QString::fromUtf8("lineEditPlayerName"));
        lineEditPlayerName->setMouseTracking(true);
        lineEditPlayerName->setTabletTracking(true);
        lineEditPlayerName->setFocusPolicy(Qt::StrongFocus);
        lineEditPlayerName->setAcceptDrops(false);
#ifndef QT_NO_TOOLTIP
        lineEditPlayerName->setToolTip(QString::fromUtf8(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_ACCESSIBILITY
        lineEditPlayerName->setAccessibleName(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        lineEditPlayerName->setAccessibleDescription(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
        lineEditPlayerName->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        lineEditPlayerName->setInputMethodHints(Qt::ImhNoAutoUppercase|Qt::ImhNoEditMenu|Qt::ImhNoPredictiveText|Qt::ImhNoTextHandles);
        lineEditPlayerName->setInputMask(QString::fromUtf8(""));
        lineEditPlayerName->setText(QString::fromUtf8(""));
        lineEditPlayerName->setPlaceholderText(QString::fromUtf8(""));
        lineEditPlayerName->setCursorMoveStyle(Qt::LogicalMoveStyle);

        verticalLayout->addWidget(lineEditPlayerName);

        labelContentUrl = new QLabel(ConfigDialog);
        labelContentUrl->setObjectName(QString::fromUtf8("labelContentUrl"));
        sizePolicy.setHeightForWidth(labelContentUrl->sizePolicy().hasHeightForWidth());
        labelContentUrl->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(labelContentUrl);

        lineEditContentUrl = new QLineEdit(ConfigDialog);
        lineEditContentUrl->setObjectName(QString::fromUtf8("lineEditContentUrl"));
        lineEditContentUrl->setMouseTracking(true);
        lineEditContentUrl->setTabletTracking(true);
        lineEditContentUrl->setFocusPolicy(Qt::StrongFocus);
        lineEditContentUrl->setAcceptDrops(false);
#ifndef QT_NO_TOOLTIP
        lineEditContentUrl->setToolTip(QString::fromUtf8(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_ACCESSIBILITY
        lineEditContentUrl->setAccessibleName(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        lineEditContentUrl->setAccessibleDescription(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
        lineEditContentUrl->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        lineEditContentUrl->setInputMethodHints(Qt::ImhNoAutoUppercase|Qt::ImhNoEditMenu|Qt::ImhNoPredictiveText|Qt::ImhNoTextHandles);
        lineEditContentUrl->setInputMask(QString::fromUtf8(""));
        lineEditContentUrl->setText(QString::fromUtf8(""));
        lineEditContentUrl->setPlaceholderText(QString::fromUtf8(""));
        lineEditContentUrl->setCursorMoveStyle(Qt::LogicalMoveStyle);

        verticalLayout->addWidget(lineEditContentUrl);

        buttonBox = new QDialogButtonBox(ConfigDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setEnabled(true);
        QFont font;
        font.setKerning(true);
        buttonBox->setFont(font);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        QWidget::setTabOrder(lineEditPlayerName, lineEditContentUrl);

        retranslateUi(ConfigDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConfigDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "Dialog", nullptr));
        labelErrorMessage->setText(QString());
        labelPlayerName->setText(QApplication::translate("ConfigDialog", "<html><head/><body><p>Player name</p></body></html>", nullptr));
        labelContentUrl->setText(QApplication::translate("ConfigDialog", "<html><head/><body><p>Content-URL (You can use SmilControl http://indexes.smil-control.com)</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
