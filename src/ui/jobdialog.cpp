#include "ui/jobdialog.h"
#include "ui_JobDialog.h"

#include <QFileDialog>
#include <QMessageBox>

JobDialog::JobDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::JobDialog), editingId(""), originalJob(nullptr) {

    ui->setupUi(this);

    connect(ui->sourceBtn, &QPushButton::clicked, this, [=]() {
        ui->sourceEdit->setText(QFileDialog::getExistingDirectory());
    });

    connect(ui->targetBtn, &QPushButton::clicked, this, [=]() {
        ui->targetEdit->setText(QFileDialog::getExistingDirectory());
    });

    connect(ui->okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

JobDialog::JobDialog(const BackupJob& job, QWidget* parent)
    : JobDialog(parent) {
    editingId = job.getId();
    originalJob = std::make_unique<BackupJob>(job);  // Копируем задачу
    loadFromJob(job);
    setupForEditMode();
}

JobDialog::~JobDialog() {
    delete ui;
}

int JobDialog::convertToSeconds(int value, QString type) const {
    if (type == "Минуты") return value * 60;
    if (type == "Часы") return value * 3600;
    if (type == "Дни") return value * 86400;
    return value;
}

void JobDialog::setupForEditMode() {
    // Блокируем поля с путями при редактировании
    ui->sourceEdit->setReadOnly(true);
    ui->targetEdit->setReadOnly(true);
    ui->sourceBtn->setEnabled(false);
    ui->targetBtn->setEnabled(false);

    // Добавляем пояснение
    ui->sourceEdit->setToolTip("Путь к источнику нельзя изменить при редактировании");
    ui->targetEdit->setToolTip("Путь к хранилищу нельзя изменить при редактировании");
}

void JobDialog::loadFromJob(const BackupJob& job) {
    ui->nameEdit->setText(QString::fromStdString(job.getName()));
    ui->sourceEdit->setText(QString::fromStdString(job.getSource().string()));
    ui->targetEdit->setText(QString::fromStdString(job.getTarget().string()));

    int sec = job.getSchedule().getInterval();

    if (sec % 86400 == 0) {
        ui->intervalSpin->setValue(sec / 86400);
        ui->intervalType->setCurrentText("Дни");
    } else if (sec % 3600 == 0) {
        ui->intervalSpin->setValue(sec / 3600);
        ui->intervalType->setCurrentText("Часы");
    } else {
        ui->intervalSpin->setValue(sec / 60);
        ui->intervalType->setCurrentText("Минуты");
    }

    ui->enabledCheck->setChecked(job.isEnabled());

    auto& r = job.getRetention();
    ui->maxSizeSpin->setValue(r.getMaxStorageSize());
    ui->maxDaysSpin->setValue(r.getMaxDays());
}

BackupJob JobDialog::getJob() const {
    std::string id = editingId.empty() ? std::to_string(std::time(nullptr)) : editingId;

    Schedule schedule(
        convertToSeconds(ui->intervalSpin->value(),
                         ui->intervalType->currentText()),
        ui->enabledCheck->isChecked()
        );

    RetentionPolicy retention;
    retention.changeStorageSize(ui->maxSizeSpin->value());
    retention.changeMaxDays(ui->maxDaysSpin->value());

    // В режиме редактирования используем исходные пути
    std::filesystem::path source;
    std::filesystem::path target;

    if (editingId.empty()) {
        // Создание новой задачи
        source = std::filesystem::path(ui->sourceEdit->text().toStdString());
        target = std::filesystem::path(ui->targetEdit->text().toStdString());
    } else {
        // Редактирование - берем пути из оригинала
        source = originalJob->getSource();
        target = originalJob->getTarget();
    }

    BackupJob newJob(
        id,
        ui->nameEdit->text().toStdString(),
        source,
        target,
        schedule,
        retention
        );

    // В режиме редактирования восстанавливаем точки восстановления
    if (!editingId.empty() && originalJob) {
        for (const auto& point : originalJob->getRestorePoints()) {
            newJob.addRestorePoint(point);
        }
    }

    // Сохраняем статус enabled
    if (ui->enabledCheck->isChecked()) {
        newJob.enable();
    } else {
        newJob.disable();
    }

    return newJob;
}
