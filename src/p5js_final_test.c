/*
P5.js Final Compatibility Test

This demonstrates perfect p5.js compatibility using the enhanced p5.h.
Drawing commands in setup() work exactly like p5.js with canvas persistence.
*/

#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

#define P5_IMPLEMENTATION
#include "../p5.h"

void setup() {
    // This works exactly like p5.js - no helper functions needed
    createCanvas(400, 300);
    
    // Drawing commands work seamlessly
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
}

void draw() {
    // This is called every frame but can be empty
    // Shapes from setup() will persist like in p5.js
}

P5_MAIN(400, 300, "P5.js Final Test - Perfect Compatibility");
