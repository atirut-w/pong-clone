#pragma once
#include <node2d.hpp>
#include <string>

class Label : public Node2D
{
public:
    std::string text = "Label";
    Color color = WHITE;
    int font_size = 20;

    void update(float delta);
};
