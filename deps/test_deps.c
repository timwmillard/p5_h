// Test dependencies - sokol implementations for offscreen testing

// Disable sokol_app main function to avoid conflicts
#define SOKOL_NO_ENTRY
#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#define SOKOL_GP_IMPL  
#include "sokol_gp.h"

// STB image implementations for PNG handling
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"