#include "ui/restoredialog.h"
#include "ui_restoredialog.h"

#include <QListWidgetItem>
#include <QDateTime>
#include <QMessageBox>

RestoreDialog::RestoreDialog(const BackupJob& job, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::RestoreDialog)
{
    ui->setupUi(this);

    // Подключаем сигналы
    connect(ui->restoreBtn, &QPushButton::clicked, this, &RestoreDialog::onRestore);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &RestoreDialog::onCancel);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, 
            this, &RestoreDialog::onSelectionChanged);

    // Заполняем список точек восстановления
    const auto& points = job.getRestorePoints();
    
    if (points.empty()) {
        // Если нет точек восстановления, показываем сообщение
        ui->listWidget->addItem("Нет доступных точек восстановления");
        ui->restoreBtn->setEnabled(false);
    } else {
        for (const auto& point : points) {
            auto time = point.getTimestamp();
            auto tt = std::chrono::system_clock::to_time_t(time);

            QString text = QDateTime::fromSecsSinceEpoch(tt)
                               .toString("dd.MM.yyyy HH:mm:ss");

            auto *item = new QListWidgetItem(text);
            
            // Добавляем информацию о типе бэкапа (полный/инкрементальный)
            // если есть такая информация в RestorePoint
            // item->setText(text + " (Full)"); // или (Incremental)

            // сохраняем ID внутри элемента
            item->setData(Qt::UserRole, QString::fromStdString(point.getId()));

            ui->listWidget->addItem(item);
        }
    }
}

RestoreDialog::~RestoreDialog() {
    delete ui;
}

std::string RestoreDialog::getSelectedPointId() const {
    auto item = ui->listWidget->currentItem();
    if (!item) return "";
    
    // Проверяем, не сообщение ли это об отсутствии точек
    if (item->data(Qt::UserRole).isNull()) return "";

    return item->data(Qt::UserRole).toString().toStdString();
}

void RestoreDialog::onRestore() {
    // Проверяем, выбрана ли точка
    if (getSelectedPointId().empty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите точку восстановления");
        return;
    }
    
    // Принимаем диалог
    accept();
}

void RestoreDialog::onCancel() {
    reject();
}

void RestoreDialog::onSelectionChanged() {
    // Активируем кнопку восстановления только если выбран реальный пункт
    bool hasSelection = (ui->listWidget->currentItem() != nullptr &&
                         !ui->listWidget->currentItem()->data(Qt::UserRole).isNull());
    ui->restoreBtn->setEnabled(hasSelection);
}