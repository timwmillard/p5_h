/*
test_colors.c - Test color functionality
Tests fill, stroke, color creation, and color state management
*/

#define P5_NO_APP
#define P5_IMPLEMENTATION
#include "../p5.h"

#include "test_utils.h"

#define TEST_WIDTH 400
#define TEST_HEIGHT 300

void test_basic_colors(void) {
    p5_init();
    
    p5_background(1.0f, 1.0f, 1.0f);  // White background
    
    // Test primary colors
    p5_fill(1.0f, 0.0f, 0.0f);  // Red
    p5_no_stroke();
    p5_rect(20, 20, 60, 60);
    
    p5_fill(0.0f, 1.0f, 0.0f);  // Green
    p5_rect(100, 20, 60, 60);
    
    p5_fill(0.0f, 0.0f, 1.0f);  // Blue
    p5_rect(180, 20, 60, 60);
    
    // Test secondary colors
    p5_fill(1.0f, 1.0f, 0.0f);  // Yellow
    p5_rect(260, 20, 60, 60);
    
    p5_fill(1.0f, 0.0f, 1.0f);  // Magenta
    p5_rect(20, 100, 60, 60);
    
    p5_fill(0.0f, 1.0f, 1.0f);  // Cyan
    p5_rect(100, 100, 60, 60);
    
    save_framebuffer_as_png("tests/test_output_colors.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_colors.png", "tests/golden/colors.png"));
}

void test_color_with_alpha(void) {
    p5_init();
    
    p5_background(0.9f, 0.9f, 0.9f);  // Light gray background
    
    // Test overlapping shapes with alpha
    p5_fill_alpha(1.0f, 0.0f, 0.0f, 0.7f);  // Semi-transparent red
    p5_no_stroke();
    p5_circle(150, 100, 100);
    
    p5_fill_alpha(0.0f, 1.0f, 0.0f, 0.7f);  // Semi-transparent green
    p5_circle(200, 100, 100);
    
    p5_fill_alpha(0.0f, 0.0f, 1.0f, 0.7f);  // Semi-transparent blue
    p5_circle(175, 140, 100);
    
    save_framebuffer_as_png("tests/test_output_alpha.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_alpha.png", "tests/golden/alpha.png"));
}

void test_stroke_colors(void) {
    p5_init();
    
    p5_background(1.0f, 1.0f, 1.0f);  // White background
    
    // Test different stroke colors and weights
    p5_no_fill();
    
    p5_stroke(1.0f, 0.0f, 0.0f);  // Red stroke
    p5_stroke_weight(1.0f);
    p5_rect(20, 20, 80, 60);
    
    p5_stroke(0.0f, 1.0f, 0.0f);  // Green stroke
    p5_stroke_weight(3.0f);
    p5_rect(120, 20, 80, 60);
    
    p5_stroke(0.0f, 0.0f, 1.0f);  // Blue stroke
    p5_stroke_weight(5.0f);
    p5_rect(220, 20, 80, 60);
    
    // Test stroke with alpha
    p5_stroke_alpha(1.0f, 0.5f, 0.0f, 0.6f);  // Semi-transparent orange
    p5_stroke_weight(4.0f);
    p5_circle(200, 150, 80);
    
    save_framebuffer_as_png("tests/test_output_stroke_colors.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_stroke_colors.png", "tests/golden/stroke_colors.png"));
}

void test_color_struct(void) {
    p5_init();
    
    p5_background(0.95f, 0.95f, 0.95f);  // Very light gray
    
    // Test p5_color_t structure
    p5_color_t red = p5_color(1.0f, 0.0f, 0.0f);
    p5_color_t green_alpha = p5_color_alpha(0.0f, 1.0f, 0.0f, 0.8f);
    p5_color_t blue = p5_color(0.0f, 0.0f, 1.0f);
    
    p5_fill_color(red);
    p5_no_stroke();
    p5_rect(50, 50, 70, 70);
    
    p5_fill_color(green_alpha);
    p5_rect(150, 50, 70, 70);
    
    p5_stroke_color(blue);
    p5_stroke_weight(3.0f);
    p5_no_fill();
    p5_rect(250, 50, 70, 70);
    
    save_framebuffer_as_png("tests/test_output_color_struct.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_color_struct.png", "tests/golden/color_struct.png"));
}

void test_no_fill_no_stroke(void) {
    p5_init();
    
    p5_background(0.8f, 0.8f, 0.8f);  // Light gray background
    
    // Test normal filled rectangle first
    p5_fill(1.0f, 0.0f, 0.0f);
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_rect(50, 50, 80, 60);
    
    // Test no fill (stroke only)
    p5_no_fill();
    p5_stroke(0.0f, 0.0f, 1.0f);
    p5_stroke_weight(2.0f);
    p5_rect(150, 50, 80, 60);
    
    // Test no stroke (fill only)
    p5_fill(0.0f, 1.0f, 0.0f);
    p5_no_stroke();
    p5_rect(250, 50, 80, 60);
    
    // Test both no fill and no stroke (should not render)
    p5_no_fill();
    p5_no_stroke();
    p5_rect(150, 150, 80, 60);  // This should be invisible
    
    save_framebuffer_as_png("tests/test_output_no_fill_stroke.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_no_fill_stroke.png", "tests/golden/no_fill_stroke.png"));
}

int main(void) {
    TEST_RUNNER_START();
    
    RUN_TEST(test_basic_colors);
    RUN_TEST(test_color_with_alpha);
    RUN_TEST(test_stroke_colors);
    RUN_TEST(test_color_struct);
    RUN_TEST(test_no_fill_no_stroke);
    
    TEST_RUNNER_END();
}