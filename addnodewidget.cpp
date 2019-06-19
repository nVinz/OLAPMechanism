#include "addnodewidget.h"
#include "ui_addnodewidget.h"

#include <QDebug>

AddNodeWidget::AddNodeWidget(QWidget *parent, QTreeWidgetItem *item_) :
    QWidget(parent),
    ui(new Ui::AddNodeWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Выберите узел для добавления");

    item = QTreeWidgetItem(item_);

    for (auto it = data_table_header.cbegin(); it != data_table_header.cend(); it++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(*it);
        ui->listWidget->addItem(item);
    }

    /*for (auto it = tabledata.cbegin(); it != tabledata.cend(); it++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(it->name);
        ui->listWidget->addItem(item);
    }*/
}

AddNodeWidget::~AddNodeWidget()
{
    delete ui;
}

void AddNodeWidget::on_putton_ok_clicked()
{
    this->close();
}

void AddNodeWidget::on_button_add_clicked()
{
    node = new Node;
    node->name = ui->listWidget->currentItem()->text();
    node->params.append("Название узла");
    node->params.append(ui->listWidget->currentItem()->text());
    node->treeitem = item;
    node->treeitem.setText(0, node->name);

    send_add_node(*node);

    this->close();
}
