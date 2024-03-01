#include <raylib.h>
#include <label.hpp>
#include <algorithm>
#include <random>
#include <array>

std::shared_ptr<NodeTree::Node> current;
std::shared_ptr<NodeTree::Node> start = std::make_shared<NodeTree::Node>();
std::shared_ptr<NodeTree::Node> gameplay = std::make_shared<NodeTree::Node>();

std::array<int, 2> scores = { 0, 0 };

class StartScreenController : public NodeTree::Node
{
public:
    void update(float delta) override
    {
        Node::update(delta);
        if (IsKeyPressed(KEY_ENTER))
        {
            current = gameplay;
        }
    }
};

class Paddle : public Node2D
{
public:
    Vector2 size = { 15, 100 };
    float speed = 200;
    KeyboardKey up_key;
    KeyboardKey down_key;
    
    void update(float delta) override
    {
        Node::update(delta);

        if (IsKeyDown(up_key))
        {
            position.y -= speed * delta;
        }
        if (IsKeyDown(down_key))
        {
            position.y += speed * delta;
        }

        position.y = std::clamp(position.y, 10.0f, GetScreenHeight() - size.y - 10.0f);

        DrawRectangle(position.x, position.y, size.x, size.y, WHITE);
    }
};

class Ball : public Node2D
{
public:
    Vector2 size = { 15, 15 };
    Vector2 velocity = { 200, 200 };
    std::shared_ptr<Paddle> p1;
    std::shared_ptr<Paddle> p2;

    void update(float delta) override
    {
        Node::update(delta);

        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // Vertical bounce
        if (position.y <= 0 || position.y >= GetScreenHeight() - size.y)
        {
            velocity.y = -velocity.y;
        }

        // Paddle check
        if (CheckCollisionRecs({ position.x, position.y, size.x, size.y }, { p1->position.x, p1->position.y, p1->size.x, p1->size.y }) ||
            CheckCollisionRecs({ position.x, position.y, size.x, size.y }, { p2->position.x, p2->position.y, p2->size.x, p2->size.y }))
        {
            velocity.x = -velocity.x;
        }

        DrawRectangle(position.x, position.y, size.x, size.y, WHITE);
    }
};

void init_scenes()
{
    start->add_child(std::make_shared<StartScreenController>());

    auto intro_text = std::make_shared<Label>();
    intro_text->text = "Welcome to Pong Clone\n\n";
    intro_text->text += "This is a simple pong game made for a university assignment.\n";
    intro_text->text += "It's very barebones, but enjoy!\n\n";
    intro_text->text += "Press Enter to start the game.";
    intro_text->position = { 10, 10 };
    start->add_child(intro_text);

    auto p1 = std::make_shared<Paddle>();
    p1->position = { 10, 10 };
    p1->up_key = KEY_W;
    p1->down_key = KEY_S;
    gameplay->add_child(p1);

    auto p2 = std::make_shared<Paddle>();
    p2->position = { GetScreenWidth() - p2->size.x - 10, 10 };
    p2->up_key = KEY_UP;
    p2->down_key = KEY_DOWN;
    gameplay->add_child(p2);

    auto ball = std::make_shared<Ball>();
    ball->position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
    ball->p1 = p1;
    ball->p2 = p2;
    ball->velocity.x = rand() % 2 == 0 ? ball->velocity.x : -ball->velocity.x;
    ball->velocity.y = rand() % 2 == 0 ? ball->velocity.y : -ball->velocity.y;
    gameplay->add_child(ball);

    // current = start;
    current = gameplay; // for testing
}

int main()
{
    InitWindow(640, 480, "Pong Clone");
    SetTargetFPS(60);

    init_scenes();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        current->update(GetFrameTime());
        EndDrawing();
    }
    
    return 0;
}
