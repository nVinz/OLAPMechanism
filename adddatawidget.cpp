#include "adddatawidget.h"
#include "ui_adddatawidget.h"

#include <QListWidget>
#include <QDebug>

AddDataWidget::AddDataWidget(QWidget *parent, QTreeWidgetItem *item_) :
    QWidget(parent),
    ui(new Ui::AddDataWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Выберите свойство для добавления");

    item = QTreeWidgetItem(item_);
    item.setText(0, item_->text(0));

    for (auto it = data_table_header.cbegin(); it != data_table_header.cend(); it++)
    {
        QListWidgetItem *item1 = new QListWidgetItem;
        item1->setText(*it);
        ui->listWidget->addItem(item1);
    }
}

AddDataWidget::~AddDataWidget()
{
    delete ui;
}

void AddDataWidget::on_button_ok_clicked()
{
    this->close();
}

void AddDataWidget::on_button_add_clicked()
{
    for (QList<Node>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if (it->name == item.text(0))
        {
            //qDebug() << it->name;
            Node node = *it;
            node.params.append(ui->listWidget->currentItem()->text());
            node.params.append(" ");

            it->operator=(node);

            send_add_data();

            this->close();
        }
    }
}
