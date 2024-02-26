#pragma once
#include <string>
#include <vector>
#include <memory>

namespace NodeTree
{
    class Node : public std::enable_shared_from_this<Node>
    {
    protected:
        std::vector<std::shared_ptr<Node>> children;
        std::weak_ptr<Node> parent;
    public:
        std::string name;

        Node() = default;
        virtual ~Node() = default;

        virtual void update(float delta);
        void add_child(std::shared_ptr<Node> child);
        void remove_child(std::shared_ptr<Node> child);
    };
}
