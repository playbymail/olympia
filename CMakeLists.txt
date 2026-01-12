cmake_minimum_required(VERSION 4.1)
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
set(G2_C_FLAGS -Wno-error -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-int-conversion -Wno-deprecated-non-prototype)
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

add_executable(count-g1 g1/mapgen/count.c)
target_compile_options(count-g1 PRIVATE ${G1_C_FLAGS})

add_executable(shuffle-g1
        g1/mapgen/shuffle.c
        g1/mapgen/z.c g1/mapgen/z.h
)
target_include_directories(shuffle-g1 PRIVATE g1/mapgen/)
target_compile_options(shuffle-g1 PRIVATE ${G1_C_FLAGS})

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

# ------------------------------------------------------------------------------
# G1 cmd utilities
# ------------------------------------------------------------------------------
add_executable(acct-g1 g1/cmd/acct.c)
target_compile_options(acct-g1 PRIVATE ${G1_C_FLAGS})

add_executable(conv-g1 g1/cmd/conv.c)
target_compile_options(conv-g1 PRIVATE ${G1_C_FLAGS})

add_executable(gen_acct_alloc-g1 g1/cmd/gen_acct_alloc.c)
target_compile_options(gen_acct_alloc-g1 PRIVATE ${G1_C_FLAGS})

add_executable(healtsim-g1 g1/cmd/healtsim.c)
target_compile_options(healtsim-g1 PRIVATE ${G1_C_FLAGS})

add_executable(mailsplit-g1
        g1/cmd/mailsplit.c
        g1/cmd/z.c g1/cmd/z.h
)
target_include_directories(mailsplit-g1 PRIVATE g1/cmd/)
target_compile_options(mailsplit-g1 PRIVATE ${G1_C_FLAGS})

add_executable(percent-g1 g1/cmd/percent.c)
target_compile_options(percent-g1 PRIVATE ${G1_C_FLAGS})

add_executable(sim-g1 g1/cmd/sim.c)
target_compile_options(sim-g1 PRIVATE ${G1_C_FLAGS})

add_executable(sim2-g1 g1/cmd/sim2.c)
target_compile_options(sim2-g1 PRIVATE ${G1_C_FLAGS})

add_executable(u-g1 g1/cmd/u.c)
target_compile_options(u-g1 PRIVATE ${G1_C_FLAGS})

add_executable(v-g1 g1/cmd/v.c)
target_compile_options(v-g1 PRIVATE ${G1_C_FLAGS})

# ------------------------------------------------------------------------------
# G1 doc utilities
# ------------------------------------------------------------------------------
add_executable(toc-g1 g1/doc/toc.c)
target_compile_options(toc-g1 PRIVATE ${G1_C_FLAGS})

# ------------------------------------------------------------------------------
# G1 test utilities
# ------------------------------------------------------------------------------
add_executable(test-prng-g1 g1/src/z.c)
target_compile_options(test-prng-g1 PRIVATE ${G1_C_FLAGS})
target_compile_definitions(test-prng-g1 PRIVATE DRIVER)

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

add_executable(count-g2 g2/mapgen/count.c)
target_compile_options(count-g2 PRIVATE ${G2_C_FLAGS})

add_executable(pick-g2
        g2/mapgen/pick.c
        g2/mapgen/rnd.c
        g2/mapgen/z.c g2/mapgen/z.h
)
target_include_directories(pick-g2 PRIVATE g2/mapgen/)
target_compile_options(pick-g2 PRIVATE ${G2_C_FLAGS})

add_executable(shuffle-g2
        g2/mapgen/shuffle.c
        g2/mapgen/rnd.c
        g2/mapgen/z.c g2/mapgen/z.h
)
target_include_directories(shuffle-g2 PRIVATE g2/mapgen/)
target_compile_options(shuffle-g2 PRIVATE ${G2_C_FLAGS})

add_executable(j-g2
        g2/mapgen/j.c
        g2/mapgen/rnd.c
        g2/mapgen/z.c g2/mapgen/z.h
)
target_include_directories(j-g2 PRIVATE g2/mapgen/)
target_compile_options(j-g2 PRIVATE ${G2_C_FLAGS})

# WARNING: mapgen-old-g2 does not compile - missing symbols (ascii_map, init_random)
# This appears to be a G1 file that was not updated for G2.
#add_executable(mapgen-old-g2
#        g2/mapgen/old.c
#        g2/mapgen/rnd.c
#        g2/mapgen/z.c g2/mapgen/z.h
#)
#target_include_directories(mapgen-old-g2 PRIVATE g2/mapgen/)
#target_compile_options(mapgen-old-g2 PRIVATE ${G2_C_FLAGS})

