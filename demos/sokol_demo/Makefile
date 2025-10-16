# Platform detection
UNAME_S := $(shell uname -s)

# Common compiler flags
CFLAGS = -std=c99 -Wall -Wextra -O2
INCLUDES =  -I. -I.. -Ideps -I../../renderers/sokol -I../../renderers/sokol/deps

# Platform-specific configuration
ifeq ($(UNAME_S), Linux)
    PLATFORM = linux
    BACKEND = -DSOKOL_GLCORE
    LIBS = -lGL -lm -lpthread -ldl
    CFLAGS += -D_GNU_SOURCE
    CC = gcc
endif

ifeq ($(UNAME_S), Darwin)
    PLATFORM = macos
    BACKEND = -DSOKOL_METAL
    LIBS = -framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit
    CFLAGS += -ObjC
    CC = clang
endif

ifeq ($(OS), Windows_NT)
    PLATFORM = windows
    BACKEND = -DSOKOL_D3D11
    LIBS = -lgdi32 -lole32 -ld3d11 -ldxgi
    CFLAGS += -D_WIN32_WINNT=0x0601
    EXE_SUFFIX = .exe
    CC = gcc
ifneq ($(BUILD), debug)
    CFLAGS += -mwindows
endif
endif

# Emscripten web build
ifeq ($(MAKECMDGOALS), web)
    CC = emcc
    PLATFORM = web
    BACKEND = -DSOKOL_GLES3
    CFLAGS = -std=c99 -O2 -s USE_WEBGL2=1 -s FULL_ES3=1
    LIBS = -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 --shell-file deps/shell.html
    EXE_SUFFIX = .html
endif


# Debug build option
ifeq ($(BUILD), debug)
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O2 -DNDEBUG
endif

CFLAGS += $(BACKEND) $(INCLUDES)
#
# DEPS = deps/sokol_app.h \
# 	   deps/sokol_gfx.h \
# 	   deps/sokol_glue.h


# Generic rule for building any .c file
demo: main.c deps.o ../../p5.h
	@echo "Building $@ for $(PLATFORM)..."
	$(CC) $(CFLAGS) -o $@$(EXE_SUFFIX) $< deps.o $(LIBS)
	@echo "Build complete: $@$(EXE_SUFFIX)"

deps.o: deps.c
	$(CC) -c $(CFLAGS) -o deps.o deps.c



# Update Depedencies
sokol:
	wget -O deps/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_app.h
	wget -O deps/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_gfx.h
	wget -O deps/sokol_glue.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_glue.h
	wget -O deps/sokol_gl.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/util/sokol_gl.h
	wget -O deps/sokol_fontstash.h https://github.com/floooh/sokol/raw/refs/heads/master/util/sokol_fontstash.h
	wget -O deps/fontstash.h https://github.com/memononen/fontstash/raw/refs/heads/master/src/fontstash.h
	wget -O deps/stb_truetype.h https://github.com/memononen/fontstash/raw/refs/heads/master/src/stb_truetype.h

open-simplex-noise:
	wget -O deps/open-simplex-noise.c https://raw.githubusercontent.com/smcameron/open-simplex-noise-in-c/refs/heads/master/open-simplex-noise.c
	wget -O deps/open-simplex-noise.h https://raw.githubusercontent.com/smcameron/open-simplex-noise-in-c/refs/heads/master/open-simplex-noise.h

# TODO: delete - just a reference
clay:
	wget -O deps/clay.h https://raw.githubusercontent.com/nicbarker/clay/refs/heads/main/clay.h


# Compile Flags
compile_flags.txt: FORCE
	@echo "Generating compile_flags.txt for IDE support"
	@echo $(CFLAGS) | tr ' ' '\n' > $@

FORCE:

