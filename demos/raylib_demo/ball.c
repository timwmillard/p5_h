#include <stdio.h>
#include <time.h>

#include "p5.h"


typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    int size;
    Color color;
} Ball;

#define BALL_COUNT 20
Ball balls[BALL_COUNT];

float randfrom(float start, float end)
{
    return (end - start) * rand() / RAND_MAX + start;
}

void setup()
{
    srand(time(NULL));
    for (int i = 0; i < BALL_COUNT; i++) {
        balls[i].x = randfrom(0, width());
        balls[i].y = randfrom(0, height());
        balls[i].dx = randfrom(0.9, 20);
        balls[i].dy = randfrom(0.9, 20);
        balls[i].size = randfrom(50, 90);
        balls[i].color.r = randfrom(0, 255);
        balls[i].color.g = randfrom(0, 255);
        balls[i].color.b = randfrom(0, 255);
    }
    // printf("width = %d, height = %d\n", width(), height());
}

void draw_ball(Ball *ball) {
    fill_rgba(ball->color.r, ball->color.g, ball->color.b, 100);
    printf("ball(%f, %f, %d)\n", ball->x, ball->y, ball->size);
    stroke_rgba(ball->color.r * 1.1, ball->color.g * 1.1, ball->color.b * 1.1, 100);
    strokeWeight(ball->size*0.02);
    circle(ball->x, ball->y, ball->size);
    ball->x += ball->dx;
    ball->y += ball->dy;

    if (ball->x < 0 || ball->x > width()) ball->dx *= -1;
    if (ball->y < 0 || ball->y > height())  ball->dy *= -1;
}

void draw()
{
    background(COLOR("#888"));
    stroke(COLOR("#157ed4"));
    // circle(50, 50, 100);
    square(50, 50, 100);

    for (int i = 0; i < BALL_COUNT; i++) {
        Ball *ball = &balls[i];
        draw_ball(ball);
    }
}
