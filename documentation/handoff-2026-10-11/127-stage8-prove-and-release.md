# 127. Stage 8: prove it and release 1.0 (step 6, #244)

Step 6 proves the host-decides design on two Steam accounts (this PC hosts, the lab VM joins with the second account) and then publishes 1.0. This chapter records the proofs, the fixes they forced, and what is still open. Every take named here is under `work/natural-moveset/` unless a path says otherwise.

## The proofs

| Brief item | Result | Evidence |
|---|---|---|
| A one-hour session on two accounts with zero differences | **Pass** on progress-v2211: 73 minutes, Emily host and Corvo partner, Dunwall, Edge of the World and The Good Doctor, then three boat walk-and-wait cycles; every diff equal (286, 2,158 and 530 values) | `soak-hour-vm-v2211h` |
| The whole campaign in one pairing | Pass, Corvo host and Emily partner, across candidates: Dunwall, Edge of the World, The Good Doctor, Dust District and the Grand Palace (v2180); A Crack in the Slab and Death to the Empress (v2183); The Clockwork Mansion and The Royal Conservatory (v2194, v2197, v2204) | `campaign-<mission>-vm-*` |
| The other three pairings through the first, a middle and the last mission | Pass: Corvo / Corvo all three; Emily / Emily (Dunwall `v2192e`, the Conservatory, the Empress); Emily / Corvo (Dunwall `v2192c`, the Conservatory `v2194q` and `v2197q`, the Empress) | `campaign-*-vm-*` |
| Save and load with the partner present | Pass: host load `hostload-v2185a` and `c`; pause-menu save and load `coop-hostsave-v2198a`, all eight checks | `work/release/coop-hostsave-v2198a` |
| An update from 0.9.6 and a fresh install from GitHub on the clean VM | **Pass:** the update passes 5 of 5 cases and the install 8 of 8. On the clean VM, `Install.cmd` alone installs the GitHub download, then two accounts play Dunwall: join, walk, a story trigger, a shared death and restart. The uninstall rolls back byte for byte | `work/release/release-update-v2227a`, `release-install-v2227b`, `product-two-account-v2235a` |

The release regression repeated the hour session on the release candidate: `soak-hour-vm-v2227h` passed in 78 minutes, with every diff equal.

### The four pairings again, on progress-v2240 (Run-V2241P, Run-V2243E)
Each pairing (host / partner) ran the first mission, a middle one and the last on the same candidate, with every diff the routes require:

| Pairing | Dunwall (286 values) | The Conservatory (520–522) | Death to the Empress (249) |
|---|---|---|---|
| a Corvo / Emily | `campaign-dunwall-vm-v2241a` equal | `campaign-curator-vm-v2241a` equal | `campaign-empress-vm-v2241a` equal |
| b Emily / Corvo | `v2241b` equal | `v2241b` equal | `v2243b` equal |
| c Corvo / Corvo | `v2241c` equal | `v2241c` equal | `v2241c` equal |
| d Emily / Emily | `v2241d` equal | `v2241d` equal | `v2243d` equal |

