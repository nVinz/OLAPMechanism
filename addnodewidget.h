#ifndef ADDNODEWIDGET_H
#define ADDNODEWIDGET_H

#include "globals.h"
#include <QWidget>
#include <QTreeWidgetItem>
#include "node.h"

namespace Ui {
class AddNodeWidget;
}

class AddNodeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddNodeWidget(QWidget *parent = nullptr, QTreeWidgetItem *item_ = nullptr);
    AddNodeWidget(Node node_);
    Node *node;
    QTreeWidgetItem item;
    ~AddNodeWidget();

private slots:
    void on_putton_ok_clicked();
    void on_button_add_clicked();

signals:
    void send_add_node(Node node_);

private:
    Ui::AddNodeWidget *ui;
};

#endif // ADDNODEWIDGET_H
