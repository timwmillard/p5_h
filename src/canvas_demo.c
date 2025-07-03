#include <stdio.h>
#include <math.h>

// sokol dependencies
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

// p5 library with implementation
#define P5_IMPLEMENTATION
#include "../p5.h"

static float time = 0.0f;

void setup() {
    // Create a 400x300 canvas centered in the 800x600 window
    createCanvas(400, 300);
    
    printf("Window: %dx%d\n", windowWidth(), windowHeight());
    printf("Canvas: %dx%d\n", p5_width(), p5_height());
}

void draw() {
    time += 1.0f/60.0f;
    
    // This background will only fill the canvas area, not the whole window
    p5_background(0.2f, 0.4f, 0.6f);
    
    // Draw a border around the canvas to show its bounds
    p5_no_fill();
    p5_stroke(1.0f, 1.0f, 1.0f);
    p5_rect(2, 2, p5_width()-4, p5_height()-4);
    
    // Animated circle that bounces within canvas bounds
    float x = p5_width()/2 + 100 * sinf(time);
    float y = p5_height()/2 + 50 * cosf(time * 1.5f);
    
    p5_fill(1.0f, 0.5f, 0.2f);
    p5_no_stroke();
    p5_circle(x, y, 40);
    
    // Some text info (simulated with rectangles)
    p5_fill(0.8f, 0.8f, 0.8f);
    p5_rect(10, 10, 80, 20);  // "Canvas Demo" text area
    
    // Corner markers to show canvas bounds
    p5_fill(1.0f, 0.0f, 0.0f);
    p5_rect(0, 0, 10, 10);                    // Top-left
    p5_rect(p5_width()-10, 0, 10, 10);           // Top-right  
    p5_rect(0, p5_height()-10, 10, 10);          // Bottom-left
    p5_rect(p5_width()-10, p5_height()-10, 10, 10); // Bottom-right
}

// 800x600 window with 4x antialiasing
P5_MAIN(800, 600, "Canvas Demo - 400x300 canvas in 800x600 window", 4);
