#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    ui->tabWidget->setTabText(0, "Настройки БД");
    ui->tabWidget->setTabText(1, "Настройки CSV");
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_pushButton_clicked()
{
    this->close();
}

void SettingsWindow::on_pushButton_2_clicked()
{
    this->close();
}

void SettingsWindow::on_pushButton_3_clicked()
{
    this->close();
}

void SettingsWindow::on_pushButton_4_clicked()
{
    this->close();
}
