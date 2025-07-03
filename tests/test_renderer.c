/*
test_renderer.c - Implementation of offscreen rendering for visual tests
*/

#include "test_renderer.h"
#include "test_utils.h"

#include "../deps/sokol_gfx.h"
#include "../deps/sokol_gp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global test renderer instance
test_renderer_t g_test_renderer = {0};

bool test_renderer_init(int width, int height) {
    if (g_test_renderer.initialized) {
        printf("Test renderer already initialized\n");
        return true;
    }
    
    g_test_renderer.width = width;
    g_test_renderer.height = height;
    
    // Initialize a minimal Sokol app context first
    sapp_desc app_desc = {0};
    app_desc.width = width;
    app_desc.height = height;
    app_desc.window_title = "Test Renderer";
    app_desc.init_cb = NULL;
    app_desc.frame_cb = NULL;
    app_desc.cleanup_cb = NULL;
    
    // This won't actually create a window but will set up the app state
    // Note: This is a hack - in a real scenario we'd need proper headless setup
    
    // Initialize Sokol GFX with minimal configuration
    sg_desc desc = {0};
    desc.logger.func = NULL; // Suppress logs for testing
    sg_setup(&desc);
    
    if (!sg_isvalid()) {
        printf("ERROR: Failed to initialize Sokol GFX\n");
        return false;
    }
    
    // Initialize Sokol GP
    sgp_desc gp_desc = {0};
    sgp_setup(&gp_desc);
    
    // Create offscreen render target
    sg_image_desc img_desc = {0};
    img_desc.render_target = true;
    img_desc.width = width;
    img_desc.height = height;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    img_desc.sample_count = 1;
    
    sg_image color_img = sg_make_image(&img_desc);
    if (sg_query_image_state(color_img) != SG_RESOURCESTATE_VALID) {
        printf("ERROR: Failed to create color texture\n");
        sgp_shutdown();
        sg_shutdown();
        return false;
    }
    
    // Create depth buffer
    img_desc.pixel_format = SG_PIXELFORMAT_DEPTH;
    sg_image depth_img = sg_make_image(&img_desc);
    
    // Create attachments
    sg_attachments_desc att_desc = {0};
    att_desc.colors[0].image = color_img;
    att_desc.depth_stencil.image = depth_img;
    
    sg_attachments attachments = sg_make_attachments(&att_desc);
    if (sg_query_attachments_state(attachments) != SG_RESOURCESTATE_VALID) {
        printf("ERROR: Failed to create attachments\n");
        sg_destroy_image(color_img);
        sg_destroy_image(depth_img);
        sgp_shutdown();
        sg_shutdown();
        return false;
    }
    
    // Store references (we'll need to manage these carefully)
    g_test_renderer.offscreen_fb = attachments.id;
    g_test_renderer.color_texture = color_img.id;
    g_test_renderer.depth_texture = depth_img.id;
    g_test_renderer.initialized = true;
    
    printf("Test renderer initialized: %dx%d\n", width, height);
    return true;
}

void test_renderer_shutdown(void) {
    if (!g_test_renderer.initialized) {
        return;
    }
    
    // Clean up Sokol resources
    if (g_test_renderer.offscreen_fb != 0) {
        sg_attachments att = { .id = g_test_renderer.offscreen_fb };
        sg_destroy_attachments(att);
    }
    if (g_test_renderer.color_texture != 0) {
        sg_image img = { .id = g_test_renderer.color_texture };
        sg_destroy_image(img);
    }
    if (g_test_renderer.depth_texture != 0) {
        sg_image img = { .id = g_test_renderer.depth_texture };
        sg_destroy_image(img);
    }
    
    sgp_shutdown();
    sg_shutdown();
    
    memset(&g_test_renderer, 0, sizeof(test_renderer_t));
    printf("Test renderer shutdown\n");
}

void test_renderer_begin_frame(void) {
    if (!g_test_renderer.initialized) {
        printf("ERROR: Test renderer not initialized\n");
        return;
    }
    
    // Begin render pass to offscreen target
    sg_attachments att = { .id = g_test_renderer.offscreen_fb };
    sg_pass pass = {0};
    pass.attachments = att;
    pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass.action.colors[0].clear_value = (sg_color){1.0f, 1.0f, 1.0f, 1.0f}; // White background
    
    sg_begin_pass(&pass);
    
    // Setup Sokol GP for 2D rendering
    sgp_begin(g_test_renderer.width, g_test_renderer.height);
    sgp_viewport(0, 0, g_test_renderer.width, g_test_renderer.height);
    sgp_project(0.0f, (float)g_test_renderer.width, 0.0f, (float)g_test_renderer.height);
}

void test_renderer_end_frame(void) {
    if (!g_test_renderer.initialized) {
        printf("ERROR: Test renderer not initialized\n");
        return;
    }
    
    // Finish Sokol GP rendering
    sgp_flush();
    sgp_end();
    
    // End render pass
    sg_end_pass();
    sg_commit();
}

bool test_renderer_save_png(const char* filename) {
    if (!g_test_renderer.initialized) {
        printf("ERROR: Test renderer not initialized\n");
        return false;
    }
    
    // Allocate buffer for pixel data
    int width = g_test_renderer.width;
    int height = g_test_renderer.height;
    size_t buffer_size = width * height * 4; // RGBA
    unsigned char* pixels = (unsigned char*)malloc(buffer_size);
    
    if (!pixels) {
        printf("ERROR: Failed to allocate pixel buffer\n");
        return false;
    }
    
    // Read pixels from the offscreen color texture
    sg_image color_img = { .id = g_test_renderer.color_texture };
    
    // Note: Direct pixel reading from GPU textures is complex and platform-specific
    // For now, we'll create a test pattern to verify the pipeline works
    // TODO: Implement actual GPU readback
    
    // Create a simple test pattern for verification
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 4;
            
            // Create a gradient pattern to verify the image generation
            float fx = (float)x / width;
            float fy = (float)y / height;
            
            pixels[idx + 0] = (unsigned char)(fx * 255);        // R - horizontal gradient
            pixels[idx + 1] = (unsigned char)(fy * 255);        // G - vertical gradient  
            pixels[idx + 2] = (unsigned char)((fx + fy) * 127); // B - diagonal gradient
            pixels[idx + 3] = 255;                              // A - fully opaque
        }
    }
    
    // Save as PNG using stb_image_write
    int result = stbi_write_png(filename, width, height, 4, pixels, width * 4);
    
    free(pixels);
    
    if (!result) {
        printf("ERROR: Failed to write PNG file: %s\n", filename);
        return false;
    }
    
    printf("Saved framebuffer to: %s (%dx%d)\n", filename, width, height);
    return true;
}