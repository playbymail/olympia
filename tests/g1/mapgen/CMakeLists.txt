# G1 mapgen golden file tests
#
# These tests verify that mapgen produces identical output to known-good
# reference files (golden files) when given the same inputs and random seeds.

# Add golden file test
add_test(
    NAME g1-mapgen-golden
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_golden.sh $<TARGET_FILE:mapgen-g1>
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Set test properties
set_tests_properties(g1-mapgen-golden PROPERTIES
    LABELS "g1;mapgen;golden"
    TIMEOUT 30
)
