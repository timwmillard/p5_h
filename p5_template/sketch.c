// Simple p5.h sketch template
// This is a basic template for creating p5.js-style graphics in C

#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"
#include "deps/sokol_glue.h"
#include "deps/sokol_gp.h"
#define P5_IMPLEMENTATION
#include "p5.h"

void setup() {
    createCanvas(400, 300);
    // Initialize your sketch here
}

void draw() {
    background(220, 220, 220);  // Light gray background
    
    // Draw a rectangle
    rect(50, 50, 100, 75);
    
    // Draw a circle at center
    circle(width()/2, height()/2, 50);
    
    // Draw a line
    line(0, 0, width(), height());
}

P5_MAIN(640, 480, "My P5 Sketch", 4);