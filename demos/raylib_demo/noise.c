#include <stdio.h>
#include "p5.h"

void setup() {
}

void draw() {
    background(color("white"));
    float dt = deltaTime;
    printf("deltaTime = %f\n", deltaTime);
    float x = noise(dt*100, 0);
    float y = noise(0, dt*100);
    printf("x= %f, dt= %f\n", x, dt);
    fill(color_rgb(150, 150, 150));
    circle(x, y, 0.1);
}
