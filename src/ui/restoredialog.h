#pragma once

#include <QDialog>
#include "domain/BackupJob.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RestoreDialog; }
QT_END_NAMESPACE

class RestoreDialog : public QDialog {
    Q_OBJECT

public:
    explicit RestoreDialog(const BackupJob& job, QWidget *parent = nullptr);
    ~RestoreDialog();

    std::string getSelectedPointId() const;

private slots:
    void onRestore();      // Слот для кнопки восстановления
    void onCancel();       // Слот для кнопки отмены
    void onSelectionChanged();  // Слот для активации кнопки при выборе

private:
    Ui::RestoreDialog *ui;
};