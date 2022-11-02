#include <iostream>
#include "raylib.h"

int main()
{
    bool keepGameAlive = true;
    InitWindow(800, 600, "Physics Engine");
    SetWindowState(FLAG_VSYNC_HINT);
    
    while (keepGameAlive){
        BeginDrawing();
            ClearBackground(BLACK);
            DrawFPS(0, 0);
        EndDrawing();
    }
    
    CloseWindow();
}

