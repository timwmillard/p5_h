#include <stdio.h>
#include "p5.h"

void setup() {
}

void draw() {
    float dt = 0;
    float x = noise(dt, 0);
    float y = noise(0, dt);
    printf("x= %f, dt= %f\n", x, dt);
    fill(color_rgb(150, 150, 150));
    circle(x, y, 0.1);
}
