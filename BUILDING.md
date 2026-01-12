# Building Olympia

This guide walks you through building the Olympia game engine from source using CMake.

## Prerequisites

You'll need:
- **CMake 4.2 or newer** - a build system generator
- **A C compiler** - gcc (Linux) or clang (macOS) work fine
- **Git** - to clone the repository

### Check your CMake version

```bash
cmake --version
```

If you see a version below 4.2, you'll need to upgrade. On macOS, use `brew install cmake`. On Linux, check your package manager or download from https://cmake.org.

## Quick Start

If you just want to build everything:

```bash
cmake -B build
cmake --build build
```

Your executables will be in the `build/` directory.

## Step-by-Step Build Process

### Step 1: Generate the build files

From the repository root directory, run:

```bash
cmake -B build
```

**What this does:** CMake reads `CMakeLists.txt` and generates a `Makefile` (or Ninja build files) inside a new `build/` directory. This is called an "out-of-source build" - it keeps generated files separate from your source code.

**Expected output:**
```
-- The C compiler identification is AppleClang 17.0.0.17000013
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Configuring done (0.2s)
-- Generating done (0.0s)
-- Build files have been written to: /path/to/olympia/build
```

### Step 2: Compile the code

```bash
cmake --build build
```

**What this does:** This runs the actual compilation. CMake figures out whether to use `make`, `ninja`, or another build tool.

**Expected output:** You'll see compiler warnings (this is legacy code), but it should end with:
```
[100%] Built target island-g3
```

### Step 3: Find your executables

After a successful build, the executables are in the `build/` directory:

```bash
ls build/
```

You should see:
- `mapgen-g1` - Map generator for Olympia G1
- `mapgen-g2` - Map generator for Olympia G2
- `mapgen-g3` - Map generator for Olympia G3
- `island-g3` - Island generator for G3

## Common Problems and Solutions

### "CMake version too old"

**Error:** `CMake 4.2 or higher is required. You are running version 3.x`

**Fix:** Upgrade CMake. On macOS: `brew install cmake`. On Ubuntu/Debian: you may need to install from https://cmake.org since apt packages are often outdated.

### "Build directory is corrupted" or strange errors after editing CMakeLists.txt

**Fix:** Delete the build directory and regenerate:

```bash
rm -rf build
cmake -B build
cmake --build build
```

This is safe - the `build/` directory only contains generated files, never source code.

### Compiler warnings

You'll see many warnings like:
```
warning: a function definition without a prototype is deprecated
warning: non-void function does not return a value
```

**This is expected.** The Olympia codebase is legacy C from the 1990s and uses conventions that modern compilers warn about. The code still compiles and runs correctly.

### "No such file or directory" for source files

**Error:** `g1/mapgen/mapgen.c: No such file or directory`

**Fix:** Make sure you're running cmake from the repository root (the directory containing `CMakeLists.txt`):

```bash
cd /path/to/olympia
cmake -B build
```

### Build fails after pulling new changes

If someone updated `CMakeLists.txt`, you may need to regenerate:

```bash
rm -rf build
cmake -B build
cmake --build build
```

## Rebuilding After Code Changes

If you only changed `.c` or `.h` files (not `CMakeLists.txt`), just run:

```bash
cmake --build build
```

CMake is smart enough to only recompile files that changed.

## Clean Build

To start completely fresh:

```bash
rm -rf build
cmake -B build
cmake --build build
```

## Legacy Makefiles

The original `Makefile` files still exist in `g1/`, `g2/`, and `g3/` subdirectories. These are preserved for historical reference but are not maintained. Use the CMake build instead.
