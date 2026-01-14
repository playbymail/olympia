#!/bin/bash

OLYMPIA_ROOT=/Users/wraith/Software/mdhender/golympia
cd "${OLYMPIA_ROOT}" || {
  echo "error: unable to set def to OLYMPIA_ROOT"
  exit 2
}

# build targets for this test run
cmake --build build --target mapgen-g1 || {
  echo "error: build failed"
  exit 2
}
OLYMPIA_BIN="${OLYMPIA_ROOT}/build"

# set def to the run folder
G1_PATH=run/g1/mapgen
cd "${G1_PATH}" || {
  echo "error: unable to set def to G1_PATH"
  exit 2
}

# set seed values for random number generator
export G1_SEED_1=18481
export G1_SEED_2=28078
export G1_SEED_3=26982

# run the program
# inputs: Map, Land, Cities
# outputs: gate, loc, road
"${OLYMPIA_BIN}/mapgen-g1" || {
  echo "error: mapgen failed"
  exit 2
}

# compare outputs and launch visual diff if needed
cd "${OLYMPIA_ROOT}" || {
  echo "error: unable to set def to OLYMPIA_ROOT"
  exit 2
}
for file in gate loc road; do
  echo " test: comparing '${file}' to 'golden/${file}'"
  diff "tests/g1/mapgen/golden/${file}" "run/g1/mapgen/${file}" || {
    echo " diff: file '${file}' does not match golden file"
    clion diff "tests/g1/mapgen/golden/${file}" "run/g1/mapgen/${file}"
    exit 2
  }
done
