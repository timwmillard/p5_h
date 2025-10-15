#include <stdio.h>
#include <time.h>

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#define P5_IMPLEMENTATION
#include "p5.h"


typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    int size;
    Color color;
} Ball;

#define BALL_COUNT 50
Ball balls[BALL_COUNT];

float randfrom(const float start, const float end)
{
    return (end - start) * rand() / RAND_MAX - start;
}

void setup()
{
    srand(time(NULL));
    
    for (int i = 0; i < BALL_COUNT; i++) {
        balls[i].x = randfrom(0, (float) width());
        balls[i].y = randfrom(0, (float) height());
        balls[i].dx = randfrom(0.9f, 20);
        balls[i].dy = randfrom(0.9f, 20);
        balls[i].size = (int) randfrom(50.0f, 90.0f);
        balls[i].color.r = randfrom(0, 255);
        balls[i].color.g = randfrom(0, 255);
        balls[i].color.b = randfrom(0, 255);

    }
}

void draw()
{
    background(COLOR("#888"));
    // fill(COLOR("#157ed4"));

    for (int i = 0; i < BALL_COUNT; i++) {
        fill_rgba(balls[i].color.r, balls[i].color.g, balls[i].color.b, 100);
        circle(balls[i].x, balls[i].y, balls[i].size);
        balls[i].x += balls[i].dx;
        balls[i].y += balls[i].dy;

        if (balls[i].x < 0 || balls[i].x > width()) balls[i].dx *= -1;
        if (balls[i].y < 0 || balls[i].y > height())  balls[i].dy *= -1;
    }
}

P5_MAIN(600, 400, "Balls");

