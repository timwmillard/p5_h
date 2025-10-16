#include "raylib.h"

#define P5RAYLIB_IMPL
#define P5_IMPL
#define P5_NO_SHORT_NAMES

#include "p5.h"
#include "p5raylib.h"

int main(void)
{
    InitWindow(800, 600, "P5 in Raylib");
    p5raylib_init();

    while(!WindowShouldClose()) {
        BeginDrawing();
            p5raylib_clear_background();
            p5raylib_render();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
