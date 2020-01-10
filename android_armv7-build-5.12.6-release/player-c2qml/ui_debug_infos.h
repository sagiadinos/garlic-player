/********************************************************************************
** Form generated from reading UI file 'debug_infos.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUG_INFOS_H
#define UI_DEBUG_INFOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DebugInfos
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *PlaylistTitle;
    QLabel *CurrentFileInUse;
    QFrame *line;
    QLabel *TotalSystemMemory;
    QLabel *FreeSystemMemory;
    QLabel *OutputMemoryUse;
    QLabel *MaxMemoryUsed;
    QLabel *ThreadsNumber;
    QLabel *MaxThreadsNumber;

    void setupUi(QWidget *DebugInfos)
    {
        if (DebugInfos->objectName().isEmpty())
            DebugInfos->setObjectName(QString::fromUtf8("DebugInfos"));
        DebugInfos->resize(772, 260);
        gridLayout_2 = new QGridLayout(DebugInfos);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        PlaylistTitle = new QLabel(DebugInfos);
        PlaylistTitle->setObjectName(QString::fromUtf8("PlaylistTitle"));
        PlaylistTitle->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(PlaylistTitle);

        CurrentFileInUse = new QLabel(DebugInfos);
        CurrentFileInUse->setObjectName(QString::fromUtf8("CurrentFileInUse"));
        CurrentFileInUse->setAutoFillBackground(false);
        CurrentFileInUse->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(CurrentFileInUse);

        line = new QFrame(DebugInfos);
        line->setObjectName(QString::fromUtf8("line"));
        line->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        TotalSystemMemory = new QLabel(DebugInfos);
        TotalSystemMemory->setObjectName(QString::fromUtf8("TotalSystemMemory"));
        TotalSystemMemory->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(TotalSystemMemory);

        FreeSystemMemory = new QLabel(DebugInfos);
        FreeSystemMemory->setObjectName(QString::fromUtf8("FreeSystemMemory"));
        FreeSystemMemory->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(FreeSystemMemory);

        OutputMemoryUse = new QLabel(DebugInfos);
        OutputMemoryUse->setObjectName(QString::fromUtf8("OutputMemoryUse"));
        OutputMemoryUse->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(OutputMemoryUse);

        MaxMemoryUsed = new QLabel(DebugInfos);
        MaxMemoryUsed->setObjectName(QString::fromUtf8("MaxMemoryUsed"));
        MaxMemoryUsed->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(MaxMemoryUsed);

        ThreadsNumber = new QLabel(DebugInfos);
        ThreadsNumber->setObjectName(QString::fromUtf8("ThreadsNumber"));
        ThreadsNumber->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(ThreadsNumber);

        MaxThreadsNumber = new QLabel(DebugInfos);
        MaxThreadsNumber->setObjectName(QString::fromUtf8("MaxThreadsNumber"));
        MaxThreadsNumber->setStyleSheet(QString::fromUtf8("background-color: transparent;"));

        verticalLayout->addWidget(MaxThreadsNumber);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(DebugInfos);

        QMetaObject::connectSlotsByName(DebugInfos);
    } // setupUi

    void retranslateUi(QWidget *DebugInfos)
    {
        DebugInfos->setWindowTitle(QApplication::translate("DebugInfos", "MainWindow", nullptr));
        PlaylistTitle->setText(QApplication::translate("DebugInfos", "-", nullptr));
        CurrentFileInUse->setText(QApplication::translate("DebugInfos", "-", nullptr));
        TotalSystemMemory->setText(QApplication::translate("DebugInfos", "Total Memory System:", nullptr));
        FreeSystemMemory->setText(QApplication::translate("DebugInfos", "Free Memory System:", nullptr));
        OutputMemoryUse->setText(QApplication::translate("DebugInfos", "Memory App use:", nullptr));
        MaxMemoryUsed->setText(QApplication::translate("DebugInfos", "Max Memory App used:", nullptr));
        ThreadsNumber->setText(QApplication::translate("DebugInfos", "Threads:", nullptr));
        MaxThreadsNumber->setText(QApplication::translate("DebugInfos", "Max Threads:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DebugInfos: public Ui_DebugInfos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUG_INFOS_H
