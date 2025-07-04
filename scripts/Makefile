# Cross-platform Makefile for p5.h template projects
# Usage: make <target> where <target> is the name of a .c file without extension
# Example: make canvas (builds canvas.c)

# Platform detection
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# Common compiler flags
CFLAGS = -std=c99 -Wall -Wextra -O2
INCLUDES = -Ideps

# Platform-specific configuration
ifeq ($(UNAME_S), Linux)
    PLATFORM = linux
    BACKEND = -DSOKOL_GLCORE
    LIBS = -lGL -lm -lpthread -ldl
    CFLAGS += -D_GNU_SOURCE
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

# Default compiler
CC ?= gcc

# Debug build option
ifeq ($(BUILD), debug)
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O2 -DNDEBUG
endif

CFLAGS += $(BACKEND) $(INCLUDES)

# Source files
SOKOL_HEADERS = deps/sokol_app.h \
				deps/sokol_gfx.h \
				deps/sokol_glue.h \
				deps/sokol_gp.h

P5_HEADER = deps/p5.h

DEPS = $(SOKOL_HEADERS) $(P5_HEADER)

.PHONY: all clean help web

# Default target
all: canvas

# Help target
help:
	@echo "Cross-platform p5.h template build system"
	@echo ""
	@echo "Usage:"
	@echo "  make <target>      - Build target.c (e.g., make canvas)"
	@echo "  make clean         - Clean build artifacts"
	@echo "  make help          - Show this help"
	@echo ""
	@echo "Build options:"
	@echo "  BUILD=debug        - Build with debug symbols"
	@echo "  CC=<compiler>      - Use specific compiler"
	@echo ""
	@echo "Platform: $(PLATFORM)"
	@echo "Backend: $(BACKEND)"

# Generic rule for building any .c file
%: %.c $(DEPS)
	@echo "Building $@ for $(PLATFORM)..."
	$(CC) $(CFLAGS) -o $@$(EXE_SUFFIX) $< $(LIBS)
	@echo "Build complete: $@$(EXE_SUFFIX)"

# Web build target
web: $(TARGET).c $(DEPS)
	@echo "Building $(TARGET) for web..."
	$(CC) $(CFLAGS) -o $(TARGET)$(EXE_SUFFIX) $(TARGET).c $(LIBS)
	@echo "Web build complete: $(TARGET)$(EXE_SUFFIX)"

# Clean target
clean:
	@echo "Cleaning build artifacts..."
	rm -f *.exe *.html *.js *.wasm *.data
	rm -f canvas
	rm -rf *.dSYM
	rm -f compile_flags.txt
	@echo "Clean complete"


# Example targets
canvas: canvas.c $(DEPS)

compile_flags.txt: FORCE
	@echo "Generating compile_flags.txt for IDE support"
	@echo $(CFLAGS) | tr ' ' '\n' > $@

FORCE:

