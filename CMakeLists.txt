cmake_minimum_required(VERSION 3.15)
project(p5_demo C)

set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(.)
include_directories(deps)

# Platform-specific configuration
if(APPLE)
    set(BACKEND -DSOKOL_METAL)
    set(PLATFORM_LIBS "-framework Cocoa" "-framework QuartzCore" "-framework Metal" "-framework MetalKit")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ObjC")
elseif(WIN32)
    set(BACKEND -DSOKOL_D3D11)
    set(PLATFORM_LIBS gdi32 ole32 d3d11 dxgi)
    add_definitions(-D_WIN32_WINNT=0x0601)
else()
    # Linux
    set(BACKEND -DSOKOL_GLCORE)
    set(PLATFORM_LIBS GL m pthread dl)
    add_definitions(-D_GNU_SOURCE)
endif()

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")

# Add backend define
add_definitions(${BACKEND})

# Create executable
add_executable(main work/main.c deps/deps.c)

# Link libraries
target_link_libraries(main ${PLATFORM_LIBS})

# Create executable
add_executable(balls work/balls.c deps/deps.c)

# Link libraries
target_link_libraries(balls ${PLATFORM_LIBS})
