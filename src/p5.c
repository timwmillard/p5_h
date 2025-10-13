#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#include "p5.h"

// Private buitin types
typedef struct {
    float v[2];
} p5_vec2;

typedef struct {
    float v[3];
} p5_vec3;

typedef struct {
    float v[4];
} p5_vec4;

typedef struct {
    float v[16];
} p5_mat4;

#include "shape_glsl.h"

typedef enum {
    SHAPE_CIRCLE,
    SHAPE_RECT,
    SHAPE_TRIANGLE,
    SHAPE_QUAD,
} p5_Shape;

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

typedef struct {
    p5_Color background_color;

    p5_Draw draw;
    p5_Draw draw_stack[32];
    int draw_stack_depth;

    p5_Canvas canvas;
    bool setup_called; // setup() function has been called, only run once.
    float color_maxes[4];  // Current color maximums for R/G/B/A (or H/S/B/A or H/S/L/A)
    
    sg_pipeline pip;
    sg_bindings bind;
    sg_buffer vertex_buffer;
} p5_State;


//
// GLOBAL STATE
//

static p5_State p5_state;

//
// SOKOL WRAPPER FUNCTIONS (only compiled when app mode is enabled)
//
#ifndef P5_NO_APP

void p5_sokol_init(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });
    
    p5_init();
}

void init_shape_pipeline()
{
}

static void p5_draw_sdf_shape(float x, float y, float w, float h, 
                             p5_Shape shape, float corner_radius, float skew_x, float skew_y) 
{
    sg_apply_pipeline(p5_state.pip);
    sg_apply_bindings(&p5_state.bind);

    vs_params_t vs_params = {
        .position = {x, y},
        .size = {w * 0.5f, h * 0.5f},
        // .screen_size = {p5_state.canvas.width, p5_state.canvas.height},
        .screen_size = {p5_width(), p5_height()},
    };
    sg_apply_uniforms(UB_vs_params, &SG_RANGE(vs_params));

    fs_params_t fs_params = {
        .fill_color = *(p5_vec4*)&p5_state.draw.fill_color,
        .stroke_color = *(p5_vec4*)&p5_state.draw.stroke_color,
        .smoothness = 0.02f,
        // .stroke_width = p5_state.draw.stroke_width / (w * 0.5f), // Normalize stroke width
        .shape_type = shape,
        .corner_radius = corner_radius,
        .quad_skew = {skew_x, skew_y},
    };
    sg_apply_uniforms(UB_fs_params, &SG_RANGE(fs_params));

    sg_draw(0, 6, 1);
}
void p5_circle(float x, float y, float diameter)
{
    p5_draw_sdf_shape(x, y, diameter, diameter, SHAPE_CIRCLE, 0.0f, 0.0f, 0.0f);
}

void p5_sokol_frame(void)
{
    if (!p5_state.setup_called) {
        setup();  // Call user setup - this will draw every frame now
        p5_state.setup_called = true;
    } 
    sg_begin_pass(&(sg_pass){
        .action =(sg_pass_action) {
            .colors[0] = { 
                .load_action = SG_LOADACTION_CLEAR,
                .clear_value = *(sg_color*)&p5_state.background_color
            },
        },
        .swapchain = sglue_swapchain(),
    });

    // Call draw() for any additional per-frame drawing
    draw();

    sg_end_pass();
    sg_commit();
}

void p5_sokol_cleanup(void) {
    sg_shutdown();
}

void p5_sokol_event(const sapp_event* ev)
{
    if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (ev->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_quit();
        }
    }
}
#endif // P5_NO_APP
       
//
// PUBLIC API IMPLEMENTATION
//

// Quad vertices for SDF rendering
static float quad_vertices[] = {
    -1.0f, -1.0f,  -1.0f,  1.0f,   1.0f, -1.0f,
     1.0f, -1.0f,   1.0f,  1.0f,  -1.0f,  1.0f
};

