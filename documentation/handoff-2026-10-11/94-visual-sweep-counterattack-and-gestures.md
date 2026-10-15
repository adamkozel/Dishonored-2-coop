# Visual sweep, counterattack and item gestures

Stage 3, cards #9 and #11. Candidates 1214h–1214j, audits 1260 and 1264.

## Visual-acceptance sweep (audit 1260)

**What was reviewed:** the 73 runtime-bound checklist items that had recorded real-game videos but no accepted visuals.

**Method** ([review instructions](../../work/movement-integration/visual-sweep-v1260/INSTRUCTIONS.md)):
- five review passes (locomotion/stance, traversal, combat/paired, ranged/interactions, powers);
- contact sheets from partner views only; the source's own first-person views were skipped;
- functional-first criteria: accept when the action is recognizable and reads naturally enough to ship, and reject clear defects (T-pose, frozen or popping body, wrong action, stuck pose);
- the lead reviewer spot-checked sheets before publishing.

**Result** ([audit](../../work/movement-integration/visual-sweep-audit-v1260.json)): 55 accepted, 8 rejected, 10 insufficient. With audits 1264 and 1257, 72 of 105 items now have accepted visuals.

**Rejected, with the defect:**
- Corvo's crouched heavy attack snaps to standing for about 0.2 s before the windup (take v636).
- The carried body appears on the shoulder in one frame, with no lift (take v13).
- The high grenade throw pops the body 0.35–0.75 m sideways at release.
- Pickups, food and elixirs showed no partner gesture. The native game plays none in first person. Fixed below.
- Reading a note shows no gesture (open).

**Insufficient (no judgeable view):**
- fall loop: no long fall recorded;
- Emily slides: not in the listed videos;
- run in 8 directions: no dedicated run take;
- doors: the partner is hidden;
- key use: no view of Corvo as the partner;
- map load: the camera faces a wall;
- kick: only the legs are visible;
- death and ragdoll: no Corvo-sourced take.

**Scope notes on accepted powers:**
- Devouring Swarm: the rats are not visible in the observer world.
- Shadow Walk: the transformed material is not applied.
- Possession: recorded only on an empty target.

These are effect gaps, separate from the body acceptance.

## Counterattack, sword locks and b-swing hits (#9)

Harness [v1258](../../work/natural-moveset/make_counterattack_v1258.py) and its variants b and c face the guard and cycle the native parry follow-up: hold block 700 ms, then attack at once.

- **Confirmed runtime (1258b): sword lock.**
  - A sword lock runs the exclusive Versus31 state with the four-actor scene `combat/versus_low` (`versus_in` 533 ms, `versus_loop` 600 ms, then a win or lose subscene). Before 1214h it fell to the unbound hold.
  - The Versus31 vtable RVA `1c405e8` comes from the probe's native state table (`--player-state-table`, take 1258c). Ghidra only located the class name.
  - 1214h adds `versus_low` and `versus_elite` (census 1259, ids 118/119, kind 6). They are presented on the drawn sword with the parry-idle clip. A client-sourced lock holds the host NPC and never knocks it out. **Not yet observed at runtime.**
- **Confirmed runtime (1256a, 1258b): b-swing hits.** `sword_attack_right_b_hit` and `sword_attack_left_b_hit` (Scene_1, 733 ms, player and right arm) fell to the unbound fallback. 1214h presents them as combat variants 22/23 with the slash-b clips.
- **Confirmed runtime (1258d): combat-channel blocking.**
  - The client's NPC-combat channels retired ("actor_unbound"). A retired channel then blocked its NPC forever.
  - When the host re-armed with a new nonce, the client rejected all 48 hellos ("receive_binding").
  - Result: the host's counter-kill finisher left the guard dead on the host and conscious on the client.
  - 1214i re-binds a living NPC 2 s after a non-terminal retirement, with a fresh nonce and a new health comparison. Deaths still end the NPC for good (unit-tested).
