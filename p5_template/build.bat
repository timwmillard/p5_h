@echo off
rem p5.h portable build script for Windows
rem Usage: build.bat [source_file.c] [output_name]

setlocal enabledelayedexpansion

set SOURCE_FILE=sketch.c
set OUTPUT_NAME=sketch

if not "%~1"=="" set SOURCE_FILE=%~1
if not "%~2"=="" set OUTPUT_NAME=%~2

if not exist "%SOURCE_FILE%" (
    echo Error: Source file '%SOURCE_FILE%' not found
    exit /b 1
)

set PLATFORM_FLAGS=-DSOKOL_D3D11 -ld3d11 -ldxgi -lole32 -luser32 -lgdi32

if not exist "deps.o" (
    echo Compiling dependencies...
    gcc -c -Ideps -o deps.o deps/deps.c %PLATFORM_FLAGS%
    if errorlevel 1 (
        echo Error compiling dependencies
        exit /b 1
    )
)

echo Compiling %SOURCE_FILE% -^> %OUTPUT_NAME%.exe
gcc -Ideps -o "%OUTPUT_NAME%.exe" %PLATFORM_FLAGS% deps.o "%SOURCE_FILE%"
if errorlevel 1 (
    echo Error compiling %SOURCE_FILE%
    exit /b 1
)

echo Build successful! Run with: %OUTPUT_NAME%.exe