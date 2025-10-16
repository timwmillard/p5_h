#include <stdio.h>
#include <stdlib.h>

// deps
#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"
#include "deps/sokol_glue.h"
#include "deps/sokol_gl.h"
#include "deps/fontstash.h"

#include "../p5.h"
#include "../../renderers/sokol/p5_sokol.h"

enum {
    FONT_DROID_SANS = 0
};

static char *font_files[] = {
    [FONT_DROID_SANS] = "fonts/DroidSans.ttf",
};

static struct {
    sp5_font_t fonts[sizeof(font_files)];
} state;

void frame()
{   
    sp5_new_frame();

    sg_begin_pass(&(sg_pass){
        .swapchain = sglue_swapchain(),
    });

    // call p5 render_commands
    p5_RenderCommandArray clay_commands = p5_render_commands();

    sgl_matrix_mode_modelview();
    sgl_load_identity();

    sp5_render(clay_commands, state.fonts);
    sgl_draw();

    sg_end_pass();
    sg_commit();
}

void event(const sapp_event* ev)
{
    if (ev->type == SAPP_EVENTTYPE_KEY_DOWN && ev->key_code == SAPP_KEYCODE_ESCAPE) {
        sapp_quit();
    } else {
        sp5_handle_event(ev);
    }
}

void init()
{
    // setup sokol
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
    });

    // setup sokol_gl
    sgl_setup(&(sgl_desc_t){0});

    // setup sokol clay
    sp5_setup();
    for (size_t i=0; i<sizeof(font_files)/sizeof(char*); i++) {
         sp5_font_t font = sp5_add_font(font_files[i]);
         if (font == FONS_INVALID)
             printf("[WARN] invalid font %s.\n", font_files[i]);
         state.fonts[i] = font;
    }

    // setup clay
    // uint64_t clay_mem_size = Clay_MinMemorySize();
    // Clay_Arena clay_mem = Clay_CreateArenaWithCapacityAndMemory(clay_mem_size, malloc(clay_mem_size));
    // Clay_Initialize(clay_mem, (Clay_Dimensions){ sapp_width(), sapp_height() }, (Clay_ErrorHandler){0});
    //
    // Clay_SetMeasureTextFunction(sp5_measure_text, &state.fonts);
}

void cleanup()
{
    sp5_shutdown();
    sgl_shutdown();
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (sapp_desc) {
        .width = 640,
        .height = 480,
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .sample_count = 4,
    };
}

