# First mission inventory (step 5b, #264)

*A Long Day in Dunwall* (`campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo`) was the main test map through development, and in real co-op play it broke far more often than other missions (user, 2026-11-02). This note collects every known problem, then the measured ones, one row each. Fixes follow host-decides first, each with a mutation twin.

## 1. Known from earlier work

| # | Problem | Source | State |
|---|---|---|---|
| K1 | The route's loot of Ramsey's body did not register | `hoststory-lead-dunwall-v2017d` to `v2023a`, `continue-decision-vm-v2047g` (ch. 122) | Fixed for the partner's ring in hoststory-v2024 (ch. 119: ActionSpecialLoot runs in both worlds). Recheck in the measure takes |
| K2 | The end-of-mission talk to Meagan did not register | `route-dunwall-lead-v1753e`, `v1755a`, `hoststory-lead-dunwall-v2024a`, `v2026b` | Passed from `hoststory-lead-dunwall-v2027a` on. Recheck |
| K3 | The tower safe-room doors (`env_interiors_safe_door`, `_6`, `_9`) open on the host and stay closed on the partner | ch. 119 (still open after hoststory-v2027) | Fixed in progress-v2100 (`v2100a`, `v2103a`, `c`, `d`, `e`; the twin `v2103b` shows the doors again) |
| K4 | The partner's game had to answer the native choice screen in the first mission | ch. 126 | Fixed in nativechoice-v2088 (the co-op Character box asks; the native screen is answered) |
| K5 | Co-op saves switch stalls in `profile_saving` | #263 | Open. Again in `firstmission-dunwall-v2099e` (Emily host, before the mission) and `firstmission-dunwall-v2112a` (Corvo host). In v2112a the host status was fresh (service 16 ms ago), phase 2 had started only 266 ms earlier, and there was no failure. So the switch did not stall: it began at the very end of the 180 s wait (confirmed runtime), and what held it before is unknown. Harness v2110 traces the host's saves result, phase and game logic during the wait. It did not recur in the eight `v2114` takes: co-op saves were already on at the trace's first poll (2.3 to 2.5 s) |
| K6 | The partner's presentation is not live in the scripted opening | ch. 95 (`1829a`) | Known; recheck what the partner sees at the start |
| K7 | Ramsey and Alexi alive in a partner's fresh world after a mid-mission join | ch. 106 | Fixed by the join snapshot (#240, ch. 123). Recheck |
| K8 | The user's and partner's own list | asked 2026-11-02 | **Not coming** (user, 2026-11-02: the mission was generally broken, and writing it up would take a day). Replaced by the own sweep in section 4 |

## 2. Measured (Run-FirstMission-v2095.sh)

The full first-mission route (casts, deaths, the bedroom, Mayhew, Ramsey and his loot, the ring locker, the safe room, the balcony, the streets and Meagan) runs on two Steam accounts in all four pairings, on candidate progress-v2097. A world diff is recorded after each stage: start, deaths, mayhew, ramsey-loot, ring-locker, saferoom, balcony, streets, end.

