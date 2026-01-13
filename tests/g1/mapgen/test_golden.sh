#!/bin/bash
# Golden file test for G1 mapgen
# Compares mapgen output against known-good reference files

set -e

# Check that mapgen executable was provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <path-to-mapgen-g1>"
    exit 1
fi

MAPGEN_BIN="$1"

if [ ! -x "$MAPGEN_BIN" ]; then
    echo "ERROR: mapgen executable not found or not executable: $MAPGEN_BIN"
    exit 1
fi

# Determine script location
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
FIXTURES_DIR="$SCRIPT_DIR/fixtures"
GOLDEN_DIR="$SCRIPT_DIR/golden"

# Create temporary working directory
WORK_DIR=$(mktemp -d)
trap "rm -rf $WORK_DIR" EXIT

cd "$WORK_DIR"

# Copy input fixtures to working directory
cp "$FIXTURES_DIR"/* .

# Set deterministic random seeds
export G1_SEED_1=18481
export G1_SEED_2=28078
export G1_SEED_3=26982

# Run mapgen (redirect stderr to suppress verbose output during testing)
echo "Running mapgen-g1..."
if ! "$MAPGEN_BIN" 2>/dev/null; then
    echo "FAIL: mapgen-g1 execution failed"
    exit 1
fi

# Compare outputs with golden files
FAILED=0
for file in gate loc road; do
    if [ ! -f "$file" ]; then
        echo "FAIL: Output file '$file' was not generated"
        FAILED=1
        continue
    fi

    if ! cmp -s "$file" "$GOLDEN_DIR/$file"; then
        echo "FAIL: Output file '$file' differs from golden file"
        echo ""
        echo "Diff output:"
        diff -u "$GOLDEN_DIR/$file" "$file" || true
        echo ""
        FAILED=1
    fi
done

if [ $FAILED -eq 0 ]; then
    echo "PASS: All outputs match golden files"
    exit 0
else
    echo ""
    echo "Golden files are located at: $GOLDEN_DIR"
    echo "Test outputs are in: $WORK_DIR (will be cleaned up on exit)"
    exit 1
fi
