-- build.lua - Configuration for the C build tool
-- This file defines project settings, dependencies, and build targets

project = {
    name = "p5_h",
    version = "1.0.0",
    description = "P5.js-style graphics library for C"
}

-- Compiler settings
compiler = {
    cc = "clang",
    cflags = {
        "-Ideps",
        "-ObjC",
        "-DSOKOL_METAL"
    },
    libs = {
        "-framework Cocoa",
        "-framework QuartzCore", 
        "-framework Metal",
        "-framework MetalKit"
    }
}

-- Dependencies from GitHub (single-header libraries)
dependencies = {
    {
        name = "sokol",
        repo = "floooh/sokol",
        files = {
            "sokol_app.h",
            "sokol_gfx.h", 
            "sokol_glue.h"
        },
        dest = "deps/"
    },
    {
        name = "sokol_gp",
        repo = "edubart/sokol_gp", 
        files = {
            "sokol_gp.h"
        },
        dest = "deps/"
    },
    {
        name = "stb",
        repo = "nothings/stb",
        files = {
            "stb_image.h",
            "stb_image_write.h"
        },
        dest = "deps/"
    }
}

-- Build targets
targets = {
    {
        name = "demo",
        type = "executable",
        sources = {
            "src/demo.c",
            "deps/deps.c"
        },
        headers = {
            "p5.h"
        }
    },
    {
        name = "p5style", 
        type = "executable",
        sources = {
            "src/p5_style_demo.c",
            "deps/deps.c"
        },
        headers = {
            "p5.h"
        }
    },
    {
        name = "canvas",
        type = "executable", 
        sources = {
            "src/canvas_demo.c",
            "deps/deps.c"
        },
        headers = {
            "p5.h"
        }
    },
    {
        name = "shapes",
        type = "executable",
        sources = {
            "src/shapes_demo.c", 
            "deps/deps.c"
        },
        headers = {
            "p5.h"
        }
    }
}

-- Custom commands
commands = {
    install = {
        description = "Install dependencies",
        script = function()
            -- This will be handled by the C tool
            return "install_deps"
        end
    },
    build = {
        description = "Build all targets",
        script = function()
            return "build_all"
        end
    },
    clean = {
        description = "Clean build artifacts",
        script = function()
            return "clean_all"
        end
    }
}