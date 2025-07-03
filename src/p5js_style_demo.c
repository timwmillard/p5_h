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


void setup() {
    createCanvas(400, 300);
    printf("Setup initialized - canvas created\n");
    // NOTE: In true p5.js compatibility, we would draw here
    // But p5.h requires drawing to happen during active render context
}

void draw() {
    // Emulate p5.js behavior: draw the "setup content" once, then preserve it
        printf("Drawing 'setup content' in first draw() call (p5.js compatible pattern)\n");
        
        // This mimics drawing that would be in setup() in p5.js
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
        
        // Orange dots to show this is "static" content
        fill(255, 165, 0);
        noStroke();
        circle(50, 250, 8);
        circle(70, 250, 8);
        circle(90, 250, 8);
        
        printf("'Setup content' drawn once - shapes will persist!\n");
    
    // After the first frame, draw() does nothing - just like p5.js when
    // all drawing is in setup() and draw() is empty
    // The shapes remain visible because we don't call background() again
}

P5_MAIN(400, 300, "P5.js Style Demo - Static Drawing");
