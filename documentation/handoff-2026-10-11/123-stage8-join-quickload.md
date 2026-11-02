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

**Open question:** in the host's world, are the throne room actors that stay alive and detached after the coup hidden? A two-player start from the tower would show whether a partner that runs the coup itself also ends at `[1, 0, 0]`.
