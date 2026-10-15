# combat: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-15T21:39:39.946516+02:00. All fields below retain the catalog wording.

## sword_draw_holster

Visual sweep 1260: accepted (corvo accept, emily accept). Both characters show a recognizable whole-body draw and holster, standing and crouched, with clean transitions. Exact hand-to-hilt contact is a polish item. Current v48 DLL/v46 banks pass fifteen paired-role equipment/visibility/health/protected-file checks. Actual-game sampled frames reviewed for both roles. Exact hand contact, continuous travel quality, interruption/recovery breadth and final visual acceptance remain open. Earlier failed takes are retained. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.

````json
{
  "id": "sword_draw_holster",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_batched_locomotion_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "sword draw holster",
  "motions": [
    "sword_equipsword",
    "sword_unequipsword"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Both characters show a recognizable whole-body draw and holster, standing and crouched, with clean transitions. Exact hand-to-hilt contact is a polish item. Current v48 DLL/v46 banks pass fifteen paired-role equipment/visibility/health/protected-file checks. Actual-game sampled frames reviewed for both roles. Exact hand contact, continuous travel quality, interruption/recovery breadth and final visual acceptance remain open. Earlier failed takes are retained. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-equipment-close-corvo-v3.mp4",
    "integrated-equipment-close-emily-v3.mp4",
    "equipment-review-both-v48-host.mp4",
    "equipment-review-both-v48-v2-host.mp4",
    "equipment-review-both-v48-v3-host.mp4",
    "equipment-review-both-v48-v4-host.mp4",
    "equipment-review-both-v48-v4-client.mp4",
    "unbound-presentation-v559c-host.mp4",
    "unbound-presentation-v559c-client.mp4",
    "unbound-presentation-v559b-host.mp4",
    "native-gait-batch-v645/host-gait-batch.mp4",
    "native-gait-batch-v645/host-observer.mp4",
    "native-gait-batch-v645/client-gait-batch.mp4",
    "native-gait-batch-v645/client-observer.mp4"
  ],
  "native_visibility_v26": {
    "build": "A3A1542B9245DBCE5D33FC89952FAD0CC3C90656F8F727C715A6150145130D86",
    "cases": {
      "native-paired-pickup-corvo-visibility-v26": {
        "video": "native-paired-pickup-corvo-visibility-v26/native.mp4",
        "evidence": "native-paired-pickup-corvo-visibility-v26/validation.json",
        "paired_evidence": "native-paired-pickup-corvo-visibility-v26/paired-validation.json",
        "sword_evidence": "native-paired-pickup-corvo-visibility-v26/sword-visibility-validation.json",
        "visual_review": "native-paired-pickup-corvo-visibility-v26/visual-review.json",
        "checks": 44,
        "video_sha256": "5D083A9A68D6E9C70834432C349CF0A5F7D9429F3060CCA04AB65B86D3A20240",
        "role": "corvo",
        "complete": false,
        "visual_accepted": false
      },
      "native-paired-pickup-emily-visibility-v26": {
        "video": "native-paired-pickup-emily-visibility-v26/native.mp4",
        "evidence": "native-paired-pickup-emily-visibility-v26/validation.json",
        "paired_evidence": "native-paired-pickup-emily-visibility-v26/paired-validation.json",
        "sword_evidence": "native-paired-pickup-emily-visibility-v26/sword-visibility-validation.json",
        "visual_review": "native-paired-pickup-emily-visibility-v26/visual-review.json",
        "checks": 44,
        "video_sha256": "50D60A38EAC36841F9E712E0881B6DBC9F47CFF2448073F1E5F80F67E56267E3",
        "role": "emily",
        "complete": false,
        "visual_accepted": false
      }
    },
    "tests": [
      "equipment-visibility-v26/result.json",
      "equipment-visibility-pause-v26/result.json"
    ],
    "confirmed": [
      "Exact clone inventory child retained; no replacement weapon or inventory mutation for visibility.",
      "Native entity hide/show follows confirmed holster/draw and matching native attachment tags.",
      "Both roles: standing/crouched draw-holster and unarmed directional movement; pause retires ownership and resume restores original sword."
    ],
    "limitations": [
      "Sword folding is not animated; the static clone blade is hidden while holstered.",
      "Full carry and whole movement delivery remain incomplete."
    ],
    "complete": false
  },
  "native_integration": {
    "equipment-review-both-v48-host.mp4": {
      "video": "equipment-review-both-v48-host.mp4",
      "character": "corvo",
      "label": "Sword draw/holster · corvo · equipment-review-both-v48 · failed check",
      "alias": "sword_equipsword",
      "requirements": [
        "sword_draw_holster"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "equipment-review-both-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "actual_capture_hz": 28.6875,
      "video_sha256": "B54A7359017F27B747C970B0BF7C274C3809AA4841D97CB3CCE68DAEF72479BD",
      "checks_passed": false,
      "scope": "Native source input, strict equipment scene, Steam timing, authored body and native clone sword. Visual hand contact and broader unarmed special actions remain unaccepted."
    },
    "equipment-review-both-v48-v2-host.mp4": {
      "video": "equipment-review-both-v48-v2-host.mp4",
      "character": "corvo",
      "label": "Sword draw/holster · corvo · equipment-review-both-v48-v2 · failed check",
      "alias": "sword_equipsword",
      "requirements": [
        "sword_draw_holster"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "equipment-review-both-v48-v2/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "actual_capture_hz": 25.416666666666668,
      "video_sha256": "7753C5A2FEF6FAFEF2F3B817C3E19B4F4F6E2F8F6068C48176EF34233EE8E517",
      "checks_passed": false,
      "scope": "Native source input, strict equipment scene, Steam timing, authored body and native clone sword. Visual hand contact and broader unarmed special actions remain unaccepted."
    },
    "equipment-review-both-v48-v3-host.mp4": {
      "video": "equipment-review-both-v48-v3-host.mp4",
      "character": "corvo",
      "label": "Sword draw/holster · corvo · equipment-review-both-v48-v3 · failed check",
      "alias": "sword_equipsword",
      "requirements": [
        "sword_draw_holster"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "equipment-review-both-v48-v3/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "A1236EFAB4AABA0B09B6BB3C7EAE87FF1C52E79A30A13BB1926C910685AF0779",
      "checks_passed": false,
      "scope": "Native source input, strict equipment scene, Steam timing, authored body and native clone sword. Visual hand contact and broader unarmed special actions remain unaccepted."
    },
    "equipment-review-both-v48-v4-host.mp4": {
      "video": "equipment-review-both-v48-v4-host.mp4",
      "character": "corvo",
      "label": "Sword draw/holster · corvo · equipment-review-both-v48-v4",
      "alias": "sword_equipsword",
      "requirements": [
        "sword_draw_holster"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "equipment-review-both-v48-v4/result.json",
      "visual_review": "equipment-review-both-v48-v4/visual-review-host.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "actual_capture_hz": 27.24808895066018,
      "video_sha256": "E8A6877C905795EA4CF3E2E2DE9A3C0E0B4C56A46C7D944563CAFEA779837512",
      "checks_passed": true,
      "scope": "Native source input, strict equipment scene, Steam timing, authored body and native clone sword. Visual hand contact and broader unarmed special actions remain unaccepted."
    },
    "equipment-review-both-v48-v4-client.mp4": {
      "video": "equipment-review-both-v48-v4-client.mp4",
      "character": "emily",
      "label": "Sword draw/holster · emily · equipment-review-both-v48-v4",
      "alias": "sword_equipsword",
      "requirements": [
        "sword_draw_holster"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "equipment-review-both-v48-v4/result.json",
      "visual_review": "equipment-review-both-v48-v4/visual-review-client.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "actual_capture_hz": 27.604166666666668,
      "video_sha256": "86776A4669CBEE47A9B9CF578AB223075A70E1C86A866C29D525539C3E3C3F6C",
      "checks_passed": true,
      "scope": "Native source input, strict equipment scene, Steam timing, authored body and native clone sword. Visual hand contact and broader unarmed special actions remain unaccepted."
    }
  },
  "complete": false,
  "evidence": [
    "native-gait-batch-v645/result.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ]
}
````

- [evidence: native-gait-batch-v645/result.json](<../../../../work/natural-moveset/native-gait-batch-v645/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: integrated-equipment-close-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-corvo-v3.mp4>)
- [in_game_recordings: integrated-equipment-close-emily-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-emily-v3.mp4>)
- [in_game_recordings: equipment-review-both-v48-host.mp4](<../../../../work/natural-moveset/equipment-review-both-v48-host.mp4>)
- [in_game_recordings: equipment-review-both-v48-v2-host.mp4](<../../../../work/natural-moveset/equipment-review-both-v48-v2-host.mp4>)
- [in_game_recordings: equipment-review-both-v48-v3-host.mp4](<../../../../work/natural-moveset/equipment-review-both-v48-v3-host.mp4>)
- [in_game_recordings: equipment-review-both-v48-v4-host.mp4](<../../../../work/natural-moveset/equipment-review-both-v48-v4-host.mp4>)
- [in_game_recordings: equipment-review-both-v48-v4-client.mp4](<../../../../work/natural-moveset/equipment-review-both-v48-v4-client.mp4>)
- [in_game_recordings: unbound-presentation-v559c-host.mp4](<../../../../work/natural-moveset/unbound-presentation-v559c-host.mp4>)
- [in_game_recordings: unbound-presentation-v559c-client.mp4](<../../../../work/natural-moveset/unbound-presentation-v559c-client.mp4>)
- [in_game_recordings: unbound-presentation-v559b-host.mp4](<../../../../work/natural-moveset/unbound-presentation-v559b-host.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-observer.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-observer.mp4>)
## sword_ready_idle

Visual sweep 1260: accepted (corvo accept, emily accept). The standing and crouched sword-ready idles read naturally for both characters. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending.

````json
{
  "id": "sword_ready_idle",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "evidence": [
    "walk-test-3/result.json",
    "corvo-walking.mp4",
    "emily-walking.mp4",
    "grip-closeup-corvo-v2/result.json",
    "grip-closeup-emily-v1/result.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ],
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The standing and crouched sword-ready idles read naturally for both characters. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending.",
  "title": "sword ready idle",
  "motions": [
    "sword_idle",
    "armed_idle"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-equipment-close-corvo-v3.mp4",
    "integrated-equipment-close-emily-v3.mp4",
    "grip-closeup-corvo-v2.mp4",
    "grip-closeup-emily-v1.mp4"
  ]
}
````

- [evidence: grip-closeup-corvo-v2/result.json](<../../../../work/natural-moveset/grip-closeup-corvo-v2/result.json>)
- [evidence: grip-closeup-emily-v1/result.json](<../../../../work/natural-moveset/grip-closeup-emily-v1/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: integrated-equipment-close-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-corvo-v3.mp4>)
- [in_game_recordings: integrated-equipment-close-emily-v3.mp4](<../../../../work/natural-moveset/integrated-equipment-close-emily-v3.mp4>)
- [in_game_recordings: grip-closeup-corvo-v2.mp4](<../../../../work/natural-moveset/grip-closeup-corvo-v2.mp4>)
- [in_game_recordings: grip-closeup-emily-v1.mp4](<../../../../work/natural-moveset/grip-closeup-emily-v1.mp4>)
## light_attack_variants

Visual sweep 1260: accepted (corvo accept, emily accept). Short and alternating slashes are clearly readable on both characters. Corvo's targeted observer take is occluded, so his verdict rests on the integrated combat take. Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished. 610/615: native target-bearing sword scenes now drive existing remote combat clips. Corvo611 and Emily614 each pass10 source/Steam/recovery/restoration checks; receiver target health unchanged. Existing protocol65 and bank65/353 reused. Emily body attack/recovery visually observed; Corvo611 camera is occluded. Shared NPC reactions/outcomes and other combat contexts remain unfinished; cosmetic polish deferred. Ordinary standing right/rightb target-bearing sword actor playback only. Native local first-person remains unchanged; no victim events or damage replay. Corvo611 passed numerical checks but foreground guard occludes the body. Emily614 observer sheet inspected and shows the body attack and recovery. Fine contact/grip polish deferred; no visual acceptance. Emily611 retained failure: crouched native throwhit/kick800 instead of expected sword hit. Emily612 retained failure: one independently validated target scene, below the harness minimum of two; remote playback observed but no passing receipt. Corvo614 retained fixture failure: source could not stand near geometry and observer capture timed out during early cleanup. Both checkpoints/inventories/health and14 protected hashes restored. Bank65/353 and protocol65 unchanged. Targeted crouch/left/other contexts, shared enemy reaction/health/death and full campaign coverage remain unfinished.

````json
{
  "id": "light_attack_variants",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_target_bearing_standing_slash",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Short and alternating slashes are clearly readable on both characters. Corvo's targeted observer take is occluded, so his verdict rests on the integrated combat take. Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished. 610/615: native target-bearing sword scenes now drive existing remote combat clips. Corvo611 and Emily614 each pass10 source/Steam/recovery/restoration checks; receiver target health unchanged. Existing protocol65 and bank65/353 reused. Emily body attack/recovery visually observed; Corvo611 camera is occluded. Shared NPC reactions/outcomes and other combat contexts remain unfinished; cosmetic polish deferred. Ordinary standing right/rightb target-bearing sword actor playback only. Native local first-person remains unchanged; no victim events or damage replay. Corvo611 passed numerical checks but foreground guard occludes the body. Emily614 observer sheet inspected and shows the body attack and recovery. Fine contact/grip polish deferred; no visual acceptance. Emily611 retained failure: crouched native throwhit/kick800 instead of expected sword hit. Emily612 retained failure: one independently validated target scene, below the harness minimum of two; remote playback observed but no passing receipt. Corvo614 retained fixture failure: source could not stand near geometry and observer capture timed out during early cleanup. Both checkpoints/inventories/health and14 protected hashes restored. Bank65/353 and protocol65 unchanged. Targeted crouch/left/other contexts, shared enemy reaction/health/death and full campaign coverage remain unfinished.",
  "evidence": [
    "priority-2/result.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ],
  "title": "light attack variants",
  "motions": [
    "sword_attack_fastb",
    "sword_attack_shorta",
    "sword_attack_shortb",
    "sword_attack_shortc",
    "sword_attack_shortd",
    "sword_attack_fasta",
    "sword_attack_shorte"
  ],
  "deliverable_kind": "animation",
  "runtime_priority": "Existing native action suppresses the walking/running bank; this does not validate new donor gameplay binding.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-combat-corvo-v1.mp4",
    "integrated-combat-emily-v1.mp4",
    "native-front-corvo-v607/host-front-lethal.mp4",
    "native-front-emily-v607/client-front-lethal.mp4",
    "targeted-combat-corvo-v611/host-targeted-slash.mp4",
    "targeted-combat-corvo-v611/observer.mp4",
    "targeted-combat-emily-v611/client-targeted-slash.mp4",
    "targeted-combat-emily-v612/client-targeted-slash.mp4",
    "targeted-combat-emily-v612/observer.mp4",
    "targeted-combat-emily-v614/client-targeted-slash.mp4",
    "targeted-combat-emily-v614/observer.mp4",
    "native-close-combat-emily-v623/client-close-combat.mp4"
  ],
  "complete": false,
  "visual_accepted": false
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: integrated-combat-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-combat-corvo-v1.mp4>)
- [in_game_recordings: integrated-combat-emily-v1.mp4](<../../../../work/natural-moveset/integrated-combat-emily-v1.mp4>)
- [in_game_recordings: native-front-corvo-v607/host-front-lethal.mp4](<../../../../work/natural-moveset/native-front-corvo-v607/host-front-lethal.mp4>)
- [in_game_recordings: native-front-emily-v607/client-front-lethal.mp4](<../../../../work/natural-moveset/native-front-emily-v607/client-front-lethal.mp4>)
- [in_game_recordings: targeted-combat-corvo-v611/host-targeted-slash.mp4](<../../../../work/natural-moveset/targeted-combat-corvo-v611/host-targeted-slash.mp4>)
- [in_game_recordings: targeted-combat-corvo-v611/observer.mp4](<../../../../work/natural-moveset/targeted-combat-corvo-v611/observer.mp4>)
- [in_game_recordings: targeted-combat-emily-v611/client-targeted-slash.mp4](<../../../../work/natural-moveset/targeted-combat-emily-v611/client-targeted-slash.mp4>)
- [in_game_recordings: targeted-combat-emily-v612/client-targeted-slash.mp4](<../../../../work/natural-moveset/targeted-combat-emily-v612/client-targeted-slash.mp4>)
- [in_game_recordings: targeted-combat-emily-v612/observer.mp4](<../../../../work/natural-moveset/targeted-combat-emily-v612/observer.mp4>)
- [in_game_recordings: targeted-combat-emily-v614/client-targeted-slash.mp4](<../../../../work/natural-moveset/targeted-combat-emily-v614/client-targeted-slash.mp4>)
- [in_game_recordings: targeted-combat-emily-v614/observer.mp4](<../../../../work/natural-moveset/targeted-combat-emily-v614/observer.mp4>)
- [in_game_recordings: native-close-combat-emily-v623/client-close-combat.mp4](<../../../../work/natural-moveset/native-close-combat-emily-v623/client-close-combat.mp4>)
## heavy_attack_variants

Audit 1286: Corvo's crouched heavy re-taken (coverage-v1285a) without the standing pop; accepted in both roles. Visual sweep 1260: reject (corvo reject, emily accept). The heavy attack reads for both characters, but Corvo's crouched heavy take pops from kneeling to standing and back at action entry, so Corvo is rejected until a re-take without the mid-take stream restart. Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished. 635/636/637: both roles PASS10 stationary crouched heavy charge/hold/release through native Steam, full-body adapted clips354/355/356, root independence and native stance recovery. Protocol69,bank67/357; prior354 unchanged. Both inventories/health14protected hashes restored. Exact asset635 corrects role-specific release inference: Scene_1/833 and Scene_2/1000 are attack variants. Coarse pose transitions/contact polish deferred. Full movement and shared victim/world outcomes remain unfinished.

````json
{
  "id": "heavy_attack_variants",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_standing_and_crouched_combat",
  "visual_validation": "accepted_functional_both_roles_crouched_heavy_retake",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "heavy attack variants",
  "motions": [
    "sword_attack_longa",
    "sword_attack_longb"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1286: Corvo's crouched heavy re-taken (coverage-v1285a) without the standing pop; accepted in both roles. Visual sweep 1260: reject (corvo reject, emily accept). The heavy attack reads for both characters, but Corvo's crouched heavy take pops from kneeling to standing and back at action entry, so Corvo is rejected until a re-take without the mid-take stream restart. Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished. 635/636/637: both roles PASS10 stationary crouched heavy charge/hold/release through native Steam, full-body adapted clips354/355/356, root independence and native stance recovery. Protocol69,bank67/357; prior354 unchanged. Both inventories/health14protected hashes restored. Exact asset635 corrects role-specific release inference: Scene_1/833 and Scene_2/1000 are attack variants. Coarse pose transitions/contact polish deferred. Full movement and shared victim/world outcomes remain unfinished.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-combat-corvo-v1.mp4",
    "integrated-combat-emily-v1.mp4",
    "native-heavy-corvo-v629/host-crouched-heavy.mp4",
    "native-heavy-emily-v630/client-crouched-heavy.mp4",
    "crouched-heavy-corvo-v634/host-crouched-heavy.mp4",
    "crouched-heavy-corvo-v634/observer.mp4",
    "crouched-heavy-corvo-v636/host-crouched-heavy.mp4",
    "crouched-heavy-corvo-v636/observer.mp4",
    "crouched-heavy-emily-v636/client-crouched-heavy.mp4",
    "crouched-heavy-emily-v636/observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "crouched-heavy-corvo-v636/result.json",
    "crouched-heavy-emily-v636/result.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json",
    "../movement-integration/views-audit-v1286.json"
  ]
}
````

- [evidence: crouched-heavy-corvo-v636/result.json](<../../../../work/natural-moveset/crouched-heavy-corvo-v636/result.json>)
- [evidence: crouched-heavy-emily-v636/result.json](<../../../../work/natural-moveset/crouched-heavy-emily-v636/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [evidence: ../movement-integration/views-audit-v1286.json](<../../../../work/movement-integration/views-audit-v1286.json>)
- [in_game_recordings: integrated-combat-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-combat-corvo-v1.mp4>)
- [in_game_recordings: integrated-combat-emily-v1.mp4](<../../../../work/natural-moveset/integrated-combat-emily-v1.mp4>)
- [in_game_recordings: native-heavy-corvo-v629/host-crouched-heavy.mp4](<../../../../work/natural-moveset/native-heavy-corvo-v629/host-crouched-heavy.mp4>)
- [in_game_recordings: native-heavy-emily-v630/client-crouched-heavy.mp4](<../../../../work/natural-moveset/native-heavy-emily-v630/client-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v634/host-crouched-heavy.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v634/host-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v634/observer.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v634/observer.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v636/host-crouched-heavy.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v636/host-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v636/observer.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v636/observer.mp4>)
- [in_game_recordings: crouched-heavy-emily-v636/client-crouched-heavy.mp4](<../../../../work/natural-moveset/crouched-heavy-emily-v636/client-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-emily-v636/observer.mp4](<../../../../work/natural-moveset/crouched-heavy-emily-v636/observer.mp4>)
## moving_attack

Visual sweep 1260: accepted (corvo accept, emily accept). Travelling sword actions combine readable upper-body attacks and guards with a continuing leg gait for both characters. Some foot slide is possible but not blocking. Both characters show forward, side and backward sword actions, crouched moving attacks and forward moving guard over Steam. Native scene timing drives the authored action while pelvis/legs retain distance-driven gait. Sixteen checks and actual D2 videos cover coexistence, stop/recovery, health and equipment. Close foot/hand review, speed/direction/variant/contact coverage and final acceptance remain pending.

````json
{
  "id": "moving_attack",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_moving_combat_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "moving attack",
  "motions": [
    "sword_attack_left_90a",
    "sword_attack_left_180a",
    "sword_attack_right_90a",
    "sword_attack_right_180a",
    "sword_attack_stepbacka"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Travelling sword actions combine readable upper-body attacks and guards with a continuing leg gait for both characters. Some foot slide is possible but not blocking. Both characters show forward, side and backward sword actions, crouched moving attacks and forward moving guard over Steam. Native scene timing drives the authored action while pelvis/legs retain distance-driven gait. Sixteen checks and actual D2 videos cover coexistence, stop/recovery, health and equipment. Close foot/hand review, speed/direction/variant/contact coverage and final acceptance remain pending.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-moving-combat-corvo-v1.mp4",
    "integrated-moving-combat-emily-v1.mp4",
    "native-feet-corvo-close-v5.mp4",
    "native-feet-air-corvo-v5.mp4",
    "native-feet-emily-close-v5.mp4",
    "native-feet-air-emily-v5.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: integrated-moving-combat-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-moving-combat-corvo-v1.mp4>)
- [in_game_recordings: integrated-moving-combat-emily-v1.mp4](<../../../../work/natural-moveset/integrated-moving-combat-emily-v1.mp4>)
- [in_game_recordings: native-feet-corvo-close-v5.mp4](<../../../../work/natural-moveset/native-feet-corvo-close-v5.mp4>)
- [in_game_recordings: native-feet-air-corvo-v5.mp4](<../../../../work/natural-moveset/native-feet-air-corvo-v5.mp4>)
- [in_game_recordings: native-feet-emily-close-v5.mp4](<../../../../work/natural-moveset/native-feet-emily-close-v5.mp4>)
- [in_game_recordings: native-feet-air-emily-v5.mp4](<../../../../work/natural-moveset/native-feet-air-emily-v5.mp4>)
## crouched_attack

Visual sweep 1260: accepted (corvo accept, emily accept). Crouched strikes read on both characters. Corvo's targeted observer take has a deep lunge bend and a pre-strike standing interval worth an animator look. Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished. 620/622: Corvo621 and Emily621 each pass10 targeted crouched strike, native Steam actor playback, root, native stance recovery and restoration checks. Both observer sheets show body attack/recovery; fine polish deferred. Optional hit-recovery15/16 was absent in621 and remains runtime-unverified. Protocol66, unchanged bank65/353. No duplicate receiver victim damage; shared enemy outcomes and complete movement remain unfinished. Target-bearing crouched strike and native stance recovery verified on both roles; no optional separate hit-recovery15/16 scene occurred in621. Those branches remain runtime-unverified despite native613 source observations and passing synthetic tests. Both621 observer contact sheets inspected: body crouches, strikes and recovers standing. Guard occlusion limits sword/contact inspection. Fine pose/grip/timing polish deferred, no visual acceptance. Receiver enemy health unchanged: actor playback does not duplicate damage. Enemy reactions/health/death remain independent; this is not shared-victim gameplay. Failed617 stream-setup and619 optional-branch assertions retained, with checkpoint/health/inventory and14 protected hashes restored. Complete movement and playable co-op remain unfinished. No new pipeline or third-party co-op research. 625/628: bank66/354 and protocol67 live. Corvo627 PASS10 and Emily627 PASS13 preserve crouched combat; Emily right hit-recovery15 now has owned-source, Steam body, monotonic follow-through, stance recovery and unchanged receiver-victim-health proof. Kick17 implementation/build tests pass but native626 selected another scene: kick remains runtime-unverified. Left recovery16 remains unverified. Prior353 payloads unchanged, checkpoints and14 protected files restored. Visual polish deferred. Native kick17 remains runtime-unverified. Emily626 selected crouched slash/right hit recovery15, not the required kick; the failed kick receipt remains unchanged. Corvo627 PASS10 and Emily627 PASS13 preserve targeted crouched attacks; Emily also proves owned right recovery15, monotonic follow-through, return to movement and unchanged receiver victim health. Left recovery16 remains unverified. Both627 observer sheets inspected: full-body strikes and standing recovery visible, but foreground guards limit sword/contact assessment. Fine polish deferred; no visual acceptance. All checkpoint inventories/playerpartner health and14 protected hashes restored. Shared enemy outcomes, complete movement and playable co-op remain unfinished. 635/636/637: both roles PASS10 stationary crouched heavy charge/hold/release through native Steam, full-body adapted clips354/355/356, root independence and native stance recovery. Protocol69,bank67/357; prior354 unchanged. Both inventories/health14protected hashes restored. Exact asset635 corrects role-specific release inference: Scene_1/833 and Scene_2/1000 are attack variants. Coarse pose transitions/contact polish deferred. Full movement and shared victim/world outcomes remain unfinished.

````json
{
  "id": "crouched_attack",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_standing_and_crouched_combat",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouched strikes read on both characters. Corvo's targeted observer take has a deep lunge bend and a pre-strike standing interval worth an animator look. Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished. 620/622: Corvo621 and Emily621 each pass10 targeted crouched strike, native Steam actor playback, root, native stance recovery and restoration checks. Both observer sheets show body attack/recovery; fine polish deferred. Optional hit-recovery15/16 was absent in621 and remains runtime-unverified. Protocol66, unchanged bank65/353. No duplicate receiver victim damage; shared enemy outcomes and complete movement remain unfinished. Target-bearing crouched strike and native stance recovery verified on both roles; no optional separate hit-recovery15/16 scene occurred in621. Those branches remain runtime-unverified despite native613 source observations and passing synthetic tests. Both621 observer contact sheets inspected: body crouches, strikes and recovers standing. Guard occlusion limits sword/contact inspection. Fine pose/grip/timing polish deferred, no visual acceptance. Receiver enemy health unchanged: actor playback does not duplicate damage. Enemy reactions/health/death remain independent; this is not shared-victim gameplay. Failed617 stream-setup and619 optional-branch assertions retained, with checkpoint/health/inventory and14 protected hashes restored. Complete movement and playable co-op remain unfinished. No new pipeline or third-party co-op research. 625/628: bank66/354 and protocol67 live. Corvo627 PASS10 and Emily627 PASS13 preserve crouched combat; Emily right hit-recovery15 now has owned-source, Steam body, monotonic follow-through, stance recovery and unchanged receiver-victim-health proof. Kick17 implementation/build tests pass but native626 selected another scene: kick remains runtime-unverified. Left recovery16 remains unverified. Prior353 payloads unchanged, checkpoints and14 protected files restored. Visual polish deferred. Native kick17 remains runtime-unverified. Emily626 selected crouched slash/right hit recovery15, not the required kick; the failed kick receipt remains unchanged. Corvo627 PASS10 and Emily627 PASS13 preserve targeted crouched attacks; Emily also proves owned right recovery15, monotonic follow-through, return to movement and unchanged receiver victim health. Left recovery16 remains unverified. Both627 observer sheets inspected: full-body strikes and standing recovery visible, but foreground guards limit sword/contact assessment. Fine polish deferred; no visual acceptance. All checkpoint inventories/playerpartner health and14 protected hashes restored. Shared enemy outcomes, complete movement and playable co-op remain unfinished. 635/636/637: both roles PASS10 stationary crouched heavy charge/hold/release through native Steam, full-body adapted clips354/355/356, root independence and native stance recovery. Protocol69,bank67/357; prior354 unchanged. Both inventories/health14protected hashes restored. Exact asset635 corrects role-specific release inference: Scene_1/833 and Scene_2/1000 are attack variants. Coarse pose transitions/contact polish deferred. Full movement and shared victim/world outcomes remain unfinished.",
  "evidence": [
    "priority-2/result.json",
    "crouched-heavy-corvo-v636/result.json",
    "crouched-heavy-emily-v636/result.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ],
  "title": "crouched attack",
  "motions": [
    "crouch_attack_fast_a",
    "crouch_attack_fast_b",
    "crouch_attack_short_a"
  ],
  "deliverable_kind": "animation",
  "runtime_priority": "Existing native action suppresses the walking/running bank; this does not validate new donor gameplay binding.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-combat-corvo-v1.mp4",
    "integrated-combat-emily-v1.mp4",
    "integrated-moving-combat-corvo-v1.mp4",
    "integrated-moving-combat-emily-v1.mp4",
    "native-feet-corvo-close-v5.mp4",
    "native-feet-emily-close-v5.mp4",
    "native-crouch-combat-corvo-v613/host-crouch-combat.mp4",
    "native-crouch-combat-emily-v613/client-crouch-combat.mp4",
    "crouch-combat-corvo-v621/host-crouched-slash.mp4",
    "crouch-combat-corvo-v621/observer.mp4",
    "crouch-combat-emily-v621/client-crouched-slash.mp4",
    "crouch-combat-emily-v621/observer.mp4",
    "crouch-combat-corvo-v619/host-crouched-slash.mp4",
    "crouch-combat-corvo-v619/observer.mp4",
    "crouch-regression-corvo-v627/host-crouched-slash.mp4",
    "crouch-regression-corvo-v627/observer.mp4",
    "crouch-regression-emily-v627/client-crouched-slash.mp4",
    "crouch-regression-emily-v627/observer.mp4",
    "native-kick-emily-v626/client-close-kick.mp4",
    "native-kick-emily-v626/observer.mp4",
    "native-heavy-corvo-v629/host-crouched-heavy.mp4",
    "native-heavy-emily-v630/client-crouched-heavy.mp4",
    "crouched-heavy-corvo-v634/host-crouched-heavy.mp4",
    "crouched-heavy-corvo-v634/observer.mp4",
    "crouched-heavy-corvo-v636/host-crouched-heavy.mp4",
    "crouched-heavy-corvo-v636/observer.mp4",
    "crouched-heavy-emily-v636/client-crouched-heavy.mp4",
    "crouched-heavy-emily-v636/observer.mp4"
  ],
  "complete": false,
  "visual_accepted": false
}
````

- [evidence: crouched-heavy-corvo-v636/result.json](<../../../../work/natural-moveset/crouched-heavy-corvo-v636/result.json>)
- [evidence: crouched-heavy-emily-v636/result.json](<../../../../work/natural-moveset/crouched-heavy-emily-v636/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: integrated-combat-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-combat-corvo-v1.mp4>)
- [in_game_recordings: integrated-combat-emily-v1.mp4](<../../../../work/natural-moveset/integrated-combat-emily-v1.mp4>)
- [in_game_recordings: integrated-moving-combat-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-moving-combat-corvo-v1.mp4>)
- [in_game_recordings: integrated-moving-combat-emily-v1.mp4](<../../../../work/natural-moveset/integrated-moving-combat-emily-v1.mp4>)
- [in_game_recordings: native-feet-corvo-close-v5.mp4](<../../../../work/natural-moveset/native-feet-corvo-close-v5.mp4>)
- [in_game_recordings: native-feet-emily-close-v5.mp4](<../../../../work/natural-moveset/native-feet-emily-close-v5.mp4>)
- [in_game_recordings: native-crouch-combat-corvo-v613/host-crouch-combat.mp4](<../../../../work/natural-moveset/native-crouch-combat-corvo-v613/host-crouch-combat.mp4>)
- [in_game_recordings: native-crouch-combat-emily-v613/client-crouch-combat.mp4](<../../../../work/natural-moveset/native-crouch-combat-emily-v613/client-crouch-combat.mp4>)
- [in_game_recordings: crouch-combat-corvo-v621/host-crouched-slash.mp4](<../../../../work/natural-moveset/crouch-combat-corvo-v621/host-crouched-slash.mp4>)
- [in_game_recordings: crouch-combat-corvo-v621/observer.mp4](<../../../../work/natural-moveset/crouch-combat-corvo-v621/observer.mp4>)
- [in_game_recordings: crouch-combat-emily-v621/client-crouched-slash.mp4](<../../../../work/natural-moveset/crouch-combat-emily-v621/client-crouched-slash.mp4>)
- [in_game_recordings: crouch-combat-emily-v621/observer.mp4](<../../../../work/natural-moveset/crouch-combat-emily-v621/observer.mp4>)
- [in_game_recordings: crouch-combat-corvo-v619/host-crouched-slash.mp4](<../../../../work/natural-moveset/crouch-combat-corvo-v619/host-crouched-slash.mp4>)
- [in_game_recordings: crouch-combat-corvo-v619/observer.mp4](<../../../../work/natural-moveset/crouch-combat-corvo-v619/observer.mp4>)
- [in_game_recordings: crouch-regression-corvo-v627/host-crouched-slash.mp4](<../../../../work/natural-moveset/crouch-regression-corvo-v627/host-crouched-slash.mp4>)
- [in_game_recordings: crouch-regression-corvo-v627/observer.mp4](<../../../../work/natural-moveset/crouch-regression-corvo-v627/observer.mp4>)
- [in_game_recordings: crouch-regression-emily-v627/client-crouched-slash.mp4](<../../../../work/natural-moveset/crouch-regression-emily-v627/client-crouched-slash.mp4>)
- [in_game_recordings: crouch-regression-emily-v627/observer.mp4](<../../../../work/natural-moveset/crouch-regression-emily-v627/observer.mp4>)
- [in_game_recordings: native-kick-emily-v626/client-close-kick.mp4](<../../../../work/natural-moveset/native-kick-emily-v626/client-close-kick.mp4>)
- [in_game_recordings: native-kick-emily-v626/observer.mp4](<../../../../work/natural-moveset/native-kick-emily-v626/observer.mp4>)
- [in_game_recordings: native-heavy-corvo-v629/host-crouched-heavy.mp4](<../../../../work/natural-moveset/native-heavy-corvo-v629/host-crouched-heavy.mp4>)
- [in_game_recordings: native-heavy-emily-v630/client-crouched-heavy.mp4](<../../../../work/natural-moveset/native-heavy-emily-v630/client-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v634/host-crouched-heavy.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v634/host-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v634/observer.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v634/observer.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v636/host-crouched-heavy.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v636/host-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-corvo-v636/observer.mp4](<../../../../work/natural-moveset/crouched-heavy-corvo-v636/observer.mp4>)
- [in_game_recordings: crouched-heavy-emily-v636/client-crouched-heavy.mp4](<../../../../work/natural-moveset/crouched-heavy-emily-v636/client-crouched-heavy.mp4>)
- [in_game_recordings: crouched-heavy-emily-v636/observer.mp4](<../../../../work/natural-moveset/crouched-heavy-emily-v636/observer.mp4>)
## airborne_attack

Visual sweep 1260: accepted (corvo accept, emily accept). Jump strikes, the airborne guard and the landing handoff read naturally for both characters. Both native characters perform early/late sword strikes and guarding during ordinary jumps over Steam. Native scene phase continues through landing. Fourteen checks plus actual D2 recordings cover simultaneous flight/combat, recovery, attachments and preserved health/first person. Moving jumps, heavy/double-jump/contact variants, lifecycle and final visual acceptance remain unfinished.

````json
{
  "id": "airborne_attack",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_airborne_combat_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "airborne attack",
  "motions": [
    "sword_attack_jumpa"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Jump strikes, the airborne guard and the landing handoff read naturally for both characters. Both native characters perform early/late sword strikes and guarding during ordinary jumps over Steam. Native scene phase continues through landing. Fourteen checks plus actual D2 recordings cover simultaneous flight/combat, recovery, attachments and preserved health/first person. Moving jumps, heavy/double-jump/contact variants, lifecycle and final visual acceptance remain unfinished.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-combat-corvo-v2.mp4",
    "integrated-air-combat-emily-v2.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: integrated-air-combat-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-air-combat-corvo-v2.mp4>)
- [in_game_recordings: integrated-air-combat-emily-v2.mp4](<../../../../work/natural-moveset/integrated-air-combat-emily-v2.mp4>)
## block_enter_hold_exit

Visual sweep 1260: accepted (corvo accept, emily accept). Block entry, hold and exit are readable standing, crouched and airborne for both characters. The pose is a high overhead block (a polish note). Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished.

````json
{
  "id": "block_enter_hold_exit",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Block entry, hold and exit are readable standing, crouched and airborne for both characters. The pose is a high overhead block (a polish note). Exact owned native sword scenes drive variant, phase and timing over Steam. Authored full-body short/alternating slashes, heavy windup/hold/release and standing/crouched guards pass both-role timing and recovery checks. Two actual D2 recordings show complete bodies. Heavy hold is static; moving contact, air combat, directional parries, hit reactions, paired outcomes and final visual acceptance remain unfinished.",
  "evidence": [
    "priority-2/result.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ],
  "title": "block enter hold exit",
  "motions": [
    "sword_parry_ll_in",
    "sword_parry_ll_out",
    "sword_parry_lr_in",
    "sword_parry_lr_out",
    "sword_parry_rl_in",
    "sword_parry_rl_out",
    "sword_parry_rr_in",
    "sword_parry_rr_out",
    "guard_ll_hold_sequence",
    "guard_lr_hold_sequence",
    "guard_rl_hold_sequence",
    "guard_rr_hold_sequence"
  ],
  "deliverable_kind": "animation",
  "runtime_priority": "Existing native action suppresses the walking/running bank; this does not validate new donor gameplay binding.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-combat-corvo-v1.mp4",
    "integrated-combat-emily-v1.mp4",
    "integrated-air-combat-corvo-v2.mp4",
    "integrated-air-combat-emily-v2.mp4",
    "integrated-moving-combat-corvo-v1.mp4",
    "integrated-moving-combat-emily-v1.mp4",
    "native-feet-corvo-close-v5.mp4",
    "native-feet-emily-close-v5.mp4"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: integrated-combat-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-combat-corvo-v1.mp4>)
- [in_game_recordings: integrated-combat-emily-v1.mp4](<../../../../work/natural-moveset/integrated-combat-emily-v1.mp4>)
- [in_game_recordings: integrated-air-combat-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-air-combat-corvo-v2.mp4>)
- [in_game_recordings: integrated-air-combat-emily-v2.mp4](<../../../../work/natural-moveset/integrated-air-combat-emily-v2.mp4>)
- [in_game_recordings: integrated-moving-combat-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-moving-combat-corvo-v1.mp4>)
- [in_game_recordings: integrated-moving-combat-emily-v1.mp4](<../../../../work/natural-moveset/integrated-moving-combat-emily-v1.mp4>)
- [in_game_recordings: native-feet-corvo-close-v5.mp4](<../../../../work/natural-moveset/native-feet-corvo-close-v5.mp4>)
- [in_game_recordings: native-feet-emily-close-v5.mp4](<../../../../work/natural-moveset/native-feet-emily-close-v5.mp4>)
## parry_directions

1022/1026: the receiving player's exact native defense scenes (sword_parry_hit_1/hit_2 Scene_1/533, sword_parry_win Scene_1/500, correlated with native 140859980 block/parry results) drive the remote full body over Steam (protocol70) as bank68 clips357/358, native NPC deflection donors sword_parry_ll_out / sword_parry_rl_out. Verified with Emily as defender (1023/1025/1028c) and Corvo as defender against host-native enemies (1030: 4 blocks, 2 timed parries -> client clips357 x13, 358 x11). Crouched/sneak impacts, direction-specific acting, block-idle transition and visual acceptance remain open. Directional variants are not distinguished: the native player scenes carry no attack direction.

````json
{
  "id": "parry_directions",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_block_impact_and_timed_parry_both_roles",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "parry directions",
  "motions": [
    "sword_parry_ll_in",
    "sword_parry_lr_in",
    "sword_parry_rl_in",
    "sword_parry_rr_in"
  ],
  "deliverable_kind": "animation",
  "notes": "1022/1026: the receiving player's exact native defense scenes (sword_parry_hit_1/hit_2 Scene_1/533, sword_parry_win Scene_1/500, correlated with native 140859980 block/parry results) drive the remote full body over Steam (protocol70) as bank68 clips357/358, native NPC deflection donors sword_parry_ll_out / sword_parry_rl_out. Verified with Emily as defender (1023/1025/1028c) and Corvo as defender against host-native enemies (1030: 4 blocks, 2 timed parries -> client clips357 x13, 358 x11). Crouched/sneak impacts, direction-specific acting, block-idle transition and visual acceptance remain open. Directional variants are not distinguished: the native player scenes carry no attack direction.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "evidence": [
    "../movement-integration/npc-defense-presentation-audit-v1029.json",
    "../movement-integration/defense-parity-audit-v1031.json"
  ]
}
````

- [evidence: ../movement-integration/npc-defense-presentation-audit-v1029.json](<../../../../work/movement-integration/npc-defense-presentation-audit-v1029.json>)
- [evidence: ../movement-integration/defense-parity-audit-v1031.json](<../../../../work/movement-integration/defense-parity-audit-v1031.json>)
## parry_recovery

1022/1026: the receiving player's exact native defense scenes (sword_parry_hit_1/hit_2 Scene_1/533, sword_parry_win Scene_1/500, correlated with native 140859980 block/parry results) drive the remote full body over Steam (protocol70) as bank68 clips357/358, native NPC deflection donors sword_parry_ll_out / sword_parry_rl_out. Verified with Emily as defender (1023/1025/1028c) and Corvo as defender against host-native enemies (1030: 4 blocks, 2 timed parries -> client clips357 x13, 358 x11). Crouched/sneak impacts, direction-specific acting, block-idle transition and visual acceptance remain open.

````json
{
  "id": "parry_recovery",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_block_impact_and_timed_parry_both_roles",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "parry recovery",
  "motions": [
    "sword_parry_ll_out",
    "sword_parry_lr_out",
    "sword_parry_rl_out",
    "sword_parry_rr_out"
  ],
  "deliverable_kind": "animation",
  "notes": "1022/1026: the receiving player's exact native defense scenes (sword_parry_hit_1/hit_2 Scene_1/533, sword_parry_win Scene_1/500, correlated with native 140859980 block/parry results) drive the remote full body over Steam (protocol70) as bank68 clips357/358, native NPC deflection donors sword_parry_ll_out / sword_parry_rl_out. Verified with Emily as defender (1023/1025/1028c) and Corvo as defender against host-native enemies (1030: 4 blocks, 2 timed parries -> client clips357 x13, 358 x11). Crouched/sneak impacts, direction-specific acting, block-idle transition and visual acceptance remain open.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "evidence": [
    "../movement-integration/npc-defense-presentation-audit-v1029.json",
    "../movement-integration/defense-parity-audit-v1031.json"
  ]
}
````

- [evidence: ../movement-integration/npc-defense-presentation-audit-v1029.json](<../../../../work/movement-integration/npc-defense-presentation-audit-v1029.json>)
- [evidence: ../movement-integration/defense-parity-audit-v1031.json](<../../../../work/movement-integration/defense-parity-audit-v1031.json>)
## counterattack

Audit 1285: Emily's counter-kill parked after takes 1258y-1258ak (the guard rarely attacks her clone; native god mode is stripped). Audit 1281 (1214ak): client finishers now follow on the host (corpse follow 1214ai, follow without AI lease 1214aj) and no longer launch the body (1214ak); finisher-follow-v1258i accepted functionally. Correction: the 1258l finisher was a killing blow (4th wound after a blocked hit), not the counter-kill. Emily counter-kill (parry win then attack) not yet produced. Audit 1280 (1214ah): Emily's timed parry win against a relayed guard hit then attack plays the counter finisher fatality_leftleg_medium_a, presented by the host (phase 105) and dead in both worlds (1258l). This needed the NPC attack relay to follow dynamic relevance. Visual review pending (1258l view too far). Audit 1264: Corvo's timed parry win then attack plays a counter-kill finisher (fatality_head_fast_b) presented by the partner and dead in both worlds (1258f, 1214i). Sword locks (Versus31, versus_low/elite) are presented since 1214h but not yet observed; Emily's counter not yet produced. Candidates require animation review and appropriate game-state/contact binding. 1208/1208b (audit 1211): native timed parry (off-balance) observed; follow-up attack capture failed on input ownership; parry-win (knockdown -> fatality_knockdown) is 0% in this save.

````json
{
  "id": "counterattack",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_counter_kill_finisher_shared_host_source",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "counterattack",
  "motions": [
    "sword_parry_ll_attack",
    "sword_parry_lr_attack",
    "sword_parry_rl_attack",
    "sword_parry_rr_attack",
    "guard_ll_counter_sequence",
    "guard_lr_counter_sequence",
    "guard_rl_counter_sequence",
    "guard_rr_counter_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1285: Emily's counter-kill parked after takes 1258y-1258ak (the guard rarely attacks her clone; native god mode is stripped). Audit 1281 (1214ak): client finishers now follow on the host (corpse follow 1214ai, follow without AI lease 1214aj) and no longer launch the body (1214ak); finisher-follow-v1258i accepted functionally. Correction: the 1258l finisher was a killing blow (4th wound after a blocked hit), not the counter-kill. Emily counter-kill (parry win then attack) not yet produced. Audit 1280 (1214ah): Emily's timed parry win against a relayed guard hit then attack plays the counter finisher fatality_leftleg_medium_a, presented by the host (phase 105) and dead in both worlds (1258l). This needed the NPC attack relay to follow dynamic relevance. Visual review pending (1258l view too far). Audit 1264: Corvo's timed parry win then attack plays a counter-kill finisher (fatality_head_fast_b) presented by the partner and dead in both worlds (1258f, 1214i). Sword locks (Versus31, versus_low/elite) are presented since 1214h but not yet observed; Emily's counter not yet produced. Candidates require animation review and appropriate game-state/contact binding. 1208/1208b (audit 1211): native timed parry (off-balance) observed; follow-up attack capture failed on input ownership; parry-win (knockdown -> fatality_knockdown) is 0% in this save.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "evidence": [
    "../movement-integration/native-combat-use-observations-v1211.json",
    "../movement-integration/gestures-counter-audit-v1264.json",
    "../movement-integration/counter-relay-audit-v1280.json",
    "../movement-integration/finisher-follow-audit-v1281.json",
    "../movement-integration/counter-parked-audit-v1285.json"
  ]
}
````

- [evidence: ../movement-integration/native-combat-use-observations-v1211.json](<../../../../work/movement-integration/native-combat-use-observations-v1211.json>)
- [evidence: ../movement-integration/gestures-counter-audit-v1264.json](<../../../../work/movement-integration/gestures-counter-audit-v1264.json>)
- [evidence: ../movement-integration/counter-relay-audit-v1280.json](<../../../../work/movement-integration/counter-relay-audit-v1280.json>)
- [evidence: ../movement-integration/finisher-follow-audit-v1281.json](<../../../../work/movement-integration/finisher-follow-audit-v1281.json>)
- [evidence: ../movement-integration/counter-parked-audit-v1285.json](<../../../../work/movement-integration/counter-parked-audit-v1285.json>)
## hit_reactions_directions

Visual sweep 1260: accepted (corvo accept, emily accept). The standing stun reaction reads well on both characters. Only the recorded standing direction is covered (source views skipped). Both-role v61b verifies exact owned standing Stun14 scene, directional full-body reaction, same-token Steam continuity and post-stun native travel. Only individually recorded directions are covered; crouched/airborne and all direction variants remain incomplete. See handoff 21.

````json
{
  "id": "hit_reactions_directions",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_network_standing_stun_partial_verified",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "hit reactions directions",
  "motions": [
    "sword_stinghit_back",
    "sword_stinghit_front",
    "sword_stinghit_left",
    "sword_stinghit_right"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The standing stun reaction reads well on both characters. Only the recorded standing direction is covered (source views skipped). Both-role v61b verifies exact owned standing Stun14 scene, directional full-body reaction, same-token Steam continuity and post-stun native travel. Only individually recorded directions are covered; crouched/airborne and all direction variants remain incomplete. See handoff 21.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "grenade-stun-scenes-corvo-v60.mp4",
    "grenade-stun-scenes-emily-v60.mp4",
    "stun-network-corvo-v61.mp4",
    "stun-network-corvo-v61-source.mp4",
    "stun-network-corvo-v61b.mp4",
    "stun-network-corvo-v61b-source.mp4",
    "stun-network-emily-v61b.mp4",
    "stun-network-emily-v61b-source.mp4"
  ],
  "native_integration": {
    "grenade-stun-scenes-corvo-v60.mp4": {
      "video": "grenade-stun-scenes-corvo-v60.mp4",
      "character": "corvo",
      "label": "Native source observation · grenade-stun-scenes-corvo-v60",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-stun-scenes-corvo-v60/result.json",
      "visual_review": "grenade-stun-scenes-corvo-v60/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.32590529247911,
      "video_sha256": "61CB8D61299788D905DAEFE1EEE0695C4B012D73772298FED012A476DE5EEBB8",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "grenade-stun-scenes-emily-v60.mp4": {
      "video": "grenade-stun-scenes-emily-v60.mp4",
      "character": "emily",
      "label": "Native source observation · grenade-stun-scenes-emily-v60",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "grenade-stun-scenes-emily-v60/result.json",
      "visual_review": "grenade-stun-scenes-emily-v60/visual-review.json",
      "proxy_sha256": "129137DA59399401ACFFC7CF9CD41244C43D18FD737DC37A3BA6CA2D5D411DB2",
      "actual_capture_hz": 27.24233983286908,
      "video_sha256": "2925AF839D1C8FF22E12E5340334EDE279DB659A0C77EAD036848E269A3E6D80",
      "checks_passed": true,
      "scope": "Source native one-grenade action observation only. Real native projectile/world effects are reset through the isolated checkpoints. No remote grenade animation, damage authority or projectile replication is claimed."
    },
    "stun-network-corvo-v61.mp4": {
      "video": "stun-network-corvo-v61.mp4",
      "character": "corvo",
      "label": "Native stun network · stun-network-corvo-v61",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "stun-network-corvo-v61/result.json",
      "visual_review": "stun-network-corvo-v61/visual-review.json",
      "proxy_sha256": "A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC",
      "actual_capture_hz": 27.5,
      "video_sha256": "75654D728F226A7977A03A6C1C23E789326618E5E872E57EF1A06667C43C28CF",
      "checks_passed": false,
      "scope": "Native standing grenade self-stun reaction and uninterrupted source-clock movement over Steam. Directional native NPC body clips are cosmetic and require visual review. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "stun-network-corvo-v61-source.mp4": {
      "video": "stun-network-corvo-v61-source.mp4",
      "character": "corvo",
      "label": "Native stun network · stun-network-corvo-v61-source",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "stun-network-corvo-v61/result.json",
      "visual_review": "stun-network-corvo-v61/visual-review.json",
      "proxy_sha256": "A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC",
      "actual_capture_hz": 27.75,
      "video_sha256": "76D98A5C3013994ACC20E27ACB9C7663628AA51D6A3B742E104EB7E77319D97E",
      "checks_passed": false,
      "scope": "Native standing grenade self-stun reaction and uninterrupted source-clock movement over Steam. Directional native NPC body clips are cosmetic and require visual review. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "stun-network-corvo-v61b.mp4": {
      "video": "stun-network-corvo-v61b.mp4",
      "character": "corvo",
      "label": "Native stun network · stun-network-corvo-v61b",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "stun-network-corvo-v61b/result.json",
      "visual_review": "stun-network-corvo-v61b/visual-review.json",
      "proxy_sha256": "A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC",
      "actual_capture_hz": 27.23463687150838,
      "video_sha256": "D60C3924510185E4D0D7F4FC768D5E9B663298606E60AC17B930CFFCBE8D440E",
      "checks_passed": true,
      "scope": "Native standing grenade self-stun reaction and uninterrupted source-clock movement over Steam. Directional native NPC body clips are cosmetic and require visual review. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "stun-network-corvo-v61b-source.mp4": {
      "video": "stun-network-corvo-v61b-source.mp4",
      "character": "corvo",
      "label": "Native stun network · stun-network-corvo-v61b-source",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "stun-network-corvo-v61b/result.json",
      "visual_review": "stun-network-corvo-v61b/visual-review.json",
      "proxy_sha256": "A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC",
      "actual_capture_hz": 27.910863509749305,
      "video_sha256": "F5B4CC7DC2DDA4D7B2253D122297FE2F341AC56FB63E05DC0D15F930B9AB4148",
      "checks_passed": true,
      "scope": "Native standing grenade self-stun reaction and uninterrupted source-clock movement over Steam. Directional native NPC body clips are cosmetic and require visual review. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "stun-network-emily-v61b.mp4": {
      "video": "stun-network-emily-v61b.mp4",
      "character": "emily",
      "label": "Native stun network · stun-network-emily-v61b",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "stun-network-emily-v61b/result.json",
      "visual_review": "stun-network-emily-v61b/visual-review.json",
      "proxy_sha256": "A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC",
      "actual_capture_hz": 27.916666666666668,
      "video_sha256": "AFFB2C1C9DE66CDE4322D2FF1FE77867511D99FBA4DCF1B7168158FBE314AF12",
      "checks_passed": true,
      "scope": "Native standing grenade self-stun reaction and uninterrupted source-clock movement over Steam. Directional native NPC body clips are cosmetic and require visual review. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    },
    "stun-network-emily-v61b-source.mp4": {
      "video": "stun-network-emily-v61b-source.mp4",
      "character": "emily",
      "label": "Native stun network · stun-network-emily-v61b-source",
      "alias": "hit_react_stun",
      "requirements": [
        "hit_reactions_directions"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "stun-network-emily-v61b/result.json",
      "visual_review": "stun-network-emily-v61b/visual-review.json",
      "proxy_sha256": "A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC",
      "actual_capture_hz": 27.75,
      "video_sha256": "443627E76F3C3EDF9B6E886E0C6B4A0851C489340A53DFB3BEE2FA428AA9CEFB",
      "checks_passed": true,
      "scope": "Native standing grenade self-stun reaction and uninterrupted source-clock movement over Steam. Directional native NPC body clips are cosmetic and require visual review. Native source alone creates its real projectile. Receiver cosmetic prop has no projectile or damage component; projectile/world/damage replication remains unimplemented. Harness restores both checkpoints after firing. Visual acceptance separate."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [in_game_recordings: grenade-stun-scenes-corvo-v60.mp4](<../../../../work/natural-moveset/grenade-stun-scenes-corvo-v60.mp4>)
- [in_game_recordings: grenade-stun-scenes-emily-v60.mp4](<../../../../work/natural-moveset/grenade-stun-scenes-emily-v60.mp4>)
- [in_game_recordings: stun-network-corvo-v61.mp4](<../../../../work/natural-moveset/stun-network-corvo-v61.mp4>)
- [in_game_recordings: stun-network-corvo-v61-source.mp4](<../../../../work/natural-moveset/stun-network-corvo-v61-source.mp4>)
- [in_game_recordings: stun-network-corvo-v61b.mp4](<../../../../work/natural-moveset/stun-network-corvo-v61b.mp4>)
- [in_game_recordings: stun-network-corvo-v61b-source.mp4](<../../../../work/natural-moveset/stun-network-corvo-v61b-source.mp4>)
- [in_game_recordings: stun-network-emily-v61b.mp4](<../../../../work/natural-moveset/stun-network-emily-v61b.mp4>)
- [in_game_recordings: stun-network-emily-v61b-source.mp4](<../../../../work/natural-moveset/stun-network-emily-v61b-source.mp4>)
## stagger_knockdown_recovery

Candidates require animation review and appropriate game-state/contact binding. 1209 (audit 1211): no player knockdown provoked in an attack spree; native Corvo knockdown fall/front/land/recovery assets identified (PLAYER_HITREACT_KNOCKDOWN blowback).

````json
{
  "id": "stagger_knockdown_recovery",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "not_verified",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "stagger knockdown recovery",
  "motions": [
    "sword_recovery_fast_front",
    "sword_recovery_fast_back",
    "sword_parry_lose_left",
    "sword_parry_lose_right",
    "sword_recovery_a_idle",
    "sword_recovery_fast_left"
  ],
  "deliverable_kind": "animation",
  "notes": "Candidates require animation review and appropriate game-state/contact binding. 1209 (audit 1211): no player knockdown provoked in an attack spree; native Corvo knockdown fall/front/land/recovery assets identified (PLAYER_HITREACT_KNOCKDOWN blowback).",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "evidence": [
    "../movement-integration/native-combat-use-observations-v1211.json"
  ]
}
````

- [evidence: ../movement-integration/native-combat-use-observations-v1211.json](<../../../../work/movement-integration/native-combat-use-observations-v1211.json>)
## kick_or_shove_if_present

Audit 1285: free-camera take coverage-v1284a produced no native kick in 8 close attacks per role; view still open. Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). Neither recording shows a partner's kick body, so this needs a partner-facing take. Candidates require animation review and appropriate game-state/contact binding. 1109 (candidate1108, Edge of the World): the native player kick (player_kick.damage, zero-normal hit) on a host-authoritative guard is captured once on the host and applied once on the client; guard health converges. Kick body presentation on the remote player and client-sourced kicks remain open.

````json
{
  "id": "kick_or_shove_if_present",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_player_kick_outcome_host_source_mission_map",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "kick or shove if present",
  "motions": [
    "sword_attack_kick",
    "sword_attack_punch"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1285: free-camera take coverage-v1284a produced no native kick in 8 close attacks per role; view still open. Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). Neither recording shows a partner's kick body, so this needs a partner-facing take. Candidates require animation review and appropriate game-state/contact binding. 1109 (candidate1108, Edge of the World): the native player kick (player_kick.damage, zero-normal hit) on a host-authoritative guard is captured once on the host and applied once on the client; guard health converges. Kick body presentation on the remote player and client-sourced kicks remain open.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "mission-combat-v1109/combat-client.mp4",
    "mission-combat-v1109/combat-host.mp4"
  ],
  "evidence": [
    "../movement-integration/mission-outcome-audit-v1110.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json",
    "../movement-integration/counter-parked-audit-v1285.json"
  ]
}
````

- [evidence: ../movement-integration/mission-outcome-audit-v1110.json](<../../../../work/movement-integration/mission-outcome-audit-v1110.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [evidence: ../movement-integration/counter-parked-audit-v1285.json](<../../../../work/movement-integration/counter-parked-audit-v1285.json>)
- [in_game_recordings: mission-combat-v1109/combat-client.mp4](<../../../../work/natural-moveset/mission-combat-v1109/combat-client.mp4>)
- [in_game_recordings: mission-combat-v1109/combat-host.mp4](<../../../../work/natural-moveset/mission-combat-v1109/combat-host.mp4>)
## death_variants

Audit 1267: Corvo-sourced NPC deaths seen in the client window (1254b); Emily-sourced in sweep 1260 (1117). Visual sweep 1260: insufficient (corvo insufficient, emily accept). The client-sourced NPC death and corpse read naturally. No recording of a host-sourced (Corvo) death was provided. Audit 1257: sword finishers (census-1255 fatalities) are presented as paired lethal scenes and the death is shared, 0.0 m, in both source roles: host 96, 109, 113; client generic 92 and four-binding 95. Candidates require animation review and appropriate game-state/contact binding. 1117 (candidate1113d): a client-sourced native fatality on a host-authoritative mission guard yields one death on both peers and an identical corpse (0.0 m), natural slumped pose on the client. Player-character death variants remain separate.

````json
{
  "id": "death_variants",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_npc_death_and_corpse_both_peers_mission_map",
  "visual_validation": "accepted_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "death variants",
  "motions": [
    "generic_deathfast_left",
    "generic_deathfast_front",
    "generic_deathfast_back",
    "generic_deathfast_right",
    "empty_stinghit_death"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1267: Corvo-sourced NPC deaths seen in the client window (1254b); Emily-sourced in sweep 1260 (1117). Visual sweep 1260: insufficient (corvo insufficient, emily accept). The client-sourced NPC death and corpse read naturally. No recording of a host-sourced (Corvo) death was provided. Audit 1257: sword finishers (census-1255 fatalities) are presented as paired lethal scenes and the death is shared, 0.0 m, in both source roles: host 96, 109, 113; client generic 92 and four-binding 95. Candidates require animation review and appropriate game-state/contact binding. 1117 (candidate1113d): a client-sourced native fatality on a host-authoritative mission guard yields one death on both peers and an identical corpse (0.0 m), natural slumped pose on the client. Player-character death variants remain separate.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "mission-combat-v1117/combat-client.mp4",
    "takedown-variants-v1254b/slide-host-lethal-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/mission-corpse-audit-v1118.json",
    "../movement-integration/takedown-variants-audit-v1257.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json",
    "../movement-integration/sweep-defects-audit-v1267.json"
  ]
}
````

- [evidence: ../movement-integration/mission-corpse-audit-v1118.json](<../../../../work/movement-integration/mission-corpse-audit-v1118.json>)
- [evidence: ../movement-integration/takedown-variants-audit-v1257.json](<../../../../work/movement-integration/takedown-variants-audit-v1257.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [evidence: ../movement-integration/sweep-defects-audit-v1267.json](<../../../../work/movement-integration/sweep-defects-audit-v1267.json>)
- [in_game_recordings: mission-combat-v1117/combat-client.mp4](<../../../../work/natural-moveset/mission-combat-v1117/combat-client.mp4>)
- [in_game_recordings: takedown-variants-v1254b/slide-host-lethal-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/slide-host-lethal-observer.mp4>)
## ragdoll_handoff

Audit 1267: the killed guard's corpse in the observer world for both source roles (1254b, 1117). Visual sweep 1260: insufficient (corvo insufficient, emily accept). The corpse hold after a client-sourced kill is stable, but there is no host-sourced (Corvo) recording, and the release transition isn't in frame. This requires native ragdoll ownership and a tested transition, not a baked animation alone. 1113d/1117: after a shared NPC death the host streams the corpse while the client holds its ragdoll animated (native physics mode switch) at the host pose; ragdoll release happens at channel stop. Player-character ragdoll and carry handoff remain open.

````json
{
  "id": "ragdoll_handoff",
  "family": "combat",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "missing",
  "runtime_binding": "partial_native_steam_npc_corpse_authority_mission_map",
  "visual_validation": "accepted_functional_both_roles",
  "native_reference_family_count": 195,
  "priority": 2,
  "title": "ragdoll handoff",
  "motions": [],
  "deliverable_kind": "lifecycle_transition",
  "notes": "Audit 1267: the killed guard's corpse in the observer world for both source roles (1254b, 1117). Visual sweep 1260: insufficient (corvo insufficient, emily accept). The corpse hold after a client-sourced kill is stable, but there is no host-sourced (Corvo) recording, and the release transition isn't in frame. This requires native ragdoll ownership and a tested transition, not a baked animation alone. 1113d/1117: after a shared NPC death the host streams the corpse while the client holds its ragdoll animated (native physics mode switch) at the host pose; ragdoll release happens at channel stop. Player-character ragdoll and carry handoff remain open.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "carry-snapshot-corvo-v20/native.mp4",
    "carry-snapshot-emily-v20/native.mp4",
    "carry-snapshot-cancel-corvo-v20/native.mp4",
    "carry-snapshot-cancel-corvo-v21/native.mp4",
    "carry-snapshot-cancel-emily-v21/native.mp4",
    "carry-snapshot-cancel-emily-v22/native.mp4",
    "carry-snapshot-cancel-corvo-v22/native.mp4",
    "carry-snapshot-cancel-emily-v22b/native.mp4",
    "carry-snapshot-stream-emily-v22/native.mp4",
    "mission-combat-v1117/combat-client.mp4",
    "takedown-variants-v1254b/drop-host-lethal-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/mission-corpse-audit-v1118.json",
    "../movement-integration/visual-sweep-v1260/combat_paired-review.json",
    "../movement-integration/sweep-defects-audit-v1267.json"
  ]
}
````

- [evidence: ../movement-integration/mission-corpse-audit-v1118.json](<../../../../work/movement-integration/mission-corpse-audit-v1118.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/combat_paired-review.json](<../../../../work/movement-integration/visual-sweep-v1260/combat_paired-review.json>)
- [evidence: ../movement-integration/sweep-defects-audit-v1267.json](<../../../../work/movement-integration/sweep-defects-audit-v1267.json>)
- [in_game_recordings: carry-snapshot-corvo-v20/native.mp4](<../../../../work/natural-moveset/carry-snapshot-corvo-v20/native.mp4>)
- [in_game_recordings: carry-snapshot-emily-v20/native.mp4](<../../../../work/natural-moveset/carry-snapshot-emily-v20/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-corvo-v20/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-corvo-v20/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-corvo-v21/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-corvo-v21/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-emily-v21/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-emily-v21/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-emily-v22/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-emily-v22/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-corvo-v22/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-corvo-v22/native.mp4>)
- [in_game_recordings: carry-snapshot-cancel-emily-v22b/native.mp4](<../../../../work/natural-moveset/carry-snapshot-cancel-emily-v22b/native.mp4>)
- [in_game_recordings: carry-snapshot-stream-emily-v22/native.mp4](<../../../../work/natural-moveset/carry-snapshot-stream-emily-v22/native.mp4>)
- [in_game_recordings: mission-combat-v1117/combat-client.mp4](<../../../../work/natural-moveset/mission-combat-v1117/combat-client.mp4>)
- [in_game_recordings: takedown-variants-v1254b/drop-host-lethal-observer.mp4](<../../../../work/natural-moveset/takedown-variants-v1254b/drop-host-lethal-observer.mp4>)

