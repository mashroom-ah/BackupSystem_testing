/********************************************************************************
** Form generated from reading UI file 'restoredialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTOREDIALOG_H
#define UI_RESTOREDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RestoreDialog
{
public:
    QVBoxLayout *vboxLayout;
    QListWidget *listWidget;
    QHBoxLayout *hboxLayout;
    QPushButton *restoreBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *RestoreDialog)
    {
        if (RestoreDialog->objectName().isEmpty())
            RestoreDialog->setObjectName("RestoreDialog");
        RestoreDialog->setMinimumSize(QSize(400, 300));
        vboxLayout = new QVBoxLayout(RestoreDialog);
        vboxLayout->setObjectName("vboxLayout");
        listWidget = new QListWidget(RestoreDialog);
        listWidget->setObjectName("listWidget");

        vboxLayout->addWidget(listWidget);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        restoreBtn = new QPushButton(RestoreDialog);
        restoreBtn->setObjectName("restoreBtn");
        restoreBtn->setEnabled(false);

        hboxLayout->addWidget(restoreBtn);

        cancelBtn = new QPushButton(RestoreDialog);
        cancelBtn->setObjectName("cancelBtn");

        hboxLayout->addWidget(cancelBtn);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(RestoreDialog);

        QMetaObject::connectSlotsByName(RestoreDialog);
    } // setupUi

    void retranslateUi(QDialog *RestoreDialog)
    {
        RestoreDialog->setWindowTitle(QCoreApplication::translate("RestoreDialog", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \321\202\320\276\321\207\320\272\321\203 \320\262\320\276\321\201\321\201\321\202\320\260\320\275\320\276\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));
        restoreBtn->setText(QCoreApplication::translate("RestoreDialog", "\320\222\320\276\321\201\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        cancelBtn->setText(QCoreApplication::translate("RestoreDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RestoreDialog: public Ui_RestoreDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTOREDIALOG_H
