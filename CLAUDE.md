# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

- `make demo` - Build the basic demo executable
- `make p5style` - Build the p5.js-style demo 
- `make canvas` - Build the canvas demo
- `make shapes` - Build the shapes demo
- `make run` - Build and run the basic demo
- `make run_p5style` - Build and run the p5.js-style demo
- `make run_canvas` - Build and run the canvas demo  
- `make run_shapes` - Build and run the shapes demo
- `make sokol` - Download latest Sokol headers from GitHub
- `make sokol_gp` - Download sokol_gp graphics painter library
- `make compile_flags.txt` - Generate IDE support file

All build binaries end up the root directory.  Ensure these are added to .gitignore.

## Architecture Overview

This is a single-header C library (p5.h) that provides p5.js-like creative coding functionality using the Sokol graphics framework. It enables artists and developers to create graphics applications using familiar p5.js syntax.

### Core Components

- **p5.h**: Single-header library providing p5.js-style functions (background, rect, circle, etc.) built on top of sokol_gp
- **Demo Applications**: Multiple example programs showing different usage patterns:
  - `src/demo.c`: Manual sokol setup with p5 functions
  - `src/p5_style_demo.c`: P5.js-style with setup()/draw() functions and P5_MAIN macro
  - `src/canvas_demo.c`: Canvas-focused demo showing createCanvas() usage
  - `src/shapes_demo.c`: Shape drawing examples
- **Dependencies** (`deps/`): Sokol single-header libraries (sokol_app.h, sokol_gfx.h, sokol_glue.h, sokol_gp.h)

### Usage Patterns

The library supports two main usage styles:

1. **P5.js-style (recommended)**: Use setup()/draw() functions with P5_MAIN macro for automatic sokol initialization
2. **Manual style**: Initialize sokol manually and call p5 functions in your own frame loop

### Platform Configuration

Currently configured for macOS Metal backend:
- Uses `-DSOKOL_METAL` flag and Metal frameworks (Cocoa, QuartzCore, Metal, MetalKit)
- Cross-platform support available by changing backend compilation flags
- Dependencies are single-header libraries for easy integration

### Library Structure

The p5.h library provides:
- Canvas management (createCanvas, width, height, windowWidth, windowHeight)
- Drawing functions (background, rect, circle, line, etc.)
- Color handling with p5_color_t structure
- P5.js-compatible function names and behavior
