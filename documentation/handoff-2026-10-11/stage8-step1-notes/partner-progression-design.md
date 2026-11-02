# Partner progression between joins (step 5c, #265)

**The problem (user, 2026-11-02):** a partner who earns bonecharms and unlocks powers, then leaves and rejoins (as the same or the other character), starts again with nothing. A join loads the host's mission fresh with `campaign <mapinfo> <chaos> <character>`, which gives the mission's default loadout.

**Decision (user):** keep progression **per partner, per character**. It travels with the partner to any host, and it is part of 1.0.

## What progression is in the player's inventory

Confirmed runtime: the native player inventory (`--player-inventory`, `native-pickup-research-v785/inventory-client.json`, 816 entries) holds one entry per declaration with a quantity. The declaration type sits at +0x60:

| Type | What | Examples |
|---|---|---|
| 7 | Runes | `components/interactive/rune_pickable` (max 10000) |
| 8 | Outsider rune | `outsider_rune_pickable` |
| 9 | Powers | `models/powers/blink/blink_emily`, `doppelganger`, `possession` |
| 20 | Power upgrades | `blink_assault`, `domino_link_3`, `possession_ext_duration` |
| 18, 21 | Player enhancements | `extended_health`, `improved_sprint`, `bonecharm_slots_1`, `bonecharm_slots_2` |
| equipment-upgrade vtable 0x1b555a8 | Weapon and gear upgrades | `crossbow_low_frequency_howl`, `sword_whale_bone_blade` |
| 16 | Gear, blueprints | `blueprint`, `boots_silentrunning` |
| 26 | Whalebone fragments | `whalebone_fragment` (max 999) |
| 4 | Ammunition | `bolt_regular`, `bullet`, `sticky_grenade` |
| 14 | Elixirs | `health_potion`, `mana_potion` |
| 5 | Bonecharm templates | the charms themselves live in `arkCpntBoneCharmInventory` |

Mission items, notes, keys, maps and audiographs (types 11, 12, 13, 17, 24, 25, 27) belong to the mission and are not carried.

**Restore path, confirmed by earlier lab work:** `coop_grant_named` (observer_camera.h) adds an inventory entry by declaration name through the native add +0x618de0. Powers, attributes and equipment upgrades granted this way take effect (ch. 104, 113).

## How the game carries progression (research 2026-11-02)

The research was read-only: d2-ghidra, plus a capstone disassembly of the `work/asset-game` copy (the capstone package has been installed since 2026-10-27). Labels as given. The apply function was re-checked in d2-ghidra.

- **A mission change is a save followed by a reload; there is no separate "persistent player" blob** (confirmed static). The map transition state (vtable 0x141b6a408, update 0x1404f1dd0) calls 0x140466660. Then:
  1. the save manager request 0x14065d080 ("[SAVE] Requesting save and level transition…") is made;
  2. a checkpoint is written to memory (0x14065ec30, then the per-map writer 0x14065ef30, header `ARKS` version 0x36);
  3. "Loading next map" runs `RestartFromMemoryCheckpoint` (handler 0x14065db00, then the slot loader 0x140659190(mgr,2,-1));
  4. the entity serializer 0x14087a270 runs each component's serialize slot (vtable +0xb8), which marks the component restored (+0x1c bit 0x40000000);
  5. each component's Start applies what was restored.
  
  A fresh `campaign` load has nothing restored, so Inventory Start (0x140617930) applies the default loadout and the mission loadout (0x140613ff0).
- **arkCpntInventory** (vtable 0x141b9a878, confirmed static):
  - **Serialize:** 0x1406185b0 (capture 0x140618460, write 0x1406189b0).
  - **State `savegameState_t`, 0x18 bytes:**
    - +0 money (inventory+0x16c);
    - +4 the int at +0x170;
    - +8 an idList of 0x20-byte records: +0 declaration name hash (0x1401431a0), +4 quantity, +8 maximum, +0xc/+0x10 entry+0x2c/+0x30, +0x14 entry+0x34, +0x15 entry+0x28, +0x16 equipped, +0x1c entry+0x35.
  - **Apply:** 0x1406182f0(inventory, state), re-checked in d2-ghidra. It sets money and +0x170, then for each existing entry with a matching hash it sets the maximum and quantity (with the native change notification 0x1406170b0), the flags, and equips flagged items (0x614b20). It creates no entries, but every declaration is already an entry (816, quantity 0 when not held).
