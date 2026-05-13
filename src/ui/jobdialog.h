#pragma once

#include <QDialog>
#include <memory>
#include "domain/BackupJob.h"

QT_BEGIN_NAMESPACE
namespace Ui { class JobDialog; }
QT_END_NAMESPACE

class JobDialog : public QDialog {
    Q_OBJECT

public:
    explicit JobDialog(QWidget* parent = nullptr);
    explicit JobDialog(const BackupJob& job, QWidget* parent = nullptr);
    ~JobDialog();

    BackupJob getJob() const;
    void setEditingId(const std::string& id) { editingId = id; }

    bool isEditMode() const { return !editingId.empty(); }

private:
    Ui::JobDialog* ui;
    std::string editingId;
    std::unique_ptr<BackupJob> originalJob;  // Используем умный указатель

    int convertToSeconds(int value, QString type) const;
    void loadFromJob(const BackupJob& job);
    void setupForEditMode();
};
