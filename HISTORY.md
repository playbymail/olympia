# Olympia Engine Fork History

## Fork Tree

```
G1 (Original)
├── TAG (The Age of Gods)
└── G2 (Enhanced)
    └── G3 (Windows Port)
```

## Version Summary

| Version | Parent | Focus                                                |
|---------|--------|------------------------------------------------------|
| G1      | —      | Original Olympia PBM engine                          |
| TAG     | G1     | Religion, gods, enhanced combat, strategic expansion |
| G2      | G1     | Underground Subworld, relics, simplified economy     |
| G3      | G2     | Windows port with Visual Studio and C# GUI           |

---

## G1 (Original)

The baseline Olympia PBM game engine.

- `MAX_BOXES`: 102,400
- Entity types: 14 (`T_MAX = 14`)
- Subkinds: 68 (`SUB_MAX = 69`)
- Skills: IDs 120-170 range
- Rich trade goods economy (~40 items)

---

## G1 → G2 Differences

| Feature         | G1          | G2                                                   |
|-----------------|-------------|------------------------------------------------------|
| **MAX_BOXES**   | 102,400     | 150,000                                              |
| **Underground** | None        | Subworld with tunnels, sewers, chambers              |
| **Relics**      | None        | `sub_relic` (400-series artifacts) with quest system |
| **Trade goods** | ~40 items   | Simplified, most flavor goods removed                |
| **Skills**      | IDs 120-170 | Renumbered to 600+ with finer granularity            |
| **New files**   | —           | `tunnel.c`, `rnd.c`                                  |

### G2 New Subkinds (69-73)

```c
#define sub_relic      69  /* 400 series artifacts */
#define sub_tunnel     70
#define sub_sewer      71
#define sub_chamber    72
#define sub_tradegood  73
```

### G2 New Systems

- `create_subworld()` - generates underground Subworld map
- `create_tunnels()` - sewers under cities
- Relic spawning and quest goals
- Tradegood expiry (`trade.expire` field)

---

## G1 → TAG Differences

| Feature              | G1          | TAG                                                    |
|----------------------|-------------|--------------------------------------------------------|
| **Entity types**     | 14          | 15 (+`T_nation`)                                       |
| **Subkinds**         | 68          | 87 (+religion, guilds, artifacts, etc.)                |
| **Combat**           | Simple      | Oly3: tactics, terrain bonuses, troop control          |
| **Unit types**       | Basic       | +postulant, fanatic, ninja, angel, cavalier, war wagon |
| **Monster trophies** | None        | 20+ (dragon_scale, balrog_horn, cyclops_eye, etc.)     |
| **Skills**           | IDs 120-170 | IDs 1000+ (restructured)                               |
| **New files**        | —           | 11 additional source files                             |

### TAG New Subkinds (69-87)

```c
#define sub_religion            69
#define sub_holy_symbol         70
#define sub_mist                71
#define sub_book                72
#define sub_guild               73
#define sub_trade_good          74
#define sub_city_notdone        75
#define sub_ship                76
#define sub_ship_notdone        77
#define sub_mine_shaft          78
#define sub_mine_shaft_notdone  79
#define sub_orc_stronghold      80
#define sub_orc_stronghold_notdone 81
#define sub_special_staff       82
#define sub_lost_soul           83
#define sub_undead              84
#define sub_pen_crown           85
#define sub_animal_part         86
#define sub_magic_artifact      87
```

### TAG New Source Files

- `hero.c` - Personal adventurer skills
- `artifacts.c` - Artifact system
- `trading.c` - Enhanced economy
- `mining.c` - Mine shafts
- `shipcraft.c` - Ship building
- `ranger.c` - Ranger skills
- `effect.c` - Effect system
- `win.c` - Victory conditions
- `times.c`, `map.c`, `kill.c`

### TAG Combat Enhancements (oly3.h)

```c
#define ATTACK_LIMIT 4
#define DEFAULT_CONTROLLED 10
#define GARRISON_CONTROLLED 20
#define DEFENDER_CONTROL_BONUS 20
#define TACTICS_FACTOR 0.02
#define TACTICS_LIMIT 2.0
#define CITY_DEFENSE_BONUS 1.25
#define FOREST_DEFENSE_BONUS 1.50
#define MOUNTAIN_DEFENSE_BONUS 2.00
#define SWAMP_DEFENSE_BONUS 0.75
```

---

## G2 → G3 Differences

| Category                | G2       | G3                                                      |
|-------------------------|----------|---------------------------------------------------------|
| **Directory structure** | `src/`   | `olympia/`, `olylib/`, `include/`, `olygui/`, `mapgen/` |
| **Build system**        | Makefile | Visual Studio (`.vcxproj`, `.sln`)                      |
| **Platform**            | Unix     | Cross-platform with Windows focus                       |
| **GUI**                 | None     | C# Windows GUI (`olygui/olygui/`)                       |
| **Code style**          | K&R C    | ANSI C with `#ifdef _WIN32` guards                      |

### G3 Directory Layout

```
g3/
├── olympia/      # Main game engine
├── olylib/       # Shared library
├── include/      # Headers (with libc shims)
├── olygui/       # C# Windows GUI
│   ├── mapgui/   # Map GUI
│   └── olygui/   # Game settings, orders, mail
├── mapgen/       # Map generation
├── g2rep/        # Report generator
├── entab/        # Utilities
└── sendmail/     # Mail utilities
```

### G3 Windows GUI Features

- `GameSettings.cs` - Game configuration
- `MailSettings.cs` - Email setup
- `RepOrders.cs` - Report and orders handling
- `SubmitOrders.cs` - Order submission
- POP3 email integration

### G3 Skill Renumbering

~20 skills reordered (likely for data file alignment):

- `sk_hide_self`: 641 → 638
- Alchemy skills: shuffled within 691-696
- Resurrection skills: `sk_resurrect` and `sk_last_rites` swapped
- Gate skills: reordered
- Artifact skills: reordered

Core game logic remains essentially unchanged from G2.

---

## Key Conflicts Between Forks

### Subkind ID 55

- G1/G2: `sub_undead` (undead lord)
- TAG: `sub_demon_lord` (renamed), `sub_undead` moved to 84

### Subkind IDs 69-73

| ID | G2            | TAG             |
|----|---------------|-----------------|
| 69 | sub_relic     | sub_religion    |
| 70 | sub_tunnel    | sub_holy_symbol |
| 71 | sub_sewer     | sub_mist        |
| 72 | sub_chamber   | sub_book        |
| 73 | sub_tradegood | sub_guild       |

### Skill ID Ranges

- G1/TAG: 120-170 (legacy)
- G2/G3: 600+ (renumbered)

---

## Recommendations

For merging TAG and G2 features, see [OLYTAG.md](OLYTAG.md).

The recommended approach:

1. Use TAG as base (has Oly3 combat, religion, guilds, artifacts)
2. Port G2's Subworld/tunnels, relics, tradegood expiry
3. Normalize conflicting subkind IDs
4. Regenerate world data (don't reuse old savefiles)
