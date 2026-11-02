# 123. Stage 8: mid-mission join and host quickload (area 4.4, #240)

Area 4.4 makes a partner who joins mid-mission, or rejoins from a reloaded game, end up in the host's world. It also makes a host quickload a decision both worlds follow. The research map is [join-quickload-map.md](stage8-step1-notes/join-quickload-map.md). Labels: confirmed static, confirmed runtime, hypothesis.

## Measurement first (2055)

The host plays Dunwall alone: the bedroom, Mayhew, then Ramsey killed and looted (route `campaign-dunwall-vm-v2041`, steps 0 and 7 to 12). The VM partner then joins from its main menu (`-JoinAfterStep 7`). World diffs run at the join and 60 s later, and are recorded, not required. The candidate is jindoshwheels-v2053, unchanged.

| Take | Candidate | Result |
|---|---|---|
| `join-measure-dunwall-v2055a` | jindoshwheels-v2053 | **Different, as expected:** 18 differences at the join, 60 s later and at the end (545 compared). The join itself worked: the partner loaded in 93 s, was placed 1.9 m from the host, and gameflow was equal |

**Findings (confirmed runtime, v2055a):**
- **The host's log expired during the partner's load.** The partner let 77 Kiscule entries expire. The throne room actors the host's script had removed (the Duke, Delilah, their carriers and guards, the clockwork soldiers) stayed alive in the partner's world, and so did Alexi, dead on the host.
- **The host ran the partner's mission start.** Right at the join, the host applied 69 of the partner's partner-first nodes, for example `ActionGameflowMission` 1538, `ActionDoor` 3881, and the `ld_kiscule_coup` and `mdnt_audience_kiscule` scenes. The partner ran them while loading its own mission start, and the host had no count for them. That breaks the host-decides rule, even though no difference came of it here.
- Expected per-character differences, which the diff marks: `ld_spawner_corvo_audience`, `ld_imperial_ring_pickup_corvo` and `ld_dun_audiograph_01_corvo`.

## The join snapshot (candidate to come)

- **NPC life roster** (`npc_authority_stream.h`):
  - The host clears its `sent` marks whenever the partner's session or world epoch changes, so it sends its whole roster to each new partner world. `lifeSnapshots` counts these resends.
  - The partner retries a state it could not apply every 5 s, up to 40 tries; before, it gave up after 5.
- **Kiscule log** (`kiscule_action_mirror.h`): while the partner is loading, the host's log entries wait, and their 60 s window starts in gameplay. `joinHeld` counts the entries held this way.
- **Lab switch:** `coop_join_snapshot on|off`. The twin sends it to both games before the join.
- **Candidate joinquiet-v2057** adds: a partner sends a first run of a node to the host only after its world has had 10 s of plain gameplay. The clock is set in the Kiscule service each frame. `joinQuiet` counts the runs kept local this way.

## Join snapshot takes

| Take | Candidate | Result |
|---|---|---|
| `join-snapshot-dunwall-v2056a` | joinsnapshot-v2055 | **Fail, 14 differences** (535 compared). Alexi now matches. The throne room actors are gone from the partner's world, but the host still counts each as one alive NPC detached from its spawner (`[1, 0, 0]`). The partner held 12 of the host's entries while loading, then let 51 expire while its own mission-start scene played. The host applied 110 of the partner's partner-first nodes |
| `join-snapshot-dunwall-v2056b` | joinsnapshot-v2055 | Harness fail before the diffs: the VM was placed 3.05 m from the host. Not a twin |

**Candidate joinhold-v2058:** joinquiet-v2057, plus the partner holds the host's log until its world first reaches plain gameplay, not only while it loads.

| Take | Candidate | Result |
|---|---|---|
| `join-snapshot-dunwall-v2058a` | joinhold-v2058 | Route flake before the join: the loot of Ramsey was not registered |
| `join-snapshot-dunwall-v2058b` | joinhold-v2058 | **Mutation twin, red as required** (15 differences). With `coop_join_snapshot off`, Alexi stays alive for the partner, and the host runs 110 partner-first nodes. The throne room actors differ as in v2056a, so that difference does not come from the snapshot |
| `join-snapshot-dunwall-v2058c` | joinhold-v2058 | **Fail:** the partner never reached gameplay after the join. Its own mission-start scene waited on host counts that the hold kept back (67 pending). The host also ran 49 of the partner's host-story asks from that scene |

**Confirmed runtime (v2058b):** the coup's `ActionDestroy` 643901705 (`ld_kiscule_coup`) runs 8 times in the partner's world from the host's log, which removes the throne room actors there. The host ran it 8 times too, yet still has one alive, detached actor per spawner.

