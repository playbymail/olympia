# Olympia

## Sources

| Version | Repo / Tarball                          |
|---------|-----------------------------------------|
| G1      | https://www.pbm.com/oly/olympia.tgz     |
| G2      | https://www.pbm.com/oly/olympia.tgz     |
| G3      | https://github.com/olympiag3/olympiag3  |
| TAG     | https://www.pbm.com/oly/olympia-tag.tgz |

## Modernizing

A repeatable, reusable “legacy C modernization playbook” for codebases.

### Phase 0 — Baseline inventory (no edits)

Deliverables:

* “How do I build it today?” notes (even if the answer is “it doesn’t build”)
* Identify entrypoints (`main`, drivers), generated files, data files, and any platform assumptions
* Decide: **clang** target first, **gcc** second (or vice versa)

### Phase 1 — Make it compile with *minimal* code change

Goals:

* Add a **single build path** (Make *or* CMake first). Don’t do both at once.
* Compile with conservative flags first, then tighten:

  * Start: `-O0 -g`
  * Then add: `-Wall -Wextra -Wpedantic`
  * Then add sanitizers on a separate build profile: ASan/UBSan

Rule: **no formatting, no directory reshuffle** until you can produce a binary.

### Phase 2 — 64-bit correctness pass (your “32-bit pointer list” suspicion)

This is a classic failure mode: code that stores pointers in `int` / `long` / assumes 32-bit widths.
Typical fixes:

* Replace “pointer-as-int” with `intptr_t` / `uintptr_t`
* Audit any serialization/hashing of pointers
* Replace homegrown list macros that do arithmetic on pointer-sized values
* Replace `int` indexes that should be `size_t`
* Add compile-time checks where appropriate

This is also where sanitizers + `-Werror` can save you time.

### Phase 3 — Only now: formatting + directory reorg

Do formatting *after* the build is stable, otherwise you destroy blame/history while still unsure what works.

* Add `.clang-format`, run it directory-by-directory, commit each step.
* Directory restructure: do it as a pure move/rename commit (no code changes mixed in).

### Phase 4 — CI + regression hooks

Even if you don’t have tests:

* Add a “smoke run” that executes a minimal command and checks exit code / expected output files.
* Add CI jobs for clang and gcc on Linux.

## The “Legacy C Modernization Playbook” you can reuse everywhere

This is the reusable process:

1. **Freeze behavior**

   * Baseline build attempt
   * Capture how to run a minimal scenario

2. **Create a build harness**

   * Make or CMake (pick one first)
   * Add debug + sanitize configs

3. **Triage portability**

   * 64-bit, endianness, UB, strict aliasing, signed overflow

4. **Tighten compiler discipline gradually**

   * Warnings → fix → ratchet

5. **Refactor last**

   * Once safe builds + smoke tests exist

6. **Cosmetic changes last-last**

   * Formatting
   * Directory reorg