void p5_init(void) {
    p5_state.draw.fill_color = (p5_Color){1.0f, 1.0f, 1.0f, 1.0f};
    p5_state.draw.stroke_color = (p5_Color){0.0f, 0.0f, 0.0f, 1.0f};
    p5_state.draw.has_fill = true;
    p5_state.draw.has_stroke = true;
    p5_state.draw.stroke_width = 1.0f;
    p5_state.draw.transform = (p5_Transform){0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    p5_state.draw_stack_depth = 0;
    p5_state.canvas.created = false;
    p5_state.canvas.x = 0;
    p5_state.canvas.y = 0;
    p5_state.canvas.width = sapp_width();
    p5_state.canvas.height = sapp_height();
    p5_state.setup_called = false;  // Initialize p5.js compatibility flag
    p5_state.draw.angle_mode = P5_RADIANS;  // Default to radians like p5.js
    p5_state.draw.color_mode = P5_RGB;      // Default to RGB
    p5_state.color_maxes[0] = 255.0f;  // R max
    p5_state.color_maxes[1] = 255.0f;  // G max
    p5_state.color_maxes[2] = 255.0f;  // B max
    p5_state.color_maxes[3] = 255.0f;  // A max

    p5_state.vertex_buffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(quad_vertices),
        .usage.vertex_buffer = true,
        .usage.immutable = true,
    });

    sg_shader shd = sg_make_shader(shape_shader_desc(sg_query_backend()));
    
    p5_state.pip = sg_make_pipeline(&(sg_pipeline_desc){
            .shader = shd,
            .layout = {
                .attrs = {
                    [ATTR_shape_pos].format = SG_VERTEXFORMAT_FLOAT2,
                },
            },
            .colors[0] = {
                .blend = {
                    .enabled = true,
                    .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                    .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                    .src_factor_alpha = SG_BLENDFACTOR_ONE,
                    .dst_factor_alpha = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                }
            },
    });

    p5_state.bind.vertex_buffers[0] = p5_state.vertex_buffer;
}

// Canvas functions
void p5_create_canvas(int w, int h) {
    // Center the canvas in the window
    int win_w = sapp_width();
    int win_h = sapp_height();
    int x = (win_w - w) / 2;
    int y = (win_h - h) / 2;
    p5_create_canvas_pos(w, h, x, y);
}

void p5_create_canvas_pos(int w, int h, int x, int y) {
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

void p5_background(p5_Color color)
{
    p5_state.background_color = color;
}

void p5_background_rgb(unsigned int r, unsigned int g, unsigned int b)
{
    p5_state.background_color = p5_color_rgb(r, g, b);
}

// Color functions
p5_Color p5_color_rgb(unsigned int r, unsigned int g, unsigned int b) {
    return (p5_Color){r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
}

p5_Color p5_color_rbga(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
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
static int p5_hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

static p5_Color p5_parse_hex_color(const char* hex) {
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
static p5_Color p5_parse_named_color(const char* name) {
    // Convert to lowercase for case-insensitive comparison
    char lower_name[32];
    int i = 0;
    while (name[i] && i < 31) {
        lower_name[i] = (name[i] >= 'A' && name[i] <= 'Z') ? name[i] + 32 : name[i];
        i++;
    }
    lower_name[i] = '\0';
    
    // Common named colors used in p5.js examples
    if (strcmp(lower_name, "steelblue") == 0) return p5_parse_hex_color("#4682b4");
    if (strcmp(lower_name, "red") == 0) return p5_parse_hex_color("#ff0000");
    if (strcmp(lower_name, "green") == 0) return p5_parse_hex_color("#008000");
    if (strcmp(lower_name, "blue") == 0) return p5_parse_hex_color("#0000ff");
    if (strcmp(lower_name, "white") == 0) return p5_parse_hex_color("#ffffff");
    if (strcmp(lower_name, "black") == 0) return p5_parse_hex_color("#000000");
    if (strcmp(lower_name, "gray") == 0) return p5_parse_hex_color("#808080");
    if (strcmp(lower_name, "grey") == 0) return p5_parse_hex_color("#808080");
    if (strcmp(lower_name, "yellow") == 0) return p5_parse_hex_color("#ffff00");
    if (strcmp(lower_name, "orange") == 0) return p5_parse_hex_color("#ffa500");
    if (strcmp(lower_name, "purple") == 0) return p5_parse_hex_color("#800080");
    if (strcmp(lower_name, "pink") == 0) return p5_parse_hex_color("#ffc0cb");
    
    // Default to white if color not found
    return p5_parse_hex_color("#ffffff");
}

// Public color parsing function
p5_Color p5_color(const char* color_str) {
    if (color_str[0] == '#') {
        return p5_parse_hex_color(color_str);
    } else {
        return p5_parse_named_color(color_str);
    }
}

