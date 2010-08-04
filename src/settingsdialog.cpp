#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);


}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


void SettingsDialog::clearItems()
{
    ui->tableWidget->clear();
}

void SettingsDialog::addItem(QString &name, QString &value)
{
    QTableWidgetItem *n = new QTableWidgetItem(name);
    QTableWidgetItem *v = new QTableWidgetItem(value);

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, n);
    ui->tableWidget->setItem(row, 1, v);

    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
}
