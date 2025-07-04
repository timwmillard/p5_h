# CBuild - C Build Tool

A simple npm-like build tool for C projects that uses Lua configuration files.

## Features

- **Lua Configuration**: Define your build settings in a simple `build.lua` file
- **Dependency Management**: Automatically fetch single-header libraries from GitHub
- **Build System**: Compile multiple targets with configurable compiler settings
- **Cross-platform**: Works on macOS, Linux, and other Unix-like systems

## Installation

1. Install dependencies:
   ```bash
   make -f Makefile.cbuild install_deps
   ```

2. Build the tool:
   ```bash
   make -f Makefile.cbuild cbuild
   ```

## Usage

The tool reads configuration from `build.lua` in the current directory.

### Commands

- `./cbuild install` - Install dependencies from GitHub
- `./cbuild build` - Build all targets
- `./cbuild clean` - Clean build artifacts
- `./cbuild help` - Show help message

### Configuration Format

The `build.lua` file defines your project structure:

```lua
-- Project information
project = {
    name = "my_project",
    version = "1.0.0",
    description = "My C project"
}

-- Compiler settings
compiler = {
    cc = "clang",
    cflags = {
        "-Ideps",
        "-Wall",
        "-O2"
    },
    libs = {
        "-lm",
        "-lpthread"
    }
}

-- Dependencies from GitHub
dependencies = {
    {
        name = "stb",
        repo = "nothings/stb",
        files = {
            "stb_image.h",
            "stb_image_write.h"
        },
        dest = "deps/"
    }
}

-- Build targets
targets = {
    {
        name = "my_app",
        type = "executable",
        sources = {
            "src/main.c",
            "src/utils.c"
        },
        headers = {
            "include/my_app.h"
        }
    }
}
```

## Example Workflow

1. Create a `build.lua` file with your project configuration
2. Install dependencies: `./cbuild install`
3. Build your project: `./cbuild build`
4. Run your executable: `./my_app`

## Dependencies

- **Lua**: For parsing configuration files
- **libcurl**: For downloading dependencies from GitHub
- **Standard C library**: For system operations

## Platform Support

- **macOS**: Uses Homebrew for dependencies
- **Linux**: Uses apt-get or yum for dependencies
- **Other Unix-like systems**: Manual dependency installation required

## License

This tool is designed to work with single-header libraries and follows the same licensing as your project.