- **Coins** (confirmed static) are the int at inventory+0x16c, not an item. AddMoney 0x140615620 also counts the "coins collected" statistic, so a restore sets money through the apply instead.
- **arkCpntBoneCharmInventory** (vtable 0x141b99928):
  - **Serialize:** 0x140610ff0, which restores straight into three lists:
    - +0x38: every charm (stride 0x40, flag byte +0x38, bit 1 owned);
    - +0x48: crafted charms (stride 0x58, a trait idList at +0x40);
    - +0x58: equipped slots (stride 0x38).
  - **Start (0x1406108e0):** re-applies the equipped slots (0x14060e310) when restored, else applies the default loadout (0x14060e8b0).
  - **Native helpers:**
    - give / own: 0x14060e4c0;
    - equip into the first free slot: 0x14060f520;
    - unequip: 0x140611350;
    - unequip and disown: 0x140610a50.
  - **Crafted charms** have no native adder outside the deserializer (0x140610c70 is reached only from there), so they need the serializer path.

## Restore design (from the research)

- **Inventory and coins:** at a save point, build a `savegameState_t` from the partner's own inventory, keeping only the carried types (the table above) and money. Store it per character. On restore, after the join's first gameplay frame, call the native apply 0x1406182f0 with it. It sets money, quantities and maximums, and equips what was equipped, through the game's own notifications. Missions' own items are left alone.
- **Bonecharms:** first 1.0 pass: owned found charms through 0x14060e4c0 and equipped ones through 0x14060f520, after unequipping the mission default. Crafted charms are a second pass through the component serializer, with an idFile shim of our own (Read, Write, Tell, Seek; version 0x36).

## First implementation: candidate progress-v2096 (`partner_progress.h`)

- **Where it runs:** partner only, in a session, in gameplay, on the engine thread.
- **Capture:** after this load's restore, every 3 s. It writes the carried entries (name, quantity, maximum and the entry's +0x28/+0x2c/+0x30/+0x34/+0x35 fields) and money to `coop-progress-<character>.txt` beside the co-op saves. The file is written only when its text changed.
- **Restore:** 2 s into each load's gameplay. Each saved value is raised back through the native apply 0x1406182f0 (records built with the ×31 name hash), and a value she holds is never lowered.
  - Powers, upgrades, enhancements and equipment upgrades come back after every load.
  - Runes, coins, ammunition, elixirs, gear and whalebone come back only after a join's campaign load (`partnerProgressJoinPending`, set where join-follow sends the campaign command). A checkpoint rewind must not give back what the world offers again.
- **Status file:** `.partner-progress.json`. Lab switch: `coop_partner_progress on|off`.
- **Bonecharms are not in this candidate** (next).
- **Takes:** `Run-Progress-v2096.sh`. Twin `progress-eotw-v2096b` (switch off, must lose both), then `progress-eotw-v2096a`. The VM partner is granted extended_health and tranquilizer bolts; her game is closed, relaunched and joins from the main menu; `Check-Progress-v2096.py` compares her entries.

