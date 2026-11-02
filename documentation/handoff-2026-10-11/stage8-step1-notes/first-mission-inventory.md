# First mission inventory (step 5b, #264)

*A Long Day in Dunwall* (`campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo`) was the main test map through development, and in real co-op play it broke far more often than other missions (user, 2026-11-02). This note collects every known problem, then the measured ones, one row each. Fixes follow host-decides first, each with a mutation twin.

## 1. Known from earlier work

| # | Problem | Source | State |
|---|---|---|---|
| K1 | The route's loot of Ramsey's body did not register | `hoststory-lead-dunwall-v2017d` to `v2023a`, `continue-decision-vm-v2047g` (ch. 122) | Fixed for the partner's ring in hoststory-v2024 (ch. 119: ActionSpecialLoot runs in both worlds). Recheck in the measure takes |
| K2 | The end-of-mission talk to Meagan did not register | `route-dunwall-lead-v1753e`, `v1755a`, `hoststory-lead-dunwall-v2024a`, `v2026b` | Passed from `hoststory-lead-dunwall-v2027a` on. Recheck |
| K3 | The tower safe-room doors (`env_interiors_safe_door`, `_6`, `_9`) open on the host and stay closed on the partner | ch. 119 (still open after hoststory-v2027) | Open |
| K4 | The partner's game had to answer the native choice screen in the first mission | ch. 126 | Fixed in nativechoice-v2088 (the co-op Character box asks; the native screen is answered) |
| K5 | Co-op saves switch stalls in `profile_saving` | #263 | Open. Again in `firstmission-dunwall-v2099e` (Emily host, before the mission) |
| K6 | The partner's presentation is not live in the scripted opening | ch. 95 (`1829a`) | Known; recheck what the partner sees at the start |
| K7 | Ramsey and Alexi alive in a partner's fresh world after a mid-mission join | ch. 106 | Fixed by the join snapshot (#240, ch. 123). Recheck |
| K8 | The user's and partner's own list | asked 2026-11-02 | Not received yet |

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
| M3 | Partner-loss check at Edge of the World: one docks NPC's AI pause count rose 0 to 1 after the partner's game was killed | `progress-eotw-v2097a/b` (the twin too, so not the progression code) | v2098b: docks worker 770189828 (`worker_male_small_01`) kept it for 60 s. **Our leak (confirmed runtime):** control `m3control-eotw-v2101a`, the same route with no kill, keeps the worker at 0 at the kill's moment, +15 s and +75 s. Issue [#267](https://github.com/adamkozel/Dishonored-2-coop/issues/267) |

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

## 3. Done when

The whole first mission plays through cleanly in all four pairings on two accounts, every diff equal, twice in a row on one candidate, and the user's own list is resolved or written down as known.
