#ifndef SOKOL_P5_H
#define SOKOL_P5_H
/*
    sokol_p5.h -- drop-in p5 renderer for sokol_gfx.h

    Do this:
        #define SOKOL_P5_IMPL

    before you include this file in *one* C file to create the
    implementation.

    Optionally provide the following configuration define both before including the
    the declaration and implementation:

    SOKOL_P5_NO_SOKOL_APP    - don't depend on sokol_app.h (see below for details)

    Include the following headers before sokol_p5.h (both before including
    the declaration and implementation):

        sokol_gl.h
        sokol_fontstash.h
        sokol_app.h         (except SOKOL_P5_NO_SOKOL_APP)
        p5.h

    FEATURE OVERVIEW:
    =================
    sokol_p5.h implements the rendering and event-handling code for p5
    (https://github.com/nicbarker/p5) on top of sokol_gl.h and (optionally)
    sokol_app.h.

    Since sokol_fontstash.h already depends on sokol_gl.h, the rendering is
    implemented using sokol_gl calls. (TODO: make fontstash optional?)

    The sokol_app.h dependency is optional and used for input event handling.
    If you only use sokol_gfx.h but not sokol_app.h in your application,
    define SOKOL_P5_NO_SOKOL_APP before including the implementation
    of sokol_p5.h, this will remove any dependency to sokol_app.h, but
    you must call sp5_set_layout_dimensions and handle input yourself.

    sokol_p5.h is not thread-safe, all calls must be made from the
    same thread where sokol_gfx.h is running.

    HOWTO:
    ======

    --- To initialize sokol-p5, call sp5_setup(). This can be done
        before or after p5_Initialize.

    --- Create an array of sp5_font_t and fill it by calling one of:

            sp5_font_t sp5_add_font(const char *filename);
            sp5_font_t sp5_add_font_mem(unsigned char *data, int dataLen);

        The fontId value in p5 corresponds to indices in this array. After calling
        p5_Initialize but before calling any layout code, do this:

            p5_SetMeasureTextFunction(sp5_measure_text, &fonts);

        where `fonts` is the abovementioned array.

    --- At the start of a frame, call sp5_new_frame() if you're using sokol_app.h.
        If you're not using sokol_app.h, call:
 
            void sp5_set_layout_dimensions(p5_Dimensions size, float dpi_scale);

        at the start of the frame (or just when the window is resized.)

        Either way, do some layout, then at the end of the frame call sp5_render:

            sg_begin_pass(...)
            // other rendering...
            sp5_render(renderCommands, &fonts);
            // other rendering...
            sgl_draw();
            sg_end_pass();
            sg_commit();

        One caveat: sp5_render assumes the default gl view matrix, and handles scaling
        automatically. If you've adjusted the view matrix, remember to first call:

            sgl_matrix_mode_modelview();
            sgl_load_identity();

        before calling sp5_render.

    --- if you're using sokol_app.h, from inside the sokol_app.h event callback,
        call:

            void sp5_handle_event(const sapp_event* ev);

        Unfortunately p5 does not currently provide feedback on whether a mouse
        click was handled or not.

    --- if you want to use images with p5, you should pass a pointer to a
        sp5_image to the P5 macro, like this:
                P5({
                   ...
                   .image = { .imageData = &(sp5_image){ .view = view, .sampler = 0 } },
                })
        Using 0 as a sampler uses the sokol default sampler with linear interpolation.
        The image should be created using sg_make_image from sokol_gfx.

    --- finally, on application shutdown, call

            sp5_shutdown()
 */

#if 0
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "p5.h"
#endif

#if !defined(SOKOL_P5_NO_SOKOL_APP) && !defined(SOKOL_APP_INCLUDED)
#error "Please include sokol_app.h before sokol_p5.h (or define SOKOL_P5_NO_SOKOL_APP)"
#endif

typedef int sp5_font_t;

typedef struct sp5_image {
    sg_view view;
    sg_sampler sampler;
    struct {
        float u0, v0, u1, v1;
    } uv;
} sp5_image;


void sp5_setup();
void sp5_shutdown();

sp5_font_t sp5_add_font(const char *filename);
sp5_font_t sp5_add_font_mem(unsigned char *data, int dataLen);
p5_Dimensions sp5_measure_text(p5_StringSlice text, p5_TextElementConfig *config, void *userData);

#ifndef SOKOL_P5_NO_SOKOL_APP
void sp5_new_frame();
void sp5_handle_event(const sapp_event *ev);
#endif  /* SOKOL_P5_NO_SOKOL_APP */

/* Use this if you don't call sp5_new_frame. `size` is the "virtual" size which
 * your layout is relative to (ie. the actual framebuffer size divided by dpi_scale.)
 * Set dpi_scale to 1 if you're not using high-dpi support. */
void sp5_set_layout_dimensions(p5_Dimensions size, float dpi_scale);

void sp5_render(p5_RenderCommandArray renderCommands, sp5_font_t *fonts);

#endif /* SOKOL_P5_H */

#ifdef SOKOL_P5_IMPL
#define SOKOL_P5_IMPL_INCLUDED (1)
#ifndef SOKOL_GL_INCLUDED
#error "Please include sokol_gl.h before sokol_p5.h"
#endif
#ifndef SOKOL_FONTSTASH_INCLUDED
#error "Please include sokol_fontstash.h before sokol_p5.h"
#endif
#ifndef P5_H
#error "Please include p5.h before sokol_p5.h"
#endif

typedef struct {
    sgl_pipeline pip;
#ifndef SOKOL_P5_NO_SOKOL_APP
    p5_Vector2 mouse_pos, scroll;
    bool mouse_down;
#endif
    p5_Dimensions size;
    float dpi_scale;
    FONScontext *fonts;
} _sp5_state_t;
static _sp5_state_t _sp5;

void sp5_setup() {
    _sp5.pip = sgl_make_pipeline(&(sg_pipeline_desc){
        .colors[0] = {
            .blend = {
                .enabled = true,
                .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            },
        }
    });
#ifndef SOKOL_P5_NO_SOKOL_APP
    _sp5.mouse_pos = (p5_Vector2){0, 0};
    _sp5.scroll = (p5_Vector2){0, 0};
    _sp5.mouse_down = false;
#endif
    _sp5.size = (p5_Dimensions){1, 1};
    _sp5.dpi_scale = 1;
    _sp5.fonts = sfons_create(&(sfons_desc_t){ 0 });
    //TODO p5 error handler?
}

void sp5_shutdown() {
    sgl_destroy_pipeline(_sp5.pip);
    sfons_destroy(_sp5.fonts);
}

#ifndef SOKOL_P5_NO_SOKOL_APP
void sp5_handle_event(const sapp_event* ev) {
    switch(ev->type){
    case SAPP_EVENTTYPE_MOUSE_MOVE:
        _sp5.mouse_pos.x = ev->mouse_x / _sp5.dpi_scale;
        _sp5.mouse_pos.y = ev->mouse_y / _sp5.dpi_scale;
        break;
    case SAPP_EVENTTYPE_MOUSE_DOWN:
        _sp5.mouse_down = true;
        break;
    case SAPP_EVENTTYPE_MOUSE_UP:
        _sp5.mouse_down = false;
        break;
    case SAPP_EVENTTYPE_MOUSE_SCROLL:
        _sp5.scroll.x += ev->scroll_x;
        _sp5.scroll.y += ev->scroll_y;
        break;
    default: break;
    }
}

void sp5_new_frame() {
    // sp5_set_layout_dimensions((p5_Dimensions){ (float)sapp_width(), (float)sapp_height() },
    //                             sapp_dpi_scale());
    // p5_SetPointerState(_sp5.mouse_pos, _sp5.mouse_down);
    // p5_UpdateScrollContainers(true, _sp5.scroll, sapp_frame_duration());
    // _sp5.scroll = (p5_Vector2){0, 0};
}
#endif  /* SOKOL_P5_NO_SOKOL_APP */

// void sp5_set_layout_dimensions(p5_Dimensions size, float dpi_scale) {
//     size.width /= dpi_scale;
//     size.height /= dpi_scale;
//     _sp5.size = size;
//     if(_sp5.dpi_scale != dpi_scale){
//         _sp5.dpi_scale = dpi_scale;
//         p5_ResetMeasureTextCache();
//     }
//     p5_SetLayoutDimensions(size);
// }

sp5_font_t sp5_add_font(const char *filename) {
    //TODO log something if we get FONS_INVALID
    return fonsAddFont(_sp5.fonts, "", filename);
}

sp5_font_t sp5_add_font_mem(unsigned char *data, int dataLen) {
    //TODO log something if we get FONS_INVALID
    return fonsAddFontMem(_sp5.fonts, "", data, dataLen, false);
}

p5_Dimensions sp5_measure_text(p5_StringSlice text, p5_TextElementConfig *config, void *userData) {
    sp5_font_t *fonts = (sp5_font_t *)userData;
    if(!fonts) return (p5_Dimensions){ 0 };
    fonsSetFont(_sp5.fonts, fonts[config->fontId]);
    fonsSetSize(_sp5.fonts, config->fontSize * _sp5.dpi_scale);
    fonsSetSpacing(_sp5.fonts, config->letterSpacing * _sp5.dpi_scale);
    fonsSetAlign(_sp5.fonts, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
    float ascent, descent, lineh;
    fonsVertMetrics(_sp5.fonts, &ascent, &descent, &lineh);
    return (p5_Dimensions) {
        .width = fonsTextBounds(_sp5.fonts, 0, 0, text.items, text.items + text.count, NULL) / _sp5.dpi_scale,
        .height = (ascent - descent) / _sp5.dpi_scale
    };
}

static void _draw_rect(float x, float y, float w, float h){
    sgl_v2f(x, y);
    sgl_v2f(x, y);
    sgl_v2f(x+w, y);
    sgl_v2f(x, y+h);
    sgl_v2f(x+w, y+h);
    sgl_v2f(x+w, y+h);
}

static void _draw_rect_textured(float x, float y, float w, float h, float u0, float v0, float u1, float v1){
    sgl_v2f_t2f(x, y, u0, v0);
    sgl_v2f_t2f(x, y, u0, v0);
    sgl_v2f_t2f(x+w, y, u1, v0);
    sgl_v2f_t2f(x, y+h, u0, v1);
    sgl_v2f_t2f(x+w, y+h, u1, v1);
    sgl_v2f_t2f(x+w, y+h, u1, v1);
}

static float _SIN[16] = {
    0.000000f, 0.104528f, 0.207912f, 0.309017f,
    0.406737f, 0.500000f, 0.587785f, 0.669131f,
    0.743145f, 0.809017f, 0.866025f, 0.913545f,
    0.951057f, 0.978148f, 0.994522f, 1.000000f,
};

/* rx,ry = radius */
static void _draw_corner(float x, float y, float rx, float ry){
    x -= rx;
    y -= ry;
    sgl_v2f(x, y);
    for(int i = 0; i < 16; ++i){
        sgl_v2f(x, y);
        sgl_v2f(x+(rx*_SIN[15-i]), y+(ry*_SIN[i]));
    }
    sgl_v2f(x+(rx*_SIN[0]), y+(ry*_SIN[15]));
}

static void _draw_corner_textured(float x, float y, float rx, float ry, float bx, float by, float bw, float bh, float u0, float v0, float u1, float v1) {
    x -= rx;
    y -= ry;
#define MAP_U(x) (u0+(((x)-bx)/bw)*(u1-u0))
#define MAP_V(y) (v0+(((y)-by)/bh)*(v1-v0))
    sgl_v2f_t2f(x, y, MAP_U(x), MAP_V(y));
    for(int i = 0; i < 16; ++i){
        sgl_v2f_t2f(x, y, MAP_U(x), MAP_V(y));
        float px = x+(rx*_SIN[15-i]);
        float py = y+(ry*_SIN[i]);
        sgl_v2f_t2f(px, py, MAP_U(px), MAP_V(py));
    }
    sgl_v2f_t2f(x+(rx*_SIN[0]), y+(ry*_SIN[15]), MAP_U(x+(rx*_SIN[0])), MAP_V(y+(ry*_SIN[15])));
#undef MAP_U
#undef MAP_V
}

/* rx,ry = radius   ix,iy = inner radius */
static void _draw_corner_border(float x, float y, float rx, float ry, float ix, float iy){
    x -= rx;
    y -= ry;
    sgl_v2f(x+(ix*_SIN[15]), y+(iy*_SIN[0]));
    for(int i = 0; i < 16; ++i){
        sgl_v2f(x+(ix*_SIN[15-i]), y+(iy*_SIN[i]));
        sgl_v2f(x+(rx*_SIN[15-i]), y+(ry*_SIN[i]));
    }
    sgl_v2f(x+(rx*_SIN[0]), y+(ry*_SIN[15]));
}

void sp5_render(p5_RenderCommandArray renderCommands, sp5_font_t *fonts) {
    sgl_matrix_mode_modelview();
    sgl_translate(-1.0f, 1.0f, 0.0f);
    sgl_scale(2.0f/_sp5.size.width, -2.0f/_sp5.size.height, 1.0f);
    sgl_disable_texture();
    sgl_push_pipeline();
    sgl_load_pipeline(_sp5.pip);
    for (int32_t i = 0; i < renderCommands.count; i++) {
        // p5_RenderCommand *renderCommand = p5_RenderCommandArray_Get(&renderCommands, i);
        p5_RenderCommand *renderCommand = &renderCommands.items[i];
        p5_BoundingBox bbox = renderCommand->boundingBox;
        switch (renderCommand->commandType) {
            case P5_RENDER_COMMAND_TYPE_RECTANGLE: {
                p5_RectangleRenderData *config = &renderCommand->renderData.rectangle;
                sgl_c4f(config->backgroundColor.r / 255.0f,
                        config->backgroundColor.g / 255.0f,
                        config->backgroundColor.b / 255.0f,
                        config->backgroundColor.a / 255.0f);
                p5_CornerRadius r = config->cornerRadius;
                sgl_begin_triangle_strip();
                if(r.topLeft > 0 || r.topRight > 0){
                    _draw_corner(bbox.x, bbox.y, -r.topLeft, -r.topLeft);
                    _draw_corner(bbox.x+bbox.width, bbox.y, r.topRight, -r.topRight);
                    _draw_rect(bbox.x+r.topLeft, bbox.y,
                               bbox.width-r.topLeft-r.topRight, P5_MAX(r.topLeft, r.topRight));
                }
                if(r.bottomLeft > 0 || r.bottomRight > 0){
                    _draw_corner(bbox.x, bbox.y+bbox.height, -r.bottomLeft, r.bottomLeft);
                    _draw_corner(bbox.x+bbox.width, bbox.y+bbox.height, r.bottomRight, r.bottomRight);
                    _draw_rect(bbox.x+r.bottomLeft,
                               bbox.y+bbox.height-P5_MAX(r.bottomLeft, r.bottomRight),
                               bbox.width-r.bottomLeft-r.bottomRight, P5_MAX(r.bottomLeft, r.bottomRight));
                }
                if(r.topLeft < r.bottomLeft){
                    if(r.topLeft < r.topRight){
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft);
                        _draw_rect(bbox.x+r.topLeft, bbox.y+r.topRight,
                                   r.bottomLeft-r.topLeft, bbox.height-r.topRight-r.bottomLeft);
                    } else {
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft);
                    }
                } else {
                    if(r.bottomLeft < r.bottomRight){
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft);
                        _draw_rect(bbox.x+r.bottomLeft, bbox.y+r.topLeft,
                                   r.topLeft-r.bottomLeft, bbox.height-r.topLeft-r.bottomRight);
                    } else {
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft);
                    }
                }
                if(r.topRight < r.bottomRight){
                    if(r.topRight < r.topLeft){
                        _draw_rect(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topLeft,
                                   r.bottomRight-r.topRight, bbox.height-r.topLeft-r.bottomRight);
                        _draw_rect(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight);
                    } else {
                        _draw_rect(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight);
                    }
                } else {
                    if(r.bottomRight < r.bottomLeft){
                        _draw_rect(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight-r.bottomRight, bbox.height-r.topRight-r.bottomLeft);
                        _draw_rect(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight);
                    } else {
                        _draw_rect(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight);
                    }
                }
                _draw_rect(bbox.x+P5_MAX(r.topLeft, r.bottomLeft),
                           bbox.y+P5_MAX(r.topLeft, r.topRight),
                           bbox.width-P5_MAX(r.topLeft, r.bottomLeft)-P5_MAX(r.topRight, r.bottomRight),
                           bbox.height-P5_MAX(r.topLeft, r.topRight)-P5_MAX(r.bottomLeft, r.bottomRight));
                sgl_end();
                break;
            }
            case P5_RENDER_COMMAND_TYPE_TEXT: {
                if(!fonts) break;
                p5_TextRenderData *config = &renderCommand->renderData.text;
                p5_StringSlice text = config->stringContents;
                fonsSetFont(_sp5.fonts, fonts[config->fontId]);
                uint32_t color = sfons_rgba(
                        config->textColor.r,
                        config->textColor.g,
                        config->textColor.b,
                        config->textColor.a);
                fonsSetColor(_sp5.fonts, color);
                fonsSetSpacing(_sp5.fonts, config->letterSpacing * _sp5.dpi_scale);
                fonsSetAlign(_sp5.fonts, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
                fonsSetSize(_sp5.fonts, config->fontSize * _sp5.dpi_scale);
                sgl_matrix_mode_modelview();
                sgl_push_matrix();
                sgl_scale(1.0f/_sp5.dpi_scale, 1.0f/_sp5.dpi_scale, 1.0f);
                fonsDrawText(_sp5.fonts, bbox.x*_sp5.dpi_scale, bbox.y*_sp5.dpi_scale,
                             text.items, text.items + text.count);
                sgl_pop_matrix();
                break;
            }
            case P5_RENDER_COMMAND_TYPE_SCISSOR_START: {
                sgl_scissor_rectf(bbox.x*_sp5.dpi_scale, bbox.y*_sp5.dpi_scale,
                                  bbox.width*_sp5.dpi_scale, bbox.height*_sp5.dpi_scale,
                                  true);
                break;
            }
            case P5_RENDER_COMMAND_TYPE_SCISSOR_END: {
                sgl_scissor_rectf(0, 0,
                                  _sp5.size.width*_sp5.dpi_scale, _sp5.size.height*_sp5.dpi_scale,
                                  true);
                break;
            }
            case P5_RENDER_COMMAND_TYPE_IMAGE: {
                p5_ImageRenderData *config = &renderCommand->renderData.image;
                sp5_image* img = (sp5_image*)config->imageData;
                // by default, u1 and v1 are 1. if we pass 0.
                // note, we are modifying a copy !
                float u0 = img->uv.u0;
                float v0 = img->uv.v0;
                float u1 = img->uv.u1;
                float v1 = img->uv.v1;
                if (u1 == 0.f) {
                    u1 = 1.f;
                }
                if (v1 == 0.f) {
                    v1 = 1.f;
                }

                int untinted = config->backgroundColor.r == 0 && config->backgroundColor.g == 0 && config->backgroundColor.b == 0 && config->backgroundColor.a == 0;
                float cr = untinted ? 1.f : (config->backgroundColor.r / 255.0f);
                float gr = untinted ? 1.f : (config->backgroundColor.g / 255.0f);
                float br = untinted ? 1.f : (config->backgroundColor.b / 255.0f);
                float ar = untinted ? 1.f : (config->backgroundColor.a / 255.0f);

                sgl_c4f(cr, gr, br, ar);

                p5_CornerRadius r = config->cornerRadius;

                sgl_enable_texture();
                sgl_texture(img->view, img->sampler);

                sgl_begin_triangle_strip();
                if(r.topLeft > 0 || r.topRight > 0){
                    _draw_corner_textured(bbox.x, bbox.y, -r.topLeft, -r.topLeft, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_corner_textured(bbox.x+bbox.width, bbox.y, r.topRight, -r.topRight, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_rect_textured(bbox.x+r.topLeft, bbox.y,
                               bbox.width-r.topLeft-r.topRight, P5_MAX(r.topLeft, r.topRight),
                               u0 + (r.topLeft/bbox.width)*(u1-u0), v0, u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (P5_MAX(r.topLeft, r.topRight)/bbox.height)*(v1-v0));
                }
                if(r.bottomLeft > 0 || r.bottomRight > 0){
                    _draw_corner_textured(bbox.x, bbox.y+bbox.height, -r.bottomLeft, r.bottomLeft, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_corner_textured(bbox.x+bbox.width, bbox.y+bbox.height, r.bottomRight, r.bottomRight, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_rect_textured(bbox.x+r.bottomLeft,
                               bbox.y+bbox.height-P5_MAX(r.bottomLeft, r.bottomRight),
                               bbox.width-r.bottomLeft-r.bottomRight, P5_MAX(r.bottomLeft, r.bottomRight),
                               u0 + (r.bottomLeft/bbox.width)*(u1-u0), v1 - (P5_MAX(r.bottomLeft, r.bottomRight)/bbox.height)*(v1-v0), u1 - (r.bottomRight/bbox.width)*(u1-u0), v1);
                }
                if(r.topLeft < r.bottomLeft){
                    if(r.topLeft < r.topRight){
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft,
                                u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+r.topLeft, bbox.y+r.topRight,
                               r.bottomLeft-r.topLeft, bbox.height-r.topRight-r.bottomLeft,
                               u0 + (r.topLeft/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft,
                                u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.bottomLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                    }
                } else {
                    if(r.bottomLeft < r.bottomRight){
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft,
                               u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.bottomLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+r.bottomLeft, bbox.y+r.topLeft,
                                   r.topLeft-r.bottomLeft, bbox.height-r.topLeft-r.bottomRight,
                                   u0 + (r.bottomLeft/bbox.width)*(u1-u0), v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft,
                                u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                    }
                }
                if(r.topRight < r.bottomRight){
                    if(r.topRight < r.topLeft){
                        _draw_rect_textured(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topLeft,
                                   r.bottomRight-r.topRight, bbox.height-r.topLeft-r.bottomRight,
                                   u1 - (r.bottomRight/bbox.width)*(u1-u0), v0 + (r.topLeft/bbox.height)*(v1-v0), u1 - (r.topRight/bbox.width)*(u1-u0), v1 - (r.bottomRight/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.bottomRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    }
                } else {
                    if(r.bottomRight < r.bottomLeft){
                        _draw_rect_textured(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight-r.bottomRight, bbox.height-r.topRight-r.bottomLeft,
                                   u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1 - (r.bottomRight/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.bottomRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    }
                }
                _draw_rect_textured(bbox.x+P5_MAX(r.topLeft, r.bottomLeft),
                           bbox.y+P5_MAX(r.topLeft, r.topRight),
                           bbox.width-P5_MAX(r.topLeft, r.bottomLeft)-P5_MAX(r.topRight, r.bottomRight),
                           bbox.height-P5_MAX(r.topLeft, r.topRight)-P5_MAX(r.bottomLeft, r.bottomRight),
                           u0+P5_MAX(r.topLeft,r.bottomLeft)/bbox.width*(u1-u0), v0+P5_MAX(r.topLeft,r.topRight)/bbox.height*(v1-v0),
                           u1-P5_MAX(r.topRight,r.bottomRight)/bbox.width*(u1-u0), v1-P5_MAX(r.bottomLeft,r.bottomRight)/bbox.height*(v1-v0));
                sgl_end();
                sgl_disable_texture();
                break;
            }
            case P5_RENDER_COMMAND_TYPE_BORDER: {
                p5_BorderRenderData *config = &renderCommand->renderData.border;
                sgl_c4f(config->color.r / 255.0f,
                        config->color.g / 255.0f,
                        config->color.b / 255.0f,
                        config->color.a / 255.0f);
                p5_BorderWidth w = config->width;
                p5_CornerRadius r = config->cornerRadius;
                sgl_begin_triangle_strip();
                if(w.left > 0){
                    _draw_rect(bbox.x, bbox.y + r.topLeft,
                               w.left, bbox.height - r.topLeft - r.bottomLeft);
                }
                if(w.right > 0){
                    _draw_rect(bbox.x + bbox.width - w.right, bbox.y + r.topRight,
                               w.right, bbox.height - r.topRight - r.bottomRight);
                }
                if(w.top > 0){
                    _draw_rect(bbox.x + r.topLeft, bbox.y,
                               bbox.width - r.topLeft - r.topRight, w.top);
                }
                if(w.bottom > 0){
                    _draw_rect(bbox.x + r.bottomLeft, bbox.y + bbox.height - w.bottom,
                               bbox.width - r.bottomLeft - r.bottomRight, w.bottom);
                }
                if(r.topLeft > 0 && (w.top > 0 || w.left > 0)){
                    _draw_corner_border(bbox.x, bbox.y,
                                        -r.topLeft, -r.topLeft,
                                        -r.topLeft+w.left, -r.topLeft+w.top);
                }
                if(r.topRight > 0 && (w.top > 0 || w.right > 0)){
                    _draw_corner_border(bbox.x+bbox.width, bbox.y,
                                        r.topRight, -r.topRight,
                                        r.topRight-w.right, -r.topRight+w.top);
                }
                if(r.bottomLeft > 0 && (w.bottom > 0 || w.left > 0)){
                    _draw_corner_border(bbox.x, bbox.y+bbox.height,
                                        -r.bottomLeft, r.bottomLeft,
                                        -r.bottomLeft+w.left, r.bottomLeft-w.bottom);
                }
                if(r.bottomRight > 0 && (w.bottom > 0 || w.right > 0)){
                    _draw_corner_border(bbox.x+bbox.width, bbox.y+bbox.height,
                                        r.bottomRight, r.bottomRight,
                                        r.bottomRight-w.right, r.bottomRight-w.bottom);
                }
                sgl_end();
                break;
            }
            default:
                break;
        }
    }
    sgl_pop_pipeline();
    sfons_flush(_sp5.fonts);
}
#endif /* SOKOL_P5_IMPL */
