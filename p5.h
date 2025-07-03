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

#ifndef P5_H
#define P5_H

#include <stdbool.h>
#include <math.h>

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
} p5_state_t;

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
void p5_background(float r, float g, float b);
void p5_background_color(p5_color_t color);

//
// COLOR FUNCTIONS
//

p5_color_t p5_color(float r, float g, float b);
p5_color_t p5_color_alpha(float r, float g, float b, float a);
void p5_fill(float r, float g, float b);
void p5_fill_color(p5_color_t color);
void p5_fill_alpha(float r, float g, float b, float a);
void p5_stroke(float r, float g, float b);
void p5_stroke_color(p5_color_t color);
void p5_stroke_alpha(float r, float g, float b, float a);
void p5_stroke_weight(float weight);
void p5_no_fill(void);
void p5_no_stroke(void);

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

// Type aliases
typedef p5_color_t Color;

// Canvas functions
static inline void createCanvas(int width, int height) { p5_create_canvas(width, height); }
static inline void createCanvasPositioned(int width, int height, int x, int y) { p5_create_canvas_positioned(width, height, x, y); }
static inline int width(void) { return p5_width(); }
static inline int height(void) { return p5_height(); }
static inline int windowWidth(void) { return p5_window_width(); }
static inline int windowHeight(void) { return p5_window_height(); }
static inline void background(float r, float g, float b) { p5_background(r, g, b); }
static inline void backgroundColor(Color color) { p5_background_color(color); }

// Color functions
static inline Color color(float r, float g, float b) { return p5_color(r, g, b); }
static inline Color colorAlpha(float r, float g, float b, float a) { return p5_color_alpha(r, g, b, a); }
static inline void fill(float r, float g, float b) { p5_fill(r, g, b); }
static inline void fillColor(Color color) { p5_fill_color(color); }
static inline void fillAlpha(float r, float g, float b, float a) { p5_fill_alpha(r, g, b, a); }
static inline void stroke(float r, float g, float b) { p5_stroke(r, g, b); }
static inline void strokeColor(Color color) { p5_stroke_color(color); }
static inline void strokeAlpha(float r, float g, float b, float a) { p5_stroke_alpha(r, g, b, a); }
static inline void strokeWeight(float weight) { p5_stroke_weight(weight); }
static inline void noFill(void) { p5_no_fill(); }
static inline void noStroke(void) { p5_no_stroke(); }

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
static inline void arc(float x, float y, float w, float h, float start, float stop) { p5_arc(x, y, w, h, start, stop); }

#endif // P5_NO_SHORT_NAMES

//
// CONVENIENCE MACROS
//

#ifndef P5_NO_APP
// Convenience macro to create sokol_main (use after defining setup() and draw())
// Only available when P5_NO_APP is not defined
#define P5_MAIN(window_w, window_h, title_str) \
    sapp_desc sokol_main(int argc, char* argv[]) { \
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

// sokol_gp.h should already be included before this header

//
// GLOBAL STATE
//

p5_state_t p5_state;

//
// INTERNAL FUNCTIONS (p5__ prefix)
//

#ifndef P5_NO_APP
// Sokol callback wrapper functions (only available when app mode is enabled)
static void p5__sokol_init(void);
static void p5__sokol_frame(void);
static void p5__sokol_cleanup(void);
static void p5__sokol_event(const sapp_event* ev);
#endif

// Internal helper functions
static void p5__apply_transform(void);
static void p5__restore_transform(void);

#ifndef P5_NO_APP
//
// SOKOL WRAPPER FUNCTIONS (only compiled when app mode is enabled)
//

static void p5__sokol_init(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });
    
    sgp_setup(&(sgp_desc){0});  // Inherit MSAA from sokol_app
    p5_init();
    setup();  // Call user setup function
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
    
    draw();  // Call user draw function
    
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

void p5_background(float r, float g, float b) {
    sgp_set_color(r, g, b, 1.0f);
    sgp_clear();
}

void p5_background_color(p5_color_t color) {
    sgp_set_color(color.r, color.g, color.b, color.a);
    sgp_clear();
}

// Color functions
p5_color_t p5_color(float r, float g, float b) {
    return (p5_color_t){r, g, b, 1.0f};
}

p5_color_t p5_color_alpha(float r, float g, float b, float a) {
    return (p5_color_t){r, g, b, a};
}

void p5_fill(float r, float g, float b) {
    p5_state.fill_color = (p5_color_t){r, g, b, 1.0f};
    p5_state.fill_enabled = true;
}

void p5_fill_color(p5_color_t color) {
    p5_state.fill_color = color;
    p5_state.fill_enabled = true;
}

void p5_fill_alpha(float r, float g, float b, float a) {
    p5_state.fill_color = (p5_color_t){r, g, b, a};
    p5_state.fill_enabled = true;
}

void p5_stroke(float r, float g, float b) {
    p5_state.stroke_color = (p5_color_t){r, g, b, 1.0f};
    p5_state.stroke_enabled = true;
}

void p5_stroke_color(p5_color_t color) {
    p5_state.stroke_color = color;
    p5_state.stroke_enabled = true;
}

void p5_stroke_alpha(float r, float g, float b, float a) {
    p5_state.stroke_color = (p5_color_t){r, g, b, a};
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
    p5_ellipse(x, y, diameter, diameter);
}

void p5_ellipse(float x, float y, float w, float h) {
    p5__apply_transform();
    
    // Approximate ellipse with segments
    const int segments = 32;
    float rx = w * 0.5f;
    float ry = h * 0.5f;
    float cx = x + rx;
    float cy = y + ry;
    
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
    p5_rect(x, y, size, size);
}

void p5_quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    p5__apply_transform();
    
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
    p5__apply_transform();
    
    const int segments = 32;
    float rx = w * 0.5f;
    float ry = h * 0.5f;
    float cx = x + rx;
    float cy = y + ry;
    
    // Convert angles to radians (assuming degrees like p5.js)
    float start_rad = start * PI / 180.0f;
    float stop_rad = stop * PI / 180.0f;
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
        
        // Draw closing lines for CHORD mode (default)
        float start_x = cx + cosf(start_rad) * rx;
        float start_y = cy + sinf(start_rad) * ry;
        float stop_x = cx + cosf(stop_rad) * rx;
        float stop_y = cy + sinf(stop_rad) * ry;
        sgp_draw_line(start_x, start_y, stop_x, stop_y);
    }
    
    p5__restore_transform();
}

#endif // P5_IMPLEMENTATION

#endif // P5_H
