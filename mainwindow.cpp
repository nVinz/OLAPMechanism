#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"

#include "adddatawidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //  UI  //////////////////////////////////////////////////////////

    ui->setupUi(this);
    this->setWindowTitle("OLAP Механизм");

    ui->tabWidget_left->setTabText(0, "Таблица");
    ui->tabWidget_left->setTabText(1, "Макет дерева");
    ui->tabWidget_left->setTabText(2, "Дерево объектов анализа");

    ui->tabWidget_right->setTabText(0, "Данные");
    ui->tabWidget_right->setTabText(1, "Фильтры");
    //ui->tabWidget_right->setTabText(2, "Кросс-таблицы");

    ui->tabWidget_left->setCurrentIndex(0);
    ui->tabWidget_right->setCurrentIndex(0);

    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setAnimated(true);

    ui->treeWidget_2->setHeaderHidden(true);
    ui->treeWidget_2->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget_2->setColumnCount(1);

    ui->tableWidget_data->setColumnCount(2);
    QStringList header_list = {"Название свойства", "Значение свойства"};
    ui->tableWidget_data->setHorizontalHeaderLabels(header_list);

    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_treeWidget_customContextMenuRequested(const QPoint &)));

    //////////////////////////////////////////////////////////////////
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_add_csv_triggered()
{
    load_CSV();
}

void MainWindow::load_CSV()
{
    //  FILE & DATA  ///////////////////////////////////////////////////////////////////////////////////////////

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(1);

    QString fn = QFileDialog::getOpenFileName(this, tr("Open"));
    QFile file(fn);
    file.open(QFile::ReadOnly);

    QString line;
    QTextStream in(&file);

    file_name = fn;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////


    //  TABLE HEADER  ///////////////////////////////////////////////////////////////////////////////////////////

    QString header_line = in.readLine();
    QStringList header_list = header_line.split(";", QString::SkipEmptyParts);
    data_table_header = header_list;

    for (int i=0; i<header_line.length(); i++)
    {
        if (header_line[i]==";")
        {
            ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        }
    }
    ui->tableWidget->setHorizontalHeaderLabels(header_list);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////


    //  TABLE INSERTING DATA  ////////////////////////////////////////////////////////////////////////////////////

    int r=0;
    int c=0;

    while (!in.atEnd())
    {
        line = in.readLine();

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());

        QStringList list = line.split(";", QString::SkipEmptyParts);

        for (int i=0; i<list.length(); i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(list[i]);
            ui->tableWidget->setItem(r, c, item);
            c++;
        }

        r++; c=0;
    }

    for (int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        QStringList *list = new QStringList;

        ui->tableWidget->selectColumn(i);
        list->append(header_list[i]);

        for (int j = 0; j < ui->tableWidget->rowCount(); j++)
        {
            QString value = ui->tableWidget->model()->index(j, i).data().toString();
            list->append(value);
        }

        table_data.append(*list);
    }

    /*for (auto it = table_data.begin(); it != table_data.end(); it++)
    {
        qDebug() << *it;
    }*/

    ui->statusBar->showMessage("CSV таблица загружена.");

    Node *node = new Node;
    node->name = "Top";
    node->params.append("Название узла");
    node->params.append("Top");
    nodes.append(*node);

    QTreeWidgetItem *treeItemBasic = new QTreeWidgetItem(ui->treeWidget);
    treeItemBasic->setText(0, "Top");

}

//////////////////////////////////////////////////////////////////////////////////////////


//  TREE 1  ///////////////////////////////////////////////////////////////////////

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Добавить узел", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(add_node()));
    contextMenu.addAction(&action1);

    QAction action1_1("Удалить узел", this);
    contextMenu.addAction(&action1_1);

    QAction action2("Добавить свойство", this);
    connect(&action2, SIGNAL(triggered()), this, SLOT(add_node_data()));
    contextMenu.addAction(&action2);

    QAction action2_1("Удалить свойство", this);
    contextMenu.addAction(&action2_1);

    contextMenu.exec(QCursor::pos());
}

void MainWindow::add_node()
{
    AddNodeWidget *node_widget = new AddNodeWidget(nullptr, ui->treeWidget->currentItem());
    node_widget->show();
    connect(node_widget, SIGNAL(send_add_node(Node)), this, SLOT(add_tree_item(Node)));
}

void MainWindow::add_tree_item(Node node_)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(node_.treeitem);
    item->setText(0, node_.treeitem.text(0));
    ui->treeWidget->currentItem()->addChild(item);

    for (QList<Node>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if (it->name == ui->treeWidget->currentItem()->text(0))
        {
            it->child.append(node_);
            node_.parent_.append(*it);
        }
    }

    for (auto it = table_data.begin(); it != table_data.end(); it++)
    {
        if (it->first() == node_.name)
        {
            node_.data = *it;
        }
    }

    nodes.append(node_);

    ui->treeWidget->currentItem()->setExpanded(true);
}

/////////////////////////////////////////////////////////////////////////////////////


//  TREE 1 DATA  ////////////////////////////////////////////////////////////////////////////

