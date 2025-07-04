#!/bin/bash

# p5.h project creator
# Usage: ./p5_new.sh <project_name> [dest_path]

set -e

DEST_PATH="."

# Parse arguments
if [ $# -ge 1 ]; then
    PROJECT_NAME="$1"
else
    echo "Error: missing project name"
    echo "Usage: p5_new <project_name>"
    exit 1
fi

if [ $# -ge 2 ]; then
    DEST_PATH="$2"
fi

# Create full destination path
FULL_DEST="$DEST_PATH/$PROJECT_NAME"

# Create project directory
echo "Creating new p5.h project: $PROJECT_NAME"
echo "Destination: $FULL_DEST"

if [ -d "$FULL_DEST" ]; then
    echo "Error: Directory $FULL_DEST already exists"
    exit 1
fi

# Create deps directory
mkdir -p "$FULL_DEST/deps"

# Download dependencies from GitHub
echo "Downloading dependencies from GitHub..."

# Check if wget or curl is available
if command -v wget >/dev/null 2>&1; then
    DOWNLOADER="wget -O"
elif command -v curl >/dev/null 2>&1; then
    DOWNLOADER="curl -L -o"
else
    echo "Error: Neither wget nor curl is available"
    echo "Please install wget or curl to download dependencies"
    exit 1
fi

# Download dependencies from timwmillard Github
echo "  Downloading sokol_app.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_app.h" https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_app.h

echo "  Downloading sokol_gfx.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_gfx.h" https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_gfx.h

echo "  Downloading sokol_glue.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_glue.h" https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_glue.h

echo "  Downloading sokol_gp.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_gp.h" https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_gp.h

echo "  Downloading p5.h..."
$DOWNLOADER "$FULL_DEST/deps/p5.h" https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/p5.h

# Download build scripts from timwmillard GitHub
echo "  Downloading Makefile..."
$DOWNLOADER "$FULL_DEST/Makefile" https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/p5_new/template/Makefile

echo "  Downloading canvas.c..."
$DOWNLOADER "$FULL_DEST/canvas.c" https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/p5_new/template/canvas.c

echo ""
echo "Project created successfully!"
echo ""
echo "To get started:"
echo "  cd $FULL_DEST"
echo ""
echo "Edit the canvas:"
echo "  vim canvas.c"
echo ""
echo "Then build:"
echo "  make"
echo ""
echo "Then run:"
echo "  make run"
echo ""

