# Swimming and climbing mode discovery647/648

Follow-up: [runtime fixture659–661](51-native-chain-fixture-blocker.md) completed the proposed declaration query and isolated spawn. The definition resolves but the spawned actor lacks both required physics/climbing components. No Climb29 activation was observed. This supersedes the next-step plan below; continue another functional movement case until an effective native campaign fixture is available.

This is read-only discovery, not implemented or runtime-accepted swimming/climbing. Existing authored water, ladder and rope studies remain available in `work/natural-moveset`; this investigation does not start a new authoring pipeline.

[Default-mode inspection647](../../work/movement-integration/traversal-research-v647) and [mode-slot inspection648](../../work/movement-integration/traversal-research-v648) read the live player state machine on the exact installed executable. The normal mode has80 non-null state slots. Swimming and climbing live in other mode tables, explaining why normal-mode-only readers do not cover them.

| Machine field | Observed slots | Relevant RTTI/ID |
| --- | --- | --- |
| +0x28 | Default80 | Existing grounded/air/combat states |
| +0x30 |25 | ID0 and1 `arkPlayerStateSwimming`; ID8 `arkPlayerStateSwimMantle`; ID29 `arkPlayerStateClimb` |
| +0x48 |13 | ID29 `arkPlayerStateClimb`; ID88 `arkPlayerStateCancelClimb` |
| +0x70 | Possessed-fish table | ID30 fish swimming |

The mode starts with a reciprocal machine pointer; +8 is a declaration pointer; +0x10 begins the90 state-pointer slots. The other inspected machine fields0x38,0x40,0x50,0x58,0x60,0x68,0x78,0x80,0x88 contain17,12,7,2,7,5,4,15,7 non-null slots respectively. Names/behavior of these modes must not be inferred only from counts. Live addresses are obsolete after checkpoint reload; reacquire them through current ownership before further probing.

Ghidra strings include swimming at141cc7030, swim mantle at141cc07c0, climb at141c787f0 and mode-ID enum at141c74120. The swimming string cross-reference enters a very large reflection registration function; it is not useful to decompile that whole function as a prerequisite. Next, inspect the native class callbacks and actual ordinary-input activation at a real fixture, preserving exact mode ownership and source physics.

## Current-map fixtures

Four named window handles were identified at approximately(-26.53,-153.89,67.48),(-26.53,-149.89,67.48),(-27.42,-144.29,68.79),(-27.42,-160.89,68.79).649/650 used the first closed window and produced the automatic-crouch discovery documented in [chapter49](49-automatic-crouch-and-confined-mantle.md). No through-window exit was proved.

`ld_mesh_chain_1` at(-45.193,5.39271,61.0179) is an `idDynamicEntity`. Its name alone does not prove it is climbable; inspect its components and nearby native climb volumes before testing. Name/type filtering did not find an obvious water entity, which is not proof that the current map lacks water. Ordinary player sensor/state evidence and native geometry must establish a suitable test location.

No native mode switch, state callback or gameplay outcome was forced. Swim, dive, ladder and chain/rope requirement rows remain unverified. Once a real native state is observed, prefer binding the retained authored clips and normal source position before doing further pose polish.


## Follow-up657/658

[Read-only657](../../work/movement-integration/climb-fixture-v657) proves `ld_mesh_chain_1` has only reference-frame, rigid-body and health components. It stands near the censer and intro audience; there is no climbable component. Do not use it as a climbing fixture.

The original Void catalog does contain **`models/environment/gameplay/climbable/chain_01/usable_sources/chain_01.def`**, class `arkClimbable`. [Pinned export manifest](../../outputs/d2-coop-lab/research/asset-exports/climb-fixture-v657/manifest.json) records original resource provenance and SHA256 `A15E4D3A6C60615E0AB94B724339DD973900C9CF53D413BB4E33C1EE17609A1A`. The definition uses its native chain MD6/material, `physicssystem_chain.cpntphysicssystemevolved`, `animbase_simple.cpntanimbase`, `chainclimbable.cpntclimbable`, audio, native `Usable_chain` / `Interaction_climb` UI and swinging sound/RTPC. Searches across all resource catalogs did not export the two inherited component declarations individually. This does not prove they are absent from the runtime compiled declaration.

[Fresh mode identities658](../../work/movement-integration/traversal-research-v658/mode-slots.json): Swimming VT RVA1c7dc68 in mode+30 slots0/1; SwimMantle1c7d958 in mode+30 slot8; Climb1c78498 in both mode+30 and+48 slot29; CancelClimb1c786c8 in mode+48 slot88; FishSwimming1c3b2b8 in mode+70 slot30. All were inspected while inactive; no activation or playback is claimed.

