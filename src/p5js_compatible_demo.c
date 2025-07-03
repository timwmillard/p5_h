#include <stdio.h>
#include <stdbool.h>

// sokol dependencies
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

// p5 library with implementation
#define P5_IMPLEMENTATION
#include "../p5.h"

// Override the p5__sokol_frame to be p5.js compatible
#ifdef P5_IMPLEMENTATION
#undef P5_IMPLEMENTATION

// Include p5.h again but override the frame function
static void p5__sokol_init_custom(void);
static void p5__sokol_frame_custom(void);
static bool setup_called = false;

static void p5__sokol_init_custom(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });
    
    sgp_setup(&(sgp_desc){0});
    p5_init();
    
    // Start the graphics context so setup() can draw
    sgp_begin(sapp_width(), sapp_height());
    
    if (p5_state.canvas.created) {
        sgp_viewport(p5_state.canvas.x, p5_state.canvas.y, 
                     p5_state.canvas.width, p5_state.canvas.height);
        sgp_project(0.0f, (float)p5_state.canvas.width, 
                    0.0f, (float)p5_state.canvas.height);
    } else {
        sgp_viewport(0, 0, sapp_width(), sapp_height());
        sgp_project(0.0f, (float)sapp_width(), 0.0f, (float)sapp_height());
    }
    
    setup();  // Call user setup - can now draw!
    setup_called = true;
    
    // Commit the setup drawing to the screen
    sg_begin_pass(&(sg_pass){
        .swapchain = sglue_swapchain()
    });
    sgp_flush();
    sgp_end();
    sg_end_pass();
    sg_commit();
}

static void p5__sokol_frame_custom(void) {
    if (!setup_called) return;
    
    sgp_begin(sapp_width(), sapp_height());
    
    if (p5_state.canvas.created) {
        sgp_viewport(p5_state.canvas.x, p5_state.canvas.y, 
                     p5_state.canvas.width, p5_state.canvas.height);
        sgp_project(0.0f, (float)p5_state.canvas.width, 
                    0.0f, (float)p5_state.canvas.height);
    } else {
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

// Custom P5_MAIN that uses our modified callbacks
#define P5_MAIN_CUSTOM(window_w, window_h, title_str) \
    sapp_desc sokol_main(int argc, char* argv[]) { \
        sapp_desc _p5_desc = {0}; \
        _p5_desc.width = window_w; \
        _p5_desc.height = window_h; \
        _p5_desc.sample_count = 4; \
        _p5_desc.init_cb = p5__sokol_init_custom; \
        _p5_desc.frame_cb = p5__sokol_frame_custom; \
        _p5_desc.cleanup_cb = p5__sokol_cleanup; \
        _p5_desc.event_cb = p5__sokol_event; \
        _p5_desc.window_title = title_str; \
        return _p5_desc; \
    }

#endif

void setup() {
    createCanvas(400, 300);
    printf("Drawing in setup() - p5.js compatible!\n");
    
    // This mimics the p5.js.org shape primitives example
    background(240, 240, 240);
    
    // Red rectangle
    fill(255, 0, 0);
    stroke(0, 0, 0);
    rect(50, 50, 100, 75);
    
    // Blue circle
    fill(0, 0, 255);
    circle(250, 100, 60);
    
    // Green triangle  
    fill(0, 200, 0);
    triangle(100, 200, 150, 150, 200, 200);
    
    // Yellow line
    stroke(255, 255, 0);
    strokeWeight(3);
    line(250, 150, 350, 250);
    
    printf("Setup drawing complete - shapes should persist!\n");
}

void draw() {
    // This is called every frame but we don't call background()
    // So the shapes from setup() should remain visible
    // This mimics p5.js behavior exactly
}

P5_MAIN_CUSTOM(400, 300, "P5.js Compatible Demo");