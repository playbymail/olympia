# Testing

The test infrastructure is set up for CMake.

## Directory Structure

    tests/g1/mapgen/
    ├── CMakeLists.txt          # Test definition for CTest
    ├── test_golden.sh          # Test script (executable)
    ├── fixtures/               # Input files
    │   ├── Map
    │   ├── Cities
    │   ├── Land
    │   └── Regions
    └── golden/                 # Expected outputs
        ├── gate
        ├── loc
        └── road

Notes:

1. Test directory structure includes fixtures/ (test inputs) and golden/ (expected test outputs) subdirectories
2. Input files (example: Map, Cities, Land, Regions) are saved to fixtures/
3. Generated and copied golden output files (gate, loc, road) to golden/
4. Each test contains scripts (example: test_golden.sh) that:
   - Create a temporary workspace
   - Copy fixtures
   - Set deterministic random seeds
   - Run the application (example: mapgen)
   - Compare outputs with golden files using cmp
   - Show diffs on failure
5. CMake test configuration at tests/g1/mapgen/CMakeLists.txt
6. Root test configuration at tests/CMakeLists.txt
7. Updated root CMakeLists.txt to enable testing and include the tests directory
8. Updated .gitignore to ignore generated outputs in run/g1/mapgen/
9. Verified test passes ✓

## How to Use

Run tests via CTest:

### Run all tests
    ctest --test-dir build --output-on-failure

### Run just the mapgen golden test
    ctest --test-dir build -R g1-mapgen-golden -V

### Run tests matching a label
    ctest --test-dir build -L mapgen

### Run test script directly
    ./tests/g1/mapgen/test_golden.sh build/mapgen-g1

### In CLion

- Open the "Run" tool window
- You'll see "g1-mapgen-golden" test
- Click the green play button to run it
- Click the bug icon to debug it

## Refactoring Workflow

Now you can start the refactor-compare loop:

### 1. Edit mapgen.c
    vim g1/mapgen/mapgen.c

### 2. Test your changes
    cmake --build build --target mapgen-g1
    ctest --test-dir build -R g1-mapgen-golden --output-on-failure

### 3. If test fails, check diffs (shown in test output)
### 4. Repeat until test passes

## When you intentionally change output:

### Regenerate golden files
    ./run/g1/run00.sh
    cp run/g1/mapgen/{gate,loc,road} tests/g1/mapgen/golden/
    git add tests/g1/mapgen/golden/
    git commit -m "Update golden files after refactoring"

You're all set to start refactoring G1's mapgen with confidence!