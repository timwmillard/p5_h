#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

#define P5_IMPLEMENTATION
#include "../p5.h"

void setup() {
  // Create screen reader accessible description
  // textOutput();

  createCanvas(720, 400);
}

void draw() {
  // Use degrees as units for angles
  // The arc() function uses angles
  angleMode(DEGREES);

  // Draw a light gray background
  background_rgb(220, 220, 220);

  // Draw square
  // x, y, size
  square(20, 20, 100);

  // Draw rectangle on top of square
  // This appears in front of the square because the function is called
  // after (further down)
  // Switching the order of square and rect will make the square appear on
  // top of the rectangle
  // x, y, width, height
  rect(100, 40, 200, 100);

  // Draw eye shape with ellipse, circle, and arc

  // Draw ellipse as outer eye shape
  // x, y, width, height
  ellipse(540, 100, 300, 100);

  // Draw circle as pupil
  // x, y, diameter
  circle(560, 100, 100);

  // Draw arc (compare to ellipse()) as eyelid
  // x, y, width, height, start angle, stop angle, mode
  arc(540, 100, 300, 100, 180, 360);

  // Draw line
  // x1, y1, x2, y2
  line(20, 200, 200, 350);

  // Draw triangle
  // x1, y1, x2, y2, x3, y3
  triangle(250, 350, 350, 200, 450, 350);

  // Draw quadrilateral
  // x1, y1, x2, y2, x3, y3, x4, y4
  quad(500, 250, 550, 200, 700, 300, 650, 350);
}


P5_MAIN(720, 400, "Shape Primitives");

