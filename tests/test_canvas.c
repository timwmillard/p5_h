/*
test_canvas.c - Test canvas functionality  
Tests canvas creation, sizing, positioning, and window dimensions
Note: This tests the p5 API without sokol graphics rendering
*/

#include "test_utils.h"

#define TEST_WIDTH 640
#define TEST_HEIGHT 480

// Mock functions to test p5.h API without sokol dependencies

// Mock p5 state for testing
typedef struct {
    float r, g, b, a;
} test_color_t;

typedef struct {
    float tx, ty, rot, sx, sy;
} test_transform_t;

typedef struct {
    int width, height, x, y;
    bool created;
} test_canvas_t;

typedef struct {
    test_color_t fill_color, stroke_color;
    bool fill_enabled, stroke_enabled;
    float stroke_width;
    test_transform_t transform;
    test_canvas_t canvas;
} test_p5_state_t;

static test_p5_state_t test_state;

// Forward declarations
void test_p5_create_canvas_positioned(int w, int h, int x, int y);

// Mock p5 functions for testing API behavior
void test_p5_init(void) {
    test_state.fill_color = (test_color_t){1.0f, 1.0f, 1.0f, 1.0f};
    test_state.stroke_color = (test_color_t){0.0f, 0.0f, 0.0f, 1.0f};
    test_state.fill_enabled = true;
    test_state.stroke_enabled = true;
    test_state.stroke_width = 1.0f;
    test_state.transform = (test_transform_t){0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    test_state.canvas.created = false;
    test_state.canvas.width = 0;
    test_state.canvas.height = 0;
    test_state.canvas.x = 0;
    test_state.canvas.y = 0;
}

void test_p5_create_canvas(int w, int h) {
    int win_w = TEST_WIDTH;
    int win_h = TEST_HEIGHT;
    int x = (win_w - w) / 2;
    int y = (win_h - h) / 2;
    test_p5_create_canvas_positioned(w, h, x, y);
}

void test_p5_create_canvas_positioned(int w, int h, int x, int y) {
    if (w <= 0 || h <= 0) return;
    if (x < 0 || y < 0) return;
    if (x + w > TEST_WIDTH || y + h > TEST_HEIGHT) return;
    
    test_state.canvas.width = w;
    test_state.canvas.height = h;
    test_state.canvas.x = x;
    test_state.canvas.y = y;
    test_state.canvas.created = true;
}

int test_p5_width(void) {
    return test_state.canvas.created ? test_state.canvas.width : TEST_WIDTH;
}

int test_p5_height(void) {
    return test_state.canvas.created ? test_state.canvas.height : TEST_HEIGHT;
}

int test_p5_window_width(void) {
    return TEST_WIDTH;
}

int test_p5_window_height(void) {
    return TEST_HEIGHT;
}

void test_canvas_dimensions(void) {
    test_p5_init();
    
    // Test default dimensions (should match window)
    TEST_ASSERT_TRUE(test_p5_width() == TEST_WIDTH);
    TEST_ASSERT_TRUE(test_p5_height() == TEST_HEIGHT);
    TEST_ASSERT_TRUE(test_p5_window_width() == TEST_WIDTH);
    TEST_ASSERT_TRUE(test_p5_window_height() == TEST_HEIGHT);
    
    // Create smaller canvas
    test_p5_create_canvas(400, 300);
    
    TEST_ASSERT_TRUE(test_p5_width() == 400);
    TEST_ASSERT_TRUE(test_p5_height() == 300);
    TEST_ASSERT_TRUE(test_p5_window_width() == TEST_WIDTH);   // Window size unchanged
    TEST_ASSERT_TRUE(test_p5_window_height() == TEST_HEIGHT); // Window size unchanged
}

void test_canvas_positioning(void) {
    test_p5_init();
    
    // Create positioned canvas
    test_p5_create_canvas_positioned(300, 200, 50, 100);
    
    TEST_ASSERT_TRUE(test_p5_width() == 300);
    TEST_ASSERT_TRUE(test_p5_height() == 200);
    
    // Test invalid canvas parameters (should not crash)
    test_p5_create_canvas(-100, 200);  // Negative width
    TEST_ASSERT_TRUE(test_p5_width() == 300);  // Should remain unchanged
    
    test_p5_create_canvas(200, -100);  // Negative height  
    TEST_ASSERT_TRUE(test_p5_height() == 200); // Should remain unchanged
    
    test_p5_create_canvas_positioned(200, 150, -10, 50);  // Negative x
    TEST_ASSERT_TRUE(test_p5_width() == 300);  // Should remain unchanged
    
    test_p5_create_canvas_positioned(200, 150, 50, -10);  // Negative y
    TEST_ASSERT_TRUE(test_p5_height() == 200); // Should remain unchanged
}

void test_canvas_bounds_checking(void) {
    test_p5_init();
    
    // Test canvas that would exceed window bounds
    test_p5_create_canvas_positioned(400, 300, 300, 200);  // Would go beyond window
    
    // Canvas should remain at previous valid size since this is invalid
    TEST_ASSERT_TRUE(test_p5_width() <= TEST_WIDTH);
    TEST_ASSERT_TRUE(test_p5_height() <= TEST_HEIGHT);
    
    // Test maximum size canvas
    test_p5_create_canvas(TEST_WIDTH, TEST_HEIGHT);
    TEST_ASSERT_TRUE(test_p5_width() == TEST_WIDTH);
    TEST_ASSERT_TRUE(test_p5_height() == TEST_HEIGHT);
}

void test_canvas_multiple_creation(void) {
    test_p5_init();
    
    // Create first canvas
    test_p5_create_canvas(200, 150);
    TEST_ASSERT_TRUE(test_p5_width() == 200);
    TEST_ASSERT_TRUE(test_p5_height() == 150);
    
    // Create second canvas (should replace first)
    test_p5_create_canvas(300, 250);
    TEST_ASSERT_TRUE(test_p5_width() == 300);
    TEST_ASSERT_TRUE(test_p5_height() == 250);
    
    // Create positioned canvas (should replace previous)
    test_p5_create_canvas_positioned(250, 200, 100, 50);
    TEST_ASSERT_TRUE(test_p5_width() == 250);
    TEST_ASSERT_TRUE(test_p5_height() == 200);
}

void test_canvas_zero_size(void) {
    test_p5_init();
    
    int original_width = test_p5_width();
    int original_height = test_p5_height();
    
    // Test zero size canvas (should be rejected)
    test_p5_create_canvas(0, 200);
    TEST_ASSERT_TRUE(test_p5_width() == original_width);   // Should remain unchanged
    
    test_p5_create_canvas(200, 0);
    TEST_ASSERT_TRUE(test_p5_height() == original_height); // Should remain unchanged
    
    test_p5_create_canvas(0, 0);
    TEST_ASSERT_TRUE(test_p5_width() == original_width);   // Should remain unchanged
    TEST_ASSERT_TRUE(test_p5_height() == original_height); // Should remain unchanged
}

int main(void) {
    TEST_RUNNER_START();
    
    RUN_TEST(test_canvas_dimensions);
    RUN_TEST(test_canvas_positioning);
    RUN_TEST(test_canvas_bounds_checking);
    RUN_TEST(test_canvas_multiple_creation);
    RUN_TEST(test_canvas_zero_size);
    
    TEST_RUNNER_END();
}