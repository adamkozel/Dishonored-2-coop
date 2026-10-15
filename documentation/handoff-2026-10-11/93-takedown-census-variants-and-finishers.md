# Takedown census, variants and finishers

Stage 3, card #11. Candidates 1214b–1214f, census 1253/1255, takes 1248k–1248n, 1254a–b and 1256a–c.

## Why a census

Take 1248j's host lethal ran `assassination_back` (Corvo 1933 ms), not the fast-back lethal the reader knew, so the partner showed nothing (candidate 1214b added it). In take 1248k, Emily's version of the same scene ran 2200 ms and 1214b refused it.

The native scene declarations explain this (confirmed static):
- every takedown scene has one subscene per player character, with its own duration;
- the subscene names vary (`Npc`, `npc`, `NPC`, `Scene_2`, `small`, `Hound`...).

One-off checks per scene don't scale, so the reader now uses a generated table.

## Census 1253 and 1255 (confirmed static)

[generate_takedown_table_v1255.py](../../work/movement-integration/generate_takedown_table_v1255.py) reads the read-only scene exports under `midnight/gameplay/player/base/{assassination,choke,fatalities}/`. For each subscene it records the name and the duration of the main player clip. It writes [the census](../../work/movement-integration/native-takedown-scenes-v1255.json) and generates `native_takedown_scenes.h`.

- **156 scenes** in the census; **117** are in the table (209 exact subscene/duration pairs).
- **Excluded:** carry-corpse follow-ups and drop-ready poses (body handling and airborne phases, not takedowns), and the fatality double kill (it binds no AI group).
- **Ids:** 1–6 keep their earlier wire values: choke back/loop/win/combat_lose, fast-back lethal, and `assassination_back`. Ids 7–89 are census 1253. The 28 fatalities (sword-combat finishers, read-only export `native-fatalities-v1255`) are ids 90–117.
- **Kinds:**
  - 1 choke entry (a hold follows);
  - 2 hold loop;
  - 3 knockout (`choke_win`, or a single-scene drop/slide/window/shadow-walk/knockdown choke);
  - 4 lost or cancelled (presented only);
  - 5 lethal (fatalities included).

## How the table is used (1214c–1214f)

- **Reader:** admits a scene only by exact asset, subscene name and duration.
  - Ids 1–6 keep their observed states: Choke41 and Assassination13.
  - Other takedowns accept either state, including the sliding physical stance 5 (take 1254a).
  - Generic right-arm finishers run in the sword attack state (Slash39 over Running0, takes 1248m and 1254a). Four-binding finishers run in Assassination13 (take 1256a) and may carry one empty binding slot (take 1256b).
- **Presentation:** each scene uses the existing clip of its kind (choke entry, loop, win or lose, or the fast-back lethal), normalized to the native scene clock. The game ships no third-person clips for the other scenes; [chapter 45](45-native-close-range-kick.md) normalized the kick the same way. This is functional coverage; polish is left for the animator.
- **Client-sourced takedowns on host NPCs:** every entry, loop, knockout and lethal id is a paired hold variant. A knockout scene asks the host for the native knockout when it begins. 1214c still capped that request at id 3, so the client slide choke never asked; 1214d fixed it.
- **Wire:** sword state follows the lethal kind. Durations are validated against the table.

## Runtime results

| Take | Candidate | Case | Result |
|---|---|---|---|
| 1248k | 1214b | host lethal `assassination_back` (Corvo 1933 ms) | presented as id 6, dead in both worlds, 0.1 m apart |
| 1248k | 1214b | client lethal `assassination_back` (Emily 2200 ms) | refused by 1214b: the reason for the census |
| 1248l | 1214c | rear chokes, host `assassination_back` | pass: 0.15 m / 0.0 m / 0.0 m |
| 1248n | 1214c | client fast-back lethal | pass, 0.0 m |
| 1254a | 1214c | side choke, client (`choke_left` → loop → win) | ids 68/2/3 presented, unconscious in both worlds, 0.71 m |
| 1254a | 1214c | front choke, host (`choke_front` → loop → win) | ids 62/2/3, unconscious, 0.0 m |
| 1254a | 1214c | drop lethal, both roles (`assassination_drop_back`) | id 19, dead in both worlds, 0.0 m |
| 1254a | 1214c | slide choke, host (`choke_slide_back`) | id 73, unconscious, 0.0 m |
| 1254a | 1214c | slide lethal, both roles | not presented: stance 5 refused (fixed in 1214d) |
| 1254a | 1214c | slide choke, client | host guard stayed conscious: knockout request capped (fixed in 1214d) |
| 1256a | 1214d | host finisher `fatality_leftarm_medium_a` | ran in Assassination13; 1214d looked only in Slash39 (fixed in 1214e) |
| 1256b | 1214e | host finisher `fatality_torso_medium_a` | refused: five bindings, one of them empty (fixed in 1214f) |
| 1256b | 1214e | client finisher `fatality_generic_right` | admitted on the source (id 92), not presented by the host; the source dropped the sample for lacking a combat scene (fixed in 1214g) |
| 1254b | 1214e | drop choke, both roles (`choke_drop_back`) | id 58, unconscious in both worlds, 0.0/0.01 m |
| 1254b | 1214e | drop lethal, both roles | id 19, dead in both worlds, 0.0 m |
| 1254b | 1214e | slide choke, both roles | id 73, unconscious in both worlds, 0.0/0.29 m (client knockout request now sent) |
| 1254b | 1214e | slide lethal, both roles (`assassination_slide_back`) | id 33 presented at stance 5, dead in both worlds, 0.0 m |
| 1254b | 1214e | side chokes, Emily (`choke_left`, `choke_right`) | ids 68/69, then loop and win, 0.72/0.08 m |
| 1256c–e | 1214f/g | host finishers `fatality_torso_fast_b`, `fatality_head_medium_a`, `fatality_rightleg_fast_a` | ids 113/96/109, dead in both worlds, 0.0 m |
| 1256f–g | 1214g | client finishers `fatality_generic_right`, `fatality_head_fast_c` | ids 92/95, dead in both worlds, 0.0 m |

Side and front lethals by a standing attacker were noticed by the guard and became sword fights (fixture limitation, not a reader refusal). In one of them the host's strike played a finisher.

## Shared outcomes and visuals

[Audit 1257](../../work/movement-integration/takedown-variants-audit-v1257.json) records all of the above.

**Bound in both source roles (confirmed runtime):** `aerial_drop_kill`, `aerial_nonlethal_takedown` and `sliding_takedown`. For each, the partner presents the scene and both worlds agree on the outcome, with victims 0.0–0.29 m apart.

**Visual acceptance (functional):** contact sheets from both windows show the partner dropping or sliding onto the guard, then stabbing, or holding and lowering him, with the body ending at their feet. The motion is the existing fast-back stab or choke donor normalized to the native scene clock. Drop- and slide-specific motion is left for the animator.

Sword finishers (the 28 census-1255 fatalities) are presented as paired lethal scenes in both roles, and the death is shared (`death_variants` notes).

**Still partial:**
- side and front lethals (the guard notices a standing attacker);
- Corvo's side chokes and Emily's front choke;
- window and wall variants (`wall_and_window_variants`);
- enemy body sizes (`enemy_body_size_variants`);
- Emily's 2200 ms `assassination_back`, admitted statically but not yet seen at runtime.
