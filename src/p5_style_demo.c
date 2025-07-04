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
    // Called once at startup - initialize your sketch here
    createCanvas(600, 450);  // Create canvas smaller than window
}

void draw() {
    // Called every frame - your drawing code here
    time += 1.0f/60.0f;
    
    // Background
    p5_background(25, 25, 50);
    
    // Animated rotating rectangle (responsive to canvas size)
    p5_push();
    p5_translate(width()/2, height()/2);
    p5_rotate(time);
    p5_fill(255, 128, 50);
    p5_stroke(255, 255, 255);
    p5_rect(-50.0f, -30.0f, 100.0f, 60.0f);
    p5_pop();
    
    // Colorful circles (responsive to canvas width)
    for (int i = 0; i < 4; i++) {
        float x = 80.0f + i * (width() - 160) / 3.0f;
        float y = 80.0f;
        float r = (float)i / 3.0f;
        float g = 0.5f + 0.5f * sinf(time + i);
        float b = 0.8f;
        
        p5_fill((unsigned int)(r * 255), (unsigned int)(g * 255), (unsigned int)(b * 255));
        p5_no_stroke();
        p5_circle(x, y, 40.0f);
    }
    
    // Animated triangle (responsive to canvas size)
    p5_push();
    p5_translate(width()/2, height() - 80);
    p5_scale(1.5f + 0.5f * sinf(time * 2.0f));
    p5_fill(50, 255, 128);
    p5_stroke(0, 0, 0);
    p5_triangle(-30.0f, 15.0f, 30.0f, 15.0f, 0.0f, -20.0f);
    p5_pop();
}

// Create the sokol_main function with p5.js-style
P5_MAIN(640, 480, "P5.js Style Demo");