| Take | Pairing (host / partner) | Result |
|---|---|---|
| `firstmission-dunwall-v2095a` | Corvo / Emily | **Failed at the end:** the host in the Void, the partner on the streets (M1). Diffs equal through the ring locker; saferoom digest error (M2); balcony and streets 3 differences each (K3) |
| `firstmission-dunwall-v2095b` | Emily / Corvo | **Same as a**, value for value |
| `firstmission-dunwall-v2095c` | Corvo / Corvo | **Failed: talk to Meagan not registered** (K2 again). Diffs equal through the ring locker; saferoom, balcony and streets 3 differences each (K3) |
| `firstmission-dunwall-v2095d` | Emily / Emily | **Lost in setup** ("The credential is invalid"): a side script of mine polled the VM over PowerShell Direct during the take. Repeated as `firstmission-dunwall-v2099e` |
| `firstmission-dunwall-v2099e` | Emily / Emily | **Failed at the start: co-op saves did not switch on.** The host stayed in `profile_saving` (K5, #263), after the harness's retry too |
| `firstmission-dunwall-v2099f` | Corvo / Emily | **Same as a** (M1 and K3, everything else equal). The follow status explains M1, below |

Rechecks of section 1 (confirmed runtime):
- **K1:** the ramsey-loot diff is equal in a, b and c (552 values), and the route's loot step registered.
- **K2:** in a the scene had already run when the route arrived (`skipped: npc gone`); in c (both Corvo) the talk did not register. It is open again.
- **K3:** the same three doors (`env_interiors_safe_door`, `_6`, `_9`; host `[1, 1]`, partner `[0, 1]`) in all three takes, from the safe room to the streets. Every other value is equal.

New rows:

| # | Problem | Evidence | State |
|---|---|---|---|
| M1 | **The partner does not follow the host into the Void.** The host's Continue is sent (D2MD) and the partner runs it, but its `RestartFromMemoryCheckpoint` reloads its own last streets checkpoint (load 2.8 s; its gameflow is back at `Task_DW_TalkToBoatCaptain`). Its both-ready hold then times out after 90 s (`timeout_other_map`), and no mission-follow load is logged in the VM's log. | a and b (confirmed runtime). The earlier passes (`hoststory-lead-dunwall-v2029a/b`) were lab takes in which the partner's own world led the transition | **Partly a harness artifact (confirmed runtime, v2099f):** mission follow was off in both games for the whole take (`enabled: false`). The harness turns it on only when the VM joins from its menu, and this route starts both games on the mission, while the product turns it on (`productMode`). The defect underneath stays: the partner's Continue reloads her own streets checkpoint, and only follow can take her to the host's mission, after the 90 s hold times out. The v2100 first-mission routes turn `coop_follow_host` on in both games at the start, as the product does |
| M2 | The saferoom world diff failed on the host: probe exit 45 (usable catalog unreadable) | a and b; c's saferoom diff ran | Hypothesis: the host was in a load or scene at that moment. Harness-side; recheck in v2099e/f |
| M3 | Partner-loss check at Edge of the World: one docks NPC's AI pause count rose 0 to 1 after the partner's game was killed | `progress-eotw-v2097a/b` (the twin too, so not the progression code) | v2098b: docks worker 770189828 (`worker_male_small_01`) kept it for 60 s. **Our leak (confirmed runtime):** control `m3control-eotw-v2101a`, the same route with no kill, keeps the worker at 0 at the kill's moment, +15 s and +75 s. Issue [#267](https://github.com/adamkozel/Dishonored-2-coop/issues/267). **Cause, v2104a:** the host's keep-awake (`npc_authority_stream.h`, #241) held the worker, far from the host and near the partner, at LOD level 6. The host status snapshots around the kill show `keepAwakeReleased` 0 → 1 as the channels stopped. The release hands the NPC back to the engine, whose LOD change handler (0x14053e9e0 → 0x1407e7560) pauses the AI component through `D2_Component_Pause` once the level reaches the component's threshold (confirmed static), the same counter the check reads. So the count is the native LOD pause of a far NPC, as in single player, not a leaked reference. Harness v2107 counts as many raised NPCs as keep-awake releases as LOD pauses (`notes.kill.lodPaused`) and fails on any more. `progress-eotw-v2107c`, with keep-awake off from the start, must show the worker paused before the kill and unchanged at it |

K3's cause (confirmed static): usable sync (`usable_sync.h`) mirrors the host's two-state toggles, including state changes made by a script. But `nativeUsableToggleFamily` (`usable_catalog.h`) drops every name containing `safe`, a rule from chapter 106, when safes stayed per world. Chapter 106 also found the plain Dunwall safe doors to be ordinary Open/Close toggles. Vault doors are disabled until dialled, so the family's `enabled` rule already leaves them out at catalog time. Whether the three tower doors pass the family's other rules is the next runtime check.

**K3 fix, candidate progress-v2100:** names containing `safe` join the family while `usableSafeDoorsShared` is on. That is the default; the lab switch is `coop_usable_safe_doors on|off`, read at the next map load, and the product switch is `d2coop.cfg` `safe_doors=0`. A disabled vault door stays out. Unit tests: an enabled Open/Close safe door joins, it stays out with the switch off, and a disabled one stays out.
- `firstmission-dunwall-v2100a`: Corvo / Emily with the switch on. The saferoom, balcony and streets diffs must be equal.
- `firstmission-dunwall-v2100b`, the twin: the switch is off in both games and must show the three doors again.
- Queued in `Run-V2100.sh`. Both v2100 routes also turn `coop_follow_host` on in both games, as the product does.

| Take | Result |
|---|---|
| `firstmission-dunwall-v2100a` (switch on) | **K3 fixed:** the balcony and streets diffs are equal (1152 values each; 3 differences before). The saferoom digest failed again on the host (M2). **With follow on, the partner reached the Void.** Her Continue reloaded her streets checkpoint, the host's hold at the Void timed out (90 s), and then follow loaded the Void for her. The end diff has 1 difference: Void gameflow node `bf0f0d6f` (host 3, partner 0), because she arrived about 2 minutes after the host |
| `firstmission-dunwall-v2100b` (twin) | **Lost to K5:** co-op saves did not switch on (#263). Repeated as v2103b |

**Why M1 takes 90 s (confirmed static):** the host holds at its new map's load end until the partner reports the same map (`waitMissionTransition`, `mission_end_mirror.h`), while it publishes its mission for follow only in gameplay. Each waits for the other until the 90 s timeout.

**M1 fix, candidate progress-v2103 (`mission_decision.h`):** with follow on, the partner's Continue waits on her result screen for the map the host's hold announces (D2MR, re-sent every second), then loads that mission at once (`campaign <mapinfo> <chaos> <her character>`). A follow load already works this way, and partner progression restores her items after it. With no announcement in 60 s she falls back to her own Continue. Lab switch `coop_continue_follow on|off`.
- `Run-V2103.sh`: a Corvo / Emily with both fixes on; b, the twin, with safe doors and continue-follow off; then c, d and e for the other pairings.

| Take | Pairing (host / partner) | Result |
|---|---|---|
| `firstmission-dunwall-v2103a` | Corvo / Emily | **M1 and K3 fixed.** The partner's Continue loaded the host's Void at once (`continued_to_host_mission`, `followContinues` 1, no fallback); the host's hold released after 6.9 s instead of timing out at 90 s. Balcony and streets equal. One end difference: M4 |
| `firstmission-dunwall-v2103b` | Corvo / Emily, twin | **Both faults back:** the three safe-room doors differ at the balcony and streets (K3), and the host's hold timed out at 90 s (`timeout_other_map`) with the partner's own Continue (M1). End: M4 |
| `firstmission-dunwall-v2103c` | Emily / Corvo | Same as a (hold released after 6.0 s) |
| `firstmission-dunwall-v2103d` | Corvo / Corvo | Same as a (4.2 s) |
| `firstmission-dunwall-v2103e` | Emily / Emily | Same as a (4.3 s). Co-op saves switched on this time (K5 did not recur) |

In all five the saferoom digest failed on the host (M2) and the Meagan step was skipped because her scene had already run (`npc gone`), so K2 was not exercised.

| # | Problem | Evidence | State |
|---|---|---|---|
| M4 | **The ended mission stays 0 in the partner's world.** At the Void, gameflow node `bf0f0d6f` (the first mission, `Mission_DW`) is 3 on the host and 0 on the partner | v2100a and v2103a to e, all pairings, the twin too | Cause confirmed runtime and static: the partner reaches the next mission through `campaign <mapinfo>` (follow or her Continue), which leaves the ended mission at 0, while the host's world ran its end. The gameflow mirror sent only active missions (state 2) and the partner skipped every mission node (`skippedMission` 1392 in v2103a) |
| M2 | (cause) | v2103a to e | Hypothesis, harness-side: the route's saferoom diff comes right after the ring locker use, which starts the load into the streets (the step's snapshot shows both on the streets map, the host held at its load end), so the host's probe reads the usable catalog mid-load (exit 45). The balcony diff 15 s later passes in every take |

**M4 fix, candidate progress-v2104 (`mission_end_mirror.h`):** the host's gameflow mirror also sends completed missions (state 3). A partner whose node is 0 gets 3 written into the node (+0xc) without the native set-state routine, so no mission notice shows and no script reacts to a mission of another map. The status adds `gameflowMirror.missionCompleted`. **M2:** harness `Route-MissionVM-v2106.ps1` retries a failed world diff once after 10 s and records the first error as `retried`. `Run-V2106.sh` runs a (Corvo / Emily) and b (Emily / Corvo) on both; the end and saferoom diffs must be equal.

| Take | Pairing (host / partner) | Result |
|---|---|---|
| `firstmission-dunwall-v2106a` | Corvo / Emily | **Passed. M4 fixed:** the end diff is equal (286 keys on both sides). The partner continued to the host's Void (hold released after 4.3 s). Every diff equal except the saferoom one (M2) |
| `firstmission-dunwall-v2106b` | Emily / Corvo | **Passed**, the same as a (4.2 s) |

**M2 is not load timing:** the retry 10 s later failed the same way in both takes. The host frame at that step shows it in gameplay in the safe room. The check's 16384 entity cap is the engine registry's own size (`game+0x3130b0`), so the cause is one of the probe's other reads. Candidate progress-v2106 changes only the probe: it writes the reason for exit 45 (`no_game`, `game_vtable`, `entity_list`, `entity_count`, `entity_read`) and the values read to stderr, which the take log keeps. `Run-V2108.sh` runs the done-when check on it: all four pairings twice in a row (takes a to d, then e to h).

**Done-when rounds on progress-v2106 (`Run-V2108.sh`, harness v2107):**

| Take | Pairing (host / partner) | Result |
|---|---|---|
| `firstmission-dunwall-v2108a` | Corvo / Emily | Reached the Void (`continued_to_host_mission`). Every diff equal (start 538 to end 286 values) except the saferoom one (M2) |
| `firstmission-dunwall-v2108b` | Emily / Corvo | Same as a |
| `firstmission-dunwall-v2108c` | Corvo / Corvo | **Lost in setup:** the host's game played Emily after `campaign ... corvo` (the previous take's character). Harness-side, not a co-op fault; harness v2108 loads both missions again (up to 2 tries) |
| `firstmission-dunwall-v2108d` | Emily / Emily | Same as a |
| `firstmission-dunwall-v2108e` | Corvo / Emily | Same as a |
| `firstmission-dunwall-v2108f` | Emily / Corvo | **Every diff equal, the saferoom one too** (1148 values) |
| `firstmission-dunwall-v2108g` | Corvo / Corvo | Same as a |
| `firstmission-dunwall-v2108h` | Emily / Emily | Same as a |

Seven of eight reached the next mission with no difference anywhere they were measured: no co-op fault in the scripted route on progress-v2106 (confirmed runtime).

**M2's reason (confirmed runtime, the v2106 probe):** in every failing take the host's digest reads `entity_list` (the game object is there, its entity list empty, count 0), and the retry 10 s later reads `no_game` (no game object). That is the host tearing down one map and building the next. In f the digest got through, with 1148 values, the size of the later balcony and streets digests (1152) and not of the tower's (552). So the saferoom diff lands in the load into the streets map, and the earlier "not load timing" note was wrong: one 10 s retry was too short (hypothesis for the length, to confirm). Harness v2108 retries the digest every 10 s for up to 60 s and records the tries. `Run-V2112.sh` repeats both rounds on it, on progress-v2109 if the wait-notice take `v2111a` passes, else on progress-v2108.

**Wait-notice take on progress-v2109 (`firstmission-dunwall-v2111a`, Corvo / Emily): failed, two new rows.**

| # | Problem | Evidence | State |
|---|---|---|---|
| M5 | The partner's game faulted at the end. Her wait notice was open on the result screen when her continue-follow load (`campaign .../void_01.mapinfo low emily`) came (Dishonored2.exe+0x1f7dd28) | v2111a VM engine log, the VM desktop in the step 35 frame (confirmed runtime) | **Fixed by removal:** progress-v2110 never raises the notice (partner-quality-plan) |
| M6 | **Breakable presence broke the partner's streets speakers.** The streets diff showed `speaker_01_a0_brk#gen` on the host and none on the partner. Those names end in a number made at spawn, different in each world (host `_5276`/`_5414`, partner `_5462`/`_5502` in v2108a). So the host's list never has the partner's names, and presence (progress-v2107 on) broke every one of them | v2111a streets digests against v2108a and f (confirmed runtime) | **Fixed in progress-v2110:** presence leaves names ending in `_<4+ digits>` alone, the rule the world diff uses (#gen). Unit tests added. The twin is v2111a itself |

Every other diff of v2111a was equal (start to balcony; saferoom M2 as before). `Run-V2112.sh` (progress-v2108, which still has M6) was cut to its take a, and its folders b to h were left empty so the runner skips them. `Run-V2113.sh` now runs the forced bottle test on progress-v2110, and `Run-V2114.sh` runs both done-when rounds on it.

**Done-when rounds on progress-v2110 (`Run-V2114.sh`, harness v2110):**

| Take | Pairing (host / partner) | Result |
|---|---|---|
| `firstmission-dunwall-v2114a` | Corvo / Emily | Reached the Void (`continued_to_host_mission`). Every diff equal (start 538 to end 286 values, the streets 1152) except the saferoom one, unread after 7 tries. Co-op saves were already on at the first poll (K5 did not recur) |
| `firstmission-dunwall-v2114b` | Emily / Corvo | Same as a. The saferoom retries were refused because the first try had already written its digest file (a harness fault) |
| `firstmission-dunwall-v2114c` | Corvo / Corvo | Same as a |
| `firstmission-dunwall-v2114d` | Emily / Emily | Same as a |
| `firstmission-dunwall-v2114e` | Corvo / Emily | **Lost at step 11 (die):** a lab input packet expired before it was published (`Publish-D2Packet`). Harness-side |
| `firstmission-dunwall-v2114f` | Emily / Corvo | **Lost at the Ramsey loot:** guards fought the host over the body, and the ring stayed about 3 m away (failure frame), so no press landed. Diffs to mayhew equal. Harness-side (route) |
| `firstmission-dunwall-v2114g` | Corvo / Corvo | Lost like e, after step 19 |
| `firstmission-dunwall-v2114h` | Emily / Emily | Same as a |

No co-op difference anywhere a diff read, and in all five finished takes the partner followed into the Void (confirmed runtime). The round did not meet the done-when, for three reasons, all fixed:
- **M2 timing (confirmed runtime):** the ring-locker use starts the host's load into the streets, and that load takes about 75 s. In a, the host's logic went 15 to 1 at ticks 121548078 and 121623250. The 60 s of retries end before it. Harness v2111 first settles both worlds in gameplay (up to 150 s, noted as `diffwait-<tag>`), and gives each retry its own digest tag.
- **Expired input packets:** `Send-D2Input.ps1` set the expiry to the press length, so a host stall of that length threw. It now rebuilds an expired, unpublished press with a new sequence, up to 3 tries.
- **The loot:** the loot step makes a second pass 8 s later.

`Run-V2116.sh` runs both rounds again on harness v2111 and progress-v2111, which is progress-v2110 plus the lab-only break command.

## 3. Done when

The whole first mission plays through cleanly in all four pairings on two accounts, every diff equal, twice in a row on one candidate, and every sweep row in section 4 has passed or is written down as known.

## 4. Own sweep (replaces K8)

The user's report is "generally broken", so the scripted route alone is not enough: it starts both games on the mission in lab mode and has the host do the story. Real play differs in four ways, and each gets its own takes on the passing candidate, in all four pairings:

| # | What real play does | Take shape | State |
|---|---|---|---|
| S1 | The partner joins from the co-op menu (product mode) mid-mission, not at the start | host plays to the bedroom, the VM joins from its menu, then the full route; diffs at every stage | Queued: `Run-V2119.sh` (route `sweep-s1-dunwall-v2119`). The host plays alone through Ramsey's loot, and the VM joins after step 9. There is a diff at the join and a required one 30 s later, then the rest to the Void. Four pairings, progress-v2113 |
| S2 | The partner does the story beats (kills, Ramsey's loot, the ring, the safe room, Meagan) | the route with the partner as actor; diffs at every stage | Queued: `Run-V2121.sh` (route `sweep-s2-dunwall-v2121`, harness v2112). The partner enters every trigger volume, kills Ramsey and loots him. `partnerloot` passes when the host's gameflow completes the node. The host still uses the ring lockers and talks to Meagan. Four pairings, progress-v2113 |
| S3 | Deaths and loads mid-mission (quickload, checkpoint reload, a death of either player) | the route with a load after mayhew and a partner death after the safe room; diffs after each rewind | Queued: `Run-V2120.sh` (route `sweep-s3-dunwall-v2120`). The full route plus a host death after Mayhew and a partner death on the streets, each followed by a diff after both reloads. Four pairings, progress-v2113 |
| S4 | What both players see | a recorded clip of both screens at each stage, reviewed for presence, duplicate or missing characters and stuck animations (visual acceptance, recorded separately) | To do |

Each failure becomes an M row with its fix and mutation twin, as above.
