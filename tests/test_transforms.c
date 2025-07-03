/*
test_transforms.c - Test transformation functionality
Tests push/pop, translate, rotate, scale transformations
*/

#define P5_NO_APP
#define P5_IMPLEMENTATION
#include "../p5.h"

#include "test_utils.h"

#define TEST_WIDTH 400
#define TEST_HEIGHT 300

void test_translate(void) {
    p5_init();
    
    p5_background(1.0f, 1.0f, 1.0f);  // White background
    
    // Draw rectangle without translation
    p5_fill(1.0f, 0.0f, 0.0f);
    p5_no_stroke();
    p5_rect(0, 0, 50, 50);
    
    // Draw rectangle with translation
    p5_translate(100, 50);
    p5_fill(0.0f, 1.0f, 0.0f);
    p5_rect(0, 0, 50, 50);  // Should appear at (100, 50)
    
    // Multiple translations accumulate
    p5_translate(100, 50);
    p5_fill(0.0f, 0.0f, 1.0f);
    p5_rect(0, 0, 50, 50);  // Should appear at (200, 100)
    
    save_framebuffer_as_png("tests/test_output_translate.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_translate.png", "tests/golden/translate.png"));
}

void test_rotate(void) {
    p5_init();
    
    p5_background(0.9f, 0.9f, 0.9f);  // Light gray background
    
    p5_fill(1.0f, 0.5f, 0.0f);
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_stroke_weight(1.0f);
    
    // Draw rectangle at origin
    p5_translate(200, 150);  // Move to center
    
    // Draw multiple rotated rectangles
    for (int i = 0; i < 8; i++) {
        p5_push();
        p5_rotate(i * PI / 4);  // Rotate by 45 degrees each time
        p5_rect(-25, -10, 50, 20);
        p5_pop();
    }
    
    save_framebuffer_as_png("tests/test_output_rotate.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_rotate.png", "tests/golden/rotate.png"));
}

void test_scale(void) {
    p5_init();
    
    p5_background(1.0f, 1.0f, 1.0f);  // White background
    
    p5_fill(0.0f, 0.8f, 0.8f);
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_stroke_weight(1.0f);
    
    // Draw normal size rectangle
    p5_rect(50, 50, 40, 40);
    
    // Draw scaled rectangle (uniform)
    p5_push();
    p5_translate(150, 50);
    p5_scale(1.5f);
    p5_rect(0, 0, 40, 40);  // Should be 60x60
    p5_pop();
    
    // Draw scaled rectangle (non-uniform)
    p5_push();
    p5_translate(250, 50);
    p5_scale_xy(2.0f, 0.5f);
    p5_rect(0, 0, 40, 40);  // Should be 80x20
    p5_pop();
    
    save_framebuffer_as_png("tests/test_output_scale.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_scale.png", "tests/golden/scale.png"));
}

void test_push_pop(void) {
    p5_init();
    
    p5_background(0.95f, 0.95f, 0.95f);  // Very light gray
    
    p5_fill(1.0f, 0.0f, 0.0f);
    p5_no_stroke();
    
    // Draw at origin
    p5_rect(20, 20, 30, 30);
    
    p5_push();  // Save current transform
    p5_translate(100, 0);
    p5_scale(1.5f);
    p5_fill(0.0f, 1.0f, 0.0f);
    p5_rect(20, 20, 30, 30);  // Should be green, translated, and scaled
    
    p5_push();  // Save transform again
    p5_translate(0, 100);
    p5_rotate(PI / 4);
    p5_fill(0.0f, 0.0f, 1.0f);
    p5_rect(20, 20, 30, 30);  // Should be blue, further transformed
    p5_pop();   // Restore to previous state
    
    p5_fill(1.0f, 1.0f, 0.0f);
    p5_rect(60, 20, 30, 30);  // Should be yellow, same scale as green
    p5_pop();   // Restore to original state
    
    p5_fill(1.0f, 0.0f, 1.0f);
    p5_rect(20, 60, 30, 30);  // Should be magenta, back to normal scale
    
    save_framebuffer_as_png("tests/test_output_push_pop.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_push_pop.png", "tests/golden/push_pop.png"));
}

void test_combined_transforms(void) {
    p5_init();
    
    p5_background(0.8f, 0.8f, 1.0f);  // Light blue background
    
    p5_fill(1.0f, 0.5f, 0.2f);
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_stroke_weight(2.0f);
    
    // Create a complex transformation combining translate, rotate, and scale
    p5_translate(200, 150);  // Move to center
    p5_rotate(PI / 6);       // Rotate 30 degrees
    p5_scale_xy(1.5f, 0.8f); // Scale non-uniformly
    p5_translate(-25, -25);  // Offset from center
    
    // Draw shape with combined transforms
    p5_rect(0, 0, 50, 50);
    
    // Draw a circle to show the center point
    p5_push();
    p5_translate(25, 25);    // Move back to center
    p5_scale(0.5f);          // Make smaller
    p5_fill(0.0f, 0.0f, 0.0f);
    p5_no_stroke();
    p5_circle(0, 0, 20);
    p5_pop();
    
    save_framebuffer_as_png("tests/test_output_combined_transforms.png", TEST_WIDTH, TEST_HEIGHT);
    
    TEST_ASSERT_TRUE(compare_images("tests/test_output_combined_transforms.png", "tests/golden/combined_transforms.png"));
}

int main(void) {
    TEST_RUNNER_START();
    
    RUN_TEST(test_translate);
    RUN_TEST(test_rotate);
    RUN_TEST(test_scale);
    RUN_TEST(test_push_pop);
    RUN_TEST(test_combined_transforms);
    
    TEST_RUNNER_END();
}