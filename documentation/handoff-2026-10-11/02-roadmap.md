# Complete roadmap and completion gates

The requested product is a single-player campaign played cooperatively by Corvo and Emily, with shared cutscenes, native game UI, native Steam sessions and consistent gameplay outcomes. Stages 3 to 5 are complete (their umbrella cards are closed). Releases up to 0.9.6 covered stages 6 and 7; the current priority is stage 8, host authority and 1.0 ([brief](115-stage8-plan-brief.md)). On 2026-10-11 the user explicitly authorized an active goal of finished playable co-op and asked development to continue beyond movement without milestone approval pauses while they sleep. Finishing movement is a transition to the next development work, not a stop condition.

The exact 105-item movement checklist, role applicability, motion candidates, evidence links, power requirements and remaining notes is generated in [movement requirements](generated/movement-requirements.md). That document is the detailed moveset list; do not replace it with a short selection of locomotion clips.

## Project stages

| Stage | Work already established | Required exit criteria |
|---|---|---|
| 1. Protected development lab | Original baseline, independent save paths, native input automation, side-by-side windows and background play | Maintain exact executable guards, original-file integrity and reproducible restoration |
| 2. Native transport and remote identity | Native Steam connection; epochs, roles, handles, leases; presentation actors | Real two-account host/join/invite workflow, disconnect and reconnect with no stale ownership |
| 3. Complete movement phase | Authored/native/retargeted candidates, D2 playback, many isolated role/state tests and videos | Every applicable motion bound to genuine game state, natural full-body quality, transitions, contact/equipment/VFX, both roles and reviewable real-game videos |
| 4. Shared combat and world authority | Bounded guard and native damage experiments | Enemies perceive/fight both players; one authoritative damage/outcome path; synchronized health, deaths, spawn, bodies and physics |
| 5. Interactions and progression | Native state/resource investigations, contextual animation candidates | Doors, pickups, inventory, objectives, dialogue, mission decisions and scripted interactions remain consistent |
| 6. Campaign and recovery — active | Fixed Dunwall shared reload/death and cinematic experiments | General checkpoint/save transfer, role restoration, simultaneous request deduplication, both-ready barriers, every relevant map/load/scene boundary |
| 7. Native UI and release | Existing pause/death/character/menu assets identified | Native host/join/choice/recovery flows, packaging, rollback, sustained stability and online validation |
| 8. Host authority and 1.0 — active | Release 0.9.6: the full campaign on two Steam accounts; host-authoritative combat and nearby guards; story, items, doors, safes and far guards run in both worlds and are mirrored | The host decides the shared world and the partner sends requests; items per player; a difference detector, flight recorder and network-trouble tests; zero differences through the campaign in all four pairings; release 1.0 |

Stages overlap where dependencies demand it. A new animation must not independently apply combat damage or invent world authority merely to look correct.

Partners must be completely excluded from each other's direct interactions, including powers, damage, prompts and contextual actions. Normal native movement collision between partners is allowed, as explicitly clarified by the user. There are no partner-targeted cooperative abilities. Shared session recovery and progression remain required. The detailed acceptance matrix and findings are in [mutual partner exclusion](34-partner-interaction-exclusion.md).

## Added scope: co-op menu and character roles (2026-10-21)

The user added these to the goal's "done". They come after #140 and release 0.8.4, in this order:

1. **Co-op menu and saves** (stage 7):
   - a **Co-op** entry under Campaign in the native main menu;
   - it opens the same save selection and creation screen, with co-op saves isolated from the campaign slots (#156).
2. **Leave co-op** (stage 7): in a session, the pause menu's Quit game becomes Leave co-op and acts as F8 (#157).
3. **Character choice** (stage 6): the host chooses Corvo or Emily once per co-op save, when creating it, because the
   game ties story state to the save's protagonist. The partner chooses on every join (#158).
4. **All four pairings** (stage 4):
   - Corvo-Emily, Emily-Corvo, Corvo-Corvo and Emily-Emily (#159);
   - Corvo's powers used by the partner act on the host's characters, as #131 did for Emily's (#160).
5. **Validation** (stage 6, #161): the depth is chosen for stable results.
   - The full campaign on two Steam accounts in at least one new pairing.
   - A mission sample in each other pairing, per-character power checks on the partner side, and the menu and Leave
     co-op in the same sessions.
   - Then a release and the clean-PC proof.

## Stage 8: host authority and 1.0 (2026-10-29)

The user approved [the stage 8 brief](115-stage8-plan-brief.md) as the active goal. It applies the coop-anything playbook ([chapter 114](114-coop-anything-playbook.md)). The steps, in order:

1. **Measure before changing:** the activation range, every one-player assumption, the functions that write shared state, and a check of the video capture.
2. **The difference detector and flight recorder,** plus honest tests that assert their mode first.
3. **Bad connections, crashes and rejoining:** a network-trouble relay on the lab leg, Steam keep-alive and timeout, and a versioned hello.
4. **The host takes control,** one area at a time, each with an off-switch and a mutation twin:
   1. story, objectives, mission decisions and the ending;
   2. items, per player;
   3. doors, levers, lifts, switches and safes;
   4. joining mid-mission and quickload;
   5. guards far from the host;
   6. partner powers that touch the world.
5. **Partner quality:** movement measured in numbers (animcheck), and polish.
6. **Proof and release 1.0;** the final playtest with the user's friend stays Needs-you.

The user's decisions are in the brief's section 8. Items are per player: each player can take every item once.

## Movement families

The catalog covers locomotion; stance changes, crouching and leaning; jumps, falls, landings, slides, mantles, vaults, ladders, water and other traversal; sword attacks/defense/reactions; ranged weapons and items; contextual interactions; paired takedowns and body handling; and both characters' powers. There are 11 locomotion, 10 stance, 19 traversal, 16 combat, 9 ranged/item, 13 interaction, 11 paired-action and 16 power requirements.

Completion requires all applicable directions, equipment/stance contexts, starts/stops/turns, interruptions and recovery, not just a playable loop. A donor forward crouch walk does not cover strafing, reverse movement, stops, sword actions or climbing by itself. A first-person clip is inspiration and timing data, not a full-body rig constraint.

## Acceptance gates for each movement

1. Identify the exact native semantic source and evidence. Differentiate input request, active state, clip playback and successful gameplay outcome.
2. Author a natural complete-body motion on the correct Corvo/Emily rig with native lengths, helpers, attachments and corrective behavior preserved.
3. Keep root position under native game/network control. Match gait phase to travel or the relevant native scene clock, not arbitrary elapsed time.
4. Bind entry, progression, interruption and exit. Validate drawn/holstered equipment, stance, airborne/grounded context and pause/load/session boundaries.
5. For paired actions, bind both identities and interaction transforms; prevent duplicate damage and restore physics safely.
6. Use native visual effects where the action depends on them. Demonstrate spawn, timing, attachment and cleanup, including abort and world replacement.
7. Record actual D2 playback with exact build, bank, source and capture rate. Inspect the video from useful angles; retain failed takes.
8. Mark complete only when the requirement's full scope is met. Passing one fixture or having candidate footage leaves broader gaps explicitly open.

## Near-term continuation

Finish body-handling contacts and remaining stance/turn/cancel cases; broaden the now-scoped native throw and compact-release bindings; resolve paired/contextual motions; expand the per-role weapon/power/VFX matrix; then complete visual review, transition coverage and automatic gameplay binding for the full checklist. General co-op authority and native session UI still require substantial work afterward. Continue directly into those stages under the active playable-co-op goal.

No defensible single completion percentage is available from the current candidate counts. If providing an estimate, state its scope and weighting and keep it separate from measured coverage.


12 October priority update: basic partner health/choke/sword protections are retained; the user requested moving on instead of exhaustive interaction testing. Remaining mutual exclusion edge cases stay in the backlog. Resume movement integration with Emily Doppelganger casting and native presentation, then remaining movement and shared world authority. Normal movement collision stays enabled.


Doppelganger update412: standing armed Emily native cast phases and complete-body Steam playback pass411, with original first-person and existing bank payload retained. Remote clone/FX/lifecycle and shared outcomes remain unfinished; continue those and remaining movement/world integration. Partner interaction edge-case work remains deferred per user. See [chapter35](35-doppelganger-cast-and-clone-lifecycle.md).


Doppelganger update421: source ownership/retirement417 and native Steam clone identity/root/lifetime419 now verified. Remote visible clone, bone playback, VFX, shared gameplay, upgrades and alternate cast contexts remain open. Existing standing armed body cast regresses cleanly. Continue cosmetic clone presentation; partner interaction edge cases remain deferred. See [chapter35](35-doppelganger-cast-and-clone-lifecycle.md).


Doppelganger update431: separate cosmetic Emily clone now replicates all219 native joints, interpolated root and authoritative lifetime over Steam in two passing tests.430 supplies clear actual-game visual evidence. Native summon/retirement effects, upgrades/alternate contexts/multiple clones and shared outcomes remain. See [chapter35](35-doppelganger-cast-and-clone-lifecycle.md).
