#pragma once

#include <QMainWindow>
#include <QTimer>
#include "application/ApplicationFacade.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ApplicationFacade facade;
    QTimer timer;

    void loadJobs();

private slots:
    void onCreate();
    void onEdit();
    void onDelete();
    void onRun();
    void onRestore();
    void onTimer();
};
