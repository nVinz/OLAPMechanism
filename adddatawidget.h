#ifndef ADDDATAWIDGET_H
#define ADDDATAWIDGET_H

#include "globals.h"
#include "node.h"
#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class AddDataWidget;
}

class AddDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddDataWidget(QWidget *parent = nullptr, QTreeWidgetItem *item_ = nullptr);
    ~AddDataWidget();
    Node *node;
    QTreeWidgetItem item;

private slots:
    void on_button_ok_clicked();
    void on_button_add_clicked();

signals:
    void send_add_data();

private:
    Ui::AddDataWidget *ui;
};

#endif // ADDDATAWIDGET_H
