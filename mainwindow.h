#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include "node.h"
#include "addnodewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_add_csv_triggered();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_button_maketree_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void add_node();
    void add_node_data();
    void update_data_table();
    void add_tree_item(Node node_);

    void on_treeWidget_2_itemClicked(QTreeWidgetItem *item, int column);

    QStringList make_cross(QStringList a, QStringList b);

private:
    Ui::MainWindow *ui;
    QString file_name;
    void load_CSV();
};

#endif // MAINWINDOW_H
