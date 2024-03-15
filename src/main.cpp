#include <raylib.h>
#include <label.hpp>
#include <algorithm>
#include <random>
#include <array>
#include <fstream>

std::shared_ptr<NodeTree::Node> current;
std::shared_ptr<NodeTree::Node> start = std::make_shared<NodeTree::Node>();
std::shared_ptr<NodeTree::Node> gameplay = std::make_shared<NodeTree::Node>();
std::array<int, 2> scores = { 0, 0 };
std::shared_ptr<NodeTree::Node> endgame = std::make_shared<NodeTree::Node>();

int lives_p1 = 3;
int lives_p2 = 3;
int attack_p1 = 1;
int attack_p2 = 1;
int score_p1 =0;
int score_p2 =0;

void sevescore()
{
    // เขียนไฟล์เก็บคะแนน
    std::ofstream file("scores.txt");
    if (file.is_open())
    {
        file << score_p1 << " " << score_p2 << std::endl;
        file.close();
    }
}

void add_score_p1()
{
    lives_p2 = lives_p2-attack_p1;
}

void add_score_p2()
{
    lives_p1 = lives_p1-attack_p2;
}

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
    float speed = 300;
    KeyboardKey up_key;
    KeyboardKey down_key;
    Color color;
    
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

        DrawRectangle(position.x, position.y, size.x, size.y, color);
    }
};

class Ball : public Node2D
{
public:
    Vector2 size = { 15, 15 };
    Vector2 velocity;
    float speed_increment = 35;
    std::shared_ptr<Paddle> p1;
    std::shared_ptr<Paddle> p2;

    Ball()
    {
        reset();
    }

    void reset()
    {
        position = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        velocity = { 200, 200 };
        velocity.x = rand() % 2 == 0 ? velocity.x : -velocity.x;
        velocity.y = rand() % 2 == 0 ? velocity.y : -velocity.y;
    }

    void update(float delta) override
    {
        Node::update(delta);

        // Vertical bounce
        if (position.y <= 0 || position.y >= GetScreenHeight() - size.y)
        {
            velocity.y = -velocity.y;
        }

        // Paddle collision
        // We have to check on separate axes because checking for "any collision
        // with the paddles" and inverting X velocity can cause a nasty bug when
        // the ball collide with the paddles on the Y axis.

        Rectangle p1_rect = { p1->position.x, p1->position.y, p1->size.x, p1->size.y };
        Rectangle p2_rect = { p2->position.x, p2->position.y, p2->size.x, p2->size.y };

        Rectangle new_x = { position.x + velocity.x * delta, position.y, size.x, size.y };
        if (CheckCollisionRecs(new_x, p1_rect) || CheckCollisionRecs(new_x, p2_rect))
        {
            velocity.x = -velocity.x;
            velocity.x += velocity.x < 0 ? -speed_increment : speed_increment;
            velocity.y += velocity.y < 0 ? -speed_increment : speed_increment;
        }
        Rectangle new_y = { position.x, position.y + velocity.y * delta, size.x, size.y };
        if (CheckCollisionRecs(new_y, p1_rect) || CheckCollisionRecs(new_y, p2_rect))
        {
            velocity.y = -velocity.y;
            velocity.x += velocity.x < 0 ? -speed_increment : speed_increment;
            velocity.y += velocity.y < 0 ? -speed_increment : speed_increment;
        }

        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        if (position.x + size.x < 0 || position.x > GetScreenWidth())
        {
            // We're out of bound, check which side and add a point
            if (position.x + size.x < 0)
            {
                add_score_p2();
            }
            else if (position.x > GetScreenWidth())
            {
                add_score_p1();
            }

            if(lives_p1 == 0 || lives_p2 == 0)
            {
                current = endgame;
                if(position.x>GetScreenWidth()/2)
                {
                    score_p1++;
                }
                else
                {
                    score_p2++;
                }
            }

            reset();
        }

        DrawRectangle(position.x, position.y, size.x, size.y, {0x83, 0x59, 0xff, 0xff});
    }
};
class endgame101 : public NodeTree::Node
{
    public:
    void update(float delta) override
    {
        if(IsKeyPressed(KEY_ONE))
        {
            lives_p1 = 3;
            lives_p2 = 3;
            current = gameplay;
        }
        else if (IsKeyPressed(KEY_ZERO))
        {
            sevescore();
            exit(0);
        }
    }
};

void init_scenes()
{
    auto endgame_text =std::make_shared<Label>();
    endgame_text->text =" \n \n       Want to start a new game, press 1. Want to exit the game press 0\n";
    endgame_text-> font_size = 18;
    endgame->add_child(endgame_text);
    endgame-> add_child (std::make_shared<endgame101>());
    start->add_child(std::make_shared<StartScreenController>());

    auto intro_text = std::make_shared<Label>();
    intro_text->text = "\n\n\nWelcome to Pong Clone\n\n";
    intro_text->text += "This is a simple pong game made for a university assignment.\n";
    intro_text->text += "It's very barebones, but enjoy!\n\n";
    intro_text->text += "Press Enter to start the game.";
    intro_text->position = { 10, 10 };
    start->add_child(intro_text);

    auto p1 = std::make_shared<Paddle>();
    p1->position = { 10, 10 };
    p1->up_key = KEY_W;
    p1->down_key = KEY_S;
    p1->color = {0x67, 0xff, 0x59, 0xff};
    gameplay->add_child(p1);

    auto p2 = std::make_shared<Paddle>();
    p2->position = { GetScreenWidth() - p2->size.x - 10, 10 };
    p2->up_key = KEY_UP;
    p2->down_key = KEY_DOWN;
    p2->color = {0xff, 0x59, 0xc8, 0xff};
    gameplay->add_child(p2);

    auto ball = std::make_shared<Ball>();
    ball->p1 = p1;
    ball->p2 = p2;
    gameplay->add_child(ball);

    current = start;
   
}

void show_lives()
{
    DrawText(std::to_string(lives_p1).c_str(), 100, 10, 30, WHITE);
    DrawText(std::to_string(lives_p2).c_str(), GetScreenWidth() - 100, 10, 30, WHITE);
}

int main()
{
    {
        std::ifstream file("scores.txt");
        file>>score_p1;
        file>>score_p2;

        file.close();
    }
    InitWindow(640, 480, "Pong Clone");
    SetTargetFPS(60);

    init_scenes();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        current->update(GetFrameTime());
        
        if (current == gameplay)
        {
            show_lives();
        }
        else if (current == endgame)
        {
            DrawText(std::to_string(score_p1).c_str(), 100, 10, 30, WHITE);
            DrawText(std::to_string(score_p2).c_str(), GetScreenWidth() - 100, 10, 30, WHITE);
        }
        
        EndDrawing();
    }
    
    sevescore();
    
    return 0;
}
