#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QtConcurrent>

#include "ui/jobdialog.h"
#include "ui/restoredialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    connect(ui->createBtn, &QPushButton::clicked, this, &MainWindow::onCreate);
    connect(ui->editBtn, &QPushButton::clicked, this, &MainWindow::onEdit);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onRun);
    connect(ui->restoreBtn, &QPushButton::clicked, this, &MainWindow::onRestore);

    timer.start(10000);
    connect(&timer, &QTimer::timeout, this, &MainWindow::onTimer);

    // ЗАГРУЖАЕМ СОХРАНЁННЫЕ ЗАДАЧИ
    facade.loadJobs();

    loadJobs();  // Отображаем в таблице
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadJobs() {
    auto& jobs = facade.getJobs();

    ui->jobTable->setRowCount(jobs.size());
    ui->jobTable->setColumnCount(6);

    ui->jobTable->setHorizontalHeaderLabels({
        "Имя", "Источник", "Хранилище", "Период (сек)", "Статус", "Размер (МБ)"
    });

    for (int i = 0; i < jobs.size(); i++) {
        auto& job = jobs[i];

        ui->jobTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(job.getName())));
        ui->jobTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(job.getSource().string())));
        ui->jobTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(job.getTarget().string())));
        ui->jobTable->setItem(i, 3, new QTableWidgetItem(QString::number(job.getSchedule().getInterval())));
        ui->jobTable->setItem(i, 4, new QTableWidgetItem(job.isEnabled() ? "Включена" : "Выключена"));
        ui->jobTable->setItem(i, 5, new QTableWidgetItem(QString::number(job.getRetention().getMaxStorageSize())));
    }
}

void MainWindow::onCreate() {
    JobDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        auto job = dialog.getJob();
        facade.createJob(job);
        loadJobs();
    }
}

void MainWindow::onEdit() {
    int row = ui->jobTable->currentRow();
    if (row < 0) return;

    auto& job = facade.getJobs()[row];

    // Сохраняем ID и точки восстановления оригинальной задачи
    JobDialog dialog(job, this);

    if (dialog.exec() == QDialog::Accepted) {
        auto updatedJob = dialog.getJob();

        // Убеждаемся, что ID не изменился
        if (updatedJob.getId() != job.getId()) {
            QMessageBox::warning(this, "Ошибка", "ID задачи не должен меняться!");
            return;
        }

        facade.updateJob(updatedJob);
        loadJobs();

        QMessageBox::information(this, "Успех",
                                 "Задача обновлена. Точки восстановления сохранены.");
    }
}

void MainWindow::onDelete() {
    int row = ui->jobTable->currentRow();
    if (row < 0) return;

    auto id = facade.getJobs()[row].getId();
    facade.deleteJob(id);

    loadJobs();
}

void MainWindow::onRun() {
    int row = ui->jobTable->currentRow();
    if (row < 0) return;

    auto id = facade.getJobs()[row].getId();

    QFuture<void> future = QtConcurrent::run([=]() {
        try {
            facade.runBackup(id);

            QMetaObject::invokeMethod(this, [=]() {
                QMessageBox::information(this, "Успех", "Резервное копирование завершено");
            });

        } catch (...) {
            QMetaObject::invokeMethod(this, [=]() {
                QMessageBox::critical(this, "Ошибка", "Ошибка при копировании. Закройте файлы.");
            });
        }
    });
    static_cast<void>(future);
}

void MainWindow::onRestore() {
    int row = ui->jobTable->currentRow();
    if (row < 0) return;

    auto& job = facade.getJobs()[row];

    RestoreDialog dialog(job, this);

    if (dialog.exec() == QDialog::Accepted) {
        auto pointId = dialog.getSelectedPointId();

        QFuture<void> future = QtConcurrent::run([=]() {
            try {
                facade.restore(job.getId(), pointId);

                QMetaObject::invokeMethod(this, [=]() {
                    QMessageBox::information(this, "Успех", "Восстановление завершено");
                });

            } catch (...) {
                QMetaObject::invokeMethod(this, [=]() {
                    QMessageBox::critical(this, "Ошибка", "Ошибка восстановления");
                });
            }
        });
        static_cast<void>(future);
    }
}

void MainWindow::onTimer() {
    auto& jobs = facade.getJobs();

    for (auto& job : jobs) {
        //  Проверяем enabled и у BackupJob, и у Schedule
        if (job.isEnabled() && job.getSchedule().shouldRun(std::chrono::system_clock::now())) {
            QFuture<void> future = QtConcurrent::run([=]() mutable {
                facade.runBackup(job.getId());
            });
            static_cast<void>(future);
        }
    }
}
