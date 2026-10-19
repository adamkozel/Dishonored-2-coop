# 101. Native player states outside the movement checklist (#102)

The executable's player-state classes include native actions that no row of the 105-item movement checklist covers.
Each one now has a decision: presented on the partner, covered by an existing row, first person only, or not
applicable. Until a state is decided, the hold and grace receivers (1214cg, 1214ch, [chapter 99](99-movement-completion-coverage.md))
keep the partner's authored body on the replicated physical state, so no unclassified state flashes the native pose.

## Decisions

| State (id) | Decision | Evidence |
|---|---|---|
| Flying, Whisper, ShadowHide | Not applicable: not instantiated for either character | Confirmed runtime (player state table, both roles) |
| StoneShroud (86), EatRat (87) | Not applicable: instantiated, but nothing in the game data enters them | Confirmed static (state and controller decls) |
| Stumble (17) | No body motion: `PLAYER_HITREACT_STUMBLE` plays a camera-only reaction | Confirmed static; chapter 99 |
| KillCam (83), Journal (25) | Camera and UI states; the partner keeps the replicated body | Confirmed static; Journal confirmed runtime (1580a) |
| Recoil (40), KnockedDown (16) | Already covered: the 1214cz recoil stagger and the knockdown audit v1441 | Chapter 99 |
| BriskEye (36) | A world outcome (Snap Reaction slows world time), no body animation; belongs with #117 | Confirmed static |
| Spyglass (76), SpyglassZoom (77) | First person only: R3 toggles it natively in both roles; no body clip or scene exists | Confirmed runtime (1581a, 1583a/b, 1584a/b, 1585a/b) |
| WallCrouch (11) | Not reached in play: no controller action enters it; crouching, walking and leaning at a wall never did | Hypothesis: unused (1586a, 1587a) |
| UseTimeShiftDevice (68), UnfoldTimeShiftDevice (69), UnfoldTimeShiftLoop (70) | Presented on the partner like the Heart (below) | Confirmed runtime (1589b, 1590b/c, 1591b, 1592a, 1593b/c) |

## The Timepiece (A Crack in the Slab)

**Native behaviour (confirmed runtime, both roles, takes 1589b and 1590b/c):**
- Each world keeps its own pickup (`pr_a_ld_timeshift_device_pickup_1`): Emily picked hers up first, then Corvo
  (route-bk-timepiece-v1590a; Route-Mission-v1590 runs 'at' and 'use' steps as either role).
- **LT, the shift:** UseTimeShiftDevice 68 for about 1.5 s; `gadget_timeshift_scan_loop` (Scene_4, 500 ms, looping about
  0.9 s), then `gadget_timeshift_scan_teleport` (Scene_2, 500 ms). The player moves from z ≈ −300 to +300 (the past) and
  back.
- **X, the peek:** Undraw 81 with UnfoldTimeShiftDevice 69, then UnfoldTimeShiftLoop 70; `scan_in` (500 ms), then
  `unfold_idle_loop` (3333 ms, looping) until X again, which plays `scan_out` (500 ms).
- Each scene binds the player's left arm and the Timepiece entity.

**Native identities (confirmed runtime, timepiece-research-v1591b):**
- the inventory item `models/weapons/timeshift_device/timeshift_device.inventoryitem`, type 2 (the Heart's left-hand
  type), maximum 1, selected in slot 1;
- the component `arkCpntTimeShiftDevice` (VT `1b53ae8`), owned by `arkDeclCpntTimeShiftDevice` (VT `1b539a8`,
  `models/weapons/timeshift_device/timeshift_device.cpnttimeshiftdevice`, slot 1);
- the entity: `idEntity` (VT `1c87d58`), parent handle 1;
- the state vtables: 68 `1c415f8`, 69 `1c3e878`, 70 `1c3eb98`.

**Presentation (candidate 1214eu, protocol 80):**
- **Reader:** [native_player_timepiece.h](../../outputs/d2-coop-lab/src/native_player_timepiece.h) reads the item
  exactly (twice) and its five owned scenes. The states over Running or Sneaking keep the root motion valid
  (`nativePlayerGroundTimepieceState`). Any other `gadget_timeshift` scene is refused: equip, unequip, the slashes, a
  failed shift, and `scan_loop`'s Scene_2.
- **Wire:** the Timepiece travels on the Heart fields as held kind 2, with variants 4 `scan_in`, 5 `unfold_idle_loop`,
  6 `scan_out`, 7 `scan_loop` and 8 `scan_teleport` ([heart_presentation.h](../../outputs/d2-coop-lab/src/heart_presentation.h)).
  The looping scenes wrap their clock inside one instance. So the peek is sent at elapsed 0, and the shift's raise is
  sent as the time since its instance began, capped at 500 ms.
- **Body:** the partner plays the Heart's authored held-item acting: the idle 336, and stretches of the use clip 339. The
  raise is .47–.60 (the wrist's peak at .60), the peek holds .60, the fold lowers .60–1, and the teleport runs .60–.70.
- **Prop:** [timeshift_prop.def](../../outputs/d2-coop-lab/assets/timeshift_prop.def) is render only (the native
  `timeshift_device.md6`, prop kind 7). It follows the Heart's wrist-relative hand tracks at the same clip times, and the
  partner switches between the Heart and Timepiece props as the traps do.
  [Build-D2TimepiecePropAssets.ps1](../../outputs/d2-coop-lab/Build-D2TimepiecePropAssets.ps1) installs it into the
  isolated asset game; the Steam indices are unchanged.
- **Lab only:** `coop_grant_named models/weapons/timeshift_device/timeshift_device.inventoryitem` selects a Timepiece in
  the left hand outside the bunker. The probe modes `--player-timepiece` and `--watch-timepiece-scene` read the item and
  its scenes.

**Runtime and visual results:**
- coverage-timepiece-v1592a (Dunwall fixture, both roles) passed:
  - the partner presented held kind 2 with the idle, unfold, peek and fold;
  - the prop was attached to the left wrist while held;
  - in the films, Corvo and Emily hold the device at chest height, raise it toward the face for the peek and lower it
    after the fold.
- state-trigger-v1593b (Corvo) and -v1593c (Emily), in A Crack in the Slab after both pickups (confirmed runtime):
  - the partner presented held kind 2 with prop kind 7 throughout;
  - the unfold and peek showed variants 0, 4 and 5, and the fold 5 and 6;
  - both real shifts (z −300 to +300 and back) showed 0, 7 and 8, the raise then the teleport;
  - in Emily's film, Corvo stands in view and is gone once he shifts to the other time.

**Gaps:**
- The prop shows the model's bind pose (petals out); its fold and unfold mechanism is not animated on the partner (as with
  the Heart's internal animation).
- The equip and unequip scenes are not presented.
- A shift moves the source 600 m to the other time, where the partner (in its own time) does not see it. This matches
  single-player geometry.
- The release's decl recipes (#30) must include `timeshift_prop.def` ([#122](https://github.com/adamkozel/Dishonored-2-coop/issues/122), stage 7).
