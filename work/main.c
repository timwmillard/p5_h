#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#define P5_IMPLEMENTATION
#include "p5.h"

void setup() {
}

int x, y = 0;

void draw() {
    background(COLOR("#062764"));
    fill(COLOR("#157ed4"));
    circle(x++, y++, 100);
}

P5_MAIN(600, 400, "p5 Demo");

