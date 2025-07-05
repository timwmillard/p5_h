/*
p5.h - Single header C library providing p5.js-like functionality using sokol_gp
Version: 1.0.0

USAGE:
    Two usage styles available:

    1) P5.js-style (recommended):
    #include "sokol_app.h"
    #include "sokol_gfx.h" 
    #include "sokol_glue.h"
    #include "sokol_gp.h"
    #define P5_IMPLEMENTATION
    #include "p5.h"

    void setup() {
        createCanvas(400, 300);  // Canvas smaller than window
        // Initialize your sketch here (no drawing calls)
    }

    void draw() {
        background(220, 220, 220);  // Light gray background
        rect(50, 50, 100, 75);      // Rectangle
        circle(width()/2, height()/2, 50);  // Centered circle
        // Use p5.js-style function names directly!
    }

    P5_MAIN(640, 480, "My Sketch", 4);  // Window: 640x480, Canvas: 400x300

    2) Manual style (or with #define P5_NO_APP):
    #define P5_NO_APP
    #define P5_IMPLEMENTATION 
    #include "p5.h"
    
    // Setup sokol manually, then call p5_init() and use p5 functions
    void init() {
        sg_setup(&(sg_desc){ .environment = sglue_environment() });
        sgp_setup(&(sgp_desc){0});
        p5_init();
    }
    
    void frame() {
        sgp_begin(sapp_width(), sapp_height());
        sgp_viewport(0, 0, sapp_width(), sapp_height());
        sgp_project(0.0f, (float)sapp_width(), 0.0f, (float)sapp_height());
        
        p5_background(0.1f, 0.1f, 0.2f);
        p5_rect(100, 100, 200, 150);
        // Use p5_ prefixed functions directly
        
        sg_begin_pass(&(sg_pass){ .swapchain = sglue_swapchain() });
        sgp_flush(); sgp_end(); sg_end_pass(); sg_commit();
    }

CONFIGURATION:
    #define P5_NO_SHORT_NAMES       // Disable p5.js-style short names (createCanvas, rect, etc.)
                                    // Use full names (p5_create_canvas, p5_rect, etc.) instead
    #define P5_NO_APP               // Disable automatic app setup (P5_MAIN, setup/draw callbacks)
                                    // Use manual sokol initialization like demo.c

DEPENDENCIES:
    Requires sokol_gp.h to be included before this header

LICENSE:
    Public Domain
*/


#ifdef P5_SOKOL
// sokol dependencies
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#define SOKOL_GP_IMPL
#include "sokol_gp.h"
#define P5_IMPLEMENTATION
#endif // P5_SOKOL


#ifndef P5_H
#define P5_H

#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

// TODO macro for unimplemented functions
#define TODO(msg) do { \
    printf("[WARNING] TODO: %s (function: %s, file: %s, line: %d)\n", \
           msg, __func__, __FILE__, __LINE__); \
} while(0)

//////////////////////////////////////////////////////////////////////////////
//
//                             PUBLIC API
//
//////////////////////////////////////////////////////////////////////////////

#ifndef P5_NO_APP
// Forward declarations for callback functions (only available when app mode is enabled)
void setup(void);    // User-defined setup function (called once)
void draw(void);     // User-defined draw function (called every frame)
#endif

//
// TYPES
//

// Color structure
typedef struct {
    float r, g, b, a;
} p5_color_t;

// Angle mode enumeration
typedef enum {
    P5_DEGREES,
    P5_RADIANS
} p5_angle_mode_t;

// Color mode enumeration
typedef enum {
    P5_RGB,
    P5_HSB,
    P5_HSL
} p5_color_mode_t;

// Arc mode enumeration
typedef enum {
    P5_OPEN,
    P5_CHORD,
    P5_PIE
} p5_arc_mode_t;

// Text output display mode enumeration
typedef enum {
    P5_FALLBACK,
    P5_LABEL
} p5_text_output_mode_t;

//
// INITIALIZATION
//

void p5_init(void);

//
// CANVAS FUNCTIONS
//

void p5_create_canvas(int width, int height);
void p5_create_canvas_positioned(int width, int height, int x, int y);
int p5_width(void);
int p5_height(void);
int p5_window_width(void);
int p5_window_height(void);
void p5_background(unsigned int r, unsigned int g, unsigned int b);
void p5_background_color(p5_color_t color);

//
// COLOR FUNCTIONS
//

p5_color_t p5_color(unsigned int r, unsigned int g, unsigned int b);
p5_color_t p5_color_alpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void p5_fill(unsigned int r, unsigned int g, unsigned int b);
void p5_fill_color(p5_color_t color);
void p5_fill_alpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void p5_stroke(unsigned int r, unsigned int g, unsigned int b);
void p5_stroke_color(p5_color_t color);
void p5_stroke_alpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
void p5_stroke_weight(float weight);
void p5_stroke_hex(const char* hex_string);
void p5_background_named(const char* color_name);
void p5_no_fill(void);
void p5_no_stroke(void);
void p5_angle_mode(p5_angle_mode_t mode);
void p5_color_mode(p5_color_mode_t mode);
void p5_color_mode_range(p5_color_mode_t mode, float max1, float max2, float max3, float maxA);
void p5_text_output(void);
void p5_text_output_mode(p5_text_output_mode_t display_mode);
void p5_describe_canvas(void);
p5_color_t p5_parse_color_string(const char* color_str);

