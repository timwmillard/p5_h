/*

p5.h - Single header C library providing p5.js-like functionality using sokol
Version: 0.0.1


USAGE

void setup() {
    createCanvas(800, 600);
}

void draw() {
    background_rgb(220, 220, 220);  // Light gray background
    rect(50, 50, 100, 75);      // Rectangle
    circle(width()/2, height()/2, 50);  // Centered circle
}


CREDITS
    Tim Millard


LICENSE

Copyright (c) 2024 Tim Millard

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef P5_H
#define P5_H

#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// TODO macro for unimplemented functions
#define TODO(msg) do { \
    printf("[WARNING] TODO: %s (function: %s, file: %s, line: %d)\n", \
           msg, __func__, __FILE__, __LINE__); \
} while(0)


// App function
void setup(void);
void draw(void);
                     
// Vector 2 dimension
typedef struct {
    float x, y;
} p5_Vector2;

// Vector 3 dimension
typedef struct {
    float x, y, z;
} p5_Vector3;

// Color structure
typedef struct {
    float r, g, b, a;
} p5_Color;

// Angle mode enumeration
typedef enum {
    P5_DEGREES,
    P5_RADIANS
} p5_AngleMode;

// Color mode enumeration
typedef enum {
    P5_RGB,
    P5_HSB,
    P5_HSL
} p5_ColorMode;

// Arc mode enumeration
typedef enum {
    P5_ARC_OPEN,
    P5_ARC_CHORD,
    P5_ARC_PIE
} p5_ArcMode;

// Draw modes
typedef enum {
    P5_CORNER = 0,     // x,y is top-left corner
    P5_CENTER = 1,     // x,y is center
    P5_RADIUS = 2      // width/height are radius values
} p5_DrawMode;

// Shape close modes
typedef enum {
    P5_SHAPE_OPEN = 0,
    P5_SHAPE_CLOSE = 1
} p5_ShapeCloseMode;

// Math constants
#ifndef PI
#define PI 3.14159265358979323846f
#endif
#ifndef TWO_PI
#define TWO_PI 6.28318530717958647693
#endif
#ifndef HALF_PI
#define HALF_PI 1.57079632679489661923
#endif
#ifndef QUARTER_PI
#define QUARTER_PI 0.7853982
#endif
#ifndef TAU
#define TAU TWO_PI
#endif

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

// Vector functions
char *p5_vector2_to_string(p5_Vector2 vec);
void p5_vector2_set(p5_Vector2 *vec, float x, float y);
p5_Vector2 p5_vector2_copy(p5_Vector2 vec);
p5_Vector2 p5_vector2_add(p5_Vector2 vec1, p5_Vector2 vec2);

// Canvas functions
void p5_create_canvas(int width, int height);
void p5_create_canvas_pos(int width, int height, int x, int y);
int p5_width(void);
int p5_height(void);
int p5_window_width(void);
int p5_window_height(void);
void p5_background(p5_Color color);
void p5_background_rgb(unsigned int r, unsigned int g, unsigned int b);

// Color functions
p5_Color p5_color(const char* color);
p5_Color p5_color_rgb(unsigned int r, unsigned int g, unsigned int b);
p5_Color p5_color_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void p5_fill(p5_Color color);
void p5_fill_rgb(unsigned int r, unsigned int g, unsigned int b);
void p5_fill_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void p5_stroke(p5_Color color);
void p5_stroke_rgb(unsigned int r, unsigned int g, unsigned int b);
void p5_stroke_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void p5_stroke_weight(float weight);
void p5_no_fill(void);
void p5_no_stroke(void);
void p5_angle_mode(p5_AngleMode mode);
void p5_color_mode(p5_ColorMode mode);
void p5_color_mode_range(p5_ColorMode mode, float max1, float max2, float max3, float maxA);
void p5_text_output(void);

// Transform functions
void p5_push(void);
void p5_pop(void);
void p5_translate(float x, float y);
void p5_rotate(float angle);
void p5_scale(float sx, float sy);
void p5_reset_matrix(void);

// Shape functions
void p5_point(float x, float y);
void p5_line(float x1, float y1, float x2, float y2);
void p5_rect(float x, float y, float w, float h);
void p5_square(float x, float y, float size);
void p5_circle(float x, float y, float diameter);
void p5_ellipse(float x, float y, float w, float h);
void p5_triangle(float x1, float y1, float x2, float y2, float x3, float y3);
void p5_quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void p5_arc(float x, float y, float w, float h, float start, float stop);
void p5_arc_with_mode(float x, float y, float w, float h, float start, float stop, p5_ArcMode mode);


/*** Short names ***/
#ifndef P5_NO_SHORT_NAMES

#define DEGREES P5_DEGREES
#define RADIANS P5_RADIANS
#define RGB P5_RGB
#define HSB P5_HSB
#define HSL P5_HSL
#define OPEN P5_OPEN
#define CHORD P5_CHORD
#define PIE P5_PIE

#define max P5_MAX
#define min P5_MIN

// should this be deprecated for color
#define COLOR p5_color

// Type aliases
typedef p5_Color Color;
typedef p5_ColorMode ColorMode;
typedef p5_AngleMode AngleMode;
typedef p5_ArcMode ArcMode;

