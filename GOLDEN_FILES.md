# Generating Golden Test Files

This document describes how to generate golden test files from the original 32-bit code before refactoring to 64-bit.

## Why 32-bit?

The original Olympia codebase uses `int` to store pointers in the `ilist` data structure. This works on 32-bit systems where `sizeof(int) == sizeof(void*)`, but causes pointer truncation and crashes on 64-bit systems.

To generate reliable golden test files, we need to:
1. Build the **original unmodified code** as 32-bit binaries
2. Run them to generate outputs
3. Use these outputs as the "golden" reference for testing refactored code

## Setup: DigitalOcean Droplet

### 1. Create Droplet

```bash
# Use DigitalOcean web interface or CLI
# - Ubuntu 22.04 or 20.04 LTS x64
# - Basic droplet ($4-6/month)
# - Any datacenter region
```

### 2. SSH and Clone Repo

```bash
ssh root@your-droplet-ip

# Install git if needed
apt-get update
apt-get install -y git

# Clone the repository
git clone https://github.com/mdhender/golympia.git
cd golympia
git checkout g1-build-errors-oly
```

### 3. Build 32-bit Binaries

```bash
# This script installs dependencies and builds everything
bash build32.sh
```

**Or manually:**

```bash
# Install build tools
apt-get install -y gcc-multilib g++-multilib cmake build-essential

# Configure and build
mkdir build32 && cd build32
cmake -DBUILD_32BIT=ON ..
cmake --build . --target mapgen-g1
cmake --build . --target oly-g1

# Verify 32-bit
file mapgen-g1  # Should say "ELF 32-bit"
file oly-g1     # Should say "ELF 32-bit"
```

## Generate Golden Files

### G1 mapgen

```bash
cd ~/golympia

# Update run script to use build32 directory
sed -i 's|build/mapgen-g1|build32/mapgen-g1|' run/g1/mapgen.sh

# Run mapgen
bash run/g1/mapgen.sh

# Check outputs
ls -lh run/g1/mapgen/{gate,loc,road}
```

### G1 oly

```bash
# Update run script to use build32 directory
sed -i 's|build/oly-g1|build32/oly-g1|' run/g1/oly.sh

# Run oly-g1 in immediate mode
bash run/g1/oly.sh

# If successful, package the outputs
cd run/g1/oly
tar czf golden-outputs.tgz lib/
```

## Package and Download

```bash
cd ~/golympia

# Create a tarball with all golden outputs
tar czf golden-files.tar.gz \
    run/g1/mapgen/{gate,loc,road} \
    run/g1/oly/golden-outputs.tgz

# On your Mac, download it
scp root@your-droplet-ip:~/golympia/golden-files.tar.gz .
```

## Extract on Mac

```bash
cd /Users/wraith/Software/mdhender/golympia

# Extract the golden files
tar xzf golden-files.tar.gz

# The mapgen golden files are already in place
ls tests/g1/mapgen/golden/

# Create golden files for oly if needed
mkdir -p tests/g1/oly/golden
cd tests/g1/oly/golden
tar xzf ~/golympia/run/g1/oly/golden-outputs.tgz
```

## Cleanup

```bash
# Destroy the droplet when done (via DigitalOcean web interface)
# Or keep it for future golden file generation
```

## Next Steps

Once you have golden files:

1. **Port mapgen fixes to g1/src/z.[ch]**
   - Change `int *ilist` to `intptr_t *ilist`
   - Fix all `(int)` pointer casts to `(intptr_t)`

2. **Enable strict compiler flags**
   - Add `-Wall -Wextra -Werror` etc.

3. **Test against golden files**
   - Run tests to verify refactored code produces identical output

4. **Iterate**
   - Fix issues
   - Re-test
   - Commit when tests pass
