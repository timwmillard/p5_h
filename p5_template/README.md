# P5.h Project Template

This is a portable template for creating p5.js-style graphics programs in C using the p5.h library.

## Quick Start

1. **Edit your sketch**: Modify `sketch.c` or create a new `.c` file
2. **Build and run**:
   - **Unix/macOS**: `./build.sh`
   - **Windows**: `build.bat`

## Usage

### Basic Usage
```bash
# Build default sketch.c
./build.sh

# Build a specific file
./build.sh my_sketch.c

# Build with custom output name
./build.sh my_sketch.c my_program
```

### Creating a New Sketch

Create a new `.c` file with this basic structure:

```c
#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"
#include "deps/sokol_glue.h"
#include "deps/sokol_gp.h"
#define P5_IMPLEMENTATION
#include "p5.h"

void setup() {
    createCanvas(400, 300);
    // Initialize your sketch here
}

void draw() {
    background(220, 220, 220);
    // Your drawing code here
    rect(50, 50, 100, 75);
    circle(width()/2, height()/2, 50);
}

P5_MAIN(640, 480, "My Sketch", 4);
```

## Platform Support

- **macOS**: Uses Metal backend
- **Linux**: Uses OpenGL backend  
- **Windows**: Uses DirectX 11 backend

## Dependencies

Dependencies are automatically downloaded from GitHub when you create a new project:
- `sokol_app.h` - Application framework (from floooh/sokol)
- `sokol_gfx.h` - Graphics API (from floooh/sokol)
- `sokol_glue.h` - Glue code (from floooh/sokol)  
- `sokol_gp.h` - Graphics painter (from edubart/sokol_gp)
- `p5.h` - P5.js-style API (from timwmillard/p5_h)
- `build.sh` / `build.bat` - Build scripts (from timwmillard/p5_h)

## Available Functions

The p5.h library provides p5.js-compatible functions:

- **Canvas**: `createCanvas(width, height)`
- **Shapes**: `rect()`, `circle()`, `line()`, `triangle()`, `quad()`
- **Colors**: `background()`, `fill()`, `stroke()`, `noFill()`, `noStroke()`
- **Dimensions**: `width()`, `height()`, `windowWidth()`, `windowHeight()`

## Examples

See `sketch.c` for a basic example. The syntax is designed to be as close to p5.js as possible for easy translation of existing sketches.