#include <stdio.h>
#include "p5.h"

void setup() {
}

void draw() {
    background(color("white"));

    stroke(color("#0000ff"));
    strokeWeight(10.0f);
    // fill(color("#aa4433"));
    // noFill();
    rect(3, 400, 50, 40);
    rect(50, 4, 400, 40);
    rect(100, 40, 200, 100);
    fill(color("red"));
    // noFill();
    circle(200, 300, 200);
}
