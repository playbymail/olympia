# OLYTAG: Merging TAG and G2

Sprint plan for merging Olympia TAG (The Age of Gods) and G2, both forked from G1.

## Design Constraints

- **Strategic focus**: Olympia is strategic, not personal adventure
- **No hero skills**: Exclude personal adventurer/hero mechanics from TAG
- **No nation victory**: No winner/loser nation mechanics

## Base Strategy

Use **TAG as the primary codebase**, then port G2's structural changes and features.

**Rationale**: TAG already contains Oly3 combat, religion, guilds, artifacts, and expanded economy. G2's additions (Subworld, relics, tradegood expiry) are largely orthogonal and can be plugged in.

---

## Sprint 1: Strip Unwanted TAG Features

**Goal**: Remove personal hero and nation victory mechanics from TAG

### Tasks

1. **Remove T_nation entity type**
   - Delete `T_nation` define from `oly.h`
   - Set `T_MAX` back to 14
   - Audit and remove all code creating/using `T_nation` boxes

2. **Remove win.c**
   - Delete `tag/src/win.c`
   - Remove from build system
   - Audit callers of any win-condition APIs

3. **Disable hero/adventurer skills**
   - Identify skills in `hero.c` that only affect individual nobles:
     - Survive Fatal Wound
     - Personal Fight to Death
     - Avoid Wounds
     - Extra Attacks
     - Blinding Speed
     - Uncanny Accuracy
   - Comment out from skill tables (keep IDs reserved)
   - Remove `hero.c` from build or stub it out

4. **Audit effect types**
   - Review `ef_*` constants for hero-only effects
   - Disable any that don't apply to stack-level combat

**Deliverable**: TAG builds and runs without hero skills or nation mechanics

---

## Sprint 2: Normalize oly.h Enums

**Goal**: Create unified enum mapping that accommodates both TAG and G2 features

### Subkind Remapping

| ID Range | Assignment |
|----------|------------|
| 1-68 | G1 baseline (unchanged) |
| 69-75 | TAG religion/guild core + G2 relics |
| 76-83 | TAG structural additions |
| 84-89 | Special types, undead/demon |
| 90-92 | G2 underground locations |
| 93+ | Reserved for future |

### Detailed Mapping

```c
/* Religion & Guild (from TAG) */
#define sub_religion       69
#define sub_holy_symbol    70
#define sub_book           71
#define sub_guild          72
#define sub_tradegood      73   /* unified spelling */
#define sub_relic          74   /* from G2 */
#define sub_mist           75

/* TAG structural */
#define sub_city_notdone       76
#define sub_ship               77
#define sub_ship_notdone       78
#define sub_mine_shaft         79
#define sub_mine_shaft_notdone 80
#define sub_orc_stronghold     81
#define sub_orc_stronghold_notdone 82
#define sub_lost_soul          83

/* Special types */
#define sub_undead         84   /* canonical undead lord */
#define sub_demon_lord     85   /* TAG-specific */
#define sub_special_staff  86
#define sub_pen_crown      87
#define sub_animal_part    88
#define sub_magic_artifact 89

/* G2 underground */
#define sub_tunnel         90
#define sub_sewer          91
#define sub_chamber        92

#define SUB_MAX            93
```

### Tasks

1. **Create merged oly.h**
   - Start from TAG's `oly.h`
   - Apply remapping above
   - Set `MAX_BOXES` to 150000 (from G2)
   - Rename `sub_trade_good` to `sub_tradegood`

2. **Update TAG source files**
   - Global search/replace for remapped subkind IDs
   - Files affected: `glob.c`, `code.c`, `build.c`, `dir.c`, `move.c`, `u.c`, `loc.c`, `display.c`, `combat.c`

3. **Prepare G2 files for porting**
   - Update `tunnel.c` references to use new IDs (90-92)
   - Update `quest.c` relic references to use ID 74

