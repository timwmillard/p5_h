#include <stdio.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

#define P5_IMPLEMENTATION
#include "../p5.h"

void setup() {
  // Create screen reader accessible description
  textOutput();

  createCanvas(720, 400);
  angleMode(DEGREES);

  // CSS color name
  // For a list of available color names, see:
  // https://www.w3.org/wiki/CSS/Properties/color/keywords
  background(COLOR("steelblue"));

  // Set width of stroke to 4 units
  strokeWeight(4);

  // The default color mode uses
  // Red, green, and blue values
  // On a scale of 0-255
  // Light blue
  fill_rgb(200, 200, 255);

  // Dark blue
  stroke_rgb(20, 20, 100);
  square(20, 20, 100);

  // Dark red
  stroke_rgb(100, 20, 20);

  // The rectangle uses the last set fill color,
  // which is light blue, set before drawing the square
  rect(100, 40, 200, 100);

  // Hue, saturation, and brightness values
  // On scales of 0-360°, 0-100%, and 0-100% respectively
  colorMode(HSB);

  // Light green
  fill_rgb(120, 70, 90);

  // Dark green
  stroke_rgb(120, 60, 30);
  ellipse(540, 100, 300, 100);

  // Dark fuchsia
  fill_rgb(300, 90, 30);

  // Draw without lines
  noStroke();
  circle(560, 100, 100);

  // Hue, saturation, and lightness values
  // On scales of 0-360°, 0-100%, and 0-100% respectively
  // This is similar to HSB (above),
  // but whereas 100% brightness is the brightest version of that hue,
  // 100% lightness is always white.
  colorMode(HSL);

  // Light green
  fill_rgb(120, 70, 90);

  // Dark green
  stroke_rgb(120, 60, 30);
  arcMode(540, 100, 300, 100, 180, 360, CHORD);

  // Save current settings
  push();

  // Switch back to red, green, blue color mode
  colorMode(RGB);

  // Navy blue
  stroke_rgb(20, 10, 80);
  line(20, 200, 200, 350);

  // Restore last saved settings
  pop();

  // Because the color settings were saved after drawing the arc
  // and then restored before drawing the triangle,
  // the triangle uses the same settings as the arc
  triangle(250, 350, 350, 200, 450, 350);

  // Hex string
  // This is a set of red, green, blue values
  // Encoded in base 16
  stroke(COLOR("#EFD8D8"));

  // Draw without inner color
  noFill();
  quad(500, 250, 550, 200, 700, 300, 650, 350);
}

void draw() {
    // This is called every frame but can be empty
    // Shapes from setup() will persist like in p5.js
}

P5_MAIN(720, 400, "Shape Primitives");

