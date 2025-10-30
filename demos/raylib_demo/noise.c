#include <stdio.h>
#include "p5.h"

void setup() {
}
float t = 0;

void draw() {
  // background(color("white"));
  float n = noise(t, 0);
  float x = map(n, 0, 1, 0, width());
    // Use map() to customize the range of Perlin noise.

  fill(color("green"));
  circle(x, 180, 16);
  t += 0.1;
   // Move forward in time.

}

// void draw() {
//     background(color("white"));
//
//     // Use accumulated time for smooth animation
//     static float time = 0;
//     time += deltaTime;
//
//     // Get noise values (returns 0-1)
//     float noiseX = noise(time * 2, 0);
//     float noiseY = noise(0, time * 2);
//
//     // Map noise to canvas dimensions
//     float x = noiseX * width();
//     float y = noiseY * height();
//
//     printf("time=%.2f, noise=(%.3f, %.3f), pos=(%.1f, %.1f)\n",
//            time, noiseX, noiseY, x, y);
//
//     // Draw circle at noise-driven position
//     fill(color_rgb(100, 150, 255));
//     noStroke();
//     circle(x, y, 30);
// }
