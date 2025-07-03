// Test dependencies - unified implementation for all tests
// Use TEST_NEEDS_SOKOL to control whether Sokol is included

// STB image implementations (always needed for tests)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../deps/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb_image.h"

// Sokol implementations (only when needed)
#ifdef TEST_NEEDS_SOKOL
// Disable sokol_app main function to avoid conflicts
#define SOKOL_NO_ENTRY
#define SOKOL_IMPL
#include "../deps/sokol_app.h"
#include "../deps/sokol_gfx.h"
#include "../deps/sokol_glue.h"

#define SOKOL_GP_IMPL  
#include "../deps/sokol_gp.h"
#endif
