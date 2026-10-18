# Water exit and action-stream hold (candidate 1056)

Updated 2026-10-14. Continues [swimming and climbing](50-swimming-climbing-mode-discovery.md). Audit: [water-exit-audit-v1058](../../work/movement-integration/water-exit-audit-v1058.json).

## Identity

| | Value |
|---|---|
| Candidate | [candidate-npc-combat-v1056](../../work/movement-integration/candidate-npc-combat-v1056/identity.json), previous 1042 |
| DLL | `0352516B0A005701…` |
| Probe | `EE8ADF67AFA32648…` |
| Action protocol | 73 |
| Bank | 70/369, unchanged from 1042 |
| Map | Edge of the World, loaded with the native `campaign` command; Dunwall fixture restored after every take |

## Native findings

- **Confirmed static:** the game ships `swim_mantle.playerstateswimmantle` (`arkPlayerStateSwimMantle`) and common player-body swim animations `ply_body_generic_swim_{idle,n,s,e,w,neutral}`. Neither appeared in the exits tested here; the swim animations are an option for later fidelity work.
- **Confirmed runtime, steps (1052):** swimming hands over directly from `arkPlayerStateSwimming` to default-mode `arkPlayerStateRunning` (`1c85628`). There is no mantle state.
  - The remote body rises upright out of the water and runs up the steps.
  - The swim layer fades out in about 400 ms. One frame of about 100 ms has bank weight 0 (a neutral pose).
- **Confirmed runtime, ledges (1054, 1054c, 1057):** jumping at a reachable ledge enters the ordinary default-mode high mantle: state 8, `mantle_idle` or `mantle_sneak` scene, 833 ms, kind 2. From swim depth the rise is 2.69–3.15 m.
- **The 1051 dock wall below the spawn is too high**, so no exit happened there (retained failed run).

## Defect on 1042: action stream retired permanently (1053, 1054d)

**Sequence (confirmed runtime):**
1. A mantle straight from swim depth rose 3.07 m (start z −1.81, goal 1.26).
2. The source traversal reader requires a rise of 3 m or less (`validTraversalGeometry`), so it failed at stage 2.
3. The ground fallback refused every send because the player is not in default ground locomotion.
4. After 250 ms without a valid state, both roles retired the stream with `native_state_stale`.
5. The remote body lost all authored presentation (bank weight 0) for the rest of the session.

**Generalisation:** any native state that no binding classifies, lasting more than 250 ms off the ground, had the same effect.

## Candidate 1056

1. **Mantle rise bound raised from 3 m to 4 m.** This lives in `native_player_traversal_scene.h` and is shared by the source reader and the receiver validator. The 4 m horizontal bound is unchanged.
2. **Protocol 73, action state 24 "hold":**
   - *When it is sent:* only when neither a binding nor the ground fallback accepts the state.
   - *What it proves* (`readNativePresentationHold`): the same live game and player in gameplay, a stable state list across two reads, and validated equipment.
   - *What it carries:* only `swordState` 1 or 2. The protocol refuses any action, scene, held item, lean or air stage.
   - *Receiver behaviour:* it renews the stream but presents nothing authored (bank weight 0, root still network-owned). **Changed in 1214cg ([chapter 99](99-movement-completion-coverage.md)):** the hold is action-eligible, so the authored body continues on the replicated physical state; yielding to the native pose flashed it upright for about two frames.
   - *What still retires the stream:* loads, menus and identity changes, as before.
   - *Status:* the field `holdSamples` was added to the stream status.
3. **Tests:** the protocol suite covers the hold (valid with sword 1 or 2; refused with sword 0, traversal, combat, power, air, lean or protocol 72) and the water-mantle geometry (3.07 m accepted, 4.7 m refused). All Build-1056 suites pass.

## Runtime on 1056 (1057)

| Source | Rise | Presented | Stream | Video |
|---|---|---|---|---|
| Corvo (host) | 3.07 m | variant 5 (high mantle into sneak), 11 samples on the client | ready, 0 misses | Climbs out, hands on the dock edge, ends crouched |
| Emily (client) | 3.15 m | variant 4, 11 samples on the host | ready; 1 transition-frame miss sent as a hold | Climbs out, hands on the edge, stands |

**Checklist:** `dive_surface_water_exit` → `partial_native_steam_water_exit_both_roles_mission_map`, visual validation `bounded_actual_game_frames_inspected`.

## Interruption and recovery

The user interrupted take 1054b after the mission load, before its cleanup. [`Recover-InterruptedTake-v1055.ps1`](../../work/natural-moveset/Recover-InterruptedTake-v1055.ps1) restores the lab with ordinary input:
- A on load prompts, B on modals, X held on intros;
- lab features turned off;
- `PrepareShadowSword` and the note inventory restored.

It writes `recovery.json` into the interrupted take. The receipt passed and the protected files are unchanged. Use it whenever a harness is stopped before its own cleanup.

## Open

- **Hold coverage:** only one natural transition frame has exercised it. A sustained unclassified-state fixture is still needed (for example the slope `Slipping` state seen in 1041b).
- **Not observed:** the native `arkPlayerStateSwimMantle`.
- **Not re-run on 1056:** the boat-ledge geometry from 1053.
- **Polish:** wading, a smoother exit and use of the native swim body animations are deferred to the animator.
