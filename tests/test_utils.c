/*
test_utils.c - Implementation of testing utilities for p5.h library
*/

#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Note: STB implementations are now in deps/test_deps.c

bool file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

bool save_framebuffer_as_png(const char* filename, int width, int height) {
    // This is a stub implementation for simple tests
    // In real visual tests, this would use test_renderer_save_png()
    printf("save_framebuffer_as_png: %s (%dx%d) - stub implementation\n", filename, width, height);
    return true;
}

bool compare_images(const char* test_image, const char* golden_image) {
    if (!file_exists(test_image)) {
        printf("ERROR: Test image does not exist: %s\n", test_image);
        return false;
    }
    
    if (!file_exists(golden_image)) {
        printf("WARNING: Golden image does not exist: %s\n", golden_image);
        printf("Creating golden image from test image...\n");
        
        // Copy test image to golden image location for first run
        FILE* src = fopen(test_image, "rb");
        FILE* dst = fopen(golden_image, "wb");
        
        if (!src || !dst) {
            printf("ERROR: Failed to create golden image\n");
            if (src) fclose(src);
            if (dst) fclose(dst);
            return false;
        }
        
        // Copy file contents
        char buffer[4096];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
            fwrite(buffer, 1, bytes, dst);
        }
        
        fclose(src);
        fclose(dst);
        
        printf("Golden image created. Test passes by default.\n");
        return true;
    }
    
    // Load both images
    int test_w, test_h, test_channels;
    int golden_w, golden_h, golden_channels;
    
    unsigned char* test_data = stbi_load(test_image, &test_w, &test_h, &test_channels, 4);
    unsigned char* golden_data = stbi_load(golden_image, &golden_w, &golden_h, &golden_channels, 4);
    
    if (!test_data || !golden_data) {
        printf("ERROR: Failed to load images for comparison\n");
        if (test_data) stbi_image_free(test_data);
        if (golden_data) stbi_image_free(golden_data);
        return false;
    }
    
    // Check dimensions match
    if (test_w != golden_w || test_h != golden_h) {
        printf("ERROR: Image dimensions don't match - Test: %dx%d, Golden: %dx%d\n", 
               test_w, test_h, golden_w, golden_h);
        stbi_image_free(test_data);
        stbi_image_free(golden_data);
        return false;
    }
    
    // Compare pixel data
    int total_pixels = test_w * test_h * 4;
    int diff_pixels = 0;
    
    for (int i = 0; i < total_pixels; i++) {
        if (test_data[i] != golden_data[i]) {
            diff_pixels++;
        }
    }
    
    stbi_image_free(test_data);
    stbi_image_free(golden_data);
    
    // Allow for small differences (threshold of 1% different pixels)
    float diff_percentage = (float)diff_pixels / total_pixels * 100.0f;
    bool images_match = diff_percentage < 1.0f;
    
    if (images_match) {
        printf("Images match (%.2f%% difference)\n", diff_percentage);
    } else {
        printf("Images differ significantly (%.2f%% difference)\n", diff_percentage);
    }
    
    return images_match;
}
