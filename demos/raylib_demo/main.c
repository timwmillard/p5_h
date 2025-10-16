#include "raylib.h"

#define P5RAYLIB_IMPLEMENTATION
#define P5_IMPLEMENTATION
#define P5_NO_SHORT_NAMES
#include "p5.h"
#include "p5raylib.h"

#include "canvas.c"

int main(void)
{
    InitWindow(800, 600, "P5 in Raylib");
    p5raylib_init();

    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            int posX = 50;
            int posY = 50;
            int width = 100;
            int height = 200;
            Color color = ORANGE;
            DrawRectangle(posX, posY, width, height, color);

            p5raylib_render();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
