#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#define P5_IMPLEMENTATION
#include "p5.h"

void setup() {
}

void draw() {
    background(COLOR("#345"));
    fill(COLOR("#222"));
    circle(200, 100, 50);
}

P5_MAIN(600, 400, "p5 Demo");

