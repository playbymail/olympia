cmake_minimum_required(VERSION 4.2)
project(olympia C)

set(CMAKE_C_STANDARD 90)

# Platform-specific defines
if (LINUX)
    add_compile_definitions(OLYMPIA_CC_LINUX)
endif ()
if (WIN32)
    add_compile_definitions(OLYMPIA_CC_WINDOWS)
endif ()

# Common flags for legacy K&R C code
set(LEGACY_C_FLAGS -Wno-error -Wno-implicit-function-declaration -Wno-return-mismatch)

# ------------------------------------------------------------------------------
# G1 mapgen
# ------------------------------------------------------------------------------
add_executable(mapgen-g1
        g1/mapgen/mapgen.c
        g1/mapgen/z.c g1/mapgen/z.h
)
target_include_directories(mapgen-g1 PRIVATE g1/mapgen/)
target_compile_options(mapgen-g1 PRIVATE ${LEGACY_C_FLAGS})
target_compile_definitions(mapgen-g1 PRIVATE NEW_TRADE)

# ------------------------------------------------------------------------------
# G2 mapgen
# ------------------------------------------------------------------------------
add_executable(mapgen-g2
        g2/mapgen/mapgen.c
        g2/mapgen/rnd.c
        g2/mapgen/z.c g2/mapgen/z.h
)
target_include_directories(mapgen-g2 PRIVATE g2/mapgen/)
target_compile_options(mapgen-g2 PRIVATE ${LEGACY_C_FLAGS})
target_compile_definitions(mapgen-g2 PRIVATE NEW_TRADE)

# ------------------------------------------------------------------------------
# G3 mapgen
# ------------------------------------------------------------------------------
add_executable(mapgen-g3
        g3/mapgen/mapgen.c
        g3/mapgen/rnd.c
        g3/mapgen/z.c g3/mapgen/z.h
)
target_include_directories(mapgen-g3 PRIVATE g3/mapgen/)
target_compile_options(mapgen-g3 PRIVATE ${LEGACY_C_FLAGS})
target_compile_definitions(mapgen-g3 PRIVATE NEW_TRADE)

# G3 island tool
add_executable(island-g3
        g3/mapgen/island.c
        g3/mapgen/rnd.c
)
target_include_directories(island-g3 PRIVATE g3/mapgen/)
target_compile_options(island-g3 PRIVATE ${LEGACY_C_FLAGS})
