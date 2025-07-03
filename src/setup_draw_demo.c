#include <stdio.h>
#include <stdbool.h>

// sokol dependencies
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

// p5 library with implementation
#define P5_IMPLEMENTATION
#include "../p5.h"

// Static flag to draw only once (like p5.js behavior)
static bool has_drawn = false;

void setup() {
    // Called once at startup - initialize variables, canvas, etc.
    createCanvas(400, 300);
    printf("Setup complete! Drawing will happen once in first draw() call.\n");
}

void draw() {
    // This mimics p5.js behavior where you can draw "static" content once
    if (!has_drawn) {
        printf("Drawing static shapes once...\n");
        
        // Set background
        p5_background(0.95f, 0.95f, 0.95f);
        
        // Draw static shapes that will remain on screen
        
        // Red rectangle
        p5_fill(1.0f, 0.0f, 0.0f);
        p5_stroke(0.0f, 0.0f, 0.0f);
        p5_rect(50, 50, 100, 75);
        
        // Blue circle  
        p5_fill(0.0f, 0.0f, 1.0f);
        p5_circle(250, 100, 60);
        
        // Green triangle
        p5_fill(0.0f, 0.8f, 0.0f);
        p5_triangle(100, 200, 150, 150, 200, 200);
        
        // Yellow line
        p5_stroke(1.0f, 1.0f, 0.0f);
        p5_stroke_weight(3.0f);
        p5_line(250, 150, 350, 250);
        
        // Orange text-like dots to show "static" drawing
        p5_fill(1.0f, 0.5f, 0.0f);
        p5_no_stroke();
        p5_circle(50, 250, 8);
        p5_circle(70, 250, 8);
        p5_circle(90, 250, 8);
        
        has_drawn = true;
        printf("Static drawing complete!\n");
    }
    
    // After the first frame, draw() does nothing
    // This creates the effect of "drawing once" like in p5.js setup()
}

// Create the sokol_main function with p5.js-style
P5_MAIN(400, 300, "Static Drawing Demo");