- **Confirmed runtime (1258f, 1214i): counterattack.** Corvo's timed parry win, then an attack, plays the counter-kill finisher `fatality_head_fast_b` (id 94). The partner presents it and the guard is dead in both worlds.
- **Confirmed runtime (1258l, 1214ah): Emily's killing-blow finisher, not yet her counter.** After she blocked a relayed guard hit (`sword_parry_hit_1`), her attack 0.44 s later was her 4th committed wound on the 40-health guard and played the finisher `fatality_leftleg_medium_a` (phase 105). The guard is dead in both worlds, 0.0 m apart. Audit 1280 called this a counter. The native counter-kill follows `sword_parry_win` (Corvo, 1258f), and Emily's is not yet produced (correction in audit 1281).
  - In 1258i and 1258j she only parried and slashed: the NPC attack relay was off on both peers, so the guard's swings at her clone never reached her world and no native parry was possible ([chapter 95](95-shared-combat-authority.md#the-npc-attack-relay-under-dynamic-relevance-1214ah)).
  - With the relay fixed (1214ah), 1258k still saw no hit: the guard never swung while she attacked every 1.15 s.
  - Harness [1258e](../../work/natural-moveset/Test-Counterattack-v1258e.ps1) holds block 1.6 s per cycle, so the guard attacks into it: 9 relayed hits (3 blocked, 1 parry-lose, 5 applied). One parry win came at 9.7 s, but the attack after it did not finish the guard.
  - Audit: [counter-relay-audit-v1280.json](../../work/movement-integration/counter-relay-audit-v1280.json).
  - **Confirmed runtime (video, 1258l): the host did not present the finisher.** Emily's own view shows the close paired stab, but on Corvo's screen the guard stayed about 2 m away and dropped where it stood. Fixed in 1214ai–1214ak ([chapter 95](95-shared-combat-authority.md#client-sword-finishers-on-the-host-1214ai1214ak)); the client finisher is accepted functionally in take finisher-follow-v1258i.
  - **Provoking Emily's counter (takes 1258m–1258y):** in many takes the guard never swung at Emily's clone. A held block mostly gives plain blocks (`sword_parry_hit`), and an attack after one only slashes (1258y). An attack that replaces a held block in the same frame never slashes (1258q). Harness [1258n](../../work/natural-moveset/make_counterattack_v1258n.py) taps block and attacks only after `sword_parry_win`, so the guard keeps full health and any finisher is the counter-kill.
  - Takes 1258ac–1258ae (harness 1258n, 1214ak): the guard never swung at Emily in any of them (0 relayed hits), so no parry win was possible. Her counter-kill stays open; a lab fixture that makes the guard attack would make it deterministic.

## Counter-kill parked, kick view open (audit 1285)

[Audit 1285](../../work/movement-integration/counter-parked-audit-v1285.json) covers takes 1258y–1258ak and coverage-v1284a.
- Emily's counter-kill is parked on #9. Most takes never got a guard swing at her clone. The native god mode is stripped ([chapter 96](96-external-research-references.md)), so 1214ap adds a lab health hold (`coop_player_health_hold`), built but not yet runtime-verified.
- **Kick:** the free-camera kick take produced no native kick (`throwhit`) in 8 close attacks per role, so the kick view stays open.

## Long fall and landing (audit 1283)

Harness [Record-Coverage-v1283](../../work/natural-moveset/make_coverage_v1283.py) drops the source 6 m above its usual spot while the partner films at a level pitch. In take [coverage-v1283a](../../work/natural-moveset/coverage-v1283a) both roles fell 5.3–6.1 m (native falling state 3, then grounded). The partner shows the falling pose with arms out, a deep landing crouch and the recovery. **Fall loop and hard landing are accepted (functional) in both roles.**

## Pickup, food and elixir gestures (1214j)

The native game plays no first-person gesture for these actions, so the partner showed none.

1214j adds two triggers on the source:
- **Pickup or food:** native `LootObject47` over Running0 or Sneaking6 presents the generic use reach ([chapter 92](92-native-use-interactions.md)) toward the focused item.
- **Elixir:** a health or mana potion quantity drop presents the same reach to the mouth. The last valid count is kept across the item wheel.

Both are presentation only: no item, inventory or health outcome is synthesized.

**Confirmed runtime:**
- **1261b:** coin and grapes in both roles. The partner reach reaches weight 1.0 with a valid target, and the item is taken. `LootObject47` lasts under the 80 ms sample interval, so the harness checks the item instead.
- **1262a:**
  - picking up the map health elixir plays the reach;
  - none of the four D-pad slots drinks it in this save.
- **1263a:** drinking through the native consume callback (isolated potion fixture, stream on), in both roles.
  - The partner raises the hand to the face about 1 s later, because the two potion kinds are each polled every 1.2 s, alternating.
  - The old harness then failed only its own bank-continuity check after the drink.

**Visual:** pickup, food and health elixir are accepted from both windows as functional. The reach uses the sword hand. A dedicated gesture is left for animator polish.

**Open:**
- mana elixir: same path, not recorded;
- note-reading pose: the native ReadNote4 UI suspends the stream and the observer holds the body frozen, so a reading pose would need observer-side playback during that hold;
- body looting and pickpocketing.

## Sweep defects and missing views (audit 1267)

[Audit 1267](../../work/movement-integration/sweep-defects-audit-v1267.json).

**Grenade release pop (fixed).** The release and recovery clips (bank clips 256/257; crouched 269/270) carry about 0.75 m of authored horizontal pelvis travel (crouched about 0.45 m) while the native root stays still.
- On 1214j the pop was still present (take 1265a, re-recorded with harness v1265, derived from v56 by `modernize_bank_harness_v1265.py`).
- Bank 1266 ([builder](../../work/natural-moveset/build_grenade_release_bank_v1266.py)) scales that travel to 25%. All other clip payloads are byte-identical, and the bank stays at version 71 with 374 clips.
- Take 1266a shows a short step into the throw instead.
- Candidate 1214l deploys bank 1266. 1214k's stage failed on a stale bank-manifest path, and its partial archive is kept.
- Product regression 1261c passes in both roles.

**Carry (accepted).** The sweep's input sorted videos oldest-first, so the carry item was judged from the v13 take. The newest v38 recordings show a floor-to-shoulder lift, carry and throw for both characters.

**Coverage takes (1267a, [harness](../../work/natural-moveset/make_coverage_v1267.py)):** runs in 8 directions and sprint-to-slide with a crouched exit, in both roles. Presentation stayed live and the visuals are accepted.

**Deaths and ragdolls:** Corvo-sourced NPC deaths and corpses are visible in the client window of the 1254b takedowns, so both roles are covered.

**Open:**
- **Crouched heavy:** the sweep's 0.2 s standing snap is not reproduced on the current build. The 1265b re-record never held the charge, so it is inconclusive.
- **No judgeable view yet:** long fall loop, doors, key use as Corvo, map-load recovery as Corvo, kick.

## Holding and throwing objects (1214m, audit 1269)

[Audit 1269](../../work/movement-integration/object-carry-audit-v1269.json).

**What the native game does (confirmed runtime, 1269b):**
- Picking up a native movable (a bottle) adds HoldObject49 (RVA `1c7a998`, `arkPlayerStateHoldObject`) over Running0, with Undraw81 while the sword is put away.
- RT throws the object and returns to Running0.

**Before 1214m:** the source classified none of these states. It dropped every sample, so the partner's presentation weight was 0 for the whole hold.

**1214m:**
- HoldObject is a ground action with valid root motion.
- Use variant 6 holds the generic reach at its contact pose for the whole hold; the legs keep the locomotion gait while moving.
- Variant 7 finishes the reach in 1.3 s on the release.

**Confirmed runtime (1269c), both roles:** the partner stays fully presented and holds the reach, then finishes it on the throw. Presentation dips briefly at the pick-up and release moments.

**Visual:** accepted as functional.

**Open:** the object isn't drawn in the partner's hand, and movables aren't shared between worlds yet (stage 5).

Checklist: 95 of 105 runtime-bound, 80 visually accepted.
