#if defined(P5_IMPL) && !defined(P5_RAYLIB_IMPL)
#define P5_RAYLIB_IMPL
#endif

#ifndef P5_RAYLIB_H
#define P5_RAYLIB_H

#include <stdio.h>
#ifndef P5_H
#define P5_NO_SHORT_NAMES
#include "p5.h"
#endif
#include "raylib.h"

void p5raylib_init();
void p5raylib_clear_background();
void p5raylib_render();

#endif // P5_RAYLIB_H


// ██ ███    ███ ██████  ██      ███████ ███    ███ ███████ ███    ██ ████████  █████  ████████ ██  ██████  ███    ██
// ██ ████  ████ ██   ██ ██      ██      ████  ████ ██      ████   ██    ██    ██   ██    ██    ██ ██    ██ ████   ██
// ██ ██ ████ ██ ██████  ██      █████   ██ ████ ██ █████   ██ ██  ██    ██    ███████    ██    ██ ██    ██ ██ ██  ██
// ██ ██  ██  ██ ██      ██      ██      ██  ██  ██ ██      ██  ██ ██    ██    ██   ██    ██    ██ ██    ██ ██  ██ ██
// ██ ██      ██ ██      ███████ ███████ ██      ██ ███████ ██   ████    ██    ██   ██    ██    ██  ██████  ██   ████
//
// >>implementation
//
#ifdef P5_RAYLIB_IMPL

Color p5raylib_color(p5_Color p5_color)
{
    return (Color){p5_color.r * 255.0f, p5_color.g * 255.0f, p5_color.b * 255.0f, p5_color.a * 255.0f};
}

void p5raylib_init()
{
    setup();
}

void p5raylib_clear_background()
{
    p5_Color p5_color = p5render_background_color();
    Color color = p5raylib_color(p5_color);
    ClearBackground(color);
}

void p5raylib_render()
{
    draw();
    p5render_CommandArray cmds = p5render_commands();
    for (int i = 0; i < cmds.count; i++) {
        switch (cmds.items[i].type) {
            case P5_RENDER_RECT: {
                p5render_Rect rect = cmds.items[i].rect;
                if (rect.border_width > 0) {
                    Color color =  p5raylib_color(rect.border_color);
                    float w = rect.border_width;
                    float w2 = w * 2.0;
                    DrawRectangle(rect.x - w, rect.y - w, rect.w + w2, rect.h + w2, color);
                }
                Color color =  p5raylib_color(rect.bg_color);
                DrawRectangle(rect.x, rect.y, rect.w, rect.h, color);
                break;
             }
            case P5_RENDER_ARC:
            case P5_RENDER_POINT:
            case P5_RENDER_QUAD:
            case P5_RENDER_TRIANGLE:
        }

    }
    p5render_commands_reset();
}

#endif // P5_RAYLIB_IMPL