- `campaign-empress-vm-v2241b` and `v2241d` stopped on the boat, before any diff. The host's skiff use had focus on "Go to Dunwall Streets" and the presses arrived (loader log), but no level change started in either world. Both worlds were identical, on the boat. The reruns `v2243b` and `v2243d` on the same candidate passed, so this was the route's press and not a difference (hypothesis: the press landed while the native use was not yet accepting input).
- No take stopped in the co-op saves switch (#263): 0 of 16 VM takes on v2240, counting the door takes below.
- progress-v2240 differs from the 1.0.0 proxy (progress-v2227) in the linked-door sync (v2228, `coop_door_linked`, on by default), the probe digest and the #263 watchdog. A 1.0.x built from it needs the install, update and clean-VM proofs again.

## Release 1.0.0 (published 2026-11-10)

- **Package:** `d2coop-v1.0.0.zip`, built from progress-v2227 with setup tool v2209 and decl-recipes-v3. It is 37,474,983 bytes, SHA-256 `D74DCBEFDCAACCE7B16E89EDE38A8A2381467ED0FAEDA2AC65760FCB18089CA1`.
- **Published:** https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v1.0.0. It went up as a prerelease first, at the user's choice, and was marked latest after the clean-VM proof.
- **Local proofs:** `release-install-v2227b` passes 8 of 8 cases and `release-update-v2227a` 5 of 5.
  - `release-install-v2227a` had failed `built_banks_load`. Since 2114 the bank test expected a walk from rest to start at phase 0. The test now checks the start phase on its own.
- **Clean VM from the GitHub download:** `product-two-account-v2235a` passes. Its test, `Test-ProductTwoAccount-v2235`, answers the partner's main-menu Character dialog. `product-two-account-v2227a` (the local zip) waited at that dialog under the 0.9.3-era test.
- **Final playtest with a friend:** a Needs-you step.

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

#### The last difference was the digest (progress-v2239)
After v2227 the end diff still read both checkpoint keys absent in the host's world and alive in the partner's. That happened in `corpse-possess-eotw-v2227t`, `v2231x`, `v2232y`, `v2233z2` and `v2238f`. Three readings of the host's world settled it:
- **v2236:** the whole entity list before and after the end diff;
- **v2238:** the out-of-process catalog with spawners (`--npc-authority-all`).

**What the host's world holds (confirmed runtime).** Each key has its active reinforcement plus a pooled spare. The spare has health 1, `ownerFlags` 129 and is dormant, the same kind of NPC that `outpost_reinf_01` keeps in both worlds.

**Why the key went missing (confirmed static).** The digest wrote the unique NPC and then the spare's pack count under the same key. Its `put()` treats a key written twice as a clash and drops it.

**The fix (probe only; the shipped proxy is unchanged).** The digest now counts every NPC of a key and writes the key once:
- a dormant NPC counts only when the key has nothing else;
- one active NPC reads as its state;
- several read as `[alive,unconscious,dead]`.

**Results:**
- `corpse-possess-eotw-v2239t` on progress-v2239: every diff is equal.
- `corpse-possess-eotw-v2239r` on candidate `release-v2239r` (the 1.0.0 package's proxy, progress-v2227, with the v2239 probe): every diff is equal, with both keys `alive` in both worlds at the end and final diffs.

### A door the host's world opens (#279)
`corpse-possess-eotw-v2217t` ended with the glass door `env_interiors_door_glass_350cm_01_2` open in the host's world only.
- **What that diff was (confirmed runtime).** The door was closed in both worlds at the spawn diff and at all six tries of the killed diff. The end diff was one snapshot with no retry, taken while a host NPC was going through the door: host `[2,2,0,0]`, partner `[0,0,0,0]`. That fits a transition still on its way to the partner's world.
- **Recovery (confirmed static, `door_sync.h`).** A stopped or re-armed session starts each host channel from the host's current native state and sends it, so a re-arm reconciles. A single channel that fails (`failDoorSync`) stays off until the next map or checkpoint. 101 door-sync status files in the takes hold no such failure: 1,067 converged channels, and only session-level stops that re-arm.
- **The release behaviour.** `doorlinked-eotw-v2228t` (`coop_door_linked off`, as in 1.0.0): the host's native open and close converge in the partner's world.
- **The proof and its twin (progress-v2240, Run-V2242D):**
  - `doorsync-eotw-v2242s`: the host's world opens and closes the door natively; the spawn, opened and closed diffs are equal (2,278 values).
  - `doorsync-eotw-v2242t`: the same route with the product maintenance and the door sync turned off in both games first. The opened diff differs in exactly one value, that door (host `[2,2,0,0]`, partner `[0,0,0,0]`). The closed and final diffs are equal. So the door sync is what keeps that door the same.

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
- **Correction:** the lab switch runs in gameplay with the partner connected (the harness requires host gameplay, opens the lobby and joins the VM first). A product host who presses F7 mid-mission takes the same path (`session_product.h`: `requestCoopSaves` and `startOnlineHost`). So this is not lab-only until shown otherwise.
- **Measured over all VM takes:** 18 of 672 stopped this way, every one on the host, every one after `SaveProfile` with `startedAgoMs` under 300. The status file is rewritten about every 300 ms, so the hang is at or before the 300 ms step that locks and loads the profile.
- **progress-v2240 adds a watchdog** (`coop_saves_watchdog.h`). Step breadcrumbs mark each native call of the switch. When the service stops for 5 s mid-switch, the watchdog writes `<status>.coop-saves-stall.json` with the last step and every thread's stack (module and RVA). The four-pairing runner `Run-V2241P` keeps it with a stalled take.
- **Stress, progress-v2244** (lab only): `coop_saves stress on` makes every switch call `SaveProfile`, dirty or not. `Run-V2244S` ran three takes (`coopsaves-stress-vm-v2244a`-`c`, Dunwall, VM partner joined), each with 40 off/on cycles in gameplay. Each take made 41 switches and 41 profile saves (0 skips, 0 failures), and its end diff is equal. The first take came right after the deploy, like the five stalls of 2026-11-09. No stall in 123 forced saves (confirmed runtime), so repeating `SaveProfile` in a running session does not reproduce #263. Hypothesis: the stall needs a state that only the first switch of a fresh game process has (for example a profile write still pending from the boot).

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
- **#237, Hypatia's cure by the partner:** the route reaches Grim Alexandra (`institute-cure-partner-vm-v2215d`, the whole antidote chain on the host), but the choke misses a moving, fighting target.
- **#278:** a corpse walked under the partner's control.
- **#263:** the stall's cause. The v2240 watchdog has not caught one yet (0 of 16 takes), and 123 forced saves on v2244 did not stall.