//
// TRANSFORM FUNCTIONS
//

void p5_push(void);
void p5_pop(void);
void p5_translate(float x, float y);
void p5_rotate(float angle);
void p5_scale(float s);
void p5_scale_xy(float sx, float sy);

//
// SHAPE FUNCTIONS
//

void p5_point(float x, float y);
void p5_line(float x1, float y1, float x2, float y2);
void p5_rect(float x, float y, float w, float h);
void p5_square(float x, float y, float size);
void p5_circle(float x, float y, float diameter);
void p5_ellipse(float x, float y, float w, float h);
void p5_triangle(float x1, float y1, float x2, float y2, float x3, float y3);
void p5_quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void p5_arc(float x, float y, float w, float h, float start, float stop);
void p5_arc_mode(float x, float y, float w, float h, float start, float stop, p5_arc_mode_t mode);
void p5_arc_with_mode(float x, float y, float w, float h, float start, float stop, p5_arc_mode_t mode);

//
// MATH CONSTANTS
//

#ifndef PI
#define PI 3.14159265358979323846f
#endif
#ifndef TWO_PI
#define TWO_PI (2.0f * PI)
#endif
#ifndef HALF_PI
#define HALF_PI (PI * 0.5f)
#endif

//
// P5.JS-STYLE SHORT NAMES (disable with #define P5_NO_SHORT_NAMES)
//

#ifndef P5_NO_SHORT_NAMES

// Angle mode constants
#define DEGREES P5_DEGREES
#define RADIANS P5_RADIANS

// Color mode constants
#define RGB P5_RGB
#define HSB P5_HSB
#define HSL P5_HSL

// Arc mode constants
#define OPEN P5_OPEN
#define CHORD P5_CHORD
#define PIE P5_PIE

// Text output display mode constants
#define FALLBACK P5_FALLBACK
#define LABEL P5_LABEL

// Named color constants

#define COLOR p5_parse_color_string

// Type aliases
typedef p5_color_t Color;

// Canvas functions
static inline void createCanvas(int width, int height) { p5_create_canvas(width, height); }
static inline void createCanvasPositioned(int width, int height, int x, int y) { p5_create_canvas_positioned(width, height, x, y); }
static inline int width(void) { return p5_width(); }
static inline int height(void) { return p5_height(); }
static inline int windowWidth(void) { return p5_window_width(); }
static inline int windowHeight(void) { return p5_window_height(); }
static inline void background(unsigned int r, unsigned int g, unsigned int b) { p5_background(r, g, b); }
static inline void backgroundColor(Color color) { p5_background_color(color); }

// Color functions
static inline Color color(unsigned int r, unsigned int g, unsigned int b) { return p5_color(r, g, b); }
static inline Color colorAlpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a) { return p5_color_alpha(r, g, b, a); }
static inline void fill(unsigned int r, unsigned int g, unsigned int b) { p5_fill(r, g, b); }
static inline void fillColor(Color color) { p5_fill_color(color); }
static inline void fillAlpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a) { p5_fill_alpha(r, g, b, a); }
static inline void stroke(unsigned int r, unsigned int g, unsigned int b) { p5_stroke(r, g, b); }
static inline void strokeColor(Color color) { p5_stroke_color(color); }
static inline void strokeAlpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a) { p5_stroke_alpha(r, g, b, a); }
static inline void strokeWeight(float weight) { p5_stroke_weight(weight); }
static inline void noFill(void) { p5_no_fill(); }
static inline void noStroke(void) { p5_no_stroke(); }
static inline void angleMode(p5_angle_mode_t mode) { p5_angle_mode(mode); }
static inline void colorMode(p5_color_mode_t mode) { p5_color_mode(mode); }
static inline void textOutput(void) { p5_text_output(); }
static inline void textOutputMode(p5_text_output_mode_t display_mode) { p5_text_output_mode(display_mode); }
static inline void describeCanvas(void) { p5_describe_canvas(); }

// Transform functions
static inline void push(void) { p5_push(); }
static inline void pop(void) { p5_pop(); }
static inline void translate(float x, float y) { p5_translate(x, y); }
static inline void rotate(float angle) { p5_rotate(angle); }
static inline void scale(float s) { p5_scale(s); }
static inline void scaleXY(float sx, float sy) { p5_scale_xy(sx, sy); }

// Shape functions
static inline void point(float x, float y) { p5_point(x, y); }
static inline void line(float x1, float y1, float x2, float y2) { p5_line(x1, y1, x2, y2); }
static inline void rect(float x, float y, float w, float h) { p5_rect(x, y, w, h); }
static inline void square(float x, float y, float size) { p5_square(x, y, size); }
static inline void circle(float x, float y, float diameter) { p5_circle(x, y, diameter); }
static inline void ellipse(float x, float y, float w, float h) { p5_ellipse(x, y, w, h); }
static inline void triangle(float x1, float y1, float x2, float y2, float x3, float y3) { p5_triangle(x1, y1, x2, y2, x3, y3); }
static inline void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) { p5_quad(x1, y1, x2, y2, x3, y3, x4, y4); }
// Note: In p5.js, arc() with 7 parameters includes mode, but C doesn't support function overloading
// Use arc() for default CHORD mode, or arcWithMode() for specific modes
static inline void arc(float x, float y, float w, float h, float start, float stop) { p5_arc(x, y, w, h, start, stop); }
static inline void arcWithMode(float x, float y, float w, float h, float start, float stop, p5_arc_mode_t mode) { p5_arc_with_mode(x, y, w, h, start, stop, mode); }

