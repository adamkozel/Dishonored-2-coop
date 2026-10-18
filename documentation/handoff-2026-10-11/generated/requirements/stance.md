# stance: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-18T19:25:10.745598+02:00. All fields below retain the catalog wording.

## crouch_idle

Visual sweep 1260: accepted (corvo accept, emily accept). Crouch idles hold a stable low pose for both characters, in the baseline and in the better-balanced candidate. The auto-crouch observer view (v653) was insufficient: Corvo is hidden behind the display case. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending. 651/656: exact native AutoCrouch10/raw2, low confined mantle8 next10 and fast333ms sword recovery now bind to existing crouch/low-mantle presentation. Corvo653/655 and Emily654 pass36 scoped checks; both streams remain ready, held crouch and final standing fully weighted. Original635 and all357 clips preserved. Clear654/655 observer sheets inspected; foot/sill contact and brief entry/retreat blend dips remain polish/continuity work. No open-window, high confined mantle or auto-crouch combat/power coverage claimed.

````json
{
  "id": "crouch_idle",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_manual_and_automatic_crouch_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "crouch idle",
  "motions": [
    "crouch_idle"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouch idles hold a stable low pose for both characters, in the baseline and in the better-balanced candidate. The auto-crouch observer view (v653) was insufficient: Corvo is hidden behind the display case. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending. 651/656: exact native AutoCrouch10/raw2, low confined mantle8 next10 and fast333ms sword recovery now bind to existing crouch/low-mantle presentation. Corvo653/655 and Emily654 pass36 scoped checks; both streams remain ready, held crouch and final standing fully weighted. Original635 and all357 clips preserved. Clear654/655 observer sheets inspected; foot/sill contact and brief entry/retreat blend dips remain polish/continuity work. No open-window, high confined mantle or auto-crouch combat/power coverage claimed.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "crouch-balance-corvo-baseline-v1.mp4",
    "crouch-balance-corvo-candidate-v1.mp4",
    "crouch-balance-emily-baseline-v1.mp4",
    "crouch-balance-emily-candidate-v1.mp4",
    "native-window-corvo-v649/host-window-open-traverse.mp4",
    "native-window-corvo-v649/observer.mp4",
    "native-window-emily-v650/client-window-open-traverse.mp4",
    "native-window-emily-v650/observer.mp4",
    "native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v652/observer.mp4",
    "native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v653/observer.mp4",
    "native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4",
    "native-auto-crouch-emily-v654/observer.mp4",
    "native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v655/observer.mp4"
  ],
  "evidence": [
    "crouch-balance-corvo-baseline-v1/result.json",
    "crouch-balance-corvo-candidate-v1/result.json",
    "crouch-balance-emily-baseline-v1/result.json",
    "crouch-balance-emily-candidate-v1/result.json",
    "native-auto-crouch-corvo-v653/result.json",
    "native-auto-crouch-emily-v654/result.json",
    "native-auto-crouch-corvo-v655/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1318e/result.json",
    "coverage-v1319e/result.json"
  ],
  "complete": true,
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Held crouch while still between crouched moves, before and after crouched leans, and through crouched in-place turns (stance mix >= .96); both roles.",
    "takes": [
      {
        "take": "coverage-v1318e",
        "cases": [
          "host-crouch8",
          "client-crouch8",
          "host-turns",
          "client-turns"
        ]
      },
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-stance",
          "client-stance",
          "host-lean",
          "client-lean"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1318c/host-crouch8-observer.mp4",
        "verdict": "Corvo holds a low crouch between crouched moves."
      },
      {
        "video": "coverage-v1319c/client-stance-observer.mp4",
        "verdict": "Emily holds a planted crouch while still."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: crouch-balance-corvo-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1/result.json>)
- [evidence: crouch-balance-corvo-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1/result.json>)
- [evidence: crouch-balance-emily-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1/result.json>)
- [evidence: crouch-balance-emily-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1/result.json>)
- [evidence: native-auto-crouch-corvo-v653/result.json](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/result.json>)
- [evidence: native-auto-crouch-emily-v654/result.json](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/result.json>)
- [evidence: native-auto-crouch-corvo-v655/result.json](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1318e/result.json](<../../../../work/natural-moveset/coverage-v1318e/result.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: crouch-balance-corvo-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-corvo-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1.mp4>)
- [in_game_recordings: native-window-corvo-v649/host-window-open-traverse.mp4](<../../../../work/natural-moveset/native-window-corvo-v649/host-window-open-traverse.mp4>)
- [in_game_recordings: native-window-corvo-v649/observer.mp4](<../../../../work/natural-moveset/native-window-corvo-v649/observer.mp4>)
- [in_game_recordings: native-window-emily-v650/client-window-open-traverse.mp4](<../../../../work/natural-moveset/native-window-emily-v650/client-window-open-traverse.mp4>)
- [in_game_recordings: native-window-emily-v650/observer.mp4](<../../../../work/natural-moveset/native-window-emily-v650/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v652/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v652/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v653/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/observer.mp4>)
- [in_game_recordings: native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-emily-v654/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v655/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/observer.mp4>)
## crouch_8_directions

Visual sweep 1260: accepted (corvo accept, emily accept). Crouch-walking reads in forward, lateral and toward-camera directions for both, with a correct low posture. All 8 directions are not shown individually. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending.

````json
{
  "id": "crouch_8_directions",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "crouch 8 directions",
  "motions": [
    "mixamo_crouched_walking_posture100",
    "crouch_forward",
    "crouch_walk_forward",
    "crouch_walk_forward_left",
    "crouch_walk_left",
    "crouch_walk_back_left",
    "crouch_walk_back",
    "crouch_walk_back_right",
    "crouch_walk_right",
    "crouch_walk_forward_right"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouch-walking reads in forward, lateral and toward-camera directions for both, with a correct low posture. All 8 directions are not shown individually. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "unbound-presentation-v559c-host.mp4",
    "unbound-presentation-v559c-client.mp4",
    "unbound-presentation-v559b-host.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1318e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Crouched walks in all 8 direction sectors presented crouched in both roles.",
    "takes": [
      {
        "take": "coverage-v1318e",
        "cases": [
          "host-crouch8",
          "client-crouch8"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1318c/host-crouch8-observer.mp4",
        "verdict": "Corvo crouch-walks in a low stepping posture in every heading."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1318e/result.json](<../../../../work/natural-moveset/coverage-v1318e/result.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: unbound-presentation-v559c-host.mp4](<../../../../work/natural-moveset/unbound-presentation-v559c-host.mp4>)
- [in_game_recordings: unbound-presentation-v559c-client.mp4](<../../../../work/natural-moveset/unbound-presentation-v559c-client.mp4>)
- [in_game_recordings: unbound-presentation-v559b-host.mp4](<../../../../work/natural-moveset/unbound-presentation-v559b-host.mp4>)
## stand_to_crouch

Visual sweep 1260: accepted (corvo accept, emily accept). Stand-to-crouch transitions blend smoothly over ~0.5 s for both characters, while walking, turning and stationary. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending. 651/656: exact native AutoCrouch10/raw2, low confined mantle8 next10 and fast333ms sword recovery now bind to existing crouch/low-mantle presentation. Corvo653/655 and Emily654 pass36 scoped checks; both streams remain ready, held crouch and final standing fully weighted. Original635 and all357 clips preserved. Clear654/655 observer sheets inspected; foot/sill contact and brief entry/retreat blend dips remain polish/continuity work. No open-window, high confined mantle or auto-crouch combat/power coverage claimed.

````json
{
  "id": "stand_to_crouch",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_manual_and_automatic_crouch_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "stand to crouch",
  "motions": [
    "stand_to_crouch"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Stand-to-crouch transitions blend smoothly over ~0.5 s for both characters, while walking, turning and stationary. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending. 651/656: exact native AutoCrouch10/raw2, low confined mantle8 next10 and fast333ms sword recovery now bind to existing crouch/low-mantle presentation. Corvo653/655 and Emily654 pass36 scoped checks; both streams remain ready, held crouch and final standing fully weighted. Original635 and all357 clips preserved. Clear654/655 observer sheets inspected; foot/sill contact and brief entry/retreat blend dips remain polish/continuity work. No open-window, high confined mantle or auto-crouch combat/power coverage claimed.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "integrated-sprint-corvo-v2.mp4",
    "integrated-sprint-emily-v2.mp4",
    "integrated-equipment-close-corvo-v3.mp4",
    "integrated-equipment-close-emily-v3.mp4",
    "turn-stance-corvo-knee-v4.mp4",
    "turn-stance-corvo-unarmed-knee-v4.mp4",
    "turn-stance-emily-knee-v4.mp4",
    "turn-stance-emily-unarmed-knee-v4.mp4",
    "crouch-balance-corvo-baseline-v1.mp4",
    "crouch-balance-corvo-candidate-v1.mp4",
    "crouch-balance-emily-baseline-v1.mp4",
    "crouch-balance-emily-candidate-v1.mp4",
    "native-window-corvo-v649/host-window-open-traverse.mp4",
    "native-window-corvo-v649/observer.mp4",
    "native-window-emily-v650/client-window-open-traverse.mp4",
    "native-window-emily-v650/observer.mp4",
    "native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v652/observer.mp4",
    "native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v653/observer.mp4",
    "native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4",
    "native-auto-crouch-emily-v654/observer.mp4",
    "native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v655/observer.mp4"
  ],
  "evidence": [
    "crouch-balance-corvo-baseline-v1/result.json",
    "crouch-balance-corvo-candidate-v1/result.json",
    "crouch-balance-emily-baseline-v1/result.json",
    "crouch-balance-emily-candidate-v1/result.json",
    "native-auto-crouch-corvo-v653/result.json",
    "native-auto-crouch-emily-v654/result.json",
    "native-auto-crouch-corvo-v655/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1319e/result.json"
  ],
  "complete": true,
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Stand-to-crouch changes while still, while walking and while turning; at least three presented per role (four in 1319c), settling; crouching during a lean keeps the lean (probe 1322a).",
    "takes": [
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-stance",
          "client-stance"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1319c/client-stance-observer.mp4",
        "verdict": "Emily's crouches blend continuously, still and moving."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: crouch-balance-corvo-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1/result.json>)
- [evidence: crouch-balance-corvo-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1/result.json>)
- [evidence: crouch-balance-emily-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1/result.json>)
- [evidence: crouch-balance-emily-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1/result.json>)
- [evidence: native-auto-crouch-corvo-v653/result.json](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/result.json>)
- [evidence: native-auto-crouch-emily-v654/result.json](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/result.json>)
- [evidence: native-auto-crouch-corvo-v655/result.json](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: integrated-sprint-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-corvo-v2.mp4>)
- [in_game_recordings: integrated-sprint-emily-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-emily-v2.mp4>)
- [in_game_recordings: integrated-equipment-close-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-corvo-v3.mp4>)
- [in_game_recordings: integrated-equipment-close-emily-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-emily-v3.mp4>)
- [in_game_recordings: turn-stance-corvo-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-knee-v4.mp4>)
- [in_game_recordings: turn-stance-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-unarmed-knee-v4.mp4>)
- [in_game_recordings: crouch-balance-corvo-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-corvo-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1.mp4>)
- [in_game_recordings: native-window-corvo-v649/host-window-open-traverse.mp4](<../../../../work/natural-moveset/native-window-corvo-v649/host-window-open-traverse.mp4>)
- [in_game_recordings: native-window-corvo-v649/observer.mp4](<../../../../work/natural-moveset/native-window-corvo-v649/observer.mp4>)
- [in_game_recordings: native-window-emily-v650/client-window-open-traverse.mp4](<../../../../work/natural-moveset/native-window-emily-v650/client-window-open-traverse.mp4>)
- [in_game_recordings: native-window-emily-v650/observer.mp4](<../../../../work/natural-moveset/native-window-emily-v650/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v652/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v652/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v653/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/observer.mp4>)
- [in_game_recordings: native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-emily-v654/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v655/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/observer.mp4>)
## crouch_to_stand

Visual sweep 1260: accepted (corvo accept, emily accept). Crouch-to-stand transitions are continuous and land in a planted idle for both characters, including on top of the desk after a crouched mantle. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending. 651/656: exact native AutoCrouch10/raw2, low confined mantle8 next10 and fast333ms sword recovery now bind to existing crouch/low-mantle presentation. Corvo653/655 and Emily654 pass36 scoped checks; both streams remain ready, held crouch and final standing fully weighted. Original635 and all357 clips preserved. Clear654/655 observer sheets inspected; foot/sill contact and brief entry/retreat blend dips remain polish/continuity work. No open-window, high confined mantle or auto-crouch combat/power coverage claimed.

````json
{
  "id": "crouch_to_stand",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_manual_and_automatic_crouch_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "crouch to stand",
  "motions": [
    "crouch_to_stand"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouch-to-stand transitions are continuous and land in a planted idle for both characters, including on top of the desk after a crouched mantle. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending. 651/656: exact native AutoCrouch10/raw2, low confined mantle8 next10 and fast333ms sword recovery now bind to existing crouch/low-mantle presentation. Corvo653/655 and Emily654 pass36 scoped checks; both streams remain ready, held crouch and final standing fully weighted. Original635 and all357 clips preserved. Clear654/655 observer sheets inspected; foot/sill contact and brief entry/retreat blend dips remain polish/continuity work. No open-window, high confined mantle or auto-crouch combat/power coverage claimed.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "integrated-sprint-corvo-v2.mp4",
    "integrated-sprint-emily-v2.mp4",
    "integrated-equipment-close-corvo-v3.mp4",
    "integrated-equipment-close-emily-v3.mp4",
    "integrated-crouch-mantle-corvo-v3.mp4",
    "integrated-crouch-mantle-emily-v3.mp4",
    "turn-stance-corvo-knee-v4.mp4",
    "turn-stance-corvo-unarmed-knee-v4.mp4",
    "turn-stance-emily-knee-v4.mp4",
    "turn-stance-emily-unarmed-knee-v4.mp4",
    "crouch-balance-corvo-baseline-v1.mp4",
    "crouch-balance-corvo-candidate-v1.mp4",
    "crouch-balance-emily-baseline-v1.mp4",
    "crouch-balance-emily-candidate-v1.mp4",
    "native-window-corvo-v649/host-window-open-traverse.mp4",
    "native-window-corvo-v649/observer.mp4",
    "native-window-emily-v650/client-window-open-traverse.mp4",
    "native-window-emily-v650/observer.mp4",
    "native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v652/observer.mp4",
    "native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v653/observer.mp4",
    "native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4",
    "native-auto-crouch-emily-v654/observer.mp4",
    "native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4",
    "native-auto-crouch-corvo-v655/observer.mp4"
  ],
  "evidence": [
    "crouch-balance-corvo-baseline-v1/result.json",
    "crouch-balance-corvo-candidate-v1/result.json",
    "crouch-balance-emily-baseline-v1/result.json",
    "crouch-balance-emily-candidate-v1/result.json",
    "native-auto-crouch-corvo-v653/result.json",
    "native-auto-crouch-emily-v654/result.json",
    "native-auto-crouch-corvo-v655/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1319e/result.json"
  ],
  "complete": true,
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Crouch-to-stand changes while still, while walking and while turning; at least three per role (four in 1319c), settling standing at the end.",
    "takes": [
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-stance",
          "client-stance"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1319c/client-stance-observer.mp4",
        "verdict": "Emily rises continuously into a planted stand."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: crouch-balance-corvo-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1/result.json>)
- [evidence: crouch-balance-corvo-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1/result.json>)
- [evidence: crouch-balance-emily-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1/result.json>)
- [evidence: crouch-balance-emily-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1/result.json>)
- [evidence: native-auto-crouch-corvo-v653/result.json](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/result.json>)
- [evidence: native-auto-crouch-emily-v654/result.json](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/result.json>)
- [evidence: native-auto-crouch-corvo-v655/result.json](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: integrated-sprint-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-corvo-v2.mp4>)
- [in_game_recordings: integrated-sprint-emily-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-emily-v2.mp4>)
- [in_game_recordings: integrated-equipment-close-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-corvo-v3.mp4>)
- [in_game_recordings: integrated-equipment-close-emily-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-emily-v3.mp4>)
- [in_game_recordings: integrated-crouch-mantle-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-crouch-mantle-corvo-v3.mp4>)
- [in_game_recordings: integrated-crouch-mantle-emily-v3.mp4](<../../../../work/natural-moveset/integrated-crouch-mantle-emily-v3.mp4>)
- [in_game_recordings: turn-stance-corvo-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-knee-v4.mp4>)
- [in_game_recordings: turn-stance-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-unarmed-knee-v4.mp4>)
- [in_game_recordings: crouch-balance-corvo-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-corvo-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1.mp4>)
- [in_game_recordings: native-window-corvo-v649/host-window-open-traverse.mp4](<../../../../work/natural-moveset/native-window-corvo-v649/host-window-open-traverse.mp4>)
- [in_game_recordings: native-window-corvo-v649/observer.mp4](<../../../../work/natural-moveset/native-window-corvo-v649/observer.mp4>)
- [in_game_recordings: native-window-emily-v650/client-window-open-traverse.mp4](<../../../../work/natural-moveset/native-window-emily-v650/client-window-open-traverse.mp4>)
- [in_game_recordings: native-window-emily-v650/observer.mp4](<../../../../work/natural-moveset/native-window-emily-v650/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v652/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v652/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v652/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v653/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v653/observer.mp4>)
- [in_game_recordings: native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/client-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-emily-v654/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-emily-v654/observer.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/host-auto-crouch-sill.mp4>)
- [in_game_recordings: native-auto-crouch-corvo-v655/observer.mp4](<../../../../work/natural-moveset/native-auto-crouch-corvo-v655/observer.mp4>)
## crouched_turns

Visual sweep 1260: accepted (corvo accept, emily accept). Crouched in-place turns hold the crouch with stepping feet for both characters. The only defects are ~0.2 s upright flashes with heading snaps at each trial start, which coincide with the harness setviewpos resets (confirmed static). Their cause is a hypothesis, so they are worth confirming in a take without per-trial teleports. Saved both-role armed and unarmed tests passed continuous left/right standing/crouched/standing turns and recovery with stationary native root. This is not full per-angle90/180 acceptance or fresh regression on the current build. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.

````json
{
  "id": "crouched_turns",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_stance_turns_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "crouched turns",
  "motions": [
    "crouch_turn_left45",
    "crouch_turn_left90",
    "crouch_turn_left180",
    "crouch_turn_right45",
    "crouch_turn_right90",
    "crouch_turn_right180"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouched in-place turns hold the crouch with stepping feet for both characters. The only defects are ~0.2 s upright flashes with heading snaps at each trial start, which coincide with the harness setviewpos resets (confirmed static). Their cause is a hypothesis, so they are worth confirming in a take without per-trial teleports. Saved both-role armed and unarmed tests passed continuous left/right standing/crouched/standing turns and recovery with stationary native root. This is not full per-angle90/180 acceptance or fresh regression on the current build. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "crouch-turn-corvo-knee-v4.mp4",
    "crouch-turn-corvo-unarmed-knee-v4.mp4",
    "crouch-turn-emily-knee-v4.mp4",
    "crouch-turn-emily-unarmed-knee-v4.mp4",
    "turn-stance-corvo-knee-v4.mp4",
    "turn-stance-corvo-unarmed-knee-v4.mp4",
    "turn-stance-emily-knee-v4.mp4",
    "turn-stance-emily-unarmed-knee-v4.mp4"
  ],
  "complete": true,
  "visual_accepted": false,
  "evidence": [
    "turn-stance-corvo-knee-v4/result.json",
    "turn-stance-corvo-unarmed-knee-v4/result.json",
    "turn-stance-emily-knee-v4/result.json",
    "turn-stance-emily-unarmed-knee-v4/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1318e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Crouched in-place turns left and right of 90 and 180 degrees in both roles, crouch held throughout (stance mix >= .96), all turn variants presented; the sweep's upright flashes came from per-trial teleports and do not reproduce.",
    "takes": [
      {
        "take": "coverage-v1318e",
        "cases": [
          "host-turns",
          "client-turns"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1318c/host-turns-observer.mp4",
        "verdict": "Corvo's crouched turns stay low with stepping feet."
      },
      {
        "video": "coverage-v1318c/client-turns-observer.mp4",
        "verdict": "Emily's crouched turns stay low with stepping feet."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: turn-stance-corvo-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-corvo-knee-v4/result.json>)
- [evidence: turn-stance-corvo-unarmed-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-corvo-unarmed-knee-v4/result.json>)
- [evidence: turn-stance-emily-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-emily-knee-v4/result.json>)
- [evidence: turn-stance-emily-unarmed-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-emily-unarmed-knee-v4/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1318e/result.json](<../../../../work/natural-moveset/coverage-v1318e/result.json>)
- [in_game_recordings: crouch-turn-corvo-knee-v4.mp4](<../../../../work/natural-moveset/crouch-turn-corvo-knee-v4.mp4>)
- [in_game_recordings: crouch-turn-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/crouch-turn-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: crouch-turn-emily-knee-v4.mp4](<../../../../work/natural-moveset/crouch-turn-emily-knee-v4.mp4>)
- [in_game_recordings: crouch-turn-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/crouch-turn-emily-unarmed-knee-v4.mp4>)
- [in_game_recordings: turn-stance-corvo-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-knee-v4.mp4>)
- [in_game_recordings: turn-stance-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-unarmed-knee-v4.mp4>)
## crouch_stop_start

Visual sweep 1260: accepted (corvo accept, emily accept). Crouched travel starts and stops cleanly into a held crouch for both characters, then recovers to standing. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending.

````json
{
  "id": "crouch_stop_start",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "crouch stop start",
  "motions": [
    "crouch_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouched travel starts and stops cleanly into a held crouch for both characters, then recovers to standing. Actual native stance controls the full-body bank. Both roles pass directional, distance-based cadence, sustained playback and reversible stance transition checks; native actions retain priority. Floor contact, interruptions and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "integrated-sprint-corvo-v2.mp4",
    "integrated-sprint-emily-v2.mp4",
    "crouch-balance-corvo-baseline-v1.mp4",
    "crouch-balance-corvo-candidate-v1.mp4",
    "crouch-balance-emily-baseline-v1.mp4",
    "crouch-balance-emily-candidate-v1.mp4"
  ],
  "evidence": [
    "crouch-balance-corvo-baseline-v1/result.json",
    "crouch-balance-corvo-candidate-v1/result.json",
    "crouch-balance-emily-baseline-v1/result.json",
    "crouch-balance-emily-candidate-v1/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1318e/result.json",
    "coverage-v1319e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Crouched starts and stops in each of the 8 directions, and crouched walks interrupted by an attack and a jump that resume the crouched gait, in both roles.",
    "takes": [
      {
        "take": "coverage-v1318e",
        "cases": [
          "host-crouch8",
          "client-crouch8"
        ]
      },
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-interrupt",
          "client-interrupt"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1318c/host-crouch8-observer.mp4",
        "verdict": "Corvo's crouched starts and stops are clean into a held crouch."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: crouch-balance-corvo-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1/result.json>)
- [evidence: crouch-balance-corvo-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1/result.json>)
- [evidence: crouch-balance-emily-baseline-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1/result.json>)
- [evidence: crouch-balance-emily-candidate-v1/result.json](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1318e/result.json](<../../../../work/natural-moveset/coverage-v1318e/result.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: integrated-sprint-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-corvo-v2.mp4>)
- [in_game_recordings: integrated-sprint-emily-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-emily-v2.mp4>)
- [in_game_recordings: crouch-balance-corvo-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-corvo-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-corvo-candidate-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-baseline-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-baseline-v1.mp4>)
- [in_game_recordings: crouch-balance-emily-candidate-v1.mp4](<../../../../work/natural-moveset/crouch-balance-emily-candidate-v1.mp4>)
## stand_lean_left_right

Visual sweep 1260: accepted (corvo accept, emily accept). Standing left and right leans read as whole-body balance shifts with planted feet for both characters. The amplitude is modest. Native vertical and lateral lean amplitudes drive whole-body balance poses. Both roles, stances and weapon states pass 18 entry/lower/side-transition/exit/recovery checks. Actual armed D2 takes show complete bodies. Standing lift is subtle and needs closer review; obstacle-specific sight lines, collision-limited scale, combat interruptions and final visual acceptance remain pending.

````json
{
  "id": "stand_lean_left_right",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "stand lean left right",
  "motions": [
    "stand_lean_left",
    "stand_lean_right"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Standing left and right leans read as whole-body balance shifts with planted feet for both characters. The amplitude is modest. Native vertical and lateral lean amplitudes drive whole-body balance poses. Both roles, stances and weapon states pass 18 entry/lower/side-transition/exit/recovery checks. Actual armed D2 takes show complete bodies. Standing lift is subtle and needs closer review; obstacle-specific sight lines, collision-limited scale, combat interruptions and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-lean-corvo-v1.mp4",
    "integrated-lean-emily-v1.mp4",
    "integrated-vertical-peek-corvo-v1.mp4",
    "integrated-vertical-peek-emily-v1.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1319e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Native standing leans left and right (state 18) presented at full weight with the matching side in both roles, each recovering after the native exit (Y tap); the native lean persists after Y is released and ends on a Y tap, forward movement, a turn or the opposite side (probe 1322a).",
    "takes": [
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-lean",
          "client-lean"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1319c/host-lean-observer.mp4",
        "verdict": "Corvo's standing leans shift the whole body sideways over planted feet and recover (modest amplitude)."
      },
      {
        "video": "coverage-v1319c/client-lean-observer.mp4",
        "verdict": "Emily's standing leans shift visibly each way and recover."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: integrated-lean-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-lean-corvo-v1.mp4>)
- [in_game_recordings: integrated-lean-emily-v1.mp4](<../../../../work/natural-moveset/integrated-lean-emily-v1.mp4>)
- [in_game_recordings: integrated-vertical-peek-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-vertical-peek-corvo-v1.mp4>)
- [in_game_recordings: integrated-vertical-peek-emily-v1.mp4](<../../../../work/natural-moveset/integrated-vertical-peek-emily-v1.mp4>)
## crouch_lean_left_right

Visual sweep 1260: accepted (corvo accept, emily accept). Crouched left and right leans shift the body both ways and recover to standing cleanly for both characters. Native vertical and lateral lean amplitudes drive whole-body balance poses. Both roles, stances and weapon states pass 18 entry/lower/side-transition/exit/recovery checks. Actual armed D2 takes show complete bodies. Standing lift is subtle and needs closer review; obstacle-specific sight lines, collision-limited scale, combat interruptions and final visual acceptance remain pending.

````json
{
  "id": "crouch_lean_left_right",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "crouch lean left right",
  "motions": [
    "crouch_lean_left",
    "crouch_lean_right"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouched left and right leans shift the body both ways and recover to standing cleanly for both characters. Native vertical and lateral lean amplitudes drive whole-body balance poses. Both roles, stances and weapon states pass 18 entry/lower/side-transition/exit/recovery checks. Actual armed D2 takes show complete bodies. Standing lift is subtle and needs closer review; obstacle-specific sight lines, collision-limited scale, combat interruptions and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-lean-corvo-v1.mp4",
    "integrated-lean-emily-v1.mp4",
    "integrated-vertical-peek-corvo-v1.mp4",
    "integrated-vertical-peek-emily-v1.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1319e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Native crouched leans left and right presented crouched at full weight with the matching side in both roles, each recovering.",
    "takes": [
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-lean",
          "client-lean"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1319c/host-lean-observer.mp4",
        "verdict": "Corvo's crouched leans are clear both ways and recover."
      },
      {
        "video": "coverage-v1319c/client-lean-observer.mp4",
        "verdict": "Emily's crouched leans are clear both ways and recover."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: integrated-lean-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-lean-corvo-v1.mp4>)
- [in_game_recordings: integrated-lean-emily-v1.mp4](<../../../../work/natural-moveset/integrated-lean-emily-v1.mp4>)
- [in_game_recordings: integrated-vertical-peek-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-vertical-peek-corvo-v1.mp4>)
- [in_game_recordings: integrated-vertical-peek-emily-v1.mp4](<../../../../work/natural-moveset/integrated-vertical-peek-emily-v1.mp4>)
## ledge_peek

Visual sweep 1260: accepted (corvo accept, emily accept). The crouched upward peek clearly lifts the torso for both characters and recovers cleanly. The standing upward lift is nearly imperceptible: polish, not a break. Native vertical and lateral lean amplitudes drive whole-body balance poses. Both roles, stances and weapon states pass 18 entry/lower/side-transition/exit/recovery checks. Actual armed D2 takes show complete bodies. Standing lift is subtle and needs closer review; obstacle-specific sight lines, collision-limited scale, combat interruptions and final visual acceptance remain pending.

````json
{
  "id": "ledge_peek",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "ledge peek",
  "motions": [
    "ledge_peek"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The crouched upward peek clearly lifts the torso for both characters and recovers cleanly. The standing upward lift is nearly imperceptible: polish, not a break. Native vertical and lateral lean amplitudes drive whole-body balance poses. Both roles, stances and weapon states pass 18 entry/lower/side-transition/exit/recovery checks. Actual armed D2 takes show complete bodies. Standing lift is subtle and needs closer review; obstacle-specific sight lines, collision-limited scale, combat interruptions and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-vertical-peek-corvo-v1.mp4",
    "integrated-vertical-peek-emily-v1.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "coverage-v1319e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Native upward peeks standing and crouched (lean up 1.0) presented in both roles and recovering. The crouched peek unfolds the crouch clearly; the standing peek is a tiptoe rise (18 degree heel pivot, about 5 cm of head lift, bones unstretched) by design: the first-person camera rise is not reachable by a body.",
    "takes": [
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-lean",
          "client-lean"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1319c/host-lean-observer.mp4",
        "verdict": "Corvo's crouched peek rises clearly; the standing peek is a subtle tiptoe rise."
      },
      {
        "video": "coverage-v1319c/client-lean-observer.mp4",
        "verdict": "Emily's crouched peek rises clearly; the standing peek is a subtle tiptoe rise."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: integrated-vertical-peek-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-vertical-peek-corvo-v1.mp4>)
- [in_game_recordings: integrated-vertical-peek-emily-v1.mp4](<../../../../work/natural-moveset/integrated-vertical-peek-emily-v1.mp4>)
## keyhole_enter_look_exit

Owned native Peeking scenes select full-body NPC-derived entry/look/exit, both door sides and original stances. Native sword holster/recovery and bounded native exit handoff are replicated. Reviewed captures identify the tested roles and variants; global movement completion is not implied.

````json
{
  "id": "keyhole_enter_look_exit",
  "family": "stance",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_keyhole_phases_through_Steam",
  "visual_validation": "bounded_actual_game_recordings_inspected",
  "native_reference_family_count": 142,
  "priority": 1,
  "title": "keyhole enter look exit",
  "motions": [
    "empty_distraction_look_keyhole_loop",
    "empty_distraction_look_keyhole_out_fast",
    "empty_distraction_look_keyhole_in_walkright",
    "empty_distraction_look_keyhole_out_walk",
    "empty_distraction_look_keyhole_out",
    "empty_distraction_look_keyhole_in",
    "empty_native_keyhole_left_in",
    "empty_native_keyhole_left_loop",
    "empty_native_keyhole_left_out",
    "empty_native_keyhole_left_crouch_in",
    "empty_native_keyhole_left_crouch_out",
    "empty_native_keyhole_right_in",
    "empty_native_keyhole_right_loop",
    "empty_native_keyhole_right_out",
    "empty_native_keyhole_right_crouch_in",
    "empty_native_keyhole_right_crouch_out"
  ],
  "deliverable_kind": "animation",
  "notes": "Owned native Peeking scenes select full-body NPC-derived entry/look/exit, both door sides and original stances. Native sword holster/recovery and bounded native exit handoff are replicated. Reviewed captures identify the tested roles and variants; global movement completion is not implied.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "keyhole-corvo-left-stand-v24-v6.mp4",
    "keyhole-corvo-left-crouch-v24-v6.mp4",
    "keyhole-emily-left-stand-v24-v6.mp4",
    "keyhole-emily-left-crouch-v24-v6.mp4",
    "keyhole-emily-right-stand-v24-v6.mp4",
    "keyhole-emily-right-crouch-v24-v6.mp4",
    "keyhole-corvo-right-stand-v24-v7.mp4",
    "keyhole-corvo-left-early-v24-v9.mp4",
    "keyhole-emily-left-early-crouch-v24-v9.mp4",
    "keyhole-corvo-pause-v24-v8.mp4",
    "keyhole-emily-crouch-pause-v24-v9.mp4",
    "keyhole-corvo-right-crouch-v24-v9.mp4"
  ],
  "evidence": [
    "keyhole-corvo-left-stand-v24-v6/result.json",
    "keyhole-corvo-left-crouch-v24-v6/result.json",
    "keyhole-emily-left-stand-v24-v6/result.json",
    "keyhole-emily-left-crouch-v24-v6/result.json",
    "keyhole-emily-right-stand-v24-v6/result.json",
    "keyhole-emily-right-crouch-v24-v6/result.json",
    "keyhole-corvo-right-stand-v24-v7/result.json",
    "keyhole-corvo-left-early-v24-v9/result.json",
    "keyhole-emily-left-early-crouch-v24-v9/result.json",
    "keyhole-corvo-pause-v24-v8/result.json",
    "keyhole-emily-crouch-pause-v24-v9/result.json",
    "keyhole-corvo-right-crouch-v24-v9/result.json",
    "coverage-v1319e/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1325-stance.json",
    "decision": "complete",
    "scope": "Native keyhole peeks at the lab-spot door, standing and crouched, in both roles: the source enters native Peeking (state 24) and the partner presents the owned keyhole scene (traversal variant >= 7) in stance through entry, look and the native B exit, then recovers. Earlier bounded takes covered both door sides and holstered starts.",
    "takes": [
      {
        "take": "coverage-v1319e",
        "cases": [
          "host-keyhole",
          "client-keyhole"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1319e/host-keyhole-observer.mp4",
        "verdict": "Corvo bends to the keyhole at the door, holds, and rises back on exit, standing and crouched."
      },
      {
        "video": "coverage-v1319e/client-keyhole-observer.mp4",
        "verdict": "Emily bends to the keyhole, holds, and rises back on exit, standing and crouched."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: keyhole-corvo-left-stand-v24-v6/result.json](<../../../../work/natural-moveset/keyhole-corvo-left-stand-v24-v6/result.json>)
- [evidence: keyhole-corvo-left-crouch-v24-v6/result.json](<../../../../work/natural-moveset/keyhole-corvo-left-crouch-v24-v6/result.json>)
- [evidence: keyhole-emily-left-stand-v24-v6/result.json](<../../../../work/natural-moveset/keyhole-emily-left-stand-v24-v6/result.json>)
- [evidence: keyhole-emily-left-crouch-v24-v6/result.json](<../../../../work/natural-moveset/keyhole-emily-left-crouch-v24-v6/result.json>)
- [evidence: keyhole-emily-right-stand-v24-v6/result.json](<../../../../work/natural-moveset/keyhole-emily-right-stand-v24-v6/result.json>)
- [evidence: keyhole-emily-right-crouch-v24-v6/result.json](<../../../../work/natural-moveset/keyhole-emily-right-crouch-v24-v6/result.json>)
- [evidence: keyhole-corvo-right-stand-v24-v7/result.json](<../../../../work/natural-moveset/keyhole-corvo-right-stand-v24-v7/result.json>)
- [evidence: keyhole-corvo-left-early-v24-v9/result.json](<../../../../work/natural-moveset/keyhole-corvo-left-early-v24-v9/result.json>)
- [evidence: keyhole-emily-left-early-crouch-v24-v9/result.json](<../../../../work/natural-moveset/keyhole-emily-left-early-crouch-v24-v9/result.json>)
- [evidence: keyhole-corvo-pause-v24-v8/result.json](<../../../../work/natural-moveset/keyhole-corvo-pause-v24-v8/result.json>)
- [evidence: keyhole-emily-crouch-pause-v24-v9/result.json](<../../../../work/natural-moveset/keyhole-emily-crouch-pause-v24-v9/result.json>)
- [evidence: keyhole-corvo-right-crouch-v24-v9/result.json](<../../../../work/natural-moveset/keyhole-corvo-right-crouch-v24-v9/result.json>)
- [evidence: coverage-v1319e/result.json](<../../../../work/natural-moveset/coverage-v1319e/result.json>)
- [in_game_recordings: keyhole-corvo-left-stand-v24-v6.mp4](<../../../../work/natural-moveset/keyhole-corvo-left-stand-v24-v6.mp4>)
- [in_game_recordings: keyhole-corvo-left-crouch-v24-v6.mp4](<../../../../work/natural-moveset/keyhole-corvo-left-crouch-v24-v6.mp4>)
- [in_game_recordings: keyhole-emily-left-stand-v24-v6.mp4](<../../../../work/natural-moveset/keyhole-emily-left-stand-v24-v6.mp4>)
- [in_game_recordings: keyhole-emily-left-crouch-v24-v6.mp4](<../../../../work/natural-moveset/keyhole-emily-left-crouch-v24-v6.mp4>)
- [in_game_recordings: keyhole-emily-right-stand-v24-v6.mp4](<../../../../work/natural-moveset/keyhole-emily-right-stand-v24-v6.mp4>)
- [in_game_recordings: keyhole-emily-right-crouch-v24-v6.mp4](<../../../../work/natural-moveset/keyhole-emily-right-crouch-v24-v6.mp4>)
- [in_game_recordings: keyhole-corvo-right-stand-v24-v7.mp4](<../../../../work/natural-moveset/keyhole-corvo-right-stand-v24-v7.mp4>)
- [in_game_recordings: keyhole-corvo-left-early-v24-v9.mp4](<../../../../work/natural-moveset/keyhole-corvo-left-early-v24-v9.mp4>)
- [in_game_recordings: keyhole-emily-left-early-crouch-v24-v9.mp4](<../../../../work/natural-moveset/keyhole-emily-left-early-crouch-v24-v9.mp4>)
- [in_game_recordings: keyhole-corvo-pause-v24-v8.mp4](<../../../../work/natural-moveset/keyhole-corvo-pause-v24-v8.mp4>)
- [in_game_recordings: keyhole-emily-crouch-pause-v24-v9.mp4](<../../../../work/natural-moveset/keyhole-emily-crouch-pause-v24-v9.mp4>)
- [in_game_recordings: keyhole-corvo-right-crouch-v24-v9.mp4](<../../../../work/natural-moveset/keyhole-corvo-right-crouch-v24-v9.mp4>)

