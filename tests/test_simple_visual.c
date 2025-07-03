/*
test_simple_visual.c - Simplified visual test that creates test images
This demonstrates the golden test pipeline without complex Sokol setup
*/

#include "test_utils.h"

// External STB functions
extern int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);

#define TEST_WIDTH 400
#define TEST_HEIGHT 300

// Create a simple test pattern to verify the image pipeline
bool create_test_pattern(const char* filename, int width, int height, int pattern_type) {
    unsigned char* pixels = (unsigned char*)malloc(width * height * 4);
    if (!pixels) {
        printf("ERROR: Failed to allocate pixel buffer\n");
        return false;
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 4;
            
            switch (pattern_type) {
                case 0: // Red rectangle pattern
                    if (x >= 50 && x < 150 && y >= 50 && y < 125) {
                        pixels[idx + 0] = 255; // R
                        pixels[idx + 1] = 0;   // G
                        pixels[idx + 2] = 0;   // B
                    } else {
                        pixels[idx + 0] = 255; // R - white background
                        pixels[idx + 1] = 255; // G
                        pixels[idx + 2] = 255; // B
                    }
                    pixels[idx + 3] = 255; // A
                    break;
                    
                case 1: // Green circle pattern (approximated)
                    {
                        int cx = 100, cy = 100, radius = 40;
                        int dx = x - cx, dy = y - cy;
                        if (dx*dx + dy*dy <= radius*radius) {
                            pixels[idx + 0] = 0;   // R
                            pixels[idx + 1] = 255; // G
                            pixels[idx + 2] = 0;   // B
                        } else {
                            pixels[idx + 0] = 230; // R - light gray background
                            pixels[idx + 1] = 230; // G
                            pixels[idx + 2] = 230; // B
                        }
                    }
                    pixels[idx + 3] = 255; // A
                    break;
                    
                case 2: // Line pattern
                    if ((y >= 10 && y <= 10 && x >= 10 && x <= 390) ||  // Horizontal line
                        (x >= 10 && x <= 12 && y >= 10 && y <= 290) ||  // Vertical line  
                        (abs((y - 10) - (x - 10)) <= 1 && x >= 10 && x <= 390 && y >= 10 && y <= 290)) { // Diagonal
                        pixels[idx + 0] = 0;   // R - black line
                        pixels[idx + 1] = 0;   // G
                        pixels[idx + 2] = 0;   // B
                    } else {
                        pixels[idx + 0] = 255; // R - white background
                        pixels[idx + 1] = 255; // G
                        pixels[idx + 2] = 255; // B
                    }
                    pixels[idx + 3] = 255; // A
                    break;
                    
                default: // Default white
                    pixels[idx + 0] = 255; // R
                    pixels[idx + 1] = 255; // G
                    pixels[idx + 2] = 255; // B
                    pixels[idx + 3] = 255; // A
                    break;
            }
        }
    }
    
    int result = stbi_write_png(filename, width, height, 4, pixels, width * 4);
    free(pixels);
    
    if (!result) {
        printf("ERROR: Failed to write PNG file: %s\n", filename);
        return false;
    }
    
    printf("Created test pattern: %s (%dx%d, pattern %d)\n", filename, width, height, pattern_type);
    return true;
}

void test_rectangle_pattern(void) {
    TEST_ASSERT_TRUE(create_test_pattern("tests/test_output_rectangles.png", TEST_WIDTH, TEST_HEIGHT, 0));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_rectangles.png", "tests/golden/rectangles.png"));
}

void test_circle_pattern(void) {
    TEST_ASSERT_TRUE(create_test_pattern("tests/test_output_circles.png", TEST_WIDTH, TEST_HEIGHT, 1));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_circles.png", "tests/golden/circles.png"));
}

void test_line_pattern(void) {
    TEST_ASSERT_TRUE(create_test_pattern("tests/test_output_lines.png", TEST_WIDTH, TEST_HEIGHT, 2));
    TEST_ASSERT_TRUE(compare_images("tests/test_output_lines.png", "tests/golden/lines.png"));
}

int main(void) {
    TEST_RUNNER_START();
    
    printf("Running simplified visual tests (pattern generation)\n");
    printf("This demonstrates the golden test pipeline\n\n");
    
    RUN_TEST(test_rectangle_pattern);
    RUN_TEST(test_circle_pattern);
    RUN_TEST(test_line_pattern);
    
    TEST_RUNNER_END();
}
