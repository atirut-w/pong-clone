#include <label.hpp>
#include <raylib.h>

void Label::update(float delta)
{
    Node::update(delta);
    DrawText(text.c_str(), position.x, position.y, font_size, color);
}
