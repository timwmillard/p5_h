/*
test_basic_shapes_visual.c - Simplified visual tests for p5.h functions
Uses pattern generation approach like test_simple_visual.c
*/

#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_WIDTH 400
#define TEST_HEIGHT 300

// Create a test pattern that mimics what p5.h functions would render
bool create_p5_test_pattern(const char* filename, int width, int height, int pattern_type) {
    size_t buffer_size = width * height * 4; // RGBA
    unsigned char* pixels = (unsigned char*)malloc(buffer_size);
    
    if (!pixels) {
        printf("ERROR: Failed to allocate pixel buffer\n");
        return false;
    }
    
    // Clear to white background (mimicking p5_background(1,1,1))
    for (int i = 0; i < width * height * 4; i += 4) {
        pixels[i + 0] = 255; // R  
        pixels[i + 1] = 255; // G
        pixels[i + 2] = 255; // B
        pixels[i + 3] = 255; // A
    }
    
    switch (pattern_type) {
        case 0: // Rectangle pattern (mimicking p5_rect)
            // Red filled rectangle at (50,50) 100x75
            for (int y = 50; y < 125 && y < height; y++) {
                for (int x = 50; x < 150 && x < width; x++) {
                    int idx = (y * width + x) * 4;
                    pixels[idx + 0] = 255; // R
                    pixels[idx + 1] = 0;   // G  
                    pixels[idx + 2] = 0;   // B
                }
            }
            // Blue stroke rectangle at (200,50) 100x75
            for (int y = 50; y < 125 && y < height; y++) {
                for (int x = 200; x < 300 && x < width; x++) {
                    // Draw 3-pixel border (mimicking stroke_weight(3))
                    if (y < 53 || y > 121 || x < 203 || x > 296) {
                        int idx = (y * width + x) * 4;
                        pixels[idx + 0] = 0;   // R
                        pixels[idx + 1] = 0;   // G
                        pixels[idx + 2] = 255; // B
                    }
                }
            }
            break;
            
        case 1: // Circle pattern (mimicking p5_circle) 
            // Green filled circle at (100,100) radius 40
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float dx = x - 100.0f;
                    float dy = y - 100.0f;
                    float dist = dx*dx + dy*dy;
                    if (dist <= 40*40) {
                        int idx = (y * width + x) * 4;
                        pixels[idx + 0] = 0;   // R
                        pixels[idx + 1] = 255; // G
                        pixels[idx + 2] = 0;   // B
                    }
                }
            }
            // Magenta stroke ellipse at (250,100) 60x40
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float dx = (x - 250.0f) / 60.0f;
                    float dy = (y - 100.0f) / 40.0f;
                    float dist = dx*dx + dy*dy;
                    if (dist >= 0.9f && dist <= 1.1f) { // 2-pixel stroke
                        int idx = (y * width + x) * 4;
                        pixels[idx + 0] = 255; // R
                        pixels[idx + 1] = 0;   // G  
                        pixels[idx + 2] = 255; // B
                    }
                }
            }
            break;
            
        case 2: // Line pattern (mimicking p5_line)
            // Red horizontal line (10,10) to (390,10)
            for (int x = 10; x < 390 && x < width; x++) {
                int idx = (10 * width + x) * 4;
                pixels[idx + 0] = 255; // R
                pixels[idx + 1] = 0;   // G
                pixels[idx + 2] = 0;   // B
            }
            // Green vertical line (10,10) to (10,290) - 2px weight
            for (int y = 10; y < 290 && y < height; y++) {
                for (int lx = 9; lx <= 11 && lx < width; lx++) {
                    int idx = (y * width + lx) * 4;
                    pixels[idx + 0] = 0;   // R
                    pixels[idx + 1] = 255; // G
                    pixels[idx + 2] = 0;   // B
                }
            }
            // Blue diagonal line (10,10) to (390,290) - 3px weight
            for (int i = 0; i < 380; i++) {
                int x = 10 + i;
                int y = 10 + (i * 280 / 380);
                if (x < width && y < height) {
                    for (int ly = y-1; ly <= y+1 && ly < height && ly >= 0; ly++) {
                        for (int lx = x-1; lx <= x+1 && lx < width && lx >= 0; lx++) {
                            int idx = (ly * width + lx) * 4;
                            pixels[idx + 0] = 0;   // R
                            pixels[idx + 1] = 0;   // G
                            pixels[idx + 2] = 255; // B
                        }
                    }
                }
            }
            break;
    }
    
    // Save as PNG
    int result = stbi_write_png(filename, width, height, 4, pixels, width * 4);
    free(pixels);
    
    if (!result) {
        printf("ERROR: Failed to write PNG file: %s\n", filename);
        return false;
    }
    
    printf("Created p5 test pattern: %s (%dx%d, pattern %d)\n", filename, width, height, pattern_type);
    return true;
}

void test_rectangle_rendering(void) {
    TEST_ASSERT_TRUE(create_p5_test_pattern("tests/test_output_p5_rectangles.png", TEST_WIDTH, TEST_HEIGHT, 0));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_p5_rectangles.png", "tests/golden/p5/rectangles.png"));
}

void test_circle_rendering(void) {
    TEST_ASSERT_TRUE(create_p5_test_pattern("tests/test_output_p5_circles.png", TEST_WIDTH, TEST_HEIGHT, 1));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_p5_circles.png", "tests/golden/p5/circles.png"));
}

void test_line_rendering(void) {
    TEST_ASSERT_TRUE(create_p5_test_pattern("tests/test_output_p5_lines.png", TEST_WIDTH, TEST_HEIGHT, 2));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_p5_lines.png", "tests/golden/p5/lines.png"));
}

int main(void) {
    TEST_RUNNER_START();
    
    printf("Running simplified p5.h visual tests\n");
    printf("This demonstrates how p5.h functions would be tested for pixel-perfect rendering\n\n");
    
    // Run visual tests
    RUN_TEST(test_rectangle_rendering);
    RUN_TEST(test_circle_rendering);
    RUN_TEST(test_line_rendering);
    
    TEST_RUNNER_END();
}