// Canvas functions
static inline void createCanvas(int width, int height) { p5_create_canvas(width, height); }
static inline void createCanvas_pos(int width, int height, int x, int y) { p5_create_canvas_pos(width, height, x, y); }
static inline int width(void) { return p5_width(); }
static inline int height(void) { return p5_height(); }
static inline int windowWidth(void) { return p5_window_width(); }
static inline int windowHeight(void) { return p5_window_height(); }
static inline void background(Color color) { p5_background(color); }
static inline void background_rgb(unsigned int r, unsigned int g, unsigned int b) { p5_background_rgb(r, g, b); }

// Color functions
static inline Color color(const char* color_str) { return p5_color(color_str); }
static inline Color color_rgb(unsigned int r, unsigned int g, unsigned int b) { return p5_color_rgb(r, g, b); }
static inline Color color_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a) { return p5_color_rgba(r, g, b, a); }
static inline void fill(Color color) { p5_fill(color); }
static inline void fill_rgb(unsigned int r, unsigned int g, unsigned int b) { p5_fill_rgb(r, g, b); }
static inline void fill_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a) { p5_fill_rgba(r, g, b, a); }
static inline void stroke(Color color) { p5_stroke(color); }
static inline void stroke_rgb(unsigned int r, unsigned int g, unsigned int b) { p5_stroke_rgb(r, g, b); }
static inline void stroke_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a) { p5_stroke_rgba(r, g, b, a); }
static inline void strokeWeight(float weight) { p5_stroke_weight(weight); }
static inline void noFill(void) { p5_no_fill(); }
static inline void noStroke(void) { p5_no_stroke(); }
static inline void angleMode(AngleMode mode) { p5_angle_mode(mode); }
static inline void colorMode(ColorMode mode) { p5_color_mode(mode); }
static inline void textOutput(void) { p5_text_output(); }

// Transform functions
static inline void push(void) { p5_push(); }
static inline void pop(void) { p5_pop(); }
static inline void translate(float x, float y) { p5_translate(x, y); }
static inline void rotate(float angle) { p5_rotate(angle); }
static inline void scale(float sx, float sy) { p5_scale(sx, sy); }
static inline void reset_matrix(void) { p5_reset_matrix(); };

// Shape functions
static inline void point(float x, float y) { p5_point(x, y); }
static inline void line(float x1, float y1, float x2, float y2) { p5_line(x1, y1, x2, y2); }
static inline void rect(float x, float y, float w, float h) { p5_rect(x, y, w, h); }
static inline void square(float x, float y, float size) { p5_square(x, y, size); }
static inline void circle(float x, float y, float diameter) { p5_circle(x, y, diameter); }
static inline void ellipse(float x, float y, float w, float h) { p5_ellipse(x, y, w, h); }
static inline void triangle(float x1, float y1, float x2, float y2, float x3, float y3) { p5_triangle(x1, y1, x2, y2, x3, y3); }
static inline void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) { p5_quad(x1, y1, x2, y2, x3, y3, x4, y4); }
static inline void arc(float x, float y, float w, float h, float start, float stop) { p5_arc(x, y, w, h, start, stop); }
static inline void arc_with_mode(float x, float y, float w, float h, float start, float stop, ArcMode mode) { p5_arc_with_mode(x, y, w, h, start, stop, mode); }

#endif // P5_NO_SHORT_NAMES

/*** Dyanmic Array (List) ***/
// Credit: nob.h

#ifndef P5_ASSERT
#include <assert.h>
#define P5_ASSERT assert
#endif /* P__ASSERT */

#ifndef P5_REALLOC
#include <stdlib.h>
#define P5_REALLOC reallocf
#endif /* P5_REALLOC */

#ifndef P5_FREE
#include <stdlib.h>
#define P5_FREE free
#endif /* P5_FREE */

#define P5_ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))
#define P5_ARRAY_GET(array, index) \
    (P5_ASSERT((size_t)index < P5_ARRAY_LEN(array)), array[(size_t)index])

// Initial capacity of a dynamic array
#ifndef P5_DA_INIT_CAP
#define P5_DA_INIT_CAP 256
#endif

#ifdef __cplusplus
#define P5_DECLTYPE_CAST(T) (decltype(T))
#else
#define P5_DECLTYPE_CAST(T)
#endif // __cplusplus

#define p5_da_reserve(da, expected_capacity)                                              \
    do {                                                                                   \
        if ((expected_capacity) > (da)->capacity) {                                        \
            if ((da)->capacity == 0) {                                                     \
                (da)->capacity = P5_DA_INIT_CAP;                                          \
            }                                                                              \
            while ((expected_capacity) > (da)->capacity) {                                 \
                (da)->capacity *= 2;                                                       \
            }                                                                              \
            (da)->items = P5_DECLTYPE_CAST((da)->items)P5_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); \
            P5_ASSERT((da)->items != NULL && "Buy more RAM lol");                         \
        }                                                                                  \
    } while (0)

// Append an item to a dynamic array
#define p5_da_append(da, item)                \
    do {                                       \
        p5_da_reserve((da), (da)->count + 1); \
        (da)->items[(da)->count++] = (item);   \
    } while (0)

#define p5_da_free(da) P5_FREE((da).items)

// Append several items to a dynamic array
#define p5_da_append_many(da, new_items, new_items_count)                                      \
    do {                                                                                        \
        p5_da_reserve((da), (da)->count + (new_items_count));                                  \
        memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); \
        (da)->count += (new_items_count);                                                       \
    } while (0)

