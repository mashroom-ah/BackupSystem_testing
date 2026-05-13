/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *vboxLayout;
    QTableWidget *jobTable;
    QHBoxLayout *hboxLayout;
    QPushButton *createBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QPushButton *runBtn;
    QPushButton *restoreBtn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(768, 303);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        vboxLayout = new QVBoxLayout(centralwidget);
        vboxLayout->setObjectName("vboxLayout");
        jobTable = new QTableWidget(centralwidget);
        jobTable->setObjectName("jobTable");

        vboxLayout->addWidget(jobTable);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        createBtn = new QPushButton(centralwidget);
        createBtn->setObjectName("createBtn");

        hboxLayout->addWidget(createBtn);

        editBtn = new QPushButton(centralwidget);
        editBtn->setObjectName("editBtn");

        hboxLayout->addWidget(editBtn);

        deleteBtn = new QPushButton(centralwidget);
        deleteBtn->setObjectName("deleteBtn");

        hboxLayout->addWidget(deleteBtn);

        runBtn = new QPushButton(centralwidget);
        runBtn->setObjectName("runBtn");

        hboxLayout->addWidget(runBtn);

        restoreBtn = new QPushButton(centralwidget);
        restoreBtn->setObjectName("restoreBtn");

        hboxLayout->addWidget(restoreBtn);


        vboxLayout->addLayout(hboxLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Backup System", nullptr));
        createBtn->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        editBtn->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        runBtn->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \320\272\320\276\320\277\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
        restoreBtn->setText(QCoreApplication::translate("MainWindow", "\320\222\320\276\321\201\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
