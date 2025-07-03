/*
test_basic_shapes.c - Test basic shape rendering functionality
Tests rectangle, circle, line, triangle, and other basic shapes
*/

#define P5_NO_APP
#define P5_IMPLEMENTATION
#include "../p5.h"

#include "test_utils.h"

// Test dimensions
#define TEST_WIDTH 400
#define TEST_HEIGHT 300

void test_rectangle_rendering(void) {
    // Setup p5 state
    p5_init();
    
    // Test p5 API without actual rendering for now
    // This tests that the functions can be called without crashing
    
    // Clear background
    p5_background(1.0f, 1.0f, 1.0f);  // White background
    
    // Draw filled rectangle
    p5_fill(1.0f, 0.0f, 0.0f);  // Red fill
    p5_stroke(0.0f, 0.0f, 0.0f); // Black stroke
    p5_rect(50, 50, 100, 75);
    
    // Draw stroke-only rectangle
    p5_no_fill();
    p5_stroke(0.0f, 0.0f, 1.0f); // Blue stroke
    p5_stroke_weight(3.0f);
    p5_rect(200, 50, 100, 75);
    
    // Create a simple test image
    save_framebuffer_as_png("tests/test_output_rectangles.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_rectangles.png", "tests/golden/rectangles.png"));
}

void test_circle_rendering(void) {
    p5_init();
    
    p5_background(0.9f, 0.9f, 0.9f);  // Light gray background
    
    // Draw filled circle
    p5_fill(0.0f, 1.0f, 0.0f);  // Green fill
    p5_stroke(0.0f, 0.0f, 0.0f); // Black stroke
    p5_circle(100, 100, 80);
    
    // Draw stroke-only ellipse
    p5_no_fill();
    p5_stroke(1.0f, 0.0f, 1.0f); // Magenta stroke
    p5_stroke_weight(2.0f);
    p5_ellipse(250, 100, 120, 80);
    
    save_framebuffer_as_png("tests/test_output_circles.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_circles.png", "tests/golden/circles.png"));
}

void test_line_rendering(void) {
    p5_init();
    
    p5_background(1.0f, 1.0f, 1.0f);  // White background
    
    // Draw various lines
    p5_stroke(1.0f, 0.0f, 0.0f); // Red
    p5_stroke_weight(1.0f);
    p5_line(10, 10, 390, 10);     // Horizontal line
    
    p5_stroke(0.0f, 1.0f, 0.0f); // Green
    p5_stroke_weight(2.0f);
    p5_line(10, 10, 10, 290);     // Vertical line
    
    p5_stroke(0.0f, 0.0f, 1.0f); // Blue
    p5_stroke_weight(3.0f);
    p5_line(10, 10, 390, 290);    // Diagonal line
    
    save_framebuffer_as_png("tests/test_output_lines.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_lines.png", "tests/golden/lines.png"));
}

void test_triangle_rendering(void) {
    p5_init();
    
    p5_background(0.8f, 0.8f, 1.0f);  // Light blue background
    
    // Draw filled triangle
    p5_fill(1.0f, 1.0f, 0.0f);  // Yellow fill
    p5_stroke(0.0f, 0.0f, 0.0f); // Black stroke
    p5_triangle(100, 50, 50, 150, 150, 150);
    
    // Draw stroke-only triangle
    p5_no_fill();
    p5_stroke(1.0f, 0.0f, 0.0f); // Red stroke
    p5_stroke_weight(2.0f);
    p5_triangle(300, 50, 250, 150, 350, 150);
    
    save_framebuffer_as_png("tests/test_output_triangles.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_triangles.png", "tests/golden/triangles.png"));
}

void test_mixed_shapes(void) {
    p5_init();
    
    p5_background(0.95f, 0.95f, 0.95f);  // Very light gray
    
    // Create a composition with multiple shapes
    p5_fill(1.0f, 0.5f, 0.0f);  // Orange
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_rect(50, 50, 60, 40);
    
    p5_fill(0.0f, 0.8f, 0.8f);  // Cyan
    p5_circle(200, 70, 50);
    
    p5_fill(0.8f, 0.0f, 0.8f);  // Purple
    p5_triangle(300, 50, 280, 90, 320, 90);
    
    p5_no_fill();
    p5_stroke(0.5f, 0.5f, 0.5f);
    p5_stroke_weight(1.5f);
    p5_square(80, 150, 80);
    
    save_framebuffer_as_png("tests/test_output_mixed.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_mixed.png", "tests/golden/mixed_shapes.png"));
}

int main(void) {
    TEST_RUNNER_START();
    
    RUN_TEST(test_rectangle_rendering);
    RUN_TEST(test_circle_rendering);
    RUN_TEST(test_line_rendering);
    RUN_TEST(test_triangle_rendering);
    RUN_TEST(test_mixed_shapes);
    
    TEST_RUNNER_END();
}