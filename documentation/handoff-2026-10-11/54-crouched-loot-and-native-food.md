# Crouched loot and native food continuity

This continuation uses the existing native first-person interactions and existing remote full-body bank. It does not add a new reach/eating animation or claim shared inventory, healing or item removal. Fine acting remains deferred under the user's accepted priority.

## Confirmed missing state692

[Corvo692](../../work/natural-moveset/native-interactions-corvo-crouch-coin-v692/) collected `pickups_coin_emily_lowvalue_4` while manually crouched on the accessible desktop. Compact read-only monitoring captured native Sneaking6/VT1c84258 with LootObject47/VT1c7bcc8, both with and without Undraw81/VT1c7df88. Raw stance remained1. The old classifier reported invalid motion during both loot combinations. The exact source coin disappeared and the native Coin notification was visible. Both source and observer contact sheets were inspected; the observer's upper body was cropped, so this baseline is not a full visual acceptance.

## Candidate693

[Pinned archive](../../work/movement-integration/candidate-crouch-loot-v693/identity.json). DLL `E3DAB14778127A9F3A12432A0FB69D6A2E7E866627BC12F62C7B616A7F70E437`; probe `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Previous681 remains preserved. Protocol69 and bank67/357 are unchanged.

`nativePlayerGroundLootStates` accepts only the verified standing Running0/raw0 or crouched Sneaking6/raw1 alongside exactly one LootObject47 and optional Undraw81. Movement returns the matching physical stance, and action remains idle while native code owns pickup behavior. Mode, logic, exact class, duplicate state, air/combat and automatic-crouch exclusions remain. Tests explicitly reject mismatched physical stance/class combinations and preserve standing behavior.

Files:

- [Native state classification](../../outputs/d2-coop-lab/src/native_player_states.h)
- [Action classification](../../outputs/d2-coop-lab/src/native_player_action.h)
- [Ownership/classification tests](../../outputs/d2-coop-lab/src/world_identity_tests.cpp)
- [Build](../../work/movement-integration/Build-CrouchLoot-v693.ps1)
- [Isolated deployment](../../work/movement-integration/Deploy-CrouchLoot-v693.ps1)
- [Current interaction harness694](../../work/natural-moveset/Test-NativeInteractions-v694.ps1)
- [Behavior audit698](../../work/movement-integration/audit_native_pickups_v698.py)

Build, ownership, action, resume-policy, proxy smoke and actual357-clip bank tests pass. Runtime acceptance requires the later independent audit receipts, not merely execution PASS.

## Fixture and controller lessons

The initial attempts are retained and excluded from pickup coverage:

| Take | Observation |
|---|---|
|686|Crouched eye below the tall desk; no pickup prompt, coin remained. Native Sneaking6 + Undraw81 alone is not proof of looting. Both sheets inspected.|
|687|Lower coin fixture was obstructed/unreachable from the chosen coordinates; measured camera aim did not settle. No recording or accepted pickup.|
|688|Target angle settled but camera faced furniture/wall, outside useful interaction reach. Coin remained. Source sheet inspected; no pickup acceptance.|
|689|Post-teleport player had not settled to standing before crouch input. Precondition stopped the test.|
|690|Desktop placement and crouching worked, but fine yaw correction oscillated at steep pitch. No pickup recording.|
|692|Damped feedback aiming settled; actual crouched native coin pickup produced the missing state combinations.|
|694|First post-deployment stream startup failed before capture: native state age266ms, peer root age250ms. Both players themselves were valid. This is retained as a transport/readiness failure; both inventories, health and14 protected files restored.|

[Aim helper691](../../work/natural-moveset/Set-NativeInteractionAim-v691.ps1) uses small above-deadzone10000-axis/100ms pulses for final yaw alignment. The prior16000 minimum overcorrected close targets because the native eye position also changes as the body turns. Larger corrections remain bounded and use read-only native view feedback. No camera memory is written. Fifty iterations are a maximum, not a target delay. The harness waits for exact native Running0 before sending crouch, and checks Sneaking6 afterward. Desktop source view height69.5 is intentional; ordinary food targeting keeps68.1. Observer pitch-12 includes the elevated crouched body.

Food testing targets the actual known grapes `pickups_fruit_grape_03_3`, rather than the neighboring apple selected in the original plan. The harness records health before and after: native food may legitimately restore source health; partner health must remain unchanged and checkpoint cleanup must restore both baselines. Exact target removal and native state traces remain required.

## Current verification

Both-role native classification is verified. [Emily695](../../work/natural-moveset/native-interactions-emily-crouch-coin-v695/) and [Corvo699](../../work/natural-moveset/native-interactions-corvo-crouch-coin-v699/) capture the exact crouched loot combinations with valid motion1 throughout. The source coins disappear, both Steam streams remain ready, and the source and remote stance remain crouched. Both source/observer sheets were inspected.

The [crouched-loot audit698](../../work/movement-integration/crouch-loot-audit-v698.json) is deliberately **not an overall pass:21 checks pass and1 fails**. Emily's127 observations and892 compact native samples pass all11 checks, minimum body weight0.999999. Corvo's124 observations and893 native samples verify the classification but body weighting starts at0.26822 and later dips to0.522453 around10.174s. The later eligibility trace records mask16, native semantic age250ms, with valid crouch and otherwise valid presentation. The classifier fix does not solve this transport/animation-continuity issue. The report retains all failed checks and individual dip observations; it is not weakened to report a green result.

[Corvo food696](../../work/natural-moveset/native-interactions-corvo-food-v696/) and [Emily food697](../../work/natural-moveset/native-interactions-emily-food-v697/) pass all22 checks in [food audit698](../../work/movement-integration/food-continuity-audit-v698.json). Both use native Running0 + LootObject47 with optional Undraw81, remove the exact grapes, and restore source health56->61 while peer health stays56. Both Steam streams stay ready; minimum body weights are0.999892 and0.999998. The observations contain125/127 body samples and893/891 compact native samples. Both source/observer sheets were inspected: native grapes prompt, consumption/removal and retained remote standing body are visible. The observer's fruit remains present, explicitly demonstrating that shared world removal is still unfinished. No eating gesture is added by this change.

Every completed or failed attempt above restored both full inventories, player/partner health and14 protected files. Original Steam assets remain untouched. The earlier682 stale-packet interruption and694 startup failure remain transport robustness work; the strict freshness threshold has not been relaxed to obtain a pass.

Next investigate bounded same-identity recovery and visual continuity after transient transport gaps, using the actual250ms trace. Preserve immediate rejection of changed actors/worlds/sessions, loads, deaths and explicit user stop. Then continue broader missing movement and shared-world requirements; do not rerun unreachable pickup fixtures or spend time polishing these coarse poses.
