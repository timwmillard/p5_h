#include <stdio.h>
#include <math.h>

// sokol dependencies
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

// p5 library with implementation
#define P5_IMPLEMENTATION
#include "p5.h"

void setup() {
    // printf("setup called\n");
    createCanvas(640, 480);
}

void draw() {
    background(COLOR("#3344ff"));
    fill(COLOR("#38f2ff"));
    stroke(COLOR("#388fff"));
    strokeWeight(2);
    // rotate(45);
    // rect(50, 50, 100, 200);

    // noStroke();
    ellipse(150, 150, 200, 200);
}

P5_MAIN(640, 480, "Handmade");
