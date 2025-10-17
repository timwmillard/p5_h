#if defined(P5_IMPL) && !defined(P5_RAYLIB_IMPL)
#define P5_RAYLIB_IMPL
#endif

#ifndef P5_RAYLIB_H
#define P5_RAYLIB_H

#include <stdio.h>
#include <math.h>
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

static Image p5raylib_perlin_image;
#define P5_NOISE_TEXTURE_SIZE 512
#define P5_NOISE_SCALE 0.05f

float p5_delta_time = 0.0f;

Color p5raylib_color(p5_Color p5_color)
{
    return (Color){p5_color.r * 255.0f, p5_color.g * 255.0f, p5_color.b * 255.0f, p5_color.a * 255.0f};
}

void p5raylib_init()
{
    // Generate a large Perlin noise texture for continuous sampling
    p5raylib_perlin_image = GenImagePerlinNoise(P5_NOISE_TEXTURE_SIZE, P5_NOISE_TEXTURE_SIZE, 0, 0, P5_NOISE_SCALE);
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
    p5_delta_time = GetFrameTime();
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

// Helper function for linear interpolation
static inline float p5_lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Bilinear interpolation helper for smooth noise sampling
static float p5_sample_noise_bilinear(float x, float y) {
    // Wrap coordinates to texture size using fmod for continuous space
    float fx = fmodf(x, (float)P5_NOISE_TEXTURE_SIZE);
    float fy = fmodf(y, (float)P5_NOISE_TEXTURE_SIZE);

    // Handle negative coordinates
    if (fx < 0) fx += P5_NOISE_TEXTURE_SIZE;
    if (fy < 0) fy += P5_NOISE_TEXTURE_SIZE;

    // Get integer and fractional parts
    int x0 = (int)fx;
    int y0 = (int)fy;
    int x1 = (x0 + 1) % P5_NOISE_TEXTURE_SIZE;
    int y1 = (y0 + 1) % P5_NOISE_TEXTURE_SIZE;

    float tx = fx - x0;
    float ty = fy - y0;

    // Sample four neighboring pixels
    Color c00 = GetImageColor(p5raylib_perlin_image, x0, y0);
    Color c10 = GetImageColor(p5raylib_perlin_image, x1, y0);
    Color c01 = GetImageColor(p5raylib_perlin_image, x0, y1);
    Color c11 = GetImageColor(p5raylib_perlin_image, x1, y1);

    // Normalize to [0, 1] range
    float v00 = c00.r / 255.0f;
    float v10 = c10.r / 255.0f;
    float v01 = c01.r / 255.0f;
    float v11 = c11.r / 255.0f;

    // Bilinear interpolation
    float v0 = p5_lerp(v00, v10, tx);
    float v1 = p5_lerp(v01, v11, tx);
    return p5_lerp(v0, v1, ty);
}

float p5_noise2(float x, float y)
{
    // Map continuous coordinates to texture space
    // Scale factor controls frequency - smaller = smoother/larger features
    float scale = 10.0f;
    return p5_sample_noise_bilinear(x * scale, y * scale);
}

float p5_noise3(float x, float y, float z)
{
    // For 3D noise, we use the z coordinate to offset the 2D sampling
    // This creates a pseudo-3D effect by adding z as an offset
    (void)z; // Could use z to offset x,y but keeping it simple for now
    float scale = 10.0f;
    return p5_sample_noise_bilinear((x + z) * scale, (y + z) * scale);
}

#endif // P5_RAYLIB_IMPL