**Deliverable**: Unified `oly.h` that compiles with TAG codebase

---

## Sprint 3: Struct and Macro Unification

**Goal**: Merge struct definitions and macros from both forks

### struct trade

Use G2's version (adds `expire` field):

```c
struct trade {
    int kind;
    int item;
    int qty;
    int cost;
    int cloak;
    int have_left;
    int month_prod;
    int expire;    /* countdown timer for tradegoods (from G2) */
    int who;
    int sort;
};
```

### Tasks

1. **Add `expire` field to struct trade**
   - Update TAG's struct definition
   - Initialize `expire` to 0 for permanent goods
   - Update market code to handle expiry

2. **Merge macro sets**
   - Add G2 macros not present in TAG:
     - `player_public_turn`
     - `vision_protect`, `default_garrison`
     - `item_animal`, `item_prominent`, `item_weight`, `item_price`, `item_unique`
   - Ensure underlying structs have required fields

3. **Keep TAG's `char_gone` macro**
   - Use time-based version (more informative)
   - G2's simplified version only if tunnels require it

4. **Preserve TAG structures**
   - `struct char_religion`
   - `struct entity_artifact` with `ART_*` constants
   - `struct effect` with `ef_*` constants
   - `struct trap_struct`
   - `oly3.h` combat constants

**Deliverable**: Unified structs compile without errors

---

## Sprint 4: Port G2 Subworld System

**Goal**: Integrate underground tunnel/sewer system from G2

### Files to Port

- `g2/src/tunnel.c` → `merged/src/tunnel.c`
- `g2/src/rnd.c` → `merged/src/rnd.c` (if not duplicate)

### Features

- `tunnel_region` and `under_region` region wrappers
- `create_subworld()` - generates Subworld map
- `create_tunnels()` - sewers under cities
- `create_tunnel_set()` - multi-level tunnel complexes
- `random_subworld_loc()` - surface-to-Subworld links
- Hades link support

### Tasks

1. **Port tunnel.c**
   - Copy to merged codebase
   - Update `sub_tunnel`, `sub_sewer`, `sub_chamber` to new IDs (90-92)
   - Integrate with TAG's location system

2. **Update worldgen**
   - Call `create_subworld()` during world generation
   - Link tunnels to cities via sewers

3. **Update movement code**
   - Handle tunnel travel in `move.c`
   - Apply appropriate travel times (`dir.c`: tunnel distance = 5)

4. **Update display code**
   - Show tunnel/sewer in location descriptions
   - Handle sewer visibility rules

5. **Update combat code**
   - Apply sewer combat modifier (50 from `combat.c`)

**Deliverable**: Subworld generates and is traversable

---

## Sprint 5: Port G2 Relic/Quest System

**Goal**: Integrate relic artifacts and quest mechanics from G2

### Lore Constants to Add

```c
#define lore_skeleton_npc_token  931
#define lore_orc_npc_token       932
#define lore_undead_npc_token    933
#define lore_savage_npc_token    934
#define lore_barbarian_npc_token 935
#define lore_orb                 936
#define lore_faery_stone         937
#define lore_barbarian_kill      938
#define lore_savage_kill         939
#define lore_undead_kill         940
#define lore_orc_kill            941
#define lore_skeleton_kill       942
```

### Tasks

1. **Add lore constants to oly.h**

2. **Port relic creation code**
   - From `g2/src/quest.c`
   - Use `sub_relic` (ID 74)
   - Spawn relics in tunnels, chambers, remote locations

3. **Integrate with TAG artifact system**
   - Model relics using `struct entity_artifact`
   - Use TAG's `ef_*` effect types for relic bonuses

4. **Port quest goal system**
   - Kill N monsters of type X
   - Retrieve specific relics

5. **Update day.c**
   - Port relic-related daily processing

**Deliverable**: Relics spawn and quests function

---

## Sprint 6: Port G2 Tradegood System

