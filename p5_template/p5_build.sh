#!/bin/bash

# p5.h portable build script
# Usage: ./build.sh [source_file.c] [output_name]

set -e

# Default values
SOURCE_FILE="sketch.c"
OUTPUT_NAME="sketch"

# Parse arguments
if [ $# -ge 1 ]; then
    SOURCE_FILE="$1"
fi

if [ $# -ge 2 ]; then
    OUTPUT_NAME="$2"
fi

# Check if source file exists
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: Source file '$SOURCE_FILE' not found"
    exit 1
fi

# Detect platform
OS=$(uname -s)
case $OS in
    Darwin)
        # macOS
        PLATFORM_FLAGS="-DSOKOL_METAL -framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit -ObjC"
        ;;
    Linux)
        # Linux
        PLATFORM_FLAGS="-DSOKOL_GLCORE33 -lGL -lX11 -lXcursor -lXi -ldl -lpthread -lm"
        ;;
    MINGW*|MSYS*|CYGWIN*)
        # Windows (MinGW)
        PLATFORM_FLAGS="-DSOKOL_D3D11 -ld3d11 -ldxgi -lole32 -luser32 -lgdi32"
        OUTPUT_NAME="${OUTPUT_NAME}.exe"
        ;;
    *)
        echo "Warning: Unsupported platform '$OS', using Linux defaults"
        PLATFORM_FLAGS="-DSOKOL_GLCORE33 -lGL -lX11 -lXcursor -lXi -ldl -lpthread -lm"
        ;;
esac

# Compile dependencies if needed
if [ ! -f "deps.o" ]; then
    echo "Compiling dependencies..."
    gcc -c -Ideps -o deps.o deps/deps.c $PLATFORM_FLAGS
fi

# Compile the main program
echo "Compiling $SOURCE_FILE -> $OUTPUT_NAME"
gcc -Ideps -o "$OUTPUT_NAME" $PLATFORM_FLAGS deps.o "$SOURCE_FILE"

echo "Build successful! Run with: ./$OUTPUT_NAME"