#endif // P5_NO_SHORT_NAMES

//
// CONVENIENCE MACROS
//

#ifndef P5_NO_APP
// Convenience macro to create sokol_main (use after defining setup() and draw())
// Only available when P5_NO_APP is not defined
#define P5_MAIN(window_w, window_h, title_str) \
    sapp_desc sokol_main(int argc, char* argv[]) { \
        argc; \
        argv; \
        sapp_desc _p5_desc = {0}; \
        _p5_desc.width = window_w; \
        _p5_desc.height = window_h; \
        _p5_desc.sample_count = 4; \
        _p5_desc.init_cb = p5__sokol_init; \
        _p5_desc.frame_cb = p5__sokol_frame; \
        _p5_desc.cleanup_cb = p5__sokol_cleanup; \
        _p5_desc.event_cb = p5__sokol_event; \
        _p5_desc.window_title = title_str; \
        return _p5_desc; \
    }
#endif // P5_NO_APP

//////////////////////////////////////////////////////////////////////////////
//
//                            IMPLEMENTATION
//
//////////////////////////////////////////////////////////////////////////////

#ifdef P5_IMPLEMENTATION

// Transform state (internal)
typedef struct {
    float tx, ty;     // translation
    float rot;        // rotation
    float sx, sy;     // scale
} p5_transform_t;

// Canvas state (internal)
typedef struct {
    int width, height;    // Canvas dimensions
    int x, y;            // Canvas position within window
    bool created;        // Whether canvas has been created
} p5_canvas_t;

// Shape tracking for accessibility (internal)
typedef struct {
    const char* type;
    float x, y, w, h;
    p5_color_t fill_color;
    p5_color_t stroke_color;
    bool fill_enabled;
    bool stroke_enabled;
} p5_shape_t;

// Drawing state (internal)
typedef struct {
    p5_color_t fill_color;
    p5_color_t stroke_color;
    bool fill_enabled;
    bool stroke_enabled;
    float stroke_width;
    p5_transform_t transform;
    p5_transform_t transform_stack[32];
    int transform_stack_depth;
    p5_canvas_t canvas;
    bool setup_has_drawn;  // Internal flag for p5.js compatibility  
    bool in_setup_mode;    // Currently executing setup() - for p5.js compatibility
    p5_angle_mode_t angle_mode;
    p5_color_mode_t color_mode;
    float color_maxes[4];  // Current color maximums for R/G/B/A (or H/S/B/A or H/S/L/A)
    
    // Accessibility state
    bool text_output_enabled;
    bool text_output_label_mode;
    p5_shape_t shapes[256];  // Track shapes for accessibility
    int shape_count;
} p5_state_t;


//
// GLOBAL STATE
//

p5_state_t p5_state;

//
// INTERNAL FUNCTIONS (p5__ prefix)
//

//
// SOKOL WRAPPER FUNCTIONS (only compiled when app mode is enabled)
//
#ifndef P5_NO_APP

static void p5__sokol_init(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });
    
    sgp_setup(&(sgp_desc){0});  // Inherit MSAA from sokol_app
    p5_init();
    // setup() will be called in first frame when graphics context is active
}

static void p5__sokol_frame(void) {
    sgp_begin(sapp_width(), sapp_height());
    
    // Set viewport to canvas area if canvas was created
    if (p5_state.canvas.created) {
        sgp_viewport(p5_state.canvas.x, p5_state.canvas.y, 
                     p5_state.canvas.width, p5_state.canvas.height);
        sgp_project(0.0f, (float)p5_state.canvas.width, 
                    0.0f, (float)p5_state.canvas.height);
    } else {
        // Default to full window if no canvas created
        sgp_viewport(0, 0, sapp_width(), sapp_height());
        sgp_project(0.0f, (float)sapp_width(), 0.0f, (float)sapp_height());
    }
    
    // P5.js compatibility: Execute setup() drawing commands every frame
    // This simulates canvas persistence by redrawing setup() content each frame
    p5_state.in_setup_mode = true;
    if (!p5_state.setup_has_drawn) {
        setup();  // Call user setup - this will draw every frame now
        p5_state.setup_has_drawn = true;
    } else {
        // Re-execute setup() drawing commands to maintain image persistence
        setup();  // Setup always gets called to redraw its content
    }
    p5_state.in_setup_mode = false;
    
    // Call draw() for any additional per-frame drawing
    draw();
    
    sg_begin_pass(&(sg_pass){
        .swapchain = sglue_swapchain()
    });
    sgp_flush();
    sgp_end();
    sg_end_pass();
    sg_commit();
}

static void p5__sokol_cleanup(void) {
    sgp_shutdown();
    sg_shutdown();
}

static void p5__sokol_event(const sapp_event* ev) {
    if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (ev->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_quit();
        }
    }
}
#endif // P5_NO_APP

//
// INTERNAL HELPER FUNCTIONS
//

// Helper function to apply current transform
static void p5__apply_transform(void) {
    if (p5_state.transform.tx != 0.0f || p5_state.transform.ty != 0.0f ||
        p5_state.transform.rot != 0.0f || 
        p5_state.transform.sx != 1.0f || p5_state.transform.sy != 1.0f) {
        sgp_push_transform();
        if (p5_state.transform.tx != 0.0f || p5_state.transform.ty != 0.0f) {
            sgp_translate(p5_state.transform.tx, p5_state.transform.ty);
        }
        if (p5_state.transform.rot != 0.0f) {
            sgp_rotate(p5_state.transform.rot);
        }
        if (p5_state.transform.sx != 1.0f || p5_state.transform.sy != 1.0f) {
            sgp_scale(p5_state.transform.sx, p5_state.transform.sy);
        }
    }
}

static void p5__restore_transform(void) {
    if (p5_state.transform.tx != 0.0f || p5_state.transform.ty != 0.0f ||
        p5_state.transform.rot != 0.0f || 
        p5_state.transform.sx != 1.0f || p5_state.transform.sy != 1.0f) {
        sgp_pop_transform();
    }
}

//
// PUBLIC API IMPLEMENTATION
//

void p5_init(void) {
    p5_state.fill_color = (p5_color_t){1.0f, 1.0f, 1.0f, 1.0f};
    p5_state.stroke_color = (p5_color_t){0.0f, 0.0f, 0.0f, 1.0f};
    p5_state.fill_enabled = true;
    p5_state.stroke_enabled = true;
    p5_state.stroke_width = 1.0f;
    p5_state.transform = (p5_transform_t){0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    p5_state.transform_stack_depth = 0;
    p5_state.canvas.created = false;
    p5_state.canvas.width = 0;
    p5_state.canvas.height = 0;
    p5_state.canvas.x = 0;
    p5_state.canvas.y = 0;
    p5_state.setup_has_drawn = false;  // Initialize p5.js compatibility flag
    p5_state.in_setup_mode = false;    // Not in setup initially
    p5_state.angle_mode = P5_RADIANS;  // Default to radians like p5.js
    p5_state.color_mode = P5_RGB;      // Default to RGB
    p5_state.color_maxes[0] = 255.0f;  // R max
    p5_state.color_maxes[1] = 255.0f;  // G max
    p5_state.color_maxes[2] = 255.0f;  // B max
    p5_state.color_maxes[3] = 255.0f;  // A max
    
    // Initialize accessibility state
    p5_state.text_output_enabled = false;
    p5_state.text_output_label_mode = false;
    p5_state.shape_count = 0;
}

// Canvas functions
void p5_create_canvas(int w, int h) {
    // Center the canvas in the window
    int win_w = sapp_width();
    int win_h = sapp_height();
    int x = (win_w - w) / 2;
    int y = (win_h - h) / 2;
    p5_create_canvas_positioned(w, h, x, y);
}

void p5_create_canvas_positioned(int w, int h, int x, int y) {
    // P5.js compatibility: only create canvas once (idempotent)
    if (p5_state.canvas.created) return;
    
    // Validate canvas fits within window
    if (w <= 0 || h <= 0) return;
    if (x < 0 || y < 0) return;
    if (x + w > sapp_width() || y + h > sapp_height()) return;
    
    p5_state.canvas.width = w;
    p5_state.canvas.height = h;
    p5_state.canvas.x = x;
    p5_state.canvas.y = y;
    p5_state.canvas.created = true;
}

int p5_width(void) {
    return p5_state.canvas.created ? p5_state.canvas.width : sapp_width();
}

int p5_height(void) {
    return p5_state.canvas.created ? p5_state.canvas.height : sapp_height();
}

int p5_window_width(void) {
    return sapp_width();
}

int p5_window_height(void) {
    return sapp_height();
}

void p5_background(unsigned int r, unsigned int g, unsigned int b) {
    sgp_set_color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    sgp_clear();
}

void p5_background_color(p5_color_t color) {
    sgp_set_color(color.r, color.g, color.b, color.a);
    sgp_clear();
}

// Color functions
p5_color_t p5_color(unsigned int r, unsigned int g, unsigned int b) {
    return (p5_color_t){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
}

p5_color_t p5_color_alpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    return (p5_color_t){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}

void p5_fill(unsigned int r, unsigned int g, unsigned int b) {
    p5_state.fill_color = (p5_color_t){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
    p5_state.fill_enabled = true;
}

void p5_fill_color(p5_color_t color) {
    p5_state.fill_color = color;
    p5_state.fill_enabled = true;
}

void p5_fill_alpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    p5_state.fill_color = (p5_color_t){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    p5_state.fill_enabled = true;
}

void p5_stroke(unsigned int r, unsigned int g, unsigned int b) {
    p5_state.stroke_color = (p5_color_t){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
    p5_state.stroke_enabled = true;
}

void p5_stroke_color(p5_color_t color) {
    p5_state.stroke_color = color;
    p5_state.stroke_enabled = true;
}

void p5_stroke_alpha(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
    p5_state.stroke_color = (p5_color_t){r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    p5_state.stroke_enabled = true;
}

void p5_stroke_weight(float weight) {
    p5_state.stroke_width = weight;
}

void p5_no_fill(void) {
    p5_state.fill_enabled = false;
}

void p5_no_stroke(void) {
    p5_state.stroke_enabled = false;
}

// Angle mode functions
void p5_angle_mode(p5_angle_mode_t mode) {
    p5_state.angle_mode = mode;
}

// Color mode functions
void p5_color_mode(p5_color_mode_t mode) {
    p5_state.color_mode = mode;
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

void p5_color_mode_range(p5_color_mode_t mode, float max1, float max2, float max3, float maxA) {
    p5_state.color_mode = mode;
    p5_state.color_maxes[0] = max1;
    p5_state.color_maxes[1] = max2;
    p5_state.color_maxes[2] = max3;
    p5_state.color_maxes[3] = maxA;
}

// Internal helper functions for accessibility
static void p5__track_shape(const char* type, float x, float y, float w, float h) {
    if (!p5_state.text_output_enabled || p5_state.shape_count >= 256) return;
    
    p5_shape_t* shape = &p5_state.shapes[p5_state.shape_count++];
    shape->type = type;
    shape->x = x;
    shape->y = y;
    shape->w = w;
    shape->h = h;
    shape->fill_color = p5_state.fill_color;
    shape->stroke_color = p5_state.stroke_color;
    shape->fill_enabled = p5_state.fill_enabled;
    shape->stroke_enabled = p5_state.stroke_enabled;
}

static const char* p5__get_color_name(p5_color_t color) {
    // Simple color name mapping for common colors
    if (color.r == 1.0f && color.g == 0.0f && color.b == 0.0f) return "red";
    if (color.r == 0.0f && color.g == 1.0f && color.b == 0.0f) return "green";  
    if (color.r == 0.0f && color.g == 0.0f && color.b == 1.0f) return "blue";
    if (color.r == 1.0f && color.g == 1.0f && color.b == 0.0f) return "yellow";
    if (color.r == 1.0f && color.g == 0.0f && color.b == 1.0f) return "magenta";
    if (color.r == 0.0f && color.g == 1.0f && color.b == 1.0f) return "cyan";
    if (color.r == 1.0f && color.g == 1.0f && color.b == 1.0f) return "white";
    if (color.r == 0.0f && color.g == 0.0f && color.b == 0.0f) return "black";
    if (color.r > 0.4f && color.g > 0.4f && color.b > 0.4f) return "light gray";
    if (color.r < 0.3f && color.g < 0.3f && color.b < 0.3f) return "dark gray";
    return "colored";
}

static const char* p5__get_location_description(float x, float y, float canvas_w, float canvas_h) {
    float center_x = canvas_w / 2.0f;
    float center_y = canvas_h / 2.0f;
    float threshold = 50.0f;
    
    if (fabs(x - center_x) < threshold && fabs(y - center_y) < threshold) {
        return "center";
    } else if (x < center_x / 2.0f) {
        return y < center_y / 2.0f ? "top left" : (y > center_y * 1.5f ? "bottom left" : "left");
    } else if (x > center_x * 1.5f) {
        return y < center_y / 2.0f ? "top right" : (y > center_y * 1.5f ? "bottom right" : "right");
    } else {
        return y < center_y / 2.0f ? "top" : (y > center_y * 1.5f ? "bottom" : "middle");
    }
}

static float p5__calculate_area_percentage(float w, float h, float canvas_w, float canvas_h) {
    if (canvas_w <= 0 || canvas_h <= 0) return 0.0f;
    float shape_area = w * h;
    float canvas_area = canvas_w * canvas_h;
    return (shape_area / canvas_area) * 100.0f;
}

static void p5__output_accessibility_description(void) {
    if (!p5_state.text_output_enabled) return;
    
    FILE* output = p5_state.text_output_label_mode ? stdout : stderr;
    int canvas_w = p5_width();
    int canvas_h = p5_height();
    
    // General description
    fprintf(output, "Canvas description: %d by %d pixels canvas containing %d shapes:\n", 
            canvas_w, canvas_h, p5_state.shape_count);
    
    // Shape list
    for (int i = 0; i < p5_state.shape_count; i++) {
        p5_shape_t* shape = &p5_state.shapes[i];
        const char* color_name = p5__get_color_name(shape->fill_enabled ? shape->fill_color : shape->stroke_color);
        const char* location = p5__get_location_description(shape->x, shape->y, canvas_w, canvas_h);
        float area_pct = p5__calculate_area_percentage(shape->w, shape->h, canvas_w, canvas_h);
        
        fprintf(output, "Shape %d: %s %s at %s covering %.1f%% of canvas\n", 
                i + 1, color_name, shape->type, location, area_pct);
    }
    fprintf(output, "\n");
}

// Text output functions
void p5_text_output(void) {
    p5_text_output_mode(P5_FALLBACK);
}

void p5_text_output_mode(p5_text_output_mode_t display_mode) {
    if (p5_state.text_output_enabled) return;  // Already enabled
    
    p5_state.text_output_enabled = true;
    p5_state.text_output_label_mode = (display_mode == P5_LABEL);
    p5_state.shape_count = 0;  // Reset shape tracking
    
    if (p5_state.text_output_label_mode) {
        printf("Text output accessibility mode enabled (LABEL mode - visible output)\n");
    } else {
        fprintf(stderr, "Text output accessibility mode enabled (FALLBACK mode - screen reader only)\n");
    }
}

void p5_describe_canvas(void) {
    p5__output_accessibility_description();
}

// Helper function to parse hex color strings
static int p5__hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

static p5_color_t p5__parse_hex_color(const char* hex) {
    p5_color_t color = {0.0f, 0.0f, 0.0f, 1.0f};
    int len = strlen(hex);
    
    if (len == 0 || hex[0] != '#') return color;
    
    if (len == 4) { // #RGB
        int r = p5__hex_char_to_int(hex[1]);
        int g = p5__hex_char_to_int(hex[2]);
        int b = p5__hex_char_to_int(hex[3]);
        color.r = (r * 16 + r) / 255.0f;
        color.g = (g * 16 + g) / 255.0f;
        color.b = (b * 16 + b) / 255.0f;
    } else if (len == 7) { // #RRGGBB
        int r = p5__hex_char_to_int(hex[1]) * 16 + p5__hex_char_to_int(hex[2]);
        int g = p5__hex_char_to_int(hex[3]) * 16 + p5__hex_char_to_int(hex[4]);
        int b = p5__hex_char_to_int(hex[5]) * 16 + p5__hex_char_to_int(hex[6]);
        color.r = r / 255.0f;
        color.g = g / 255.0f;
        color.b = b / 255.0f;
    } else if (len == 5) { // #RGBA
        int r = p5__hex_char_to_int(hex[1]);
        int g = p5__hex_char_to_int(hex[2]);
        int b = p5__hex_char_to_int(hex[3]);
        int a = p5__hex_char_to_int(hex[4]);
        color.r = (r * 16 + r) / 255.0f;
        color.g = (g * 16 + g) / 255.0f;
        color.b = (b * 16 + b) / 255.0f;
        color.a = (a * 16 + a) / 255.0f;
    } else if (len == 9) { // #RRGGBBAA
        int r = p5__hex_char_to_int(hex[1]) * 16 + p5__hex_char_to_int(hex[2]);
        int g = p5__hex_char_to_int(hex[3]) * 16 + p5__hex_char_to_int(hex[4]);
        int b = p5__hex_char_to_int(hex[5]) * 16 + p5__hex_char_to_int(hex[6]);
        int a = p5__hex_char_to_int(hex[7]) * 16 + p5__hex_char_to_int(hex[8]);
        color.r = r / 255.0f;
        color.g = g / 255.0f;
        color.b = b / 255.0f;
        color.a = a / 255.0f;
    }
    
    return color;
}

// Named color lookup (subset of CSS colors)
static p5_color_t p5__parse_named_color(const char* name) {
    // Convert to lowercase for case-insensitive comparison
    char lower_name[32];
    int i = 0;
    while (name[i] && i < 31) {
        lower_name[i] = (name[i] >= 'A' && name[i] <= 'Z') ? name[i] + 32 : name[i];
        i++;
    }
    lower_name[i] = '\0';
    
    // Common named colors used in p5.js examples
    if (strcmp(lower_name, "steelblue") == 0) return p5__parse_hex_color("#4682b4");
    if (strcmp(lower_name, "red") == 0) return p5__parse_hex_color("#ff0000");
    if (strcmp(lower_name, "green") == 0) return p5__parse_hex_color("#008000");
    if (strcmp(lower_name, "blue") == 0) return p5__parse_hex_color("#0000ff");
    if (strcmp(lower_name, "white") == 0) return p5__parse_hex_color("#ffffff");
    if (strcmp(lower_name, "black") == 0) return p5__parse_hex_color("#000000");
    if (strcmp(lower_name, "gray") == 0) return p5__parse_hex_color("#808080");
    if (strcmp(lower_name, "grey") == 0) return p5__parse_hex_color("#808080");
    if (strcmp(lower_name, "yellow") == 0) return p5__parse_hex_color("#ffff00");
    if (strcmp(lower_name, "orange") == 0) return p5__parse_hex_color("#ffa500");
    if (strcmp(lower_name, "purple") == 0) return p5__parse_hex_color("#800080");
    if (strcmp(lower_name, "pink") == 0) return p5__parse_hex_color("#ffc0cb");
    
    // Default to white if color not found
    return p5__parse_hex_color("#ffffff");
}

// Public color parsing function
p5_color_t p5_parse_color_string(const char* color_str) {
    if (color_str[0] == '#') {
        return p5__parse_hex_color(color_str);
    } else {
        return p5__parse_named_color(color_str);
    }
}

// Named color background function
void p5_background_named(const char* color_name) {
    p5_color_t color = p5_parse_color_string(color_name);
    p5_background_color(color);
}

// Transform functions
void p5_push(void) {
    if (p5_state.transform_stack_depth < 32) {
        p5_state.transform_stack[p5_state.transform_stack_depth] = p5_state.transform;
        p5_state.transform_stack_depth++;
    }
}

void p5_pop(void) {
    if (p5_state.transform_stack_depth > 0) {
        p5_state.transform_stack_depth--;
        p5_state.transform = p5_state.transform_stack[p5_state.transform_stack_depth];
    }
}

void p5_translate(float x, float y) {
    p5_state.transform.tx += x;
    p5_state.transform.ty += y;
}

void p5_rotate(float angle) {
    p5_state.transform.rot += angle;
}

void p5_scale(float s) {
    p5_state.transform.sx *= s;
    p5_state.transform.sy *= s;
}

void p5_scale_xy(float sx, float sy) {
    p5_state.transform.sx *= sx;
    p5_state.transform.sy *= sy;
}

// Basic shapes
void p5_point(float x, float y) {
    p5__apply_transform();
    sgp_set_color(p5_state.stroke_color.r, p5_state.stroke_color.g, 
                  p5_state.stroke_color.b, p5_state.stroke_color.a);
    sgp_draw_point(x, y);
    p5__restore_transform();
}

void p5_line(float x1, float y1, float x2, float y2) {
    if (!p5_state.stroke_enabled) return;
    
    p5__apply_transform();
    sgp_set_color(p5_state.stroke_color.r, p5_state.stroke_color.g, 
                  p5_state.stroke_color.b, p5_state.stroke_color.a);
    sgp_draw_line(x1, y1, x2, y2);
    p5__restore_transform();
}

void p5_rect(float x, float y, float w, float h) {
    p5__apply_transform();
    
    // Track shape for accessibility (only if not a square, squares are tracked separately)
    if (w != h) {  // Only track if it's not a square (squares are tracked separately)
        p5__track_shape("rectangle", x, y, w, h);
    }
    
    // Fill
    if (p5_state.fill_enabled) {
        sgp_set_color(p5_state.fill_color.r, p5_state.fill_color.g, 
                      p5_state.fill_color.b, p5_state.fill_color.a);
        sgp_draw_filled_rect(x, y, w, h);
    }
    
    // Stroke
    if (p5_state.stroke_enabled) {
        sgp_set_color(p5_state.stroke_color.r, p5_state.stroke_color.g, 
                      p5_state.stroke_color.b, p5_state.stroke_color.a);
        // Draw rectangle outline using lines
        sgp_draw_line(x, y, x + w, y);         // top
        sgp_draw_line(x + w, y, x + w, y + h); // right
        sgp_draw_line(x + w, y + h, x, y + h); // bottom
        sgp_draw_line(x, y + h, x, y);         // left
    }
    
    p5__restore_transform();
}

void p5_circle(float x, float y, float diameter) {
    p5__track_shape("circle", x, y, diameter, diameter);
    p5_ellipse(x, y, diameter, diameter);
}

void p5_ellipse(float x, float y, float w, float h) {
    p5__apply_transform();
    
    // Track shape for accessibility (only if not already tracked by p5_circle)
    if (w != h) {  // Only track if it's not a circle (circles are tracked separately)
        p5__track_shape("ellipse", x, y, w, h);
    }
    
    // Approximate ellipse with segments
    const int segments = 32;
    float rx = w * 0.5f;
    float ry = h * 0.5f;
    float cx = x;
    float cy = y;
    
    // Fill
    if (p5_state.fill_enabled) {
        sgp_set_color(p5_state.fill_color.r, p5_state.fill_color.g, 
                      p5_state.fill_color.b, p5_state.fill_color.a);
        
        // Draw triangular segments for filled ellipse
        for (int i = 0; i < segments; i++) {
            float angle1 = (float)i / segments * TWO_PI;
            float angle2 = (float)(i + 1) / segments * TWO_PI;
            
            float x1 = cx + cosf(angle1) * rx;
            float y1 = cy + sinf(angle1) * ry;
            float x2 = cx + cosf(angle2) * rx;
            float y2 = cy + sinf(angle2) * ry;
            
            sgp_draw_filled_triangle(cx, cy, x1, y1, x2, y2);
        }
    }
    
    // Stroke
    if (p5_state.stroke_enabled) {
        sgp_set_color(p5_state.stroke_color.r, p5_state.stroke_color.g, 
                      p5_state.stroke_color.b, p5_state.stroke_color.a);
        
        // Draw ellipse outline using line segments
        for (int i = 0; i < segments; i++) {
            float angle1 = (float)i / segments * TWO_PI;
            float angle2 = (float)(i + 1) / segments * TWO_PI;
            
            float x1 = cx + cosf(angle1) * rx;
            float y1 = cy + sinf(angle1) * ry;
            float x2 = cx + cosf(angle2) * rx;
            float y2 = cy + sinf(angle2) * ry;
            
            sgp_draw_line(x1, y1, x2, y2);
        }
    }
    
    p5__restore_transform();
}

void p5_triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    p5__apply_transform();
    
    // Track shape for accessibility (use bounding box for triangle)
    float min_x = fminf(fminf(x1, x2), x3);
    float max_x = fmaxf(fmaxf(x1, x2), x3);
    float min_y = fminf(fminf(y1, y2), y3);
    float max_y = fmaxf(fmaxf(y1, y2), y3);
    p5__track_shape("triangle", min_x, min_y, max_x - min_x, max_y - min_y);
    
    // Fill
    if (p5_state.fill_enabled) {
        sgp_set_color(p5_state.fill_color.r, p5_state.fill_color.g, 
                      p5_state.fill_color.b, p5_state.fill_color.a);
        sgp_draw_filled_triangle(x1, y1, x2, y2, x3, y3);
    }
    
    // Stroke
    if (p5_state.stroke_enabled) {
        sgp_set_color(p5_state.stroke_color.r, p5_state.stroke_color.g, 
                      p5_state.stroke_color.b, p5_state.stroke_color.a);
        sgp_draw_line(x1, y1, x2, y2);
        sgp_draw_line(x2, y2, x3, y3);
        sgp_draw_line(x3, y3, x1, y1);
    }
    
    p5__restore_transform();
}

void p5_square(float x, float y, float size) {
    p5__track_shape("square", x, y, size, size);
    p5_rect(x, y, size, size);
}

void p5_quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    p5__apply_transform();
    
    // Track shape for accessibility (use bounding box for quad)
    float min_x = fminf(fminf(fminf(x1, x2), x3), x4);
    float max_x = fmaxf(fmaxf(fmaxf(x1, x2), x3), x4);
    float min_y = fminf(fminf(fminf(y1, y2), y3), y4);
    float max_y = fmaxf(fmaxf(fmaxf(y1, y2), y3), y4);
    p5__track_shape("quad", min_x, min_y, max_x - min_x, max_y - min_y);
    
    // Fill (using two triangles)
    if (p5_state.fill_enabled) {
        sgp_set_color(p5_state.fill_color.r, p5_state.fill_color.g, 
                      p5_state.fill_color.b, p5_state.fill_color.a);
        sgp_draw_filled_triangle(x1, y1, x2, y2, x3, y3);
        sgp_draw_filled_triangle(x1, y1, x3, y3, x4, y4);
    }
    
    // Stroke
    if (p5_state.stroke_enabled) {
        sgp_set_color(p5_state.stroke_color.r, p5_state.stroke_color.g, 
                      p5_state.stroke_color.b, p5_state.stroke_color.a);
        sgp_draw_line(x1, y1, x2, y2);
        sgp_draw_line(x2, y2, x3, y3);
        sgp_draw_line(x3, y3, x4, y4);
        sgp_draw_line(x4, y4, x1, y1);
    }
    
    p5__restore_transform();
}

void p5_arc(float x, float y, float w, float h, float start, float stop) {
    p5_arc_with_mode(x, y, w, h, start, stop, P5_CHORD);
}

void p5_arc_mode(float x, float y, float w, float h, float start, float stop, p5_arc_mode_t mode) {
    p5_arc_with_mode(x, y, w, h, start, stop, mode);
}

void p5_arc_with_mode(float x, float y, float w, float h, float start, float stop, p5_arc_mode_t mode) {
    p5__apply_transform();
    
    const int segments = 32;
    float rx = w * 0.5f;
    float ry = h * 0.5f;
    float cx = x;
    float cy = y;
    
    // Convert angles based on current angle mode
    float start_rad = (p5_state.angle_mode == P5_DEGREES) ? start * PI / 180.0f : start;
    float stop_rad = (p5_state.angle_mode == P5_DEGREES) ? stop * PI / 180.0f : stop;
    float angle_range = stop_rad - start_rad;
    
    // Fill
    if (p5_state.fill_enabled) {
        sgp_set_color(p5_state.fill_color.r, p5_state.fill_color.g, 
                      p5_state.fill_color.b, p5_state.fill_color.a);
        
        // Draw triangular segments for filled arc
        for (int i = 0; i < segments; i++) {
            float t1 = (float)i / segments;
            float t2 = (float)(i + 1) / segments;
            float angle1 = start_rad + t1 * angle_range;
            float angle2 = start_rad + t2 * angle_range;
            
            float x1 = cx + cosf(angle1) * rx;
            float y1 = cy + sinf(angle1) * ry;
            float x2 = cx + cosf(angle2) * rx;
            float y2 = cy + sinf(angle2) * ry;
            
            sgp_draw_filled_triangle(cx, cy, x1, y1, x2, y2);
        }
    }
    
    // Stroke
    if (p5_state.stroke_enabled) {
        sgp_set_color(p5_state.stroke_color.r, p5_state.stroke_color.g, 
                      p5_state.stroke_color.b, p5_state.stroke_color.a);
        
        // Draw arc outline using line segments
        for (int i = 0; i < segments; i++) {
            float t1 = (float)i / segments;
            float t2 = (float)(i + 1) / segments;
            float angle1 = start_rad + t1 * angle_range;
            float angle2 = start_rad + t2 * angle_range;
            
            float x1 = cx + cosf(angle1) * rx;
            float y1 = cy + sinf(angle1) * ry;
            float x2 = cx + cosf(angle2) * rx;
            float y2 = cy + sinf(angle2) * ry;
            
            sgp_draw_line(x1, y1, x2, y2);
        }
        
        // Draw closing lines based on arc mode
        float start_x = cx + cosf(start_rad) * rx;
        float start_y = cy + sinf(start_rad) * ry;
        float stop_x = cx + cosf(stop_rad) * rx;
        float stop_y = cy + sinf(stop_rad) * ry;
        
        if (mode == P5_CHORD) {
            // Connect arc endpoints with straight line
            sgp_draw_line(start_x, start_y, stop_x, stop_y);
        } else if (mode == P5_PIE) {
            // Connect arc endpoints to center
            sgp_draw_line(cx, cy, start_x, start_y);
            sgp_draw_line(cx, cy, stop_x, stop_y);
        }
        // P5_OPEN mode draws no closing lines
    }
    
    p5__restore_transform();
}

#endif // P5_IMPLEMENTATION

#endif // P5_H