void MainWindow::add_node_data()
{
    AddDataWidget *data_widget = new AddDataWidget(nullptr, ui->treeWidget->currentItem());
    data_widget->show();
    connect(data_widget, SIGNAL(send_add_data()), this, SLOT(update_data_table()));
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    current_node = item->text(column);
    update_data_table();
}

void MainWindow::update_data_table()
{
    ui->tableWidget_data->setRowCount(0);

    for (auto it = nodes.cbegin(); it != nodes.cend(); it++)
    {
        if (it->name == current_node)
        {
            ui->tableWidget_data->insertRow(ui->tableWidget_data->rowCount());

            int r=0, c=0;

            for (auto it2 = it->params.cbegin(); it2 != it->params.cend(); it2++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText(*it2);
                ui->tableWidget_data->setItem(r, c, item);

                c++;

                if(c>1)
                {
                    c=0; r++;
                    ui->tableWidget_data->insertRow(ui->tableWidget_data->rowCount());
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


//  TREE 2 /////////////////////////////////////////////////////////////////////////////////////

QStringList MainWindow::make_cross(QStringList a, QStringList b)
{
    QStringList list;
    int i=1;

    for (auto it = a.begin(); it != a.end()-1; it++)
    {
        list.append(a[i]);
        list.append(b[i]);
        i++;
    }

    return list;
}

void MainWindow::on_button_maketree_clicked()
{
    ui->treeWidget_2->clear();

    QTreeWidgetItem *BaseItem = new QTreeWidgetItem(ui->treeWidget_2);
    BaseItem->setText(0, "Top");
    ui->treeWidget_2->setCurrentItem(BaseItem);

    //QList<Node> nds = nodes;

    //while(!nds.empty())
    //{
        for (QList<Node>::iterator node_it = nodes.begin(); node_it != nodes.end(); node_it++)
        {
            if (node_it->parent_.empty())
            {
                //node_it->data.removeDuplicates();

                /*for (QStringList::iterator it3 = node_it->data.begin()+1; it3 != node_it->data.end(); it3++)
                {
                    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_2);
                    item->setText(0, node_it->data.first() + ": " + *it3);
                }*/
            }
            else
            {
                if (node_it->parent_.first().name == "Top")
                {
                    node_it->data.removeDuplicates();

                    for (auto it = node_it->data.begin()+1; it != node_it->data.end(); it++)
                    {
                        QTreeWidgetItem *item = new QTreeWidgetItem();
                        item->setText(0, node_it->data.first() + ": " + *it);
                        BaseItem->addChild(item);
                    }
                }
                else
                {
                    QStringList list = make_cross(node_it->parent_.first().data, node_it->data);

                    for (auto it = list.begin(); it != list.end(); it++)
                    {
                        it++;
                        qDebug() << *it;
                        QTreeWidgetItem *item = new QTreeWidgetItem();
                        item->setText(0, node_it->data.first() + ": " + *it);
                        BaseItem->addChild(item);
                    }
                }



            }
        }
    //}

    /*for (QList<Node>::iterator node_it = nodes.begin(); node_it != nodes.end(); node_it++)
    {
        for (auto data_it = table_data.begin(); data_it != table_data.end(); data_it++)
        {
            if (node_it->name == data_it->first())
            {
                if (node_it->parent_.empty())
                {
                    data_it->removeDuplicates();

                    for (auto it3 = data_it[0].cbegin()+1; it3 != data_it[0].cend(); it3++)
                    {
                        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_2);
                        item->setText(0, data_it->first() + ": " + *it3);
                    }
                }
                else
                {
                    if (node_it->parent_.first().name == "Top")
                    {
                        data_it->removeDuplicates();
                    }
                    for (auto it3 = data_it[0].cbegin()+1; it3 != data_it[0].cend(); it3++)
                    {
                        QList<QTreeWidgetItem*> items = ui->treeWidget_2->findItems(node_it->parent_.first().name, Qt::MatchContains | Qt::MatchRecursive);

                        //qDebug() << it->parent_.first().name;

                        if (items.empty()) { qDebug() << "Error ID0"; break; }

                        foreach(QTreeWidgetItem* it4, items)
                        {
                            QTreeWidgetItem *item = new QTreeWidgetItem();
                            item->setText(0, data_it->first() + ": " + *it3);
                            //items.first()->addChild(item);
                            it4->addChild(item);

                            qDebug() <<"1";
                        }
                    }
                }
            }
        }
    }*/


    /*for (QList<Node>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if (it->parent_.empty())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_2);
            item->setText(0, it->name);
        }
        else
        {
            QList<QTreeWidgetItem*> items = ui->treeWidget_2->findItems(it->parent_.first().name, Qt::MatchRecursive);
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, it->name);
            items.first()->addChild(item);
        }
    }*/

    ui->treeWidget_2->expandAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////


/// TABLE 2 DATA  //////////////////////////////////////////////////////////////////////

void MainWindow::on_treeWidget_2_itemClicked(QTreeWidgetItem *item, int column)
{
    current_node = item->text(column);
    update_data_table();
}


////////////////////////////////////////////////////////////////////////////////////////////////
