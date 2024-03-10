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

int score_p1 = 3;
int score_p2 = 3;
int attack_p1 = 1;
int attack_p2 = 1;


void add_score_p1()
{
    score_p2 = score_p2-attack_p1;
}

void add_score_p2()
{
    score_p1 = score_p1-attack_p1;
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
         if(score_p1 == 0)
        {
            speed= 0;
              printf("test1");
        }
        if(score_p2 == 0)
        {
            speed= 0;
            printf("test2");
        }
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
        }
        Rectangle new_y = { position.x, position.y + velocity.y * delta, size.x, size.y };
        if (CheckCollisionRecs(new_y, p1_rect) || CheckCollisionRecs(new_y, p2_rect))
        {
            velocity.y = -velocity.y;
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

            reset();
        }

        DrawRectangle(position.x, position.y, size.x, size.y, WHITE);
        if(score_p1 == 0)
        {
            velocity= {0,0};
             
        }
        if(score_p2 == 0)
        {
            velocity= {0,0};
           
            
        }
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
    ball->p1 = p1;
    ball->p2 = p2;
    gameplay->add_child(ball);

    // current = start;
    current = gameplay; // for testing
}

void show_scores()
{
    DrawText(std::to_string(score_p1).c_str(), 100, 10, 30, WHITE);
    DrawText(std::to_string(score_p2).c_str(), GetScreenWidth() - 100, 10, 30, WHITE);
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
        show_scores();
        EndDrawing();
       
    }
    
    // เขียนไฟล์เก็บคะแนน
    std::ofstream file("scores.txt");
    if (file.is_open())
    {
        file << "Player 1: " << score_p1 << "\n";
        file << "Player 2: " << score_p2 << "\n";
        file.close();
    }
    
    
    return 0;
}
