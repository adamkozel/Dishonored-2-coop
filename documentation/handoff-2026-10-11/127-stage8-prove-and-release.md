# 127. Stage 8: prove it and release 1.0 (step 6, #244)

Step 6 proves the host-decides design on two Steam accounts (this PC hosts, the lab VM joins with the second account) and then publishes 1.0. This chapter records the proofs, the fixes they forced, and what is still open. Every take named here is under `work/natural-moveset/` unless a path says otherwise.

## The proofs

| Brief item | Result | Evidence |
|---|---|---|
| A one-hour session on two accounts with zero differences | **Pass** on progress-v2211: 73 minutes, Emily host and Corvo partner, Dunwall, Edge of the World and The Good Doctor, then three boat walk-and-wait cycles; every diff equal (286, 2,158 and 530 values) | `soak-hour-vm-v2211h` |
| The whole campaign in one pairing | Pass, Corvo host and Emily partner, across candidates: Dunwall, Edge of the World, The Good Doctor, Dust District and the Grand Palace (v2180); A Crack in the Slab and Death to the Empress (v2183); The Clockwork Mansion and The Royal Conservatory (v2194, v2197, v2204) | `campaign-<mission>-vm-*` |
| The other three pairings through the first, a middle and the last mission | Pass: Corvo / Corvo all three; Emily / Emily (Dunwall `v2192e`, the Conservatory, the Empress); Emily / Corvo (Dunwall `v2192c`, the Conservatory `v2194q` and `v2197q`, the Empress) | `campaign-*-vm-*` |
| Save and load with the partner present | Pass: host load `hostload-v2185a` and `c`; pause-menu save and load `coop-hostsave-v2198a`, all eight checks | `work/release/coop-hostsave-v2198a` |
| An update from 0.9.6 and a fresh install from GitHub on the clean VM | Run after the release build (below) | `work/release/release-update-*`, `product-two-account-*` |

The release regression repeats the hour session on the release candidate (`soak-hour-vm-v2227h`).

## Fixes step 6 forced

### The story reflow (progress-v2208)
`soak-hour-vm-v2206h` differed after Edge of the World on The Good Doctor's elevator buttons. The host ran node `elevetor_kiscule_node_template_1` a second time. The partner's world applied that run out of flow before its own flow reached it. Then the mirror blocked her flow there, and the buttons it re-enables stayed disabled (confirmed runtime). Now, when her own flow reaches a run the host's message already applied, the run executes again for state-setting World classes (ActionUsable, ActionInteractable, EntityStateControl, ActionNavBlocker, ActionBind). A repeated state set changes nothing. Lab switch `coop_kiscule_reflow on|off`. The v2211 hour session is equal at the same place.

### Story ints (progress-v2211)
`storyflags/global.kiscule songs_listened` differed in three corpse takes, in both directions. It is an int variable (value at +0x1c, vtable 0x1c9f998; confirmed static), and the story mirror D2SF carries bools only. D2SI now sends the host's worldspawn story ints the same way: on a change and every 2 s, written into the partner's variables. The status has an `ints` block, and the protocol tests cover it.

### Reinforcements and other partner-only NPCs (#277; progress-v2209 to v2227)
Edge of the World's checkpoint reinforcements kept ending up in the partner's world only. The steps, each found by a failing take:
- **v2209:** a partner NPC whose key the host never announced leaves her world (the own-only rule).
- **v2210:** a spawn try that reports `spawn_failed` can still spawn later, so tries are 15 s apart and a failed try counts as hers.
- **v2217:** packs, meaning several NPCs of one spawner, were never in the life roster on either side. `world_identity.h` marks them not unique, and the catalog dropped them, so neither the pack counts (v2205) nor the own-only rule ever saw one. Packs now join the roster only; streams stay unique-only. Lab switch `coop_npc_life_packs on|off`; with it off, `corpse-possess-eotw-v2217p` is red as required.
- **v2219 and v2226:** every NPC she spawns for a host present is kept on a per-key list, one capture per spawn, and all of them leave 45 s after the host stops announcing the key.
- **v2220:** her dead or unconscious copies go too, unless the host reported a death or knockout under the key.
- **v2222 was wrong.** Its broadened rule removed level NPCs that the host streams and so never announces, and the present spawn made them again (`corpse-possess-eotw-v2222t`: roof and office guards).
- **v2224** restored the late-spawn test (an NPC first seen 30 s after the roster began and active 45 s), and **v2227** applies it per NPC rather than per key: a reinforcement spawner's pooled NPC is active for a moment at the load.
- `lifeOwnOnlySkip` in the NPC stream status counts why the rule kept an NPC.

### Corpse possession by the partner (#278)
- The lab cast (`coop_lab_possess_corpse`) only set the target. The controller never left travel state 0, because the travel update `0x140825970` waits for the cast animation's timeline at `[[ctrl+0x1d8]+0x90]+0xe0`, and calling `0x140447db0` directly never starts it (confirmed static).
- Routes since `corpse-possess-eotw-v2209` cast natively: aim, then the left trigger with Possession.
- `corpse-possess-eotw-v2214a`: the partner is inside the body (Leave Host Body). The host's world hides her rig, and every diff is equal (confirmed runtime, visual).
- A body walked under her control is not tested: the body did not move in either world.

### Partner movement at reversals (#269, step 5)
- The gait picked its direction clips from each frame's displacement. A reversal or a sharp turn swapped clip pairs in one frame, and both legs jumped 0.2 to 0.8 m in one sample (confirmed static and runtime).
- Frame pacing was not the cause: with the VM's diagnostics off, the frames were even and the drift stayed.
- A jump of the travel direction over 60 degrees in one frame now continues the new clips from the phase nearest the old pose (the walk/run blend the rig plays), and crossfades from the last pose over 0.18 s.
- On progress-v2226 all four pairings pass in both roles (Run-V2225A). The twin with `coop_movement_direction_blend off` fails at 53 cm.

### The co-op saves stall (#263)
- Five takes on 2026-11-09 stopped with "Co-op saves did not switch on", all on the first take after a runner deployed a candidate.
- In each, the host called the native SaveProfile. Its `.coop-menu.json` was written once, 14 checks and 218 ms into phase 2, and never again, and the game later exited with `0x80000003`.
- Neither a 60 s settle nor the VM sync log explains it. The runners now rerun such a take once (`<take>2`).
- The product switches from the main menu and syncs no VM. The clean-VM product take covers that path.

### Lab and harness
- Harness v2213 adds a `choke` step for either player. Harness v2215 adds `focusPress`: a press made while the native use focus is on the target counts. The Institute burner valve sends only script events.
- A PC shutdown corrupted git's index. It was rebuilt from HEAD (`git reset`); `git fsck` was clean.

## Release inputs
- Version resources 1.0.0 (`version_proxy.rc`, `version_setup.rc`).
- `work/release/decl-recipes-v3`: 52 decls (v2's 48, the cosmetic fish's three, and the Deadly Shade puppet).
- `work/release/setup-tool-v2209`: d2coop_setup 1.0.0. Its packages step rebuilds all 9 files byte-identical to the lab's from the original game.
- The original game is read from `Dishonored2 - Copy`: the Steam folder has the mod installed, and its `master.index` no longer matches 0.9.6's pinned original. The builder asserts the hash.
- The notes are composed by `work/release/compose_notes_v1000.py` (`RELEASE-NOTES-1.0.0.md`). Their known limitations are the two players and the game's own memory use.
- `work/release/Run-Release-v2226.sh` builds d2coop-v1.0.0 and runs the install and update tests.

## Still open
- **#279:** a linked door that a host NPC opens stays closed for the partner. Doors with a partner leaf are outside the door catalogue.
- **#237, Hypatia's cure by the partner:** the route reaches Grim Alexandra (`institute-cure-partner-vm-v2215d`, the whole antidote chain on the host), but the choke misses a moving, fighting target.
- **#278:** a corpse walked under the partner's control.
- **#263:** the stall's cause.