| Take | Candidate | Result |
|---|---|---|
| `progress-eotw-v2097b` (twin), `v2097a` | progress-v2097 | **No verdict.** Both stopped at the harness's partner-loss check after step 8, before the relaunch: one docks NPC's AI pause count rose 0 to 1 when her game was killed. The twin did the same, so it is not this code (inventory row M3) |
| `progress-eotw-v2098b` (twin), `v2098a` | progress-v2097 | **No verdict again.** On harness v2098, which names the NPC and fails only if the count stays raised for 60 s, b stopped there: docks worker 770189828 (`worker_male_small_01`) kept the extra pause 60 s. a stopped the same way. Before the kill her capture had run 44 times and written 4 times (226 carried entries, no failures). Twin caveat: the switch defaults to on, so the twin captured 16 times and wrote twice in the 20 s before the route turned it off, and her relaunched game starts on again. The twin therefore tests that items earned while off are not kept. A product switch (`d2coop.cfg` `partner_progress=0`, read at launch) is needed anyway, and it gives a twin that is off from the start. `Run-Progress-v2098.sh` deletes both games' `coop-progress-*.txt` before and after each take. They are lab state, and the first-mission takes on this candidate leave them, because the feature is on by default |
| `progress-eotw-v2101b` (twin), `v2101a` | progress-v2102 | **No verdict, harness bug.** The pause was noted as intended, but the take then stopped at "The host did not move ( m)": v2098's naming loop used `$h` and overwrote the host snapshot that the move check reads. Harness v2104 renames it |
| `progress-eotw-v2104b` (twin), `v2104a` | progress-v2103 | **First full runs** (harness v2105, `-PauseCheckNote`). **extended_health is carried:** a fresh 0, granted 1, rejoined 1; the twin rejoined 0. **The bolt row is not a valid test:** `coop_grant_named` set the tranquilizer bolts to 1 instead of adding one (fresh 3, then 1, both takes; confirmed runtime), so "earned above fresh" could not hold. a rejoined with 3 bolts against her saved 1, because the restore only raised values and the mission's default loadout gave 3: leaving and rejoining refilled ammunition. The docks worker's pause was noted again; its cause is the keep-awake release (inventory row M3) |
| `progress-eotw-v2107b` (twin) | progress-v2105 | **RED as required:** extended_health 0 → 1 → 0, bolts 3 → 1 → 3 |
| `progress-eotw-v2107a` | progress-v2105 | **Progression PASS:** extended_health 0 → 1 → 1, bolts 3 → 1 → 1 (exact join restore). The partner-loss check counted the docks worker as the one LOD pause (`keepAwakeReleased` 1). The take failed only on the end diff: Edge of the World's `env_bottle_04_19`, absent on the host (#260, below) |
| `progress-eotw-v2107c` | progress-v2105 | **Progression PASS** (the same values), and the #267 twin: with keep-awake off from the start, the docks worker was already paused (1) before the kill and stayed 1, with no keep-awake release. Confirmed runtime, so #267 is the native LOD pause |

**#260, the Edge of the World bottle:** in v2104b and v2107a the host's world had no `env_bottle_04_19` (not a duplicate name; the digest dropped nothing), and the host's breakable sync had sent nothing (`vanishedSent` 0). So the bottle broke as the host's world loaded, before its first catalog scan, and the 2078 vanish path never saw it. Candidate progress-v2107 adds the host's breakable presence (D2BV, `breakable_protocol.h`): after each complete scan the host sends the name hashes of every breakable in its catalog, and the partner breaks its own unbroken breakables that the host's world lacks (known there 10 s before the list; more than 16 at once means the worlds differ and none are broken). `Run-V2109.sh` runs three progression takes on it after the first-mission rounds.

**Exact join restores, candidate progress-v2105 (`partner_progress.h`):** on a join's load the class 2 entries (ammunition, elixirs, runes, gear, whalebone) and money are set to the saved values, as a native mission change carries them. Powers, upgrades and enhancements are still only raised. `Check-Progress-v2107.py` asks for each granted entry to differ from its fresh value before the close and to be equal after the rejoin.

## Plan

1. **Save** on the partner's PC, per character (`coop-progress-corvo.json`, `coop-progress-emily.json` beside the isolated co-op saves): at each autosave, on leave and at mission end.
2. **Restore** after a join's first gameplay frame: grant each carried entry's missing quantity natively. Bonecharms and coins go by whatever native path the research finds.
3. **Lab switch** `coop_partner_progress on|off`. The mutation twin (off) must show the reset.
4. **Proof:** two-account takes. The partner earns progression, leaves and rejoins as the same character (it is all back), then switches character (the other character's own progression, not this one's).