**Goal**: Integrate rotating tradegood mechanics

### Tasks

1. **Implement tradegood expiry**
   - Use `trade.expire` field
   - Decrement each turn
   - Remove expired tradegoods from markets

2. **Generic tradegood generation**
   - Cities generate rotating `sub_tradegood` items
   - Coexists with TAG's fixed item economy

3. **Market code updates**
   - Handle both permanent goods (`expire = 0`) and rotating goods
   - Port G2's market simplifications where beneficial

4. **Add G2 items**
   - `item_hound` (295) - for breeding/beast skills
   - Any other G2-specific items at free IDs

**Deliverable**: Markets support both permanent and rotating goods

---

## Sprint 7: Integration and Testing

**Goal**: Verify merged systems work together

### Tasks

1. **Full rebuild with all warnings as errors**
   - Fix any remaining struct/macro mismatches

2. **Regenerate test world**
   - Do NOT reuse old TAG/G2 savefiles
   - Verify:
     - Subworld and tunnels connect to cities
     - Temples and guilds present and functional
     - Artifacts, relics, and religion interact correctly

3. **Smoke tests**
   - Oly3 combat on mixed terrain
   - Tunnel/Subworld travel
   - Relic quests with religion active
   - Tradegood expiry over multiple turns

4. **Balance review**
   - Ensure relics don't double-dip with artifacts and religion
   - Verify no overpowered combinations

**Deliverable**: Stable merged build ready for playtesting

---

## Item ID Strategy

Keep TAG's item ID table as canonical. TAG's economy (mining, shipcraft, artifacts, religious components, trophies) expects specific resources by ID.

### G2 Simplification Approach

Instead of deleting items:
- Mark "flavor-only" items as unused in scenarios
- Stop generating them from worldgen
- Remove from city production tables

This gives G2's effective simplification without ID churn.

### Adding G2-Only Items

- Assign at free IDs above TAG's highest
- Example: `item_hound 295` (verify no collision)

---

## Skill ID Strategy

Keep TAG/G1 skill numbering as canonical:

```c
#define sk_shipcraft   120
#define sk_combat      121
#define sk_stealth     122
#define sk_beast       123
#define sk_alchemy     126
#define sk_forestry    128
#define sk_mining      129
#define sk_religion    150
#define sk_basic       160
/* ... etc */
```

### G2 Skills

- Do NOT adopt G2's 600+ renumbering
- Map G2 concept-level skills to TAG equivalents where possible
- Add genuinely new G2 skills at IDs 700+

### Excluded Skills

TAG hero/adventurer skills to disable (keep IDs reserved, remove from skill tables):
- Survive Fatal Wound
- Defense (personal)
- Swordplay (personal)
- Avoid Wounds
- Personal Fight to Death
- Extra Attacks
- Blinding Speed
- Uncanny Accuracy

---

## Risks and Mitigations

| Risk | Mitigation |
|------|------------|
| Silent ID mismatches | Full grep audit after enum changes |
| Incomplete nation/win removal | Search for all `T_nation`, `win.c` references |
| Skill table inconsistencies | Verify skill tables match oly.h |
| Balance drift (relics + artifacts + religion) | Playtesting; may need effect caps |

---

## Files Summary

### From TAG (keep)
- `oly3.h` - combat constants
- `artifacts.c` - artifact system
- `trading.c` - economy
- `mining.c` - mine shafts
- `shipcraft.c` - ship building
- `ranger.c` - ranger skills
- `effect.c` - effect system
- Religion/guild mechanics throughout

### From TAG (remove/disable)
- `win.c` - victory conditions
- `hero.c` - personal adventure skills

### From G2 (port)
- `tunnel.c` - Subworld/tunnels
- `rnd.c` - random utilities
- Quest/relic code from `quest.c`
- Tradegood expiry from market code

### New (create)
- Merged `oly.h` with unified enums
- Updated build configuration
