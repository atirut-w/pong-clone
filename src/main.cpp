#include <raylib.h>

int main()
{
    InitWindow(640, 480, "Pong Clone");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);
        EndDrawing();
    }
    
    return 0;
}
