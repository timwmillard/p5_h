#include <stdio.h>
#include "p5.h"

void setup() {
}

void draw() {
    background(color("lawngreen"));

    fill(color("#00ff00"));
    rect(3, 400, 50, 40);
    rect(50, 4, 400, 40);

    // background(COLOR("steelblue"));
    rect(100, 40, 200, 100);
}
