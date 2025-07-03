/*
test_renderer.h - Offscreen rendering setup for visual tests
Provides headless Sokol initialization and framebuffer capture
*/

#ifndef TEST_RENDERER_H
#define TEST_RENDERER_H

#include <stdbool.h>

// Test renderer interface
typedef struct {
    int width;
    int height;
    unsigned int offscreen_fb;
    unsigned int color_texture;
    unsigned int depth_texture;
    bool initialized;
} test_renderer_t;

// Global test renderer instance
extern test_renderer_t g_test_renderer;

// Test renderer functions
bool test_renderer_init(int width, int height);
void test_renderer_shutdown(void);
void test_renderer_begin_frame(void);
void test_renderer_end_frame(void);
bool test_renderer_save_png(const char* filename);

#endif // TEST_RENDERER_H