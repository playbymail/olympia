#!/bin/bash

OLYMPIA_ROOT=/Users/wraith/Software/mdhender/golympia
cd "${OLYMPIA_ROOT}" || {
  echo "error: unable to set def to OLYMPIA_ROOT"
  exit 2
}

# build targets for this test run
cmake --build build --target oly-g1 || {
  echo "error: build failed"
  exit 2
}
OLYMPIA_BIN="${OLYMPIA_ROOT}/build"

# set def to the run folder
G1_PATH=run/g1/oly
cd "${G1_PATH}" || {
  echo "error: unable to set def to G1_PATH"
  exit 2
}

# How oly-g1 Runs
#
# Command-line flags:
#  oly-g1 [options]
#    -l dir    Specify libdir (default: ./lib)
#    -r        Run a turn
#    -a        Add new players mode
#    -e        Eat orders from libdir/spool
#    -i        Immediate mode (interactive commands)
#    -S        Save the database at completion
#    -M        Mail reports
#    -A        Charge player accounts
#    -R        Test random number generator
#    -t        Test ilist code
#
# Required Inputs
#  oly-g1 needs a libdir with these files:
#
#  Core Data Files (from mapgen):
#  - loc - Province/location data (1.9 MB in example)
#  - gate - Magical gate connections (20 KB)
#  - road - Road connections (21 KB)
#
#  Game State Files:
#  - system - System config (player IDs, random seeds, clock)
#  - master - Fast index file (560 KB)
#  - item - Items/resources (163 KB)
#  - skill - Skills/knowledge (9 KB)
#  - ship - Ships (can be empty)
#  - unform - Unit formations (can be empty)
#  - misc - Miscellaneous entities (156 bytes)
#
#  Player Data:
#  - players - Player registry
#  - orders/ - Directory with player orders
#  - email - Email addresses
#  - forward - Email forwarding
#  - fact/ - Faction information
#  - lore/ - Lore sheets per player
#  - spool/ - Order processing queue
#
#  Logging:
#  - log/ - Turn logs and reports
#
# Testing Strategy
#
#  Test if oly-g1 can load the database
#    oly-g1 -l ./lib
#
# Test immediate mode (should be simplest)
#    oly-g1 -l ./lib -i
#
# Eventually: run a turn
#    oly-g1 -l ./lib -r -S
#
# Then
#  1. Try loading the existing database
#  2. Capture the output as golden files
#  3. Start refactoring

# restore the database (flat files)
[ -f lib.tgz ] && {
  [ -d lib ] && {
    echo " warn: remove lib/"
    rm -rf lib
  }
  tar zxf lib.tgz
  echo " info: rebuilt lib/"

  # Remove master index file - it will be regenerated on load
  # The master file can be out of date or have box type mismatches
  rm -f lib/master
  echo " info: removed lib/master (will be regenerated)"
}

# set seed values for random number generator
# todo

# run the program in "immediate" mode
# inputs: lib/
# outputs: unknown
"${OLYMPIA_BIN}/oly-g1" -l ./lib -i || {
  echo "error: oly failed"
  exit 2
}

# compare outputs and launch visual diff if needed
cd "${OLYMPIA_ROOT}" || {
  echo "error: unable to set def to OLYMPIA_ROOT"
  exit 2
}
# todo: files are not known yet
#for file in gate loc road; do
#  echo " test: comparing '${file}' to 'golden/${file}'"
#  diff "tests/g1/mapgen/golden/${file}" "run/g1/mapgen/${file}" || {
#    echo " diff: file '${file}' does not match golden file"
#    clion diff "tests/g1/mapgen/golden/${file}" "run/g1/mapgen/${file}"
#    exit 2
#  }
#done

echo "error: fix the todo items!"
exit 2
