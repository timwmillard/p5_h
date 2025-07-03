/*
test_basic_shapes_visual.c - Visual test for basic shape rendering
Tests actual pixel-perfect rendering using offscreen framebuffer
*/

// Include Sokol headers before p5.h
#include "../deps/sokol_app.h"
#include "../deps/sokol_gfx.h"
#include "../deps/sokol_glue.h"
#include "../deps/sokol_gp.h"

#define P5_NO_APP
#define P5_IMPLEMENTATION
#include "../p5.h"

#include "test_utils.h"
#include "test_renderer.h"

#define TEST_WIDTH 400
#define TEST_HEIGHT 300

void test_rectangle_rendering(void) {
    // Begin rendering frame
    test_renderer_begin_frame();
    
    // Initialize p5 state
    p5_init();
    
    // Clear background to white
    p5_background(1.0f, 1.0f, 1.0f);
    
    // Draw filled red rectangle
    p5_fill(1.0f, 0.0f, 0.0f);
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_rect(50, 50, 100, 75);
    
    // Draw blue stroke-only rectangle
    p5_no_fill();
    p5_stroke(0.0f, 0.0f, 1.0f);
    p5_stroke_weight(3.0f);
    p5_rect(200, 50, 100, 75);
    
    // End rendering frame
    test_renderer_end_frame();
    
    // Save and compare
    TEST_ASSERT_TRUE(save_framebuffer_as_png("tests/test_output_rectangles.png", TEST_WIDTH, TEST_HEIGHT));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_rectangles.png", "tests/golden/rectangles.png"));
}

void test_circle_rendering(void) {
    test_renderer_begin_frame();
    
    p5_init();
    p5_background(0.9f, 0.9f, 0.9f);
    
    // Draw filled green circle
    p5_fill(0.0f, 1.0f, 0.0f);
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_circle(100, 100, 80);
    
    // Draw magenta stroke-only ellipse
    p5_no_fill();
    p5_stroke(1.0f, 0.0f, 1.0f);
    p5_stroke_weight(2.0f);
    p5_ellipse(250, 100, 120, 80);
    
    test_renderer_end_frame();
    
    TEST_ASSERT_TRUE(save_framebuffer_as_png("tests/test_output_circles.png", TEST_WIDTH, TEST_HEIGHT));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_circles.png", "tests/golden/circles.png"));
}

void test_line_rendering(void) {
    test_renderer_begin_frame();
    
    p5_init();
    p5_background(1.0f, 1.0f, 1.0f);
    
    // Draw various lines with different colors and weights
    p5_stroke(1.0f, 0.0f, 0.0f);
    p5_stroke_weight(1.0f);
    p5_line(10, 10, 390, 10);     // Red horizontal line
    
    p5_stroke(0.0f, 1.0f, 0.0f);
    p5_stroke_weight(2.0f);
    p5_line(10, 10, 10, 290);     // Green vertical line
    
    p5_stroke(0.0f, 0.0f, 1.0f);
    p5_stroke_weight(3.0f);
    p5_line(10, 10, 390, 290);    // Blue diagonal line
    
    test_renderer_end_frame();
    
    TEST_ASSERT_TRUE(save_framebuffer_as_png("tests/test_output_lines.png", TEST_WIDTH, TEST_HEIGHT));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_lines.png", "tests/golden/lines.png"));
}

void test_mixed_shapes(void) {
    test_renderer_begin_frame();
    
    p5_init();
    p5_background(0.95f, 0.95f, 0.95f);
    
    // Create a composition with multiple shapes
    p5_fill(1.0f, 0.5f, 0.0f);  // Orange rectangle
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_rect(50, 50, 60, 40);
    
    p5_fill(0.0f, 0.8f, 0.8f);  // Cyan circle
    p5_circle(200, 70, 50);
    
    p5_fill(0.8f, 0.0f, 0.8f);  // Purple triangle
    p5_triangle(300, 50, 280, 90, 320, 90);
    
    p5_no_fill();               // Gray stroke square
    p5_stroke(0.5f, 0.5f, 0.5f);
    p5_stroke_weight(1.5f);
    p5_square(80, 150, 80);
    
    test_renderer_end_frame();
    
    TEST_ASSERT_TRUE(save_framebuffer_as_png("tests/test_output_mixed.png", TEST_WIDTH, TEST_HEIGHT));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_mixed.png", "tests/golden/mixed_shapes.png"));
}

int main(void) {
    TEST_RUNNER_START();
    
    // Initialize the test renderer for offscreen rendering
    if (!test_renderer_init(TEST_WIDTH, TEST_HEIGHT)) {
        printf("ERROR: Failed to initialize test renderer\n");
        return 1;
    }
    
    // Run visual tests
    RUN_TEST(test_rectangle_rendering);
    RUN_TEST(test_circle_rendering);
    RUN_TEST(test_line_rendering);
    RUN_TEST(test_mixed_shapes);
    
    // Cleanup
    test_renderer_shutdown();
    
    TEST_RUNNER_END();
}