#define p5_da_resize(da, new_size)     \
    do {                                \
        p5_da_reserve((da), new_size); \
        (da)->count = (new_size);       \
    } while (0)

#define p5_da_last(da) (da)->items[(P5_ASSERT((da)->count > 0), (da)->count-1)]
#define p5_da_remove_unordered(da, i)               \
    do {                                             \
        size_t j = (i);                              \
        P5_ASSERT(j < (da)->count);                 \
        (da)->items[j] = (da)->items[--(da)->count]; \
    } while(0)

/*
Foreach over Dynamic Arrays. Example:

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} Numbers;

Numbers xs = {0};

p5_da_append(&xs, 69);
p5_da_append(&xs, 420);
p5_da_append(&xs, 1337);

p5_da_foreach(int, x, &xs) {
    // `x` here is a pointer to the current element. You can get its index by taking a difference
    // between `x` and the start of the array which is `x.items`.
    size_t index = x - xs.items;
    p5_log(INFO, "%zu: %d", index, *x);
}
*/
#define p5_da_foreach(Type, it, da) for (Type *it = (da)->items; it < (da)->items + (da)->count; ++it)


/*** Renderer Commands ***/

// typedef struct {
//     float pos[3];
//     float uv[2];
//     uint32_t rgba;
//     float psize;
// } p5render_Vertex;

typedef enum {
    P5_RENDER_ARC,
    P5_RENDER_RECT,
    P5_RENDER_POINT,
    P5_RENDER_QUAD,
    P5_RENDER_TRIANGLE,
} p5render_Type;

typedef struct {
    float x;
    float y;
    float r;
    float start_angle;
    float end_angle;

    p5_Color bg_color;
    p5_Color border_color;
    float border_width;
// void DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color)
} p5render_Arc;

typedef struct {
    float x; //j Number: x-coordinate of the rectangle.
    float y; // Number: y-coordinate of the rectangle.
    float w; // Number: width of the rectangle.
    float h; // Number: height of the rectangle.
    float tl; // Number: optional radius of top-left corner.
    float tr; // Number: optional radius of top-right corner.
    float br; // Number: optional radius of bottom-right corner.
    float bl; // Number: optional radius of bottom-left corner.
} p5render_Rect;

typedef struct {
    float x; //j Number: x-coordinate of the rectangle.
    float y; // Number: y-coordinate of the rectangle.
} p5render_Point;

typedef struct {
} p5render_Quad;

typedef struct {
} p5render_Triangle;

typedef struct {
    p5render_Type type;
    union {
        p5render_Arc arc;
        p5render_Rect rect;
        p5render_Point point;
        p5render_Quad quad;
        p5render_Triangle triangle;
    };
    p5_Color fill_color;
    p5_Color stroke_color;

    bool has_fill;
    bool has_stroke;
    float stroke_width;
} p5render_Command;

// A sized array of render commands.
typedef struct {
    // The underlying max capacity of the array, not necessarily all initialized.
    int32_t capacity;
    // The number of initialized elements in this array. Used for loops and iteration.
    int32_t count;
    // A pointer to the first element in the internal array.
    p5render_Command* items;
} p5render_CommandArray;


/*** Global State ***/

// Transform state (internal)
typedef struct {
    float tx, ty;     // translation
    float rot;        // rotation
    float sx, sy;     // scale
} p5_Transform;

// Canvas state (internal)
typedef struct {
    int width, height;    // Canvas dimensions
    int x, y;            // Canvas position within window
    bool created;        // Whether canvas has been created
} p5_Canvas;

// Drawing state (internal)
typedef struct {
    p5_Color fill_color;
    p5_Color stroke_color;

    bool has_fill;
    bool has_stroke;
    float stroke_width;
    p5_AngleMode angle_mode;
    p5_ColorMode color_mode;
    p5_ArcMode arc_mode;

    p5_Transform transform;

} p5_Draw;



p5render_CommandArray p5render_commands();
void p5render_commands_reset();
p5_Color p5render_clearbackground();

#endif // P5_H


// ██ ███    ███ ██████  ██      ███████ ███    ███ ███████ ███    ██ ████████  █████  ████████ ██  ██████  ███    ██
// ██ ████  ████ ██   ██ ██      ██      ████  ████ ██      ████   ██    ██    ██   ██    ██    ██ ██    ██ ████   ██
// ██ ██ ████ ██ ██████  ██      █████   ██ ████ ██ █████   ██ ██  ██    ██    ███████    ██    ██ ██    ██ ██ ██  ██
// ██ ██  ██  ██ ██      ██      ██      ██  ██  ██ ██      ██  ██ ██    ██    ██   ██    ██    ██ ██    ██ ██  ██ ██
// ██ ██      ██ ██      ███████ ███████ ██      ██ ███████ ██   ████    ██    ██   ██    ██    ██  ██████  ██   ████
//
// >>implementation
//

#ifdef P5_IMPL
static struct {
    p5_Color background_color;

    p5_Draw draw;
    p5_Draw draw_stack[32];
    int draw_stack_depth;

    p5_Canvas canvas;
    bool setup_called; // setup() function has been called, only run once.
    float color_maxes[4];  // Current color maximums for R/G/B/A (or H/S/B/A or H/S/L/A)
    
    p5render_CommandArray commands;

