# AGENTS.md - Olympia PBM Game Engine

## Build Commands
```bash
cmake -B build && cmake --build build    # Build with CMake (requires 4.2+)
```
No test framework is present. Legacy Makefiles exist in g1/, g2/ subdirectories.

## Architecture
This is a legacy C codebase for the **Olympia** play-by-mail game engine with four versions:
- `g1/` - Olympia G1 (original)
- `g2/` - Olympia G2 (enhanced)
- `g3/` - Olympia G3 (with Visual Studio projects)
- `tag/` - Olympia: The Age of Gods variant

Core game logic is in `*/src/` with main entry in `main.c`. Key headers: `oly.h` (defines, types, game constants), `z.h` (utility functions).

## Code Style
- **C Standard**: Depends on the engine (C90 (`-std=c90`) - legacy K&R style code requires relaxed warnings for g1, etc.)
- **Compiler flags**: `-DNEW_TRADE`, platform-specific defines (`-DOLYMPIA_CC_LINUX`, etc.)
- **Naming**: snake_case for functions/variables, UPPER_CASE for macros/constants, `sub_*`/`item_*`/`sk_*` prefixes for enum-like defines
- **Types**: Custom `uchar`/`schar` typedefs; game entities identified by integer box IDs
- **Macros**: Heavy use of accessor macros like `rp_char()`, `rp_loc()` for data access
- **No formatting tools** configured yet; see MODERNIZING ING for modernization roadmap
