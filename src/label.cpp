#include <label.hpp>
#include <raylib.h>

void Label::update(float delta)
{
  /* The code snippet `Node::update(delta);` is calling the `update` function of the `Node` class,
  passing the `delta` parameter to it. This allows the `Node` class to update its internal state
  based on the elapsed time since the last update. */
    Node::update(delta);
    DrawText(text.c_str(), position.x, position.y, font_size, color);
}
