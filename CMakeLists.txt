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
set(G3_C_FLAGS -Wno-error -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-incompatible-function-pointer-types -Wno-int-conversion)

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

add_executable(count-g3 g3/mapgen/count.c)
target_compile_options(count-g3 PRIVATE ${G3_C_FLAGS})

add_executable(j-g3
        g3/mapgen/j.c
        g3/mapgen/rnd.c
        g3/mapgen/z.c g3/mapgen/z.h
)
target_include_directories(j-g3 PRIVATE g3/mapgen/)
target_compile_options(j-g3 PRIVATE ${G3_C_FLAGS})

add_executable(pick-g3
        g3/mapgen/pick.c
        g3/mapgen/rnd.c
        g3/mapgen/z.c g3/mapgen/z.h
)
target_include_directories(pick-g3 PRIVATE g3/mapgen/)
target_compile_options(pick-g3 PRIVATE ${G3_C_FLAGS})

add_executable(shuffle-g3
        g3/mapgen/shuffle.c
        g3/mapgen/rnd.c
        g3/mapgen/z.c g3/mapgen/z.h
)
target_include_directories(shuffle-g3 PRIVATE g3/mapgen/)
target_compile_options(shuffle-g3 PRIVATE ${G3_C_FLAGS})

# WARNING: old-g3 does not compile - missing symbols (ascii_map, init_random)
# This appears to be a G1 file that was not updated for G2/G3.
#add_executable(old-g3
#        g3/mapgen/old.c
#        g3/mapgen/rnd.c
#        g3/mapgen/z.c g3/mapgen/z.h
#)
#target_include_directories(old-g3 PRIVATE g3/mapgen/)
#target_compile_options(old-g3 PRIVATE ${G3_C_FLAGS})

# ------------------------------------------------------------------------------
# G3 g2rep utilities
# ------------------------------------------------------------------------------
add_executable(g2rep-g3 g3/g2rep/rep.c)
target_compile_options(g2rep-g3 PRIVATE ${G3_C_FLAGS})

# ------------------------------------------------------------------------------
# G3 olympia (main game engine)
# ------------------------------------------------------------------------------
add_executable(olympia-g3
        g3/olympia/add.c g3/olympia/adv.c g3/olympia/alchem.c g3/olympia/art.c
        g3/olympia/basic.c g3/olympia/beast.c g3/olympia/build.c g3/olympia/buy.c
        g3/olympia/c1.c g3/olympia/c2.c g3/olympia/check.c g3/olympia/cloud.c
        g3/olympia/code.c g3/olympia/combat.c g3/olympia/day.c g3/olympia/dir.c
        g3/olympia/display.c g3/olympia/eat.c g3/olympia/faery.c g3/olympia/garr.c
        g3/olympia/gate.c g3/olympia/glob.c g3/olympia/gm.c g3/olympia/hades.c
        g3/olympia/immed.c g3/olympia/input.c g3/olympia/io.c g3/olympia/loc.c
        g3/olympia/lore.c g3/olympia/main.c g3/olympia/make.c g3/olympia/move.c
        g3/olympia/necro.c g3/olympia/npc.c g3/olympia/order.c g3/olympia/perm.c
        g3/olympia/produce.c g3/olympia/pw.c g3/olympia/quest.c g3/olympia/relig.c
        g3/olympia/report.c g3/olympia/rnd.c g3/olympia/savage.c g3/olympia/scry.c
        g3/olympia/seed.c g3/olympia/sout.c g3/olympia/stack.c g3/olympia/stealth.c
        g3/olympia/storm.c g3/olympia/summary.c g3/olympia/swear.c g3/olympia/tunnel.c
        g3/olympia/u.c g3/olympia/use.c g3/olympia/z.c
)
target_include_directories(olympia-g3 PRIVATE g3/olympia/)
target_compile_options(olympia-g3 PRIVATE ${G3_C_FLAGS})
target_compile_definitions(olympia-g3 PRIVATE NEW_TRADE)

# ------------------------------------------------------------------------------
# TAG oly (Olympia: The Age of Gods - main game engine)
# ------------------------------------------------------------------------------
set(TAG_C_FLAGS -Wno-error -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-int-conversion -Wno-deprecated-non-prototype)

add_executable(olympia-tag
        tag/src/add.c tag/src/adv.c tag/src/alchem.c tag/src/art.c tag/src/artifacts.c
        tag/src/basic.c tag/src/beast.c tag/src/build.c tag/src/buy.c tag/src/c1.c
        tag/src/c2.c tag/src/check.c tag/src/cloud.c tag/src/code.c tag/src/combat.c
        tag/src/day.c tag/src/dir.c tag/src/display.c tag/src/eat.c tag/src/effect.c
        tag/src/faery.c tag/src/garr.c tag/src/gate.c tag/src/glob.c tag/src/gm.c
        tag/src/hades.c tag/src/hero.c tag/src/immed.c tag/src/input.c tag/src/io.c
        tag/src/kill.c tag/src/loc.c tag/src/lore.c tag/src/main.c tag/src/make.c
        tag/src/map.c tag/src/mining.c tag/src/move.c tag/src/necro.c tag/src/npc.c
        tag/src/order.c tag/src/perm.c tag/src/produce.c tag/src/quest.c tag/src/ranger.c
        tag/src/relig.c tag/src/report.c tag/src/savage.c tag/src/scry.c tag/src/seed.c
        tag/src/shipcraft.c tag/src/sout.c tag/src/stack.c tag/src/stealth.c tag/src/storm.c
        tag/src/summary.c tag/src/swear.c tag/src/times.c tag/src/trading.c tag/src/u.c
        tag/src/use.c tag/src/win.c tag/src/z.c
)
target_include_directories(olympia-tag PRIVATE tag/src/)
target_compile_options(olympia-tag PRIVATE ${TAG_C_FLAGS})
target_compile_definitions(olympia-tag PRIVATE NEW_TRADE)
target_link_libraries(olympia-tag PRIVATE m)