Targeted decompilations are saved alongside those observations. Climb VT+0x68 function1408310a0 resolves state+0x220, requires at least2 entries in state+0xe8 low24 count, updates native physics through machine+0x308 and uses its authored declaration. Climb VT+0x60 function1408304e0 clears references/restores equipment-like state; lifecycle naming remains an inference. Swimming VT+0x68 function140846450 reads input machine+0x2f8, writes movement on physics controller+0x48 and handles swimming slot1 exit conditions. These functions were decompiled, not invoked. Four exact class bookmarks were saved to the persistent project.

Next bounded investigation: determine whether the original climbable definition resolves through the existing validated native declaration lookup. If available, create a disposable, owned climbable-chain fixture in the isolated tower using the established native entity creation path, then restore the checkpoint. Use ordinary native interaction/movement to observe actual mode, attachment, entry/loop/exit and cancellation before binding existing rope clips. If the resource is not loaded, document the exact dependency and continue another movement binding; do not treat a decorative chain as proof or fabricate a native mode transition. Water still needs a real native fixture.

## Real mission fixtures through the native campaign command (14 October 2026)

- **No usable saves.** The user-supplied save archive contains original *Dishonored (2012)* saves; it is not loadable ([assessment](../../work/research/user-saves-v1033/README.md)).
- **Native campaign command.** The exact executable keeps the developer command `campaign <mapinfo> [low|high] [emily|corvo]`. Its usage string lives in the console-command table next to `LoadGame <slotNumber> [overrideMapName]`.
  - Candidate 1033 (DLL `53C9948E…`) allowlists only `campaign campaign/<path>.mapinfo [low|high] [emily|corvo]`.
  - Packaged campaign map-infos are listed by `Read-D2Assets.ps1 -NamePattern mapinfo`. The `checkmap/*` developer maps ship map-info only and cannot be loaded.
- **Mission fixture discovery** ([mission-eotw-v1034](../../work/natural-moveset/mission-eotw-v1034)):
  - Edge of the World loads in the isolated host and passes the native load screen. About 70 s of logic state 5 (the intro) precede gameplay at the harbor dock (-30.8, 192.75, 1.25).
  - The census of 11,164 entities includes three genuine `arkClimbable` chains (`script_chain_01_1_8`, `script_chain_01_2xlonger_2/3`), 371 usables, 27 doors, keys and a lever.
  - The Dunwall fixture is restored afterwards (protected files unchanged).
- **Confirmed runtime swimming** ([native-swim-v1035](../../work/natural-moveset/native-swim-v1035)):
  - Entering the canal 4 m to the player's +X switches the native machine to the swim mode table. Active state 0 is `arkPlayerStateSwimming` (vtable `1c7dc68`), stance 7, player z ≈ −2.0.
  - Ordinary forward, dive (B), surface (A) and backward inputs move the player (y 192.75 → 184.28, z −2.87 dive, then −1.67) **without any further state change**. Diving and surfacing are vertical movement inside the swimming state.
  - The actual first-person video was inspected: underwater swim hands, surfacing, treading water at the surface.
  - The remote swimming body is not bound yet.

## Co-op on mission maps and remote swimming (candidates 1033/1037/1039)

[Audit 1040](../../work/movement-integration/mission-swim-audit-v1040.json).

- **Mission co-op bring-up (1036, candidate 1033): pass.**
  - Both roles load Edge of the World through the native `campaign` command (host Corvo, client Emily).
  - The Steam transport stays fresh, and both partner Doppelgangers spawn and follow exactly.
  - The client's remote Corvo tracks the host within 0.4 mm while walking the Karnaca dock. The client video shows the native masked Corvo body on the dock.
- **Action stream was Dunwall-only.** `armProbePairReady` required the Dunwall tower map, so the action/movement stream refused to start elsewhere (1038/1038b, retained failures).
  - Candidate 1039 replaces that with `coopWorldPairReady`: both peers on the same native campaign map.
  - The other Dunwall-only *gameplay* gates are listed in the audit for a dedicated map-agnostic pass: coin, usable and checkpoint sync, Doppelganger visual, ranged/grenade/heart/trap props, native power effects, and some power presentations.
- **Native swimming binding (1037/1039):**
  - The source classifies the exact swim class (`1c7dc68`, non-default mode, sole active state 0) as action state 22. Protocol 71 admits it only with every action/scene field empty.
  - Bank 69/365 (Corvo `DEB1742E…`, Emily `0FBFA430…`) appends clips 359–364:
    - 359: treading (`mixamo_treading_water`);
    - 360: stroke (`water_forward_sequence`);
    - 361: up (`water_up_sequence`);
    - 362: down (`water_down_sequence`);
    - 363/364: reserved copies.
  - The receiver blends tread/stroke by replicated horizontal speed and up/down by vertical velocity. Root and depth stay network-owned.
- **Runtime: 1038c, d, e and f pass.** Host Corvo swims, dives and surfaces in the canal. The client's remote Corvo presents swimming in about 187 of 200 samples (about 78 stroke, about 18 dive, about 18 surface). Restoration passes every time.
- **Visual capture not yet achieved.** The client camera placements (dock, boat, held overhead) did not frame the mostly submerged swimmer; the held overhead view fell to the waterline before the hold. Visual acceptance therefore remains open.

**Still open:** Emily as swimmer, swim mantle and water exit, a standing observer vantage over water, and visual acceptance.

## Native chain climbing bound (candidate 1042)

[Audit 1044](../../work/movement-integration/climb-audit-v1044.json).

- **Confirmed runtime (1041/1041b):** the ordinary interact (X) at `script_chain_01_1_8` enters alternate-mode state 29 `arkPlayerStateClimb` (vtable `1c78498`). Up and down inputs move the player inside that state; jump releases and B does not.
- **Candidate 1042** (DLL `A67A63DD…`, protocol 72) sends the exact climb class as action state 23. Bank 70/369 (Corvo `FD50D187…`, Emily `500BEC1D…`) appends authored ladder-contact clips: 365 ascend, 366 descend, and 367/368 for enter/exit (reserved).
- **Playback:** the receiver advances the climb cycle with replicated vertical distance (assumed 1.5 m per cycle) and blends ascend/descend by vertical direction.
- **1043 pass:**
  - the native intro skip shortens the mission load to 15 s;
  - the client's remote Corvo presents climbing in 146 of 198 samples (55 ascend, 91 descend, 42 phases);
  - the client observer-camera video shows grab, climb, descent and drop.
- **Open:** hand-to-chain contact is approximate with the ladder stand-in clips. Entry and release transitions, Emily as climber, real ladders and visual acceptance remain open.

## Role parity and visual acceptance (candidate 1042, audit 1050)

[Audit 1050](../../work/movement-integration/traversal-parity-audit-v1050.json). Harnesses: `Test-TraversalCoop-v1046.ps1` through `v1049.ps1` (parametric `-Source host|client`, `-Modes swim|climb|both`).

- **Emily as source (1046, confirmed runtime):** the host renders remote Emily with the Emily bank 70.
  - Swimming: 201 of 213 samples (88 stroke, 22 dive, 20 surface).
  - Climbing: 163 of 224 samples (117 ascend, 46 descend, 51 phases).
  - The host observer-camera video shows remote Emily grab the chain, climb, descend, release and land. **Visually accepted (bounded).**
- **Why swimming was never framed (1047, 1048):**
  - In native swimming the eye sits at the waterline: root about −2.0, view about −0.1, surface about 0 (native 1035). The faithfully placed remote body is therefore mostly submerged.
  - 1048 sampled the observer's remote/render position and native view and projected it. The swim lane at x = −26.8 runs just beyond a boat moored along the dock, so from dock level the swimmer was behind it.
- **Overhead framing (1049):**
  - The lane moves 5.5 m off the dock.
  - The observer yaws at the lane, pitches 50° down with native look input (`Set-ObserverPitch`; `setviewpos` preserves pitch), is placed 9.3 m above the dock edge and immediately held with `coop_observer_camera hold`. The body falls about 0.3 m before the hold, then returns to the spawn.
  - Emily source (host video) and Corvo source (client video) both show tread at entry, horizontal stroke with the lead arm extended, dive/surface (faint while submerged) and tread again. **Visually accepted (bounded)** for both roles.
- **Checklist:**
  - `swim_idle_directions` → `partial_native_steam_swimming_both_roles_mission_map`;
  - `ladder_up_down_entry_exit` → `partial_native_steam_chain_climb_both_roles_mission_map`;
  - both are `bounded_actual_game_frames_inspected`, and swim/climb recordings are now linked to the items.
- **Still open:**
  - swim mantle and water exit;
  - chain hand-over-hand acting and grab/release transitions (clips 367/368 reserved);
  - true ladders and top exits;
  - polish (head height at the waterline, stroke cadence against the native swim speed of about 3.5 m/s), deferred to the animator.
