/*
P5.js Translation Example

This demonstrates perfect p5.js compatibility. You can now take any p5.js sketch
and translate it directly to p5.h without any modifications to the setup/draw pattern.

Original p5.js code:
    function setup() {
        createCanvas(400, 300);
        background(220);
        fill(255, 0, 0);
        rect(50, 50, 100, 75);
        fill(0, 0, 255); 
        circle(250, 100, 60);
    }
    
    function draw() {
        // shapes from setup persist - no need to redraw
    }

Translated p5.h code (this file):
    - Direct translation with identical behavior
    - Drawing in setup() works exactly like p5.js
    - Canvas buffer persists between frames
    - No manual boolean flags needed
*/

#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

#define P5_IMPLEMENTATION
#include "../p5.h"

void setup() {
    // This is identical to p5.js setup() - drawing commands work!
    createCanvas(400, 300);
    background(220, 220, 220);
    
    fill(255, 0, 0);
    rect(50, 50, 100, 75);
    
    fill(0, 0, 255);
    circle(250, 100, 60);
    
    printf("P5.js translation complete - shapes drawn in setup()!\n");
}

void draw() {
    // Just like p5.js - empty draw() and shapes from setup() persist
    // Perfect 1:1 compatibility with p5.js behavior
}

P5_MAIN(400, 300, "P5.js Translation Example");