#!/bin/bash
# Build 32-bit binaries for generating golden test files
# Run this on a Linux system with gcc-multilib installed

set -e

echo "=== 32-bit Build Script for Golden File Generation ==="
echo ""

# Check if we're on Linux
if [[ "$OSTYPE" != "linux-gnu"* ]]; then
    echo "WARNING: This script is designed for Linux with gcc-multilib"
    echo "Current OS: $OSTYPE"
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Check if gcc-multilib is installed
if ! dpkg -l | grep -q gcc-multilib 2>/dev/null; then
    echo "gcc-multilib not found. Installing..."
    echo "Running: sudo apt-get install -y gcc-multilib g++-multilib"
    sudo apt-get update
    sudo apt-get install -y gcc-multilib g++-multilib cmake build-essential
fi

# Create and configure build directory
echo ""
echo "=== Configuring 32-bit build ==="
mkdir -p build32
cd build32
cmake -DBUILD_32BIT=ON ..

# Build the binaries
echo ""
echo "=== Building binaries ==="
cmake --build . --target mapgen-g1
cmake --build . --target oly-g1

# Verify they're 32-bit
echo ""
echo "=== Verifying 32-bit binaries ==="
echo "mapgen-g1:"
file mapgen-g1 | grep -o "ELF [0-9]*-bit"
echo "oly-g1:"
file oly-g1 | grep -o "ELF [0-9]*-bit"

echo ""
echo "=== Build complete! ==="
echo ""
echo "Next steps:"
echo "  1. Run the test scripts to generate golden files:"
echo "     bash run/g1/mapgen.sh"
echo "     bash run/g1/oly.sh"
echo ""
echo "  2. Package the golden outputs:"
echo "     cd run/g1/oly"
echo "     tar czf golden-outputs.tgz lib/"
echo ""
echo "  3. Copy to your Mac:"
echo "     scp golden-outputs.tgz you@your-mac:/path/to/golympia/"
