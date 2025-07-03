/*
P5.js Seamless Compatibility Test

This demonstrates perfect p5.js compatibility - you can now directly translate
p5.js code without any modifications or helper functions!

This is identical to p5.js behavior:
- createCanvas() works seamlessly (only creates once)
- Drawing commands in setup() persist on screen
- No helper functions or wrappers needed
- Perfect 1:1 translation from p5.js
*/

#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

#define P5_IMPLEMENTATION
#include "../p5.h"

void setup() {
    // This is identical to p5.js - no modifications needed!
    createCanvas(400, 300);
    
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
    
    // printf("P5.js code translated seamlessly - no modifications needed!\n");
}

void draw() {
    // Just like p5.js - empty draw() and shapes from setup() persist
    // Perfect compatibility!
}

P5_MAIN(400, 300, "P5.js Seamless Test - Perfect 1:1 Compatibility");