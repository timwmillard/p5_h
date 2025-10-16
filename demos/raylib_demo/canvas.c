#include <stdio.h>
#include "p5.h"

void setup() {
}

void draw() {
    background(p5_color("gold"));
    Color color = p5_color("#00ff00");
    fill(color);

    rect(3, 400, 50, 40);
    rect(50, 4, 400, 40);

    background(COLOR("steelblue"));
    rect(100, 40, 200, 100);
    printf("DRAW RECT\n");
}
