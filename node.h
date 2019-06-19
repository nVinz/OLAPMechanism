#ifndef NODE_H
#define NODE_H

#include <QTreeWidgetItem>

class Node
{
public:
    Node();
    Node(QString name_);
    Node(QString name_, QList<QString> params_);
    Node(QString name_, QList<QString> params_, QTreeWidgetItem treeitem_);

    QString name;
    QList<QString> params;
    QList<Node> child;
    QList<Node> parent_;
    Node *parent;
    QTreeWidgetItem treeitem;
    QStringList data;

    void add_child(Node node_);
    void add_parent(Node node_);
    QString get_name();
};

#endif // NODE_H
