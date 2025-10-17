cmake_minimum_required(VERSION 3.15)
project(raylib_demo C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(../..) # p5.h
include_directories(../../renderers/raylib) # p5_raylib.h

# Add raylib subdirectory
add_subdirectory(
    ../../renderers/raylib/raylib-5.5
    ${CMAKE_CURRENT_BINARY_DIR}/raylib
)

# Platform-specific configuration
if(EMSCRIPTEN)
    # Web/Emscripten configuration
    set(PLATFORM_LIBS "")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_WEBGL2=1 -s FULL_ES3=1 -s ASSERTIONS=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 --shell-file ${CMAKE_SOURCE_DIR}/scripts/shell.html")
elseif(APPLE)
    set(PLATFORM_LIBS "-framework CoreVideo" "-framework IOKit" "-framework Cocoa" "-framework GLUT" "-framework OpenGL")
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ObjC")
elseif(WIN32)
    set(PLATFORM_LIBS gdi32 ole32 d3d11 dxgi)
    add_definitions(-D_WIN32_WINNT=0x0601)
else()
    # Linux
    set(PLATFORM_LIBS GL m pthread dl)
    add_definitions(-D_GNU_SOURCE)
endif()

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")

# Create executables
# canvas
add_executable(canvas main.c canvas.c)
target_link_libraries(canvas ${PLATFORM_LIBS} raylib)

# ball
add_executable(ball main.c ball.c)
target_link_libraries(ball ${PLATFORM_LIBS} raylib)

# point
add_executable(point main.c point.c)
target_link_libraries(point ${PLATFORM_LIBS} raylib)

# noise
add_executable(noise main.c noise.c)
target_link_libraries(noise ${PLATFORM_LIBS} raylib)

# Web-specific targets
if(EMSCRIPTEN)
    # Set file extensions for web builds
    set_target_properties(main PROPERTIES SUFFIX ".html")
    set_target_properties(balls PROPERTIES SUFFIX ".html")
    
    # Additional web build options
    set_target_properties(main PROPERTIES 
        LINK_FLAGS "-s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 --shell-file ${CMAKE_SOURCE_DIR}/scripts/shell.html")
    set_target_properties(balls PROPERTIES 
        LINK_FLAGS "-s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 --shell-file ${CMAKE_SOURCE_DIR}/scripts/shell.html")
endif()
