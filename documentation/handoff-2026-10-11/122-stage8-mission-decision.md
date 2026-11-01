# 122. Stage 8: the host decides at a mission end

Card [#253](https://github.com/adamkozel/Dishonored-2-coop/issues/253), from [the stage 8 brief](115-stage8-plan-brief.md): the host decides the shared world, and that includes what both games do on a mission's result screen.

## What happened before (confirmed runtime)

On the result screen (logic 11), the native *Continue* runs `RestartFromMemoryCheckpoint`. That loads the next mission's start save, which the game cached when Results opened. *Replay Mission* runs `LoadGame <slot>` with this mission's start save: slot type 2, slot 20 + chapter. On A Long Day in Dunwall that is `LoadGame 21`.

In `replay-dunwall-vm-v2044a` each game made its own choice:

- **The host's Replay was consumed:** the checkpoint rules treated `LoadGame 21` as a user reload and turned it into a shared checkpoint request. That request could not start on a result screen, so the host stayed on Results (streets map, logic 11).
- **The partner's Continue passed:** the logic-11 rule lets a mission continuation through, so the partner loaded the next mission (`void_01_p`) alone.

## How it works (`mission_decision.h`, `mission_decision_protocol.h`, D2MD)

A world's mission end is *shared* when this world sent or applied the mission-end message (D2ME, [chapter 97](97-campaign-map-sequence.md)) in its current world epoch. On a shared result screen:

- **Host:** its Continue and Replay always run. The first choice in the epoch is sent as D2MD: `decision` 1 (Continue) or 2 (Replay), the slot, the map name in that slot's `game.details`, the session and the end epoch.
  - The message is reliable. It is re-sent every second for two minutes, from the services and from the both-ready hold; the partner counts the copies as duplicates.
  - A host Replay is no longer consumed as a reload, with or without a partner.
- **Partner:** while connected, its own Continue and Replay are consumed until the host's choice arrives (`partnerHeld`). Then it dispatches the same native command in its own world:
  - Continue: `RestartFromMemoryCheckpoint`;
  - Replay: `LoadGame` of its own mission-start save of the host's map. That is the host's slot number when that slot holds a type-2 save of the same map, else the newest such save in the partner's save root.
  - With no such save, the partner continues instead (`replay_save_missing`) without the both-ready hold, and mission follow then loads the host's mission.
  - If the host is lost while the partner holds a choice, the partner's own choice runs (`released_local`).
- **Both** expect the transition, so the both-ready hold ([chapter 97](97-campaign-map-sequence.md)) meets them at the new map's load end.

**The result screen** is logic 11, and also a message box over it (logic 3, Replay Mission's confirmation) once the end is shared. In `replay-decision-vm-v2047a` the host's `LoadGame 21` ran while that box was still open; v2044a had seen logic 11. A box during gameplay, such as a pause-menu load, is not the result screen, so the checkpoint rules keep it.

**The Replay's own continuation:** `LoadGame` does not load by itself. It caches the slot and queues `RestartFromMemoryCheckpoint`. On the host in `replay-decision-vm-v2047d`, kind 2 with slot 21 was followed by kind 1 with cached slot 21, which was accepted (confirmed runtime). The partner's dispatched Replay therefore also permits that continuation in the same epoch. In v2047d it did not, and her continuation was consumed as one of her own presses.

**The switch:** the host's setting holds for both players. A product partner follows control hello flag 16; a lab partner keeps its own switch. To turn it off, set `mission_decision=0` in `d2coop.cfg` (`D2COOP_MISSION_DECISION=0`), or run the lab command `coop_mission_decision off` in the host's game. Each world then chooses on its own again, as in 0.9.6.

**Status:** `<status>.missiondecision.json`, with the last decision (choice, slot, start map), `sent`/`resent`, `received`/`duplicates`/`refused`, `partnerHeld`, `continued`/`replayed`, `missingSaves` and `localReleases`.

**Unit tests** (`d2_shadow_action_tests`):
- the D2MD validator;
- the press rule for each role (`missionDecisionRule`);
- the result-screen rule (`missionResultsScreen`);
- the Replay continuation permit (`missionDecisionPermits`).

## Takes (two Steam accounts, host Corvo on this PC, VM partner Emily; co-op saves, host story and items per player on)

The harness is `Route-MissionVM-v2047.ps1` on `routes/campaign-dunwall-vm-v2041.json`. With `-ReplayAtResults` the host presses X (Replay Mission) on its first result screen and answers Yes. `-ExpectReplayMap` requires both worlds to end on that map in gameplay, with the host's Replay sent and the partner's replay done. `-DecisionOff` is the mutation twin.

| Take | Candidate | Result |
|---|---|---|
| `replay-decision-vm-v2047a` | v2047b | **Fail.** The host's `LoadGame 21` ran at logic 3 (the confirmation box), so no decision was made, and the checkpoint rules consumed it. Both games stayed on Results; the partner held her Continue (`waiting_for_host`) |
| `replay-decision-vm-v2047b` | v2047b | **Mutation twin, red as required.** With the switch off on the host (both games show `enabled` false), the host's Replay was consumed at logic 3 and the partner's Continue loaded `void_01_p` alone: v2044a's failure |
| `continue-decision-vm-v2047c` | v2047b | **Pass.** The host's Continue was sent (1 send, 79 re-sends). The partner's own A press was held and her dispatched Continue followed the host. Both worlds are on `void_01_p` in gameplay; equal gameflow; world diff equal (286) |
| `replay-decision-vm-v2047d` | v2047c | **Fail.** The host replayed to the tower (Replay sent, slot 21, start map `dunwall_escape_tower_p`). The partner received it and dispatched `LoadGame 21`, which ran, but the continuation it queued was consumed as her own press, so she stayed on Results |
| `replay-decision-vm-v2047e` | v2047c | **Mutation twin, red as required**, as v2047b |
| `replay-decision-vm-v2047f` | v2047d | **Pass.** The host replayed (Replay sent, slot 21, start map `campaign/dunwall/escape/tower/dunwall_escape_tower_p`; 1 send, 70 re-sends). The partner held her own A press, then loaded her own slot 21 (`replayed`). Both worlds end on the tower in gameplay, with equal gameflow, no story node run in the partner's world under host story, and world diff equal (554) |
| `continue-decision-vm-v2047g` | v2047d | **Fail before the mission end:** the route's loot of Ramsey's body did not register (step 12). The same route step failure is in more than ten earlier takes (for example `hoststory-lead-dunwall-v2023a`); no mission end was reached |
| `continue-decision-vm-v2047h` | v2047d | **Interrupted:** the PC shut down at step 3 (2026-11-01). Kept as evidence; the lab was restored with `Deploy-SourceClock` and the take rerun as v2047i |

The mutation twins ran on v2047b and v2047c; with the switch off, the decision code returns before any of the v2047c and v2047d changes. The Continue regression on v2047b passed (v2047c above); v2047d changes only what the partner permits after a dispatched Replay.

## Still to do

- The Continue regression on v2047d (`continue-decision-vm-v2047i`), after v2047g stopped on the flaky loot step and v2047h on a PC shutdown.
- On the result screen the partner's buttons do nothing until the host chooses; there is no on-screen hint yet.
- Replay and Continue on the other eight missions are covered by the full-campaign proof of step 8.6.