# ------------------------------------------------------------------------------
# G2 oly (main game engine)
# ------------------------------------------------------------------------------
add_executable(oly-g2
        g2/src/add.c g2/src/adv.c g2/src/alchem.c g2/src/art.c g2/src/basic.c
        g2/src/beast.c g2/src/build.c g2/src/buy.c g2/src/check.c g2/src/c1.c
        g2/src/c2.c g2/src/cloud.c g2/src/code.c g2/src/combat.c g2/src/day.c
        g2/src/dir.c g2/src/display.c g2/src/eat.c g2/src/faery.c g2/src/garr.c
        g2/src/gate.c g2/src/glob.c g2/src/gm.c g2/src/hades.c g2/src/immed.c
        g2/src/input.c g2/src/io.c g2/src/loc.c g2/src/lore.c g2/src/main.c
        g2/src/make.c g2/src/move.c g2/src/necro.c g2/src/npc.c g2/src/order.c
        g2/src/perm.c g2/src/produce.c g2/src/quest.c g2/src/relig.c
        g2/src/report.c g2/src/rnd.c g2/src/savage.c g2/src/scry.c g2/src/seed.c
        g2/src/sout.c g2/src/stack.c g2/src/stealth.c g2/src/storm.c
        g2/src/summary.c g2/src/swear.c g2/src/tunnel.c g2/src/u.c g2/src/use.c
        g2/src/z.c
)
target_include_directories(oly-g2 PRIVATE g2/src/)
target_compile_options(oly-g2 PRIVATE ${G2_C_FLAGS})
target_compile_definitions(oly-g2 PRIVATE NEW_TRADE)

# ------------------------------------------------------------------------------
# G2 cmd utilities
# ------------------------------------------------------------------------------
add_executable(acct-g2 g2/cmd/acct.c)
target_compile_options(acct-g2 PRIVATE ${G2_C_FLAGS})

add_executable(conv-g2 g2/cmd/conv.c)
target_compile_options(conv-g2 PRIVATE ${G2_C_FLAGS})

add_executable(gen_acct_alloc-g2 g2/cmd/gen_acct_alloc.c)
target_compile_options(gen_acct_alloc-g2 PRIVATE ${G2_C_FLAGS})

add_executable(healtsim-g2 g2/cmd/healtsim.c)
target_compile_options(healtsim-g2 PRIVATE ${G2_C_FLAGS})

add_executable(mailsplit-g2
        g2/cmd/mailsplit.c
        g2/cmd/z.c g2/cmd/z.h
)
target_include_directories(mailsplit-g2 PRIVATE g2/cmd/)
target_compile_options(mailsplit-g2 PRIVATE ${G2_C_FLAGS})

add_executable(newacct-g2 g2/cmd/newacct.c)
target_compile_options(newacct-g2 PRIVATE ${G2_C_FLAGS})

add_executable(old_rep-g2 g2/cmd/old_rep.c)
target_compile_options(old_rep-g2 PRIVATE ${G2_C_FLAGS})

add_executable(percent-g2 g2/cmd/percent.c)
target_compile_options(percent-g2 PRIVATE ${G2_C_FLAGS})

add_executable(rep-g2 g2/cmd/rep.c)
target_compile_options(rep-g2 PRIVATE ${G2_C_FLAGS})

add_executable(sim-g2 g2/cmd/sim.c)
target_compile_options(sim-g2 PRIVATE ${G2_C_FLAGS})

add_executable(sim2-g2 g2/cmd/sim2.c)
target_compile_options(sim2-g2 PRIVATE ${G2_C_FLAGS})

add_executable(tagacct-g2 g2/cmd/tagacct.c)
target_compile_options(tagacct-g2 PRIVATE ${G2_C_FLAGS})

add_executable(u-g2 g2/cmd/u.c)
target_compile_options(u-g2 PRIVATE ${G2_C_FLAGS})

add_executable(v-g2 g2/cmd/v.c)
target_compile_options(v-g2 PRIVATE ${G2_C_FLAGS})

# ------------------------------------------------------------------------------
# G2 doc utilities
# ------------------------------------------------------------------------------
add_executable(toc-g2 g2/doc/toc.c)
target_compile_options(toc-g2 PRIVATE ${G2_C_FLAGS})

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

