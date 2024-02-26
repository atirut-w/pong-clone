#include <nodetree/node.hpp>
#include <iostream>

using namespace NodeTree;

void Node::update(float delta)
{
    for (auto child : children)
    {
        child->update(delta);
    }
}

void Node::add_child(std::shared_ptr<Node> child)
{
    children.push_back(child);
    child->parent = shared_from_this();
}

void Node::remove_child(std::shared_ptr<Node> child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        children.erase(it);
        child->parent.reset();
    }
}
