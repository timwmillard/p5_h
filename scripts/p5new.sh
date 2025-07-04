#!/bin/bash

# 🎨 p5.h project creator
# Usage: ./p5new.sh <project_name> [dest_path]

set -e

# Terminal colors and formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
GRAY='\033[0;90m'
BOLD='\033[1m'
DIM='\033[2m'
RESET='\033[0m'

# Progress spinner
spinner() {
    local pid=$1
    local delay=0.1
    local spinstr='|/-\'
    printf " "
    while [ "$(ps a | awk '{print $1}' | grep $pid)" ]; do
        local temp=${spinstr#?}
        printf "\r  ${CYAN}[${spinstr:0:1}]${RESET}"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
    done
    printf "\r  ${GREEN}[✓]${RESET}"
}

# Print functions
print_header() {
    printf "${BOLD}${MAGENTA}╭─────────────────────────────────────────────────────╮${RESET}\n"
    printf "${BOLD}${MAGENTA}│${RESET}                  ${BOLD}${CYAN}🎨 p5.h Creator${RESET}                    ${BOLD}${MAGENTA}│${RESET}\n"
    printf "${BOLD}${MAGENTA}╰─────────────────────────────────────────────────────╯${RESET}\n"
}

print_success() {
    printf "${GREEN}✅ $1${RESET}\n"
}

print_error() {
    printf "${RED}❌ Error: $1${RESET}\n"
}

print_info() {
    printf "${BLUE}$1${RESET}\n"
}

print_warning() {
    printf "${YELLOW}⚠️  $1${RESET}\n"
}

print_step() {
    printf "${BOLD}${WHITE}$1${RESET}\n"
}

DEST_PATH="."

# Parse arguments
print_header
echo ""

if [ $# -ge 1 ]; then
    PROJECT_NAME="$1"
else
    print_error "Missing project name"
    print_info "Usage: ${BOLD}p5new <project_name>${RESET}"
    exit 1
fi

if [ $# -ge 2 ]; then
    DEST_PATH="$2"
fi

# Create full destination path
FULL_DEST="$DEST_PATH/$PROJECT_NAME"

# Create project directory
print_step "Creating new p5.h project: ${BOLD}${GREEN}$PROJECT_NAME${RESET}"
print_info "Destination: ${CYAN}$FULL_DEST${RESET}"
echo ""

if [ -d "$FULL_DEST" ]; then
    print_error "Directory ${CYAN}$FULL_DEST${RESET} already exists"
    exit 1
fi

# Create deps directory
print_step "Setting up project structure..."
mkdir -p "$FULL_DEST/deps"
print_success "Created project directory"
echo ""

# Download dependencies from GitHub
print_step "Downloading dependencies from GitHub..."
echo ""

# Check if wget or curl is available
if command -v wget >/dev/null 2>&1; then
    DOWNLOADER="wget -q -O"
    DOWNLOADER_NAME="wget"
elif command -v curl >/dev/null 2>&1; then
    DOWNLOADER="curl -s -L -o"
    DOWNLOADER_NAME="curl"
else
    print_error "Neither wget nor curl is available"
    print_info "Please install wget or curl to download dependencies"
    exit 1
fi

print_info "Using ${BOLD}${CYAN}$DOWNLOADER_NAME${RESET} to download files"
echo ""

# Download dependencies from timwmillard Github
download_file() {
    local filename="$1"
    local url="$2"
    local dest="$3"
    
    printf "  📦 ${CYAN}%-15s${RESET} " "$filename"
    
    # Start download in background and show spinner
    $DOWNLOADER "$dest" "$url" 2>/dev/null &
    local download_pid=$!
    
    # Spinner animation
    local spinner_chars="⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏"
    local i=0
    while kill -0 $download_pid 2>/dev/null; do
        local char=${spinner_chars:$i:1}
        printf "\r  📦 ${CYAN}%-15s${RESET} ${YELLOW}$char${RESET}" "$filename"
        i=$(( (i + 1) % 10 ))
        sleep 0.1
    done
    
    # Wait for download to complete and check result
    wait $download_pid
    local result=$?
    
    if [ $result -eq 0 ]; then
        printf "\r  📦 ${CYAN}%-15s${RESET} ${GREEN}✓${RESET}\n" "$filename"
    else
        printf "\r  📦 ${CYAN}%-15s${RESET} ${RED}✗${RESET}\n" "$filename"
        print_error "Failed to download $filename"
        exit 1
    fi
}

print_info "📚 Downloading Sokol dependencies..."
download_file "sokol_app.h" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_app.h" "$FULL_DEST/deps/sokol_app.h"
download_file "sokol_gfx.h" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_gfx.h" "$FULL_DEST/deps/sokol_gfx.h"
download_file "sokol_glue.h" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_glue.h" "$FULL_DEST/deps/sokol_glue.h"
download_file "sokol_gp.h" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/deps/sokol_gp.h" "$FULL_DEST/deps/sokol_gp.h"

echo ""
print_info "🎨 Downloading p5.h library..."
download_file "p5.h" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/p5.h" "$FULL_DEST/deps/p5.h"

echo ""
print_info "🔧 Downloading build scripts..."
download_file "Makefile" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/scripts/Makefile" "$FULL_DEST/Makefile"
download_file "canvas.c" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/scripts/canvas.c" "$FULL_DEST/canvas.c"
download_file "shell.html" "https://raw.githubusercontent.com/timwmillard/p5_h/refs/heads/master/scripts/shell.html" "$FULL_DEST/deps/shell.html"

echo ""
print_success "Project created successfully!"
echo ""

# Success box
printf "${BOLD}${GREEN}╭─────────────────────────────────────────────────────╮${RESET}\n"
printf "${BOLD}${GREEN}│${RESET}                   ${BOLD}🎉 All Done!${RESET}                      ${BOLD}${GREEN}│${RESET}\n"
printf "${BOLD}${GREEN}╰─────────────────────────────────────────────────────╯${RESET}\n"
echo ""

print_info "📍 ${BOLD}Next Steps:${RESET}"
printf "   ${CYAN}1.${RESET} ${BOLD}cd $FULL_DEST${RESET}\n"
printf "   ${CYAN}2.${RESET} ${BOLD}make help${RESET}  ${GRAY}# View build options${RESET}\n"
echo ""

print_info "✏️  ${BOLD}Edit your canvas:${RESET}"
printf "   ${BOLD}vim canvas.c${RESET}  ${GRAY}# or use your favorite editor${RESET}\n"
echo ""

print_info "🔨 ${BOLD}Build and run:${RESET}"
printf "   ${BOLD}make${RESET}        ${GRAY}# Build the project${RESET}\n"
printf "   ${BOLD}./canvas${RESET}    ${GRAY}# Run your creation${RESET}\n"
echo ""

print_info "🌐 ${BOLD}For web builds:${RESET}"
printf "   ${BOLD}make web TARGET=canvas${RESET}  ${GRAY}# Build for web${RESET}\n"
echo ""

printf "${DIM}Happy coding! 🚀${RESET}\n"
echo ""

