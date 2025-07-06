#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

#define P5_IMPLEMENTATION
#include "../p5.h"

int x = 100;
int y = 100;
float xspeed = 2.5;
float yspeed = 2;


void setup() {
  createCanvas(640, 240);
}

void draw() {
  // background(255, 255, 255);
  background(COLOR("#ffffff"));

  x = x + xspeed;
  y = y + yspeed;

  if (x > width() || x < 0) {
    xspeed = xspeed * -1;
  }
  if (y > height() || y < 0) {
    yspeed = yspeed * -1;
  }

  stroke_rgb(0, 0 ,0);
  fill_rgb(127, 127, 127);
  circle(x, y, 48);

}

P5_MAIN(640, 240, "The Point of Vectors");

