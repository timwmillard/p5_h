#include <stdio.h>
#include <stdbool.h>

// sokol dependencies
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#define SOKOL_GP_IMPL
#include "sokol_gp.h"

// p5 library with implementation
#define P5_IMPLEMENTATION
#include "../p5.h"


void setup() {
}

void draw() {
}

P5_MAIN(600, 400, "p5 Demo");

