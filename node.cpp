#include "node.h"

Node::Node()
{
    //child = {};
    parent = nullptr;
    parent_ = {};
    name = "";
}

void Node::add_child(Node node_)
{
    child.append(node_);
}

void Node::add_parent(Node node_)
{
    parent_.append(node_);
    //parent = &node_;
}

QString Node::get_name()
{
    return name;
}
