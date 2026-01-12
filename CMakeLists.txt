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

# Engine-specific flags for legacy K&R C code
set(G1_C_FLAGS -Wno-error -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-int-conversion -Wno-deprecated-non-prototype)
set(G2_C_FLAGS -Wno-error -Wno-implicit-function-declaration -Wno-return-mismatch)
set(G3_C_FLAGS -Wno-error -Wno-implicit-function-declaration -Wno-return-mismatch)

# ------------------------------------------------------------------------------
# G1 mapgen
# ------------------------------------------------------------------------------
add_executable(mapgen-g1
        g1/mapgen/mapgen.c
        g1/mapgen/z.c g1/mapgen/z.h
)
target_include_directories(mapgen-g1 PRIVATE g1/mapgen/)
target_compile_options(mapgen-g1 PRIVATE ${G1_C_FLAGS})
target_compile_definitions(mapgen-g1 PRIVATE NEW_TRADE)

add_executable(shuffle-g1
        g1/mapgen/shuffle.c
        g1/mapgen/z.c g1/mapgen/z.h
)
target_include_directories(shuffle-g1 PRIVATE g1/mapgen/)
target_compile_options(shuffle-g1 PRIVATE ${G1_C_FLAGS})

add_executable(count-g1 g1/mapgen/count.c)
target_compile_options(count-g1 PRIVATE ${G1_C_FLAGS})

# ------------------------------------------------------------------------------
# G2 mapgen
# ------------------------------------------------------------------------------
add_executable(mapgen-g2
        g2/mapgen/mapgen.c
        g2/mapgen/rnd.c
        g2/mapgen/z.c g2/mapgen/z.h
)
target_include_directories(mapgen-g2 PRIVATE g2/mapgen/)
target_compile_options(mapgen-g2 PRIVATE ${G2_C_FLAGS})
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
target_compile_options(mapgen-g3 PRIVATE ${G3_C_FLAGS})
target_compile_definitions(mapgen-g3 PRIVATE NEW_TRADE)

# G3 island tool
add_executable(island-g3
        g3/mapgen/island.c
        g3/mapgen/rnd.c
)
target_include_directories(island-g3 PRIVATE g3/mapgen/)
target_compile_options(island-g3 PRIVATE ${G3_C_FLAGS})

# ------------------------------------------------------------------------------
# G1 oly (main game engine)
# ------------------------------------------------------------------------------
add_executable(oly-g1
        g1/src/add.c g1/src/adv.c g1/src/alchem.c g1/src/art.c g1/src/basic.c
        g1/src/beast.c g1/src/build.c g1/src/buy.c g1/src/check.c g1/src/c1.c
        g1/src/c2.c g1/src/cloud.c g1/src/code.c g1/src/combat.c g1/src/day.c
        g1/src/dir.c g1/src/display.c g1/src/eat.c g1/src/faery.c g1/src/garr.c
        g1/src/gate.c g1/src/glob.c g1/src/gm.c g1/src/hades.c g1/src/immed.c
        g1/src/input.c g1/src/io.c g1/src/loc.c g1/src/lore.c g1/src/main.c
        g1/src/make.c g1/src/move.c g1/src/necro.c g1/src/npc.c g1/src/order.c
        g1/src/perm.c g1/src/produce.c g1/src/quest.c g1/src/relig.c
        g1/src/report.c g1/src/savage.c g1/src/scry.c g1/src/seed.c
        g1/src/sout.c g1/src/stack.c g1/src/stealth.c g1/src/storm.c
        g1/src/summary.c g1/src/swear.c g1/src/u.c g1/src/use.c g1/src/z.c
)
target_include_directories(oly-g1 PRIVATE g1/src/)
target_compile_options(oly-g1 PRIVATE ${G1_C_FLAGS})
target_compile_definitions(oly-g1 PRIVATE NEW_TRADE)
