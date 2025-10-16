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
        p5render_Command cmd = cmds.items[i];
        switch (cmds.items[i].type) {
            case P5_RENDER_RECT: {
                p5render_Rect rect = cmd.rect;
                float x = rect.x;
                float y = rect.y;
                float w = rect.w;
                float h = rect.h;
                if (cmd.has_stroke) {
                    Color color =  p5raylib_color(cmd.stroke_color);
                    float s = cmd.stroke_width;
                    DrawRectangle(x-s, y-s, w+s, s, color); // top 
                    DrawRectangle(x+w, y-s, s, s+h, color); // right
                    DrawRectangle(x-s, y, s, h+s, color); // left
                    DrawRectangle(x, y+h, w+s, s, color); // bottom
                }
                if (cmd.has_fill) { 
                    Color color = p5raylib_color(cmd.fill_color);
                    DrawRectangle(x, y, w, h, color);
                }
            } break;
            case P5_RENDER_ARC: {
                p5render_Arc arc = cmd.arc;
                float x = arc.x;
                float y = arc.y;
                float r = arc.r;
                float s = cmd.stroke_width;
                if (cmd.has_fill) { 
                    Color color = p5raylib_color(cmd.fill_color);
                    // if has_stroke increase fill by half stroke, to hide gaps.
                    DrawCircle(x, y, cmd.has_stroke? r+s/2 : r, color);
                }
                if (cmd.has_stroke) {
                    Color color =  p5raylib_color(cmd.stroke_color);
                    DrawRing((Vector2){x, y}, r, r+s, 0, 360, 32, color);
                }
            } break;
            case P5_RENDER_POINT: {
                if (cmd.has_stroke) { 
                    p5render_Point point = cmd.point;
                    float x = point.x;
                    float y = point.y;
                    float s = cmd.stroke_width;
                    Color color =  p5raylib_color(cmd.stroke_color);
                    DrawCircle(x, y, s, color);
                }
            } break;
            case P5_RENDER_QUAD: {
            } break;
            case P5_RENDER_TRIANGLE: {
            } break;
        }

    }
    p5render_commands_reset();
}

int p5_width(void)
{
    return GetRenderWidth();
}

int p5_height(void)
{
    return GetRenderHeight();
}

int p5_window_width(void)
{
    return GetScreenWidth();
}
int p5_window_height(void)
{
    return GetScreenHeight();
}

#endif // P5_RAYLIB_IMPL

