#pragma once
#include <nodetree/node.hpp>
#include <raylib.h>

class Node2D : public NodeTree::Node
{
public:
    Vector2 position = {0, 0};
};
