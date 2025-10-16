
#ifndef P5RAYLIB_H
#define P5RAYLIB_H

#include <stdio.h>
// #include "p5.h"
#include "raylib.h"

void p5raylib_init();
void p5raylib_render();

#endif // P5RAYLIB_H


// ██ ███    ███ ██████  ██      ███████ ███    ███ ███████ ███    ██ ████████  █████  ████████ ██  ██████  ███    ██
// ██ ████  ████ ██   ██ ██      ██      ████  ████ ██      ████   ██    ██    ██   ██    ██    ██ ██    ██ ████   ██
// ██ ██ ████ ██ ██████  ██      █████   ██ ████ ██ █████   ██ ██  ██    ██    ███████    ██    ██ ██    ██ ██ ██  ██
// ██ ██  ██  ██ ██      ██      ██      ██  ██  ██ ██      ██  ██ ██    ██    ██   ██    ██    ██ ██    ██ ██  ██ ██
// ██ ██      ██ ██      ███████ ███████ ██      ██ ███████ ██   ████    ██    ██   ██    ██    ██  ██████  ██   ████
//
// >>implementation
//
#ifdef P5RAYLIB_IMPLEMENTATION

void p5raylib_init()
{
    setup();
}

void p5raylib_render()
{
    draw();
    p5render_CommandArray cmds = p5render_commands();
    for (int i = 0; i < cmds.count; i++) {
        switch (cmds.items[i].type) {
            case P5_RENDER_RECT: {

                p5render_Rect rect = cmds.items[i].rect;
                // printf("p5 Raylib render rect = %f, %f\n", rect.x, rect.y);
                Color color = RED;
                DrawRectangle(rect.x, rect.y, rect.w, rect.h, color);
                break;
             }
        }

    }
    p5render_commands_reset();
}

#endif // P5RAYLIB_IMPLEMENTATION

