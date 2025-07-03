#include <stdio.h>

// sokol dependencies
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_gp.h"

// our p5 library
#define P5_IMPLEMENTATION
#define P5_NO_SHORT_NAMES  // Disable aliases to avoid conflicts with sokol
#include "../p5.h"

static struct {
    sg_pass_action pass_action;
    float time;
} state;

void frame() {
    // update time for animation
    state.time += 1.0f/60.0f;
    
    // begin sokol gp frame
    sgp_begin(sapp_width(), sapp_height());
    sgp_viewport(0, 0, sapp_width(), sapp_height());
    sgp_project(0.0f, (float)sapp_width(), 0.0f, (float)sapp_height());
    
    // p5.js style drawing
    p5_background(0.1f, 0.1f, 0.2f);
    
    // animated rotating rectangle
    p5_push();
    p5_translate(320.0f, 240.0f);
    p5_rotate(state.time);
    p5_fill(1.0f, 0.5f, 0.2f);
    p5_stroke(1.0f, 1.0f, 1.0f);
    p5_rect(-50.0f, -30.0f, 100.0f, 60.0f);
    p5_pop();
    
    // colorful circles
    for (int i = 0; i < 5; i++) {
        float x = 100.0f + i * 120.0f;
        float y = 100.0f;
        float r = (float)i / 4.0f;
        float g = 0.5f + 0.5f * sinf(state.time + i);
        float b = 0.8f;
        
        p5_fill(r, g, b);
        p5_no_stroke();
        p5_circle(x, y, 50.0f);
    }
    
    // animated triangle
    p5_push();
    p5_translate(320.0f, 350.0f);
    p5_scale(1.5f + 0.5f * sinf(state.time * 2.0f));
    p5_fill(0.2f, 1.0f, 0.5f);
    p5_stroke(0.0f, 0.0f, 0.0f);
    p5_triangle(-40.0f, 20.0f, 40.0f, 20.0f, 0.0f, -30.0f);
    p5_pop();
    
    // lines pattern
    p5_stroke(0.7f, 0.3f, 0.9f);
    for (int i = 0; i < 10; i++) {
        float offset = sinf(state.time + i * 0.5f) * 20.0f;
        p5_line(500.0f + i * 10.0f, 150.0f + offset, 
                500.0f + i * 10.0f, 250.0f + offset);
    }
    
    // dispatch draw commands to GPU
    sg_begin_pass(&(sg_pass){
        .swapchain = sglue_swapchain()
    });
    sgp_flush();
    sgp_end();
    sg_end_pass();
    sg_commit();
}

void event(const sapp_event* ev) {
    if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (ev->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_quit();
        }
    }
}

void init() {
    // setup sokol gfx with MSAA
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });
    
    // setup sokol gp (will inherit MSAA from sokol_app)
    sgp_setup(&(sgp_desc){0});
    
    // initialize p5 library
    p5_init();
    
    state.pass_action = (sg_pass_action) {
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = {0.0f, 0.0f, 0.0f, 1.0f},
        },
    };
    
    state.time = 0.0f;
}

void cleanup() {
    sgp_shutdown();
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc) {
        .width = 640,
        .height = 480,
        .sample_count = 4,  // Enable 4x MSAA at application level
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .window_title = "p5.h Demo",
    };
}
