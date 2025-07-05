#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"
#define P5_IMPLEMENTATION
#include "p5.h"

void setup() {
    createCanvas(600, 400);
    background(240, 240, 240);
}

void draw() {
    background(240, 240, 240);
    
    // Test different stroke weights
    stroke(255, 0, 0);  // Red stroke
    
    // Thin line (default weight = 1)
    strokeWeight(1);
    line(50, 50, 200, 50);
    
    // Medium line
    strokeWeight(3);
    line(50, 80, 200, 80);
    
    // Thick line
    strokeWeight(8);
    line(50, 110, 200, 110);
    
    // Very thick line
    strokeWeight(15);
    line(50, 140, 200, 140);
    
    // Test stroke weight on shapes
    stroke(0, 0, 255);  // Blue stroke
    fill(255, 255, 0);  // Yellow fill
    
    strokeWeight(2);
    rect(250, 50, 80, 60);
    
    strokeWeight(5);
    circle(350, 80, 60);
    
    strokeWeight(8);
    triangle(450, 50, 500, 110, 400, 110);
    
    // Test points with different weights
    stroke(0, 255, 0);  // Green
    strokeWeight(5);
    point(50, 200);
    point(80, 200);
    point(110, 200);
    
    strokeWeight(10);
    point(50, 230);
    point(80, 230);
    point(110, 230);
    
    strokeWeight(20);
    point(50, 270);
    point(80, 270);
    point(110, 270);
}

P5_MAIN(800, 600, "Stroke Weight Test");