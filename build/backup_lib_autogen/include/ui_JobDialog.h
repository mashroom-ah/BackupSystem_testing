/********************************************************************************
** Form generated from reading UI file 'jobdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOBDIALOG_H
#define UI_JOBDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_JobDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLineEdit *nameEdit;
    QHBoxLayout *hboxLayout;
    QLineEdit *sourceEdit;
    QPushButton *sourceBtn;
    QHBoxLayout *hboxLayout1;
    QLineEdit *targetEdit;
    QPushButton *targetBtn;
    QHBoxLayout *hboxLayout2;
    QSpinBox *intervalSpin;
    QComboBox *intervalType;
    QCheckBox *enabledCheck;
    QSpinBox *maxSizeSpin;
    QSpinBox *maxDaysSpin;
    QHBoxLayout *hboxLayout3;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *JobDialog)
    {
        if (JobDialog->objectName().isEmpty())
            JobDialog->setObjectName("JobDialog");
        vboxLayout = new QVBoxLayout(JobDialog);
        vboxLayout->setObjectName("vboxLayout");
        nameEdit = new QLineEdit(JobDialog);
        nameEdit->setObjectName("nameEdit");

        vboxLayout->addWidget(nameEdit);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName("hboxLayout");
        sourceEdit = new QLineEdit(JobDialog);
        sourceEdit->setObjectName("sourceEdit");

        hboxLayout->addWidget(sourceEdit);

        sourceBtn = new QPushButton(JobDialog);
        sourceBtn->setObjectName("sourceBtn");

        hboxLayout->addWidget(sourceBtn);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName("hboxLayout1");
        targetEdit = new QLineEdit(JobDialog);
        targetEdit->setObjectName("targetEdit");

        hboxLayout1->addWidget(targetEdit);

        targetBtn = new QPushButton(JobDialog);
        targetBtn->setObjectName("targetBtn");

        hboxLayout1->addWidget(targetBtn);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName("hboxLayout2");
        intervalSpin = new QSpinBox(JobDialog);
        intervalSpin->setObjectName("intervalSpin");

        hboxLayout2->addWidget(intervalSpin);

        intervalType = new QComboBox(JobDialog);
        intervalType->addItem(QString());
        intervalType->addItem(QString());
        intervalType->addItem(QString());
        intervalType->setObjectName("intervalType");

        hboxLayout2->addWidget(intervalType);


        vboxLayout->addLayout(hboxLayout2);

        enabledCheck = new QCheckBox(JobDialog);
        enabledCheck->setObjectName("enabledCheck");

        vboxLayout->addWidget(enabledCheck);

        maxSizeSpin = new QSpinBox(JobDialog);
        maxSizeSpin->setObjectName("maxSizeSpin");

        vboxLayout->addWidget(maxSizeSpin);

        maxDaysSpin = new QSpinBox(JobDialog);
        maxDaysSpin->setObjectName("maxDaysSpin");

        vboxLayout->addWidget(maxDaysSpin);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName("hboxLayout3");
        okBtn = new QPushButton(JobDialog);
        okBtn->setObjectName("okBtn");

        hboxLayout3->addWidget(okBtn);

        cancelBtn = new QPushButton(JobDialog);
        cancelBtn->setObjectName("cancelBtn");

        hboxLayout3->addWidget(cancelBtn);


        vboxLayout->addLayout(hboxLayout3);


        retranslateUi(JobDialog);

        QMetaObject::connectSlotsByName(JobDialog);
    } // setupUi

    void retranslateUi(QDialog *JobDialog)
    {
        JobDialog->setWindowTitle(QCoreApplication::translate("JobDialog", "\320\241\320\276\320\267\320\264\320\260\320\275\320\270\320\265 / \320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\267\320\260\320\264\320\260\321\207\320\270", nullptr));
        nameEdit->setPlaceholderText(QCoreApplication::translate("JobDialog", "\320\230\320\274\321\217 \320\267\320\260\320\264\320\260\321\207\320\270", nullptr));
        sourceBtn->setText(QCoreApplication::translate("JobDialog", "\320\230\321\201\321\202\320\276\321\207\320\275\320\270\320\272", nullptr));
        targetBtn->setText(QCoreApplication::translate("JobDialog", "\320\245\321\200\320\260\320\275\320\270\320\273\320\270\321\211\320\265", nullptr));
        intervalType->setItemText(0, QCoreApplication::translate("JobDialog", "\320\234\320\270\320\275\321\203\321\202\321\213", nullptr));
        intervalType->setItemText(1, QCoreApplication::translate("JobDialog", "\320\247\320\260\321\201\321\213", nullptr));
        intervalType->setItemText(2, QCoreApplication::translate("JobDialog", "\320\224\320\275\320\270", nullptr));

        enabledCheck->setText(QCoreApplication::translate("JobDialog", "\320\222\320\272\320\273\321\216\321\207\320\265\320\275\320\260", nullptr));
        maxSizeSpin->setPrefix(QCoreApplication::translate("JobDialog", "\320\234\320\260\320\272\321\201 \321\200\320\260\320\267\320\274\320\265\321\200: ", nullptr));
        maxDaysSpin->setPrefix(QCoreApplication::translate("JobDialog", "\320\234\320\260\320\272\321\201 \320\264\320\275\320\270: ", nullptr));
        okBtn->setText(QCoreApplication::translate("JobDialog", "OK", nullptr));
        cancelBtn->setText(QCoreApplication::translate("JobDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JobDialog: public Ui_JobDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOBDIALOG_H
