# darkvision-native-findings-v3.json

Original: [work/movement-integration/darkvision-native-findings-v3.json](<../../../../work/movement-integration/darkvision-native-findings-v3.json>)

SHA-256: `FD3AF9CF74D2AF3267604842F18E6FE50302439BA8BC1950A87606174B14EB34`; 5054 bytes.

## Original saved record

````json
{
  "executable_sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "verified_dll_sha256": "96C1FB32D85ED33B2F414955CF53E260CE2016523DD54582B98B1E25875CAD62",
  "bank": "D2MOVE26 /197 clips; prior194payloads unchanged",
  "protocol": "Native Steam semantic action version20, same236bytes, Dark Vision10/933ms with native holstered casting",
  "confirmed_runtime": [
    "Exact native quantity acquisition creates Dark Vision equipment from the zero-quantity item; equipment has1component, local player has88components.",
    "Both Corvo and Emily use power_darkvision_cast.midnightscene, Scene_1, 933ms, state3, with one reciprocal left-arm binding. Observed standing casts retain Running0.",
    "Native LT toggles the separate vision manager on and off. Source POV shows native first-person hand motion, UI and orange reveal. Observer vision manager remains inactive and ordinary rendering remains visible.",
    "Native unequip stops selection/component but does not immediately stop the active vision reveal. The isolated cleanup must wait for its own native lifetime, then restore quantity. Thirty-second bounded cleanup passed on native-darkvision-corvo-v4; prior five-second timeout failed and was preserved.",
    "Four standing integration takes (Corvo/Emily, v1/v2) pass8checks each. V2 closer views include complete body, native sword, boots and recovery; two separately identified native casts correspond to two remote events.",
    "Native inventory quantities, equipment cleanup, player56/avatar40 health and protected original Steam/save hashes are preserved. Exact runtime acquisition discovery/entity metadata is reset by isolated checkpoint reload.",
    "Native crouched casting uses Sneaking6/VT1c84258 and raw physics stance1, optionally alongside UsePower63; same933ms scene and one left-arm binding. Both roles can cast with their sword holstered.",
    "Fourteen v26 ground-context takes pass134checks:6stationary and8forward/backward moving. Both characters, standing/crouched, sword drawn/holstered. Twenty-eight actual source/observer videos indexed.",
    "Full-body crouched and weaponless source adaptations preserve bone lengths and native attachments. Moving casts retain distance-driven gait and network root ownership."
  ],
  "evidence": [
    "work/natural-moveset/native-darkvision-corvo-v4/result.json",
    "work/natural-moveset/darkvision-corvo-v25-v1/result.json",
    "work/natural-moveset/darkvision-emily-v25-v1/result.json",
    "work/natural-moveset/darkvision-corvo-v25-v2/result.json",
    "work/natural-moveset/darkvision-emily-v25-v2/result.json",
    "work/natural-moveset/darkvision-corvo-crouched-empty-moving-v26-v1/result.json",
    "work/natural-moveset/darkvision-corvo-crouched-empty-v26-v1/result.json",
    "work/natural-moveset/darkvision-corvo-crouched-moving-v26-v1/result.json",
    "work/natural-moveset/darkvision-corvo-crouched-v26-v1/result.json",
    "work/natural-moveset/darkvision-corvo-empty-moving-v26-v1/result.json",
    "work/natural-moveset/darkvision-corvo-empty-v26-v1/result.json",
    "work/natural-moveset/darkvision-corvo-moving-v26-v3/result.json",
    "work/natural-moveset/darkvision-emily-crouched-empty-moving-v26-v1/result.json",
    "work/natural-moveset/darkvision-emily-crouched-empty-v26-v2/result.json",
    "work/natural-moveset/darkvision-emily-crouched-moving-v26-v1/result.json",
    "work/natural-moveset/darkvision-emily-crouched-v26-v1/result.json",
    "work/natural-moveset/darkvision-emily-empty-moving-v26-v1/result.json",
    "work/natural-moveset/darkvision-emily-empty-v26-v1/result.json",
    "work/natural-moveset/darkvision-emily-moving-v26-v1/result.json"
  ],
  "selected_frame_visual_review": "Fourteen remote temporal contact sheets inspected, plus both7-case source POV grids showing native reveal on/off. Native source and observer videos retain actual timestamps; they are not frame-synchronized. One supporting source view averages23.92Hz and is labelled; all observer takes exceed28Hz. Fine contact and continuous visual approval remain.",
  "remaining": [
    "Sideways/run/air/slide/water and upgrades",
    "Pause, death, reload, map changes and disconnect lifecycle",
    "Continuous visual approval, fine finger contact, crouched sword-floor clearance and foot sliding"
  ],
  "complete": false,
  "static_effect_scope": {
    "manager_start": "14049ed20",
    "manager_stop": "14049eff0",
    "resource": "models/effects/gameplay/powers/dark_vision/player/dvision_ply_pp_01.fx",
    "confirmed": "Screen particle marked disabled; fullscreen quad/distortion and native vision audio. Cast scene contains animation and rumble; no independent FX track. Native source scene/manager controls these effects.",
    "not_proven": "No independent world-visible Dark Vision effect has been confirmed; do not broadcast viewer-only overlay.",
    "evidence": "darkvision-native-manager-methods-v1.json and exact asset batch movement-darkvision-effects-v1"
  }
}
````
