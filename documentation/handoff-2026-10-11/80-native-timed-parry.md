# Native timed parry and attacker blowback

Updated 14 October 2026. This continues [held block 1013c](79-enemy-combat-recovery-and-defense.md#native-pre-damage-block-resolution-and-accepted-held-block-1013c). Addresses refer to the pinned executable `B671F824…` (image base `0x140000000`).

## Native mechanism

**Confirmed static** (Ghidra transcripts in `work/movement-integration/npc-parry-ghidra-v1015/`, bookmarks `D2Coop` saved):

- `140859980` (`D2_PlayerStateBlock_ResolveIncomingMelee`) decides block vs. parry. The parry branch needs:
  - an open window (`state+58`, or linger `state+74 > 0`);
  - the re-entry gate `state+59`;
  - an attacker whose component `1439b9bc0` passes `1408598c0`.
- Inside the parry branch, `14060c180` rolls `rand(100) < table[row state+78].f24`.
  - A successful roll returns **2 (parry-win)**, uses presentation 7, and applies `71a140(sm+348, -cost)`.
  - A failed roll returns **1 (parry-lose)**, presentation `0x27`.
  - Otherwise the result is **0 (plain block)**.
  - Win versus lose is therefore probabilistic inside the window.
- The block state does not stagger the attacker itself.
  - The caller `1406357d0` calls `140636140(weaponComponent, defender)` for result 2.
  - For 0/1 it posts event `0xe6 {defender, result}`.
  - It always ends the attack (`635f30`, `634890`).
- `140636140`, when weapon `decl+128` is non-null:
  - Builds damage parameters whose inflictor and attacker (`+50/+54`) are the defender's handle, with type 5, the direction from defender to attacker, and the impulse scaled by `decl+130`.
  - Dispatches them to the weapon owner through `884920 → 884a90 → 884980`. That is a **synchronous** entity `vtable+10` dispatch of event `1439a9d20`.
  - If the defender is in the `arkPlayer` class range, it also plays `635bf0(component, attacker, decl+168)` on the attacker.
- `140858f40` (block OnEnter) clears `+58`/`+74`. It sets `+59` only if the time since the last exit (`+68`) exceeds `+70`.
- `14085a100` toggles `+58` and is called from `14084e3e8`.

**Static hypothesis (asset field order; runtime check pending):**
- The base NPC sword `components/characters/base/weaponmeleenpc_sword.cpntweaponmeleenpc` declares `m_damage`, then `m_blowbackDamage = components/characters/player/base/damage/blowback_player.damage`, then the blowback FX for player and NPC.
- `blowback_player.damage` is `DAMAGETYPE_BLOWBACK`, `NPC_HITREACT_BIG`, `PLAYER_HITREACT_KNOCKDOWN`, with no `m_maxDamage`.
- `decl+128/+158/+168` are inferred to be `m_blowbackDamage`, `m_blowBackPlayerFX` and `m_blowBackNpcFX`.

**Confirmed runtime** ([block-window-v1016c](../../work/natural-moveset/block-window-v1016c/watch.json), client, build 1012, new read-only probe `--watch-block-state`, 5 ms sampling):
- `+58` opens at block entry and closes about 265 ms later (400 ms press: entry 2313 ms, close 2578 ms).
- The re-entry cooldown `+70` is 0.266 s, and `+59` stays 1 when presses are separated by about 900 ms.
- The table rows are `+78 = 67`, `+7c = 206`, `+88 = 245`. The linger `+74` stayed 0 throughout.
- Harness failures `block-window-v1016` (100 ms input minimum) and `v1016b` (array argument through `-File`) are retained. Neither touched game state.

## Candidate 1016 / 1018

Source: `outputs/d2-coop-lab/src/npc_attack.h`, `damage_observer.h`, `engine_bridge.cpp`, `runtime_probe.cpp`. Packet v3/176 bytes and protocol 69 are unchanged.

- **Passive observer on `140859980`.**
  - A 17-byte non-relative prologue trampoline (`mov rax,rsp`; five pushes; `sub rsp,90`).
  - Records result, attacker handle, heavy flag, `+58/+59/+74`, thread, and whether the call came from the co-op relay.
  - Counts are kept separately for native and relayed calls.
  - `meleeDefenseApiMatches` accepts either the original bytes or exactly this hook.
- **Host parry-win.**
  - On an Ack of `ParryWin`, the host resolves the contact's weapon entity (its inflictor handle is stored in the pending entry). It checks the parent, the `arkCpntWeaponMeleeNPC` vtable/owner and a non-null `decl+128`.
  - It then calls native `140636140(component, ownedClone)`.
  - The owned-clone firewall has a thread-local exemption that admits only that health component, only with inflictor and attacker equal to the clone, and only the exact `blowback_player.damage` declaration. Anything else keeps the ordinary filters.
  - A health change or a missing/unexpected event counts as a failure, because a health change would need an ordered combat commit.
  - Afterwards the host plays native `635bf0(component, attacker, decl+168)`, the attacker FX that native code plays for a player defender.
- **Receiver.** On a local parry-win, the receiver plays the same FX on its replica attacker. The weapon is found through the equipped inventory slot 0. No damage is applied on the receiver; the stagger reaches it through the host NPC body stream.
- **Probe:** a new read-only `--watch-block-state MS` mode. Probe `06C18D5E…`.

## Runtime record

Audit: [npc-parry-audit-v1019.json](../../work/movement-integration/npc-parry-audit-v1019.json). Live DLL 1018 `A3A3D250…`. The probe `--watch-block-state` was extended with table row values (`probe-block-v1016b`, `6E06EBA1…`). The deployed probe stays 1016's `06C18D5E…`.

- **Confirmed runtime: the native parry-win chance is 0% in this fixture.**
  - Row `+78 = 67` resolves to `parryWinPercent 0` for both Corvo (host) and Emily (client). The cost and linger rows are also 0.
  - So in this save a timed parry can only produce result 1 (parry-lose for the attacker, which is the "off-balance" parry). Result 2 (blowback) evidently needs a stat granted elsewhere; I hypothesize an upgrade or bone charm.
- **1016** (`A838FF9D…`), take `npc-native-parry-v1017`: failed. The new `parryWin` status object duplicated the existing counter key, and the cleanup waited only 20 s for the 46 s recordings. Restoration passed.
- **1018** (same behavior; the object is renamed `blowback`):
  - `v1017b`: validator failed on summed health. Native health regenerated (21 → 35) before the final read; per-event health was consistent. 9 contacts were natively defended (5 blocks, 4 timed parries) and 3 applied.
  - `v1017c`: validator compared process-lifetime observer counters with per-session counts. The deltas matched.
  - `v1017d`: validator required a blowback result although no parry-win occurred.
  - `v1017e`: all 3 contacts landed outside a raised block with the 600/350 ms cycle.
  - All of these restored cleanly.
- **`v1017f` PASS (scoped)**, 900/320 ms cycle:
  - 8 relayed contacts: 4 native blocks, 1 native timed parry (result 1) and 3 applied.
  - No damage call for defended contacts; each event's health is consistent.
  - The host acknowledged matching outcomes and posted 5 native `0xe6` attacker events with zero failures.
  - The observer deltas match the relayed outcomes, and the native-replica calls are 0. The client replica never resolves blocks itself.
  - The duplicate was rejected, and restoration and protected files passed.
- **Visual review of `v1017f`:** [review](../../work/natural-moveset/npc-native-parry-v1017f/visual-review.json), not accepted.
  - The client shows native block/parry presentation, then Ramsey's off-balance pose arriving through the host body stream, then the native Defense Blocking tutorial.
  - The parked host camera faces a wall, so the host-side reaction isn't visible.

## Process incident

The first 1016 build script renamed only some log names. Its test logs overwrote `shadowwalk-body-{world,action,resume,checkpoint}-tests-v1012.log` with 1016 output (timestamped 2026-10-14 08:11). Those four 1012 build logs were not archived and cannot be recovered.

The 1012 runtime evidence, audit 1014, the source/DLL archive and the smoke/bank logs (07:20) are unaffected. The 1016 and later scripts write `npc-parry-*` logs.

## Open

- **Timed parry under the relay.** Latency and the 80 ms NPC body interpolation mean the client sees the replica's swing slightly after the host contact was applied. This shifts the effective parry timing earlier than the on-screen cue; it has not been measured yet.
- **Replica native resolution.** Whether the client replica's own weapon contacts also run `1406357d0` locally is now observable through the observer's native counts.
- **Still unhandled:** heavy/unblockable attacks, kicks (`npc_closecombat`), simultaneous attackers, and the defender-side knockdown `PLAYER_HITREACT_KNOCKDOWN` (blowback of the player).

## Remote-body presentation of the defender's native scenes (candidate 1022)

**Confirmed runtime source scenes** (`npc-parry-scenes-v1021`, `--watch-midnight`; `npc-combat-scene-v1024`, new read-only probe `--watch-combat-scene`):

| Native result | Defender scene | Bindings |
|---|---|---|
| 0, block | `sword_parry_hit_1` or the asset-equal alternative `sword_parry_hit_2`, `Scene_1`, 533 ms (16 ms after the result) | player camera, owned right arm, attacking NPC |
| 1, timed parry | `sword_parry_win`, `Scene_1`, 500 ms (0–31 ms after the result) | player camera, owned right arm, attacking NPC |

These scenes are what the receiving player's first-person view shows. Before 1022 the shared combat reader rejected them, so the remote body kept its block idle.

**Candidate 1022** (DLL `A7637CA1…`):
- **Reader:** the reader accepts exactly these scenes as variants 19 (`hit_1`), 20 (`hit_2`) and 21 (`win`). They must be inside block state 44 with the observed binding set.
- **Protocol:** movement/action protocol 70 accepts them only in the block (or air) state, with their exact single-phase durations. New wire tests cover wrong duration, slash state, two-phase timing, expiry and an unknown variant.
- **Bank:** bank 68/359 ([manifest](../../work/natural-moveset/parry-bank-manifest-v1022.json)) appends clip 357 (native NPC `sword_parry_ll_out`) and clip 358 (`sword_parry_rl_out`). Both are resampled from the existing native-retargeted library, with no root writes or events, and the prior 357 payloads are byte-identical.
  - Corvo bank `0750303A…`, Emily bank `D0C8CC53…`.
  - These are functional stand-ins and have no acting review.

**Runtime results:**
- **1023 pass:** a timed parry was presented as clip 358.
- **1025 pass:** a block impact (`hit_2`, variant 20) and three timed parries were presented as clips 357 and 358. 25 ms samplers show every source event received in order, with zero rejections.
- **1023b fail:** one block impact was not presented on the host. The cause is not determined.
- **1024:** diagnostic run that proved the reader accepts the impact scenes. Its cleanup left both instances on the native Press-any-key screen; separately [recovered](../../work/natural-moveset/npc-combat-scene-v1024/recovery/result.json).

## Lab observer camera (candidate 1026)

The host's parked camera faced a wall, so remote presentation could not be reviewed from the host window.

**IGCS assessment:** the user pointed to Frans Bouma's [IGCS Dishonored 2 camera](https://github.com/fransbouma/injectablegenericcamerasystem/tree/master/Cameras/Dishonored2). Its source was read on GitHub; nothing was downloaded or installed.
- It targets the same build family: its Ansel start-session code reads the game singleton `+39A5290`.
- It works by forcing an NVIDIA Ansel session, which pauses the game, so it is unsuitable for live combat observation.
- Its notes document the native camera-transform write (`+A8` position, 3×3 basis from `+CC`).

**Confirmed static on our exact image:** `140890c50(view, position, basis)` is the render-view setter. It writes `+A8/+C0` position, `+CC` basis and the `+3EB0` matrices. Its prologue is 18 bytes of non-relative instructions.

**Candidate 1026** (DLL `54FCA892…`) hooks that setter:
- Every view is recorded in `<status>.observer-camera.json`.
- Lab command `coop_observer_camera hold` freezes the most active native view at its current pose for up to 120 s; `off` releases it.
- No player or entity transform is changed, and nothing is sent over the network.
- A test moves the host to a viewpoint with `setviewpos`, holds the camera, then returns the body to its parked position. NPC perception is therefore unchanged.

**Runtime results:**
- **1027c pass:** held view, growing overrides, body parked, release.
- **1027 / 1027b:** harness allowlist and validator-field failures, both retained.
- **1028c pass:** first host-window recording of the remote player in a defended fight. Remote Emily engaged by Ramsey and two guards, with impact sparks at the contacts. Host samples recorded clips 357 (12) and 358 (7). [Visual review](../../work/natural-moveset/npc-parry-observed-v1028c/visual-review.json), not accepted.
- **1028:** native positive-control timeout, retained.
- **1028b:** the clone lost 3 health from an unattributed source. That harness version did not save the damage records; the current harness now saves them on failure.

[Audit 1029](../../work/movement-integration/npc-defense-presentation-audit-v1029.json). The checklist items `parry_recovery` and `parry_directions` are annotated `partial_native_steam_block_impact_and_timed_parry_emily_defender`.

## Close-combat kicks (investigated, not implemented)

**Confirmed runtime:**
- Local control 1011c recorded two `npc_closecombat.damage` contacts in 12 s of held block: amount 0, flags `0x80`, reactions 2/2, region mask 2 / region 1, with the NPC as both inflictor and attacker.
- Across all relayed block/parry takes there were zero close-combat contacts against the clone.

**Confirmed static:**
- `npc_closecombat.damage` is `m_maxDamage 0`, `PLAYER_HITREACT_BIG`.
- `arkCpntCloseCombat` (vtable RVA `1b3b7c8`) is a thin wrapper. The default `aicombatattack_default` declaration is empty.
- The AI uses a rule system (`arkAIMeleeDefense`, `arkRuleAction_AIMeleeDefenseOutcome`, `arkMsgAIMeleeDefenseOpportunity`, `arkRecordCombatDefensiveMode`) whose descriptors are only reached through runtime registration.

**Hypothesis:** guards choose a kick when the target publishes a defensive-mode record. The owned clone never publishes one, so the remote player cannot be guard-broken. A relay for kicks would be untestable until that perception exists, so none was added.

## Both-role parity: Corvo as defender (1030)

[Audit 1031](../../work/movement-integration/defense-parity-audit-v1031.json). On live 1026:
- **Defense:** host Corvo blocked natively against host-owned enemies (no relay) with the 1200/320 ms cycle. The observer recorded 4 blocks and 2 timed parries, all native (relayed 0).
- **Presentation:** the client's remote Corvo body played clip 357 (13 samples) and clip 358 (11 samples), with no wrong clip.
- **Camera:** the client's observer camera filmed Corvo.
- **Restoration** passed.

Defense presentation is therefore runtime-verified for both roles. The checklist items `parry_recovery` and `parry_directions` are now `partial_native_steam_block_impact_and_timed_parry_both_roles`.
