#!/bin/bash

# p5.h project creator
# Usage: ./p5_new.sh [project_name] [destination_path]

set -e

PROJECT_NAME="my_p5_project"
DEST_PATH="."
TEMPLATE_DIR="$(dirname "$0")/p5_template"

# Parse arguments
if [ $# -ge 1 ]; then
    PROJECT_NAME="$1"
fi

if [ $# -ge 2 ]; then
    DEST_PATH="$2"
fi

# Create full destination path
FULL_DEST="$DEST_PATH/$PROJECT_NAME"

# Check if template exists
if [ ! -d "$TEMPLATE_DIR" ]; then
    echo "Error: Template directory not found at $TEMPLATE_DIR"
    echo "Make sure you're running this from the p5_h directory"
    exit 1
fi

# Create project directory
echo "Creating new p5.h project: $PROJECT_NAME"
echo "Destination: $FULL_DEST"

if [ -d "$FULL_DEST" ]; then
    echo "Error: Directory $FULL_DEST already exists"
    exit 1
fi

# Copy template (excluding deps and build scripts)
cp -r "$TEMPLATE_DIR" "$FULL_DEST"

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

# Download sokol headers
echo "  Downloading sokol_app.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_app.h" https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_app.h

echo "  Downloading sokol_gfx.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_gfx.h" https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_gfx.h

echo "  Downloading sokol_glue.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_glue.h" https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_glue.h

echo "  Downloading sokol_gp.h..."
$DOWNLOADER "$FULL_DEST/deps/sokol_gp.h" https://raw.githubusercontent.com/edubart/sokol_gp/master/sokol_gp.h

# Copy deps.c from current project
cp "$(dirname "$0")/deps/deps.c" "$FULL_DEST/deps/"

# Download p5.h from timwmillard GitHub
echo "  Downloading p5.h..."
$DOWNLOADER "$FULL_DEST/p5.h" https://raw.githubusercontent.com/timwmillard/p5_h/master/p5.h

# Download build scripts from timwmillard GitHub
echo "  Downloading build.sh..."
$DOWNLOADER "$FULL_DEST/build.sh" https://raw.githubusercontent.com/timwmillard/p5_h/master/p5_template/build.sh

echo "  Downloading build.bat..."
$DOWNLOADER "$FULL_DEST/build.bat" https://raw.githubusercontent.com/timwmillard/p5_h/master/p5_template/build.bat

# Make build scripts executable
chmod +x "$FULL_DEST/build.sh"

echo ""
echo "Project created successfully!"
echo ""
echo "To get started:"
echo "  cd $FULL_DEST"
echo "  ./build.sh"
echo "  ./sketch"
echo ""
echo "To create a new sketch:"
echo "  ./build.sh my_sketch.c"
echo ""