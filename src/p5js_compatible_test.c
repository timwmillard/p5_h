#include <stdio.h>

// sokol dependencies
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

// p5 library with implementation
#define P5_IMPLEMENTATION
#include "../p5.h"

void setup() {
    // This is now 100% p5.js compatible!
    // Drawing commands in setup() work exactly like in p5.js
    
    createCanvas(400, 300);
    printf("Setup: Drawing shapes directly in setup() - p5.js compatible!\n");
    
    // Set background
    background(240, 240, 240);
    
    // Red rectangle
    fill(255, 0, 0);
    stroke(0, 0, 0);
    rect(50, 50, 100, 75);
    
    // Blue circle
    fill(0, 0, 255);
    circle(250, 100, 60);
    
    // Green triangle  
    fill(0, 200, 0);
    triangle(100, 200, 150, 150, 200, 200);
    
    // Yellow line
    stroke(255, 255, 0);
    strokeWeight(3);
    line(250, 150, 350, 250);
    
    printf("Setup: Drawing complete - shapes will persist!\n");
}

void draw() {
    // Just like in p5.js - draw() can be empty when all drawing is in setup()
    // The shapes from setup() remain visible because we don't call background() again
    // This is 100% compatible with p5.js behavior!
}

P5_MAIN(400, 300, "Enhanced P5.h - True P5.js Compatibility");