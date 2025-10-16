#include <stdio.h>
#include "p5.h"

void setup() {
}

void draw() {
    background(color("white"));

    stroke(color("#0000ff"));
    strokeWeight(2.0f);
    fill(color("#aa4433"));
    rect(3, 400, 50, 40);
    rect(50, 4, 400, 40);
    rect(100, 40, 200, 100);
}