**Candidate joinasks-v2060:** v2058's hold is reverted, so the partner holds only while loading (as in v2057). A partner's host-story asks now wait for the same 10 s of plain gameplay as partner-first runs. Harness v2059 adds a `probe` step. Route `join-probe-dunwall-v2059` runs `--npc-authority-all` on the host before the join and on both games at the end.

**Open question:** in the host's world, are the throne room actors that stay alive and detached after the coup hidden? A two-player start from the tower would show whether a partner that runs the coup itself also ends at `[1, 0, 0]`.

## Answered asks (2060 to 2062)

| Take | Candidate | Result |
|---|---|---|
| `join-probe-dunwall-v2060a` | joinasks-v2060 | **Fail:** the partner never reached gameplay. Its mission-start scene needs the host's answers to its host-story asks |
| `join-probe-dunwall-v2060b` | joinasks-v2060 | **Mutation twin, red as required** (15 differences) |
| `join-probe-dunwall-v2061a` | joinanswer-v2061 | **4 differences, down from 18** (535 compared). The host answered 155 of the joiner's asks without running them and ran 2 nodes. Left: Delilah and `ramsey_audience` alive for the partner, a door (`ld_door_350cm_int_rich_wood_01`) locked only on the host, and Alexi's scroll case. The partner let 42 of the host's entries expire |
| `join-probe-dunwall-v2061b` | joinanswer-v2061 | **Mutation twin, red as required** (15 differences, the host ran 111 of the partner's nodes) |

**Confirmed runtime (v2060a and v2060b, `--npc-authority-all` on the host before the join):** the host has no throne room actors before the join. The `[1, 0, 0]` actors of v2056a to v2058b were spawned in the host's world by the partner's mission-start nodes, which the host ran for it.

**Candidates:**
- **joinanswer-v2061:** an ask the partner sends before it has settled carries a joining bit (`kKisculeJoining`). The host answers it with the count and runs nothing (`joinAnswered`); asks are not gated.
- **joinmap-v2062:** the partner also holds the host's log while its map does not yet match the host's and it has not settled. Hypothesis: the 42 entries expired during the partner's mission-start scene, before its map name was filled.

| Take | Candidate | Result |
|---|---|---|
| `join-probe-dunwall-v2062a` | joinmap-v2062 | **1 difference** (536 compared): `ld_spawner_clockw_right` is alive for the partner and absent on the host. The host answered 168 asks. After the partner settled, it ran 49 more: 46 of them ambient fire damage from `esc_env_kiscule_fire_01_1`, plus a candlestick. 36 host entries still expired in the partner's world |

**Hypothesis:** the clockwork soldier is an ordering race in the coup. The partner's world lost a host `ActionDestroy` to expiry but kept the matching `ActionSpawner` (v2061a lost Delilah instead). Native removal (`0x8784b0`) only hides an actor, so it would not take it out of the catalog. **Candidate joinexpiry-v2063** therefore logs each expired entry as an event (`expired_samemap` or `expired_othermap`) in a 1024-entry ring, to find out why entries wait 60 s.
| `join-probe-dunwall-v2063a` | joinexpiry-v2063 | **Pass:** world diff equal at the join, 60 s later and at the end (536 compared, only per-character differences). The host answered the joiner's asks. Only 1 host entry expired in the partner's world (an `inventory` node on the same map), against 36 in v2062a, so the expiry varies from run to run. The twin v2063b and a repeat v2063c follow |
| `join-probe-dunwall-v2063b` | joinexpiry-v2063 | **Mutation twin, red as required** (15 differences). With `coop_join_snapshot off`, Alexi stays alive for the partner and the host spawns the throne room actors |
| `join-probe-dunwall-v2063c` | joinexpiry-v2063 | **Pass, repeat:** equal at the join, 60 s later and at the end (536 compared). 19 host entries expired on the same map, mostly `ActionGameflowTask`, `ActionPatrol` and `ActionUsable`, without changing the world |

**The join is done on joinexpiry-v2063:** two passes (v2063a, v2063c) and a red twin (v2063b). Open observation: host entries still expire in the partner's world on the same map, after it has settled. This changed nothing here, and the step 6 full runs watch for it.

## Host load (2064)

Today, with checkpoint sync on, any load is intercepted (`checkpointIntercept`) and becomes a shared restart from the last paired autosave, which the host starts. A partner's load is a request to the host. Take `hostload-dunwall-v2064a`: both players play Dunwall from the start, the host kills and loots Ramsey, then the host loads its last checkpoint (`RestartFromCheckpoint`, the pause menu's load). A diff is required once both have reloaded. The twin v2064b turns `coop_checkpoint_sync off` on the host before its load.