    p5render_CommandArray commands_stack[32];
    int commands_stack_depth;
} p5_state;

p5_Color p5render_background_color()
{
    return p5_state.background_color;
}

p5render_CommandArray p5render_commands()
{
    return p5_state.commands;
}

void p5render_commands_reset()
{
    p5_state.commands.count = 0;
}

void p5_background(p5_Color color)
{
    p5_state.background_color = color;
}

void p5_background_rgb(unsigned int r, unsigned int g, unsigned int b)
{
    p5_state.background_color = (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
}
void p5_point(float x, float y)
{
    p5render_Command cmd = {
        .type = P5_RENDER_POINT,
        .fill_color = p5_state.draw.fill_color,
        .stroke_color = p5_state.draw.stroke_color,
        .has_fill = p5_state.draw.has_fill,
        .has_stroke = p5_state.draw.has_stroke,
        .stroke_width = p5_state.draw.stroke_width,
        .point = {
            .x = x,
            .y = y,
        }
    };
    p5_da_append(&p5_state.commands, cmd);
}

void p5_rect(float x, float y, float w, float h)
{
    p5render_Command cmd = {
        .type = P5_RENDER_RECT,
        .fill_color = p5_state.draw.fill_color,
        .stroke_color = p5_state.draw.stroke_color,
        .has_fill = p5_state.draw.has_fill,
        .has_stroke = p5_state.draw.has_stroke,
        .stroke_width = p5_state.draw.stroke_width,
        .rect = {
            .x = x,
            .y = y,
            .w = w,
            .h = h,
        }
    };
    p5_da_append(&p5_state.commands, cmd);
}

void p5_square(float x, float y, float size)
{
    p5_rect(x, y, size, size);
}

void p5_circle(float x, float y, float diameter)
{
    p5render_Command cmd = {
        .type = P5_RENDER_ARC,
        .fill_color = p5_state.draw.fill_color,
        .stroke_color = p5_state.draw.stroke_color,
        .has_fill = p5_state.draw.has_fill,
        .has_stroke = p5_state.draw.has_stroke,
        .stroke_width = p5_state.draw.stroke_width,
        .arc = {
            .x = x,
            .y = y,
            .r = diameter/2,
        }
    };
    p5_da_append(&p5_state.commands, cmd);
}

// Color functions
p5_Color p5_color_rgb(unsigned int r, unsigned int g, unsigned int b)
{
    return (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
}

p5_Color p5_color_rbga(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    return (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}

void p5_fill(p5_Color color) {
    p5_state.draw.fill_color = color;
    p5_state.draw.has_fill = true;
}

void p5_fill_rgb(unsigned int r, unsigned int g, unsigned int b) {
    p5_state.draw.fill_color = (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
    p5_state.draw.has_fill = true;
}


void p5_fill_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    p5_state.draw.fill_color = (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    p5_state.draw.has_fill = true;
}

void p5_stroke(p5_Color color) {
    p5_state.draw.stroke_color = color;
    p5_state.draw.has_stroke = true;
}

void p5_stroke_rgb(unsigned int r, unsigned int g, unsigned int b) {
    p5_state.draw.stroke_color = (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
    p5_state.draw.has_stroke = true;
}


void p5_stroke_rgba(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    p5_state.draw.stroke_color = (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    p5_state.draw.has_stroke = true;
}

void p5_stroke_weight(float weight) {
    p5_state.draw.stroke_width = weight;
}

void p5_no_fill(void) {
    p5_state.draw.has_fill = false;
}

void p5_no_stroke(void) {
    p5_state.draw.has_stroke = false;
}

// Angle mode functions
void p5_angle_mode(p5_AngleMode mode) {
    p5_state.draw.angle_mode = mode;
}

// Color mode functions
void p5_color_mode(p5_ColorMode mode) {
    p5_state.draw.color_mode = mode;
    // Set default maximums based on color mode
    if (mode == P5_RGB) {
        p5_state.color_maxes[0] = 255.0f;  // R
        p5_state.color_maxes[1] = 255.0f;  // G
        p5_state.color_maxes[2] = 255.0f;  // B
        p5_state.color_maxes[3] = 255.0f;  // A
    } else if (mode == P5_HSB || mode == P5_HSL) {
        p5_state.color_maxes[0] = 360.0f;  // H
        p5_state.color_maxes[1] = 100.0f;  // S
        p5_state.color_maxes[2] = 100.0f;  // B/L
        p5_state.color_maxes[3] = 100.0f;  // A
    }
}

void p5_color_mode_range(p5_ColorMode mode, float max1, float max2, float max3, float maxA) {
    p5_state.draw.color_mode = mode;
    p5_state.color_maxes[0] = max1;
    p5_state.color_maxes[1] = max2;
    p5_state.color_maxes[2] = max3;
    p5_state.color_maxes[3] = maxA;
}

// Text output function (stub for accessibility)
void p5_text_output(void) {
    // This is a stub - in a full implementation this would create
    // screen reader accessible descriptions of the canvas content
    TODO("p5_text_output not implemented");
}

// Helper function to parse hex color strings
static int p5_hex_char_to_int(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

static p5_Color p5_parse_hex_color(const char* hex)
{
    p5_Color color = {0.0f, 0.0f, 0.0f, 1.0f};
    int len = strlen(hex);
    
    if (len == 0 || hex[0] != '#') return color;
    
    if (len == 4) { // #RGB
        int r = p5_hex_char_to_int(hex[1]);
        int g = p5_hex_char_to_int(hex[2]);
        int b = p5_hex_char_to_int(hex[3]);
        color.r = (r * 16 + r) / 255.0f;
        color.g = (g * 16 + g) / 255.0f;
        color.b = (b * 16 + b) / 255.0f;
    } else if (len == 7) { // #RRGGBB
        int r = p5_hex_char_to_int(hex[1]) * 16 + p5_hex_char_to_int(hex[2]);
        int g = p5_hex_char_to_int(hex[3]) * 16 + p5_hex_char_to_int(hex[4]);
        int b = p5_hex_char_to_int(hex[5]) * 16 + p5_hex_char_to_int(hex[6]);
        color.r = r / 255.0f;
        color.g = g / 255.0f;
        color.b = b / 255.0f;
    } else if (len == 5) { // #RGBA
        int r = p5_hex_char_to_int(hex[1]);
        int g = p5_hex_char_to_int(hex[2]);
        int b = p5_hex_char_to_int(hex[3]);
        int a = p5_hex_char_to_int(hex[4]);
        color.r = (r * 16 + r) / 255.0f;
        color.g = (g * 16 + g) / 255.0f;
        color.b = (b * 16 + b) / 255.0f;
        color.a = (a * 16 + a) / 255.0f;
    } else if (len == 9) { // #RRGGBBAA
        int r = p5_hex_char_to_int(hex[1]) * 16 + p5_hex_char_to_int(hex[2]);
        int g = p5_hex_char_to_int(hex[3]) * 16 + p5_hex_char_to_int(hex[4]);
        int b = p5_hex_char_to_int(hex[5]) * 16 + p5_hex_char_to_int(hex[6]);
        int a = p5_hex_char_to_int(hex[7]) * 16 + p5_hex_char_to_int(hex[8]);
        color.r = r / 255.0f;
        color.g = g / 255.0f;
        color.b = b / 255.0f;
        color.a = a / 255.0f;
    }
    
    return color;
}

// Named color lookup (subset of CSS colors)
static p5_Color p5_parse_named_color(const char* name)
{
    // Convert to lowercase for case-insensitive comparison
    char lower_name[32];
    int i = 0;
    while (name[i] && i < 31) {
        lower_name[i] = (name[i] >= 'A' && name[i] <= 'Z') ? name[i] + 32 : name[i];
        i++;
    }
    lower_name[i] = '\0';
    
    // Common named colors used in p5.js examples
    if (strcmp(lower_name, "aliceblue") == 0) return p5_color_rgb(0xf0, 0xf8, 0xff);
    if (strcmp(lower_name, "antiquewhite") == 0) return p5_color_rgb(0xfa, 0xeb, 0xd7);
    if (strcmp(lower_name, "aqua") == 0) return p5_color_rgb(0x00, 0xff, 0xff);
    if (strcmp(lower_name, "aquamarine") == 0) return p5_color_rgb(0x7f, 0xff, 0xd4);
    if (strcmp(lower_name, "azure") == 0) return p5_color_rgb(0xf0, 0xff, 0xff);
    if (strcmp(lower_name, "beige") == 0) return p5_color_rgb(0xf5, 0xf5, 0xdc);
    if (strcmp(lower_name, "bisque") == 0) return p5_color_rgb(0xff, 0xe4, 0xc4);
    if (strcmp(lower_name, "black") == 0) return p5_color_rgb(0x00, 0x00, 0x00);
    if (strcmp(lower_name, "blanchedalmond") == 0) return p5_color_rgb(0xff, 0xeb, 0xcd);
    if (strcmp(lower_name, "blue") == 0) return p5_color_rgb(0x00, 0x00, 0xff);
    if (strcmp(lower_name, "blueviolet") == 0) return p5_color_rgb(0x8a, 0x2b, 0xe2);
    if (strcmp(lower_name, "brown") == 0) return p5_color_rgb(0xa5, 0x2a, 0x2a);
    if (strcmp(lower_name, "burlywood") == 0) return p5_color_rgb(0xde, 0xb8, 0x87);
    if (strcmp(lower_name, "cadetblue") == 0) return p5_color_rgb(0x5f, 0x9e, 0xa0);
    if (strcmp(lower_name, "chartreuse") == 0) return p5_color_rgb(0x7f, 0xff, 0x00);
    if (strcmp(lower_name, "chocolate") == 0) return p5_color_rgb(0xd2, 0x69, 0x1e);
    if (strcmp(lower_name, "coral") == 0) return p5_color_rgb(0xff, 0x7f, 0x50);
    if (strcmp(lower_name, "cornflowerblue") == 0) return p5_color_rgb(0x64, 0x95, 0xed);
    if (strcmp(lower_name, "cornsilk") == 0) return p5_color_rgb(0xff, 0xf8, 0xdc);
    if (strcmp(lower_name, "crimson") == 0) return p5_color_rgb(0xdc, 0x14, 0x3c);
    if (strcmp(lower_name, "cyan") == 0) return p5_color_rgb(0x00, 0xff, 0xff);
    if (strcmp(lower_name, "darkblue") == 0) return p5_color_rgb(0x00, 0x00, 0x8b);
    if (strcmp(lower_name, "darkcyan") == 0) return p5_color_rgb(0x00, 0x8b, 0x8b);
    if (strcmp(lower_name, "darkgoldenrod") == 0) return p5_color_rgb(0xb8, 0x86, 0x0b);
    if (strcmp(lower_name, "darkgray") == 0) return p5_color_rgb(0xa9, 0xa9, 0xa9);
    if (strcmp(lower_name, "darkgrey") == 0) return p5_color_rgb(0xa9, 0xa9, 0xa9);
    if (strcmp(lower_name, "darkgreen") == 0) return p5_color_rgb(0x00, 0x64, 0x00);
    if (strcmp(lower_name, "darkkhaki") == 0) return p5_color_rgb(0xbd, 0xb7, 0x6b);
    if (strcmp(lower_name, "darkmagenta") == 0) return p5_color_rgb(0x8b, 0x00, 0x8b);
    if (strcmp(lower_name, "darkolivegreen") == 0) return p5_color_rgb(0x55, 0x6b, 0x2f);
    if (strcmp(lower_name, "darkorange") == 0) return p5_color_rgb(0xff, 0x8c, 0x00);
    if (strcmp(lower_name, "darkorchid") == 0) return p5_color_rgb(0x99, 0x32, 0xcc);
    if (strcmp(lower_name, "darkred") == 0) return p5_color_rgb(0x8b, 0x00, 0x00);
    if (strcmp(lower_name, "darksalmon") == 0) return p5_color_rgb(0xe9, 0x96, 0x7a);
    if (strcmp(lower_name, "darkseagreen") == 0) return p5_color_rgb(0x8f, 0xbc, 0x8f);
    if (strcmp(lower_name, "darkslateblue") == 0) return p5_color_rgb(0x48, 0x3d, 0x8b);
    if (strcmp(lower_name, "darkslategray") == 0) return p5_color_rgb(0x2f, 0x4f, 0x4f);
    if (strcmp(lower_name, "darkslategrey") == 0) return p5_color_rgb(0x2f, 0x4f, 0x4f);
    if (strcmp(lower_name, "darkturquoise") == 0) return p5_color_rgb(0x00, 0xce, 0xd1);
    if (strcmp(lower_name, "darkviolet") == 0) return p5_color_rgb(0x94, 0x00, 0xd3);
    if (strcmp(lower_name, "deeppink") == 0) return p5_color_rgb(0xff, 0x14, 0x93);
    if (strcmp(lower_name, "deepskyblue") == 0) return p5_color_rgb(0x00, 0xbf, 0xff);
    if (strcmp(lower_name, "dimgray") == 0) return p5_color_rgb(0x69, 0x69, 0x69);
    if (strcmp(lower_name, "dimgrey") == 0) return p5_color_rgb(0x69, 0x69, 0x69);
    if (strcmp(lower_name, "dodgerblue") == 0) return p5_color_rgb(0x1e, 0x90, 0xff);
    if (strcmp(lower_name, "firebrick") == 0) return p5_color_rgb(0xb2, 0x22, 0x22);
    if (strcmp(lower_name, "floralwhite") == 0) return p5_color_rgb(0xff, 0xfa, 0xf0);
    if (strcmp(lower_name, "forestgreen") == 0) return p5_color_rgb(0x22, 0x8b, 0x22);
    if (strcmp(lower_name, "fuchsia") == 0) return p5_color_rgb(0xff, 0x00, 0xff);
    if (strcmp(lower_name, "gainsboro") == 0) return p5_color_rgb(0xdc, 0xdc, 0xdc);
    if (strcmp(lower_name, "ghostwhite") == 0) return p5_color_rgb(0xf8, 0xf8, 0xff);
    if (strcmp(lower_name, "gold") == 0) return p5_color_rgb(0xff, 0xd7, 0x00);
    if (strcmp(lower_name, "goldenrod") == 0) return p5_color_rgb(0xda, 0xa5, 0x20);
    if (strcmp(lower_name, "gray") == 0) return p5_color_rgb(0x80, 0x80, 0x80);
    if (strcmp(lower_name, "grey") == 0) return p5_color_rgb(0x80, 0x80, 0x80);
    if (strcmp(lower_name, "green") == 0) return p5_color_rgb(0x00, 0x80, 0x00);
    if (strcmp(lower_name, "greenyellow") == 0) return p5_color_rgb(0xad, 0xff, 0x2f);
    if (strcmp(lower_name, "honeydew") == 0) return p5_color_rgb(0xf0, 0xff, 0xf0);
    if (strcmp(lower_name, "hotpink") == 0) return p5_color_rgb(0xff, 0x69, 0xb4);
    if (strcmp(lower_name, "indianred") == 0) return p5_color_rgb(0xcd, 0x5c, 0x5c);
    if (strcmp(lower_name, "indigo") == 0) return p5_color_rgb(0x4b, 0x00, 0x82);
    if (strcmp(lower_name, "ivory") == 0) return p5_color_rgb(0xff, 0xff, 0xf0);
    if (strcmp(lower_name, "khaki") == 0) return p5_color_rgb(0xf0, 0xe6, 0x8c);
    if (strcmp(lower_name, "lavender") == 0) return p5_color_rgb(0xe6, 0xe6, 0xfa);
    if (strcmp(lower_name, "lavenderblush") == 0) return p5_color_rgb(0xff, 0xf0, 0xf5);
    if (strcmp(lower_name, "lawngreen") == 0) return p5_color_rgb(0x7c, 0xfc, 0x00);
    if (strcmp(lower_name, "lemonchiffon") == 0) return p5_color_rgb(0xff, 0xfa, 0xcd);
    if (strcmp(lower_name, "lightblue") == 0) return p5_color_rgb(0xad, 0xd8, 0xe6);
    if (strcmp(lower_name, "lightcoral") == 0) return p5_color_rgb(0xf0, 0x80, 0x80);
    if (strcmp(lower_name, "lightcyan") == 0) return p5_color_rgb(0xe0, 0xff, 0xff);
    if (strcmp(lower_name, "lightgoldenrodyellow") == 0) return p5_color_rgb(0xfa, 0xfa, 0xd2);
    if (strcmp(lower_name, "lightgray") == 0) return p5_color_rgb(0xd3, 0xd3, 0xd3);
    if (strcmp(lower_name, "lightgrey") == 0) return p5_color_rgb(0xd3, 0xd3, 0xd3);
    if (strcmp(lower_name, "lightgreen") == 0) return p5_color_rgb(0x90, 0xee, 0x90);
    if (strcmp(lower_name, "lightpink") == 0) return p5_color_rgb(0xff, 0xb6, 0xc1);
    if (strcmp(lower_name, "lightsalmon") == 0) return p5_color_rgb(0xff, 0xa0, 0x7a);
    if (strcmp(lower_name, "lightseagreen") == 0) return p5_color_rgb(0x20, 0xb2, 0xaa);
    if (strcmp(lower_name, "lightskyblue") == 0) return p5_color_rgb(0x87, 0xce, 0xfa);
    if (strcmp(lower_name, "lightslategray") == 0) return p5_color_rgb(0x77, 0x88, 0x99);
    if (strcmp(lower_name, "lightslategrey") == 0) return p5_color_rgb(0x77, 0x88, 0x99);
    if (strcmp(lower_name, "lightsteelblue") == 0) return p5_color_rgb(0xb0, 0xc4, 0xde);
    if (strcmp(lower_name, "lightyellow") == 0) return p5_color_rgb(0xff, 0xff, 0xe0);
    if (strcmp(lower_name, "lime") == 0) return p5_color_rgb(0x00, 0xff, 0x00);
    if (strcmp(lower_name, "limegreen") == 0) return p5_color_rgb(0x32, 0xcd, 0x32);
    if (strcmp(lower_name, "linen") == 0) return p5_color_rgb(0xfa, 0xf0, 0xe6);
    if (strcmp(lower_name, "magenta") == 0) return p5_color_rgb(0xff, 0x00, 0xff);
    if (strcmp(lower_name, "maroon") == 0) return p5_color_rgb(0x80, 0x00, 0x00);
    if (strcmp(lower_name, "mediumaquamarine") == 0) return p5_color_rgb(0x66, 0xcd, 0xaa);
    if (strcmp(lower_name, "mediumblue") == 0) return p5_color_rgb(0x00, 0x00, 0xcd);
    if (strcmp(lower_name, "mediumorchid") == 0) return p5_color_rgb(0xba, 0x55, 0xd3);
    if (strcmp(lower_name, "mediumpurple") == 0) return p5_color_rgb(0x93, 0x70, 0xdb);
    if (strcmp(lower_name, "mediumseagreen") == 0) return p5_color_rgb(0x3c, 0xb3, 0x71);
    if (strcmp(lower_name, "mediumslateblue") == 0) return p5_color_rgb(0x7b, 0x68, 0xee);
    if (strcmp(lower_name, "mediumspringgreen") == 0) return p5_color_rgb(0x00, 0xfa, 0x9a);
    if (strcmp(lower_name, "mediumturquoise") == 0) return p5_color_rgb(0x48, 0xd1, 0xcc);
    if (strcmp(lower_name, "mediumvioletred") == 0) return p5_color_rgb(0xc7, 0x15, 0x85);
    if (strcmp(lower_name, "midnightblue") == 0) return p5_color_rgb(0x19, 0x19, 0x70);
    if (strcmp(lower_name, "mintcream") == 0) return p5_color_rgb(0xf5, 0xff, 0xfa);
    if (strcmp(lower_name, "mistyrose") == 0) return p5_color_rgb(0xff, 0xe4, 0xe1);
    if (strcmp(lower_name, "moccasin") == 0) return p5_color_rgb(0xff, 0xe4, 0xb5);
    if (strcmp(lower_name, "navajowhite") == 0) return p5_color_rgb(0xff, 0xde, 0xad);
    if (strcmp(lower_name, "navy") == 0) return p5_color_rgb(0x00, 0x00, 0x80);
    if (strcmp(lower_name, "oldlace") == 0) return p5_color_rgb(0xfd, 0xf5, 0xe6);
    if (strcmp(lower_name, "olive") == 0) return p5_color_rgb(0x80, 0x80, 0x00);
    if (strcmp(lower_name, "olivedrab") == 0) return p5_color_rgb(0x6b, 0x8e, 0x23);
    if (strcmp(lower_name, "orange") == 0) return p5_color_rgb(0xff, 0xa5, 0x00);
    if (strcmp(lower_name, "orangered") == 0) return p5_color_rgb(0xff, 0x45, 0x00);
    if (strcmp(lower_name, "orchid") == 0) return p5_color_rgb(0xda, 0x70, 0xd6);
    if (strcmp(lower_name, "palegoldenrod") == 0) return p5_color_rgb(0xee, 0xe8, 0xaa);
    if (strcmp(lower_name, "palegreen") == 0) return p5_color_rgb(0x98, 0xfb, 0x98);
    if (strcmp(lower_name, "paleturquoise") == 0) return p5_color_rgb(0xaf, 0xee, 0xee);
    if (strcmp(lower_name, "palevioletred") == 0) return p5_color_rgb(0xdb, 0x70, 0x93);
    if (strcmp(lower_name, "papayawhip") == 0) return p5_color_rgb(0xff, 0xef, 0xd5);
    if (strcmp(lower_name, "peachpuff") == 0) return p5_color_rgb(0xff, 0xda, 0xb9);
    if (strcmp(lower_name, "peru") == 0) return p5_color_rgb(0xcd, 0x85, 0x3f);
    if (strcmp(lower_name, "pink") == 0) return p5_color_rgb(0xff, 0xc0, 0xcb);
    if (strcmp(lower_name, "plum") == 0) return p5_color_rgb(0xdd, 0xa0, 0xdd);
    if (strcmp(lower_name, "powderblue") == 0) return p5_color_rgb(0xb0, 0xe0, 0xe6);
    if (strcmp(lower_name, "purple") == 0) return p5_color_rgb(0x80, 0x00, 0x80);
    if (strcmp(lower_name, "rebeccapurple") == 0) return p5_color_rgb(0x66, 0x33, 0x99);
    if (strcmp(lower_name, "red") == 0) return p5_color_rgb(0xff, 0x00, 0x00);
    if (strcmp(lower_name, "rosybrown") == 0) return p5_color_rgb(0xbc, 0x8f, 0x8f);
    if (strcmp(lower_name, "royalblue") == 0) return p5_color_rgb(0x41, 0x69, 0xe1);
    if (strcmp(lower_name, "saddlebrown") == 0) return p5_color_rgb(0x8b, 0x45, 0x13);
    if (strcmp(lower_name, "salmon") == 0) return p5_color_rgb(0xfa, 0x80, 0x72);
    if (strcmp(lower_name, "sandybrown") == 0) return p5_color_rgb(0xf4, 0xa4, 0x60);
    if (strcmp(lower_name, "seagreen") == 0) return p5_color_rgb(0x2e, 0x8b, 0x57);
    if (strcmp(lower_name, "seashell") == 0) return p5_color_rgb(0xff, 0xf5, 0xee);
    if (strcmp(lower_name, "sienna") == 0) return p5_color_rgb(0xa0, 0x52, 0x2d);
    if (strcmp(lower_name, "silver") == 0) return p5_color_rgb(0xc0, 0xc0, 0xc0);
    if (strcmp(lower_name, "skyblue") == 0) return p5_color_rgb(0x87, 0xce, 0xeb);
    if (strcmp(lower_name, "slateblue") == 0) return p5_color_rgb(0x6a, 0x5a, 0xcd);
    if (strcmp(lower_name, "slategray") == 0) return p5_color_rgb(0x70, 0x80, 0x90);
    if (strcmp(lower_name, "slategrey") == 0) return p5_color_rgb(0x70, 0x80, 0x90);
    if (strcmp(lower_name, "snow") == 0) return p5_color_rgb(0xff, 0xfa, 0xfa);
    if (strcmp(lower_name, "springgreen") == 0) return p5_color_rgb(0x00, 0xff, 0x7f);
    if (strcmp(lower_name, "steelblue") == 0) return p5_color_rgb(0x46, 0x82, 0xb4);
    if (strcmp(lower_name, "tan") == 0) return p5_color_rgb(0xd2, 0xb4, 0x8c);
    if (strcmp(lower_name, "teal") == 0) return p5_color_rgb(0x00, 0x80, 0x80);
    if (strcmp(lower_name, "thistle") == 0) return p5_color_rgb(0xd8, 0xbf, 0xd8);
    if (strcmp(lower_name, "tomato") == 0) return p5_color_rgb(0xff, 0x63, 0x47);
    if (strcmp(lower_name, "turquoise") == 0) return p5_color_rgb(0x40, 0xe0, 0xd0);
    if (strcmp(lower_name, "violet") == 0) return p5_color_rgb(0xee, 0x82, 0xee);
    if (strcmp(lower_name, "wheat") == 0) return p5_color_rgb(0xf5, 0xde, 0xb3);
    if (strcmp(lower_name, "white") == 0) return p5_color_rgb(0xff, 0xff, 0xff);
    if (strcmp(lower_name, "whitesmoke") == 0) return p5_color_rgb(0xf5, 0xf5, 0xf5);
    if (strcmp(lower_name, "yellow") == 0) return p5_color_rgb(0xff, 0xff, 0x00);
    if (strcmp(lower_name, "yellowgreen") == 0) return p5_color_rgb(0x9a, 0xcd, 0x32);
    
    // Default to white if color not found
    return p5_parse_hex_color("#ffffff");
}

// Public color parsing function
p5_Color p5_color(const char* color_str)
{
    if (color_str[0] == '#') {
        return p5_parse_hex_color(color_str);
    } else {
        return p5_parse_named_color(color_str);
    }
}


#endif // P5_IMPL

