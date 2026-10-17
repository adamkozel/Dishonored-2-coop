# locomotion: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-18T00:58:40.854295+02:00. All fields below retain the catalog wording.

## stand_idle

Visual sweep 1260: accepted (corvo accept, emily accept). Both characters hold natural, planted standing idles, armed and holstered, in the v645 observer views. Emily is partly cropped at the left edge in some segments. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.

````json
{
  "id": "stand_idle",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_batched_locomotion_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "title": "stand idle",
  "motions": [
    "empty_idle",
    "mixamo_idle"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Both characters hold natural, planted standing idles, armed and holstered, in the v645 observer views. Emily is partly cropped at the left edge in some segments. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-gait-batch-v645/host-gait-batch.mp4",
    "native-gait-batch-v645/host-observer.mp4",
    "native-gait-batch-v645/client-gait-batch.mp4",
    "native-gait-batch-v645/client-observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-gait-batch-v645/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ]
}
````

- [evidence: native-gait-batch-v645/result.json](<../../../../work/natural-moveset/native-gait-batch-v645/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: native-gait-batch-v645/host-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-observer.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-observer.mp4>)
## armed_idle

Visual sweep 1260: accepted (corvo accept, emily accept). The armed idle reads correctly for both: Corvo's sword is low and Emily holds a wide ready stance. Native attack and jump interrupt and recover cleanly. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.

````json
{
  "id": "armed_idle",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_batched_locomotion_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "evidence": [
    "walk-test-3/result.json",
    "corvo-walking.mp4",
    "emily-walking.mp4",
    "native-gait-batch-v645/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ],
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The armed idle reads correctly for both: Corvo's sword is low and Emily holds a wide ready stance. Native attack and jump interrupt and recover cleanly. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.",
  "title": "armed idle",
  "motions": [
    "sword_idle",
    "armed_idle"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "native-gait-batch-v645/host-gait-batch.mp4",
    "native-gait-batch-v645/host-observer.mp4",
    "native-gait-batch-v645/client-gait-batch.mp4",
    "native-gait-batch-v645/client-observer.mp4"
  ],
  "complete": false
}
````

- [evidence: native-gait-batch-v645/result.json](<../../../../work/natural-moveset/native-gait-batch-v645/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-observer.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-observer.mp4>)
## walk_8_directions

Visual sweep 1260: accepted (corvo accept, emily accept). Standing walking reads with alternating strides in forward, back, lateral and diagonal headings for both characters, including through the missing-binding fault. Not every one of the 8 directions is shown in isolation. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending.

````json
{
  "id": "walk_8_directions",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "evidence": [
    "walk-test-3/result.json",
    "corvo-walking.mp4",
    "emily-walking.mp4",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ],
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Standing walking reads with alternating strides in forward, back, lateral and diagonal headings for both characters, including through the missing-binding fault. Not every one of the 8 directions is shown in isolation. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending.",
  "title": "walk 8 directions",
  "motions": [
    "sword_walk_e_add",
    "sword_walk_n_add",
    "sword_walk_s_add",
    "sword_walk_w_add",
    "walk_forward",
    "walk_forward_left",
    "walk_left",
    "walk_back_left",
    "walk_back",
    "walk_back_right",
    "walk_right",
    "walk_forward_right"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "unbound-presentation-v559c-host.mp4",
    "unbound-presentation-v559c-client.mp4",
    "unbound-presentation-v559b-host.mp4"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: unbound-presentation-v559c-host.mp4](<../../../../work/natural-moveset/unbound-presentation-v559c-host.mp4>)
- [in_game_recordings: unbound-presentation-v559c-client.mp4](<../../../../work/natural-moveset/unbound-presentation-v559c-client.mp4>)
- [in_game_recordings: unbound-presentation-v559b-host.mp4](<../../../../work/natural-moveset/unbound-presentation-v559b-host.mp4>)
## run_8_directions

Audit 1267: runs in 8 directions accepted in both roles (coverage 1267a). Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). The only listed footage is labelled walking and shows one default-speed standing gait in a few headings. A distinct run tier in 8 directions cannot be identified, though the body is fine. Run strides do appear in the moving-turn and sprint takes of other items. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending.

````json
{
  "id": "run_8_directions",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "notes": "Audit 1267: runs in 8 directions accepted in both roles (coverage 1267a). Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). The only listed footage is labelled walking and shows one default-speed standing gait in a few headings. A distinct run tier in 8 directions cannot be identified, though the body is fine. Run strides do appear in the moving-turn and sprint takes of other items. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending.",
  "title": "run 8 directions",
  "motions": [
    "sword_run_e_add",
    "sword_run_n_add",
    "sword_run_s_add",
    "sword_run_w_add",
    "run_forward",
    "run_forward_left",
    "run_left",
    "run_back_left",
    "run_back",
    "run_back_right",
    "run_right",
    "run_forward_right"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "coverage-v1267a/client-run8-observer.mp4",
    "coverage-v1267a/host-run8-observer.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json",
    "../movement-integration/sweep-defects-audit-v1267.json"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [evidence: ../movement-integration/sweep-defects-audit-v1267.json](<../../../../work/movement-integration/sweep-defects-audit-v1267.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: coverage-v1267a/client-run8-observer.mp4](<../../../../work/natural-moveset/coverage-v1267a/client-run8-observer.mp4>)
- [in_game_recordings: coverage-v1267a/host-run8-observer.mp4](<../../../../work/natural-moveset/coverage-v1267a/host-run8-observer.mp4>)
## sprint_forward_and_diagonals

Visual sweep 1260: accepted (corvo accept, emily accept). Both characters sprint with a clear forward lean and long stride in several headings, and stop and crouch cleanly. Small in a wide room. Exact native Sprinting state selects forward/diagonal whole-body sprint. Distance drives cadence. Both-role checks and actual D2 recordings exist; general terrain/contact and final visual acceptance remain pending.

````json
{
  "id": "sprint_forward_and_diagonals",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Both characters sprint with a clear forward lean and long stride in several headings, and stop and crouch cleanly. Small in a wide room. Exact native Sprinting state selects forward/diagonal whole-body sprint. Distance drives cadence. Both-role checks and actual D2 recordings exist; general terrain/contact and final visual acceptance remain pending.",
  "title": "sprint forward and diagonals",
  "motions": [
    "sprint_forward",
    "sprint_forward_left",
    "sprint_forward_right"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-sprint-corvo-v2.mp4",
    "integrated-sprint-emily-v2.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ]
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: integrated-sprint-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-corvo-v2.mp4>)
- [in_game_recordings: integrated-sprint-emily-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-emily-v2.mp4>)
## start_stop_each_gait

Visual sweep 1260: accepted (corvo accept, emily accept). Walk, run and sprint starts and stops settle into planted armed idles without freezing or sliding for both characters. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.

````json
{
  "id": "start_stop_each_gait",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_batched_locomotion_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Walk, run and sprint starts and stops settle into planted armed idles without freezing or sliding for both characters. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.",
  "title": "start stop each gait",
  "motions": [
    "sword_run_e_stopleft",
    "sword_run_e_stopright",
    "sword_run_s_startleft",
    "sword_run_s_startright",
    "sword_run_s_stopleft",
    "sword_run_s_stopright",
    "sword_run_startleft",
    "sword_run_startright",
    "sword_run_stopleft",
    "sword_run_stopright",
    "sword_run_w_stopleft",
    "sword_run_w_stopright",
    "sword_walk_e_stopleft",
    "sword_walk_e_stopright",
    "sword_walk_s_startleft",
    "sword_walk_s_startright",
    "sword_walk_s_stopleft",
    "sword_walk_s_stopright",
    "sword_walk_startleft",
    "sword_walk_startright",
    "sword_walk_stopleft",
    "sword_walk_stopright",
    "sword_walk_w_stopleft",
    "sword_walk_w_stopright",
    "sword_run_e_startright",
    "sword_run_w_startleft",
    "sword_walk_e_startright",
    "sword_walk_w_startleft",
    "mixamo_run_to_stop"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-sprint-corvo-v2.mp4",
    "integrated-sprint-emily-v2.mp4",
    "moving-turn-corvo-knee-v4b.mp4",
    "moving-turn-corvo-unarmed-knee-v4.mp4",
    "moving-turn-emily-knee-v4.mp4",
    "moving-turn-emily-unarmed-knee-v4.mp4",
    "native-gait-batch-v645/host-gait-batch.mp4",
    "native-gait-batch-v645/host-observer.mp4",
    "native-gait-batch-v645/client-gait-batch.mp4",
    "native-gait-batch-v645/client-observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-gait-batch-v645/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ]
}
````

- [evidence: native-gait-batch-v645/result.json](<../../../../work/natural-moveset/native-gait-batch-v645/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: integrated-sprint-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-corvo-v2.mp4>)
- [in_game_recordings: integrated-sprint-emily-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-emily-v2.mp4>)
- [in_game_recordings: moving-turn-corvo-knee-v4b.mp4](<../../../../work/natural-moveset/moving-turn-corvo-knee-v4b.mp4>)
- [in_game_recordings: moving-turn-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/moving-turn-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: moving-turn-emily-knee-v4.mp4](<../../../../work/natural-moveset/moving-turn-emily-knee-v4.mp4>)
- [in_game_recordings: moving-turn-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/moving-turn-emily-unarmed-knee-v4.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-observer.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-observer.mp4>)
## gait_speed_changes

Visual sweep 1260: accepted (corvo accept, emily accept). Walk-to-sprint-to-stop and walk-to-idle speed changes blend plausibly for both characters. Emily is partly cropped in the v645 batch. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.

````json
{
  "id": "gait_speed_changes",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_batched_locomotion_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "title": "gait speed changes",
  "motions": [
    "gait_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Walk-to-sprint-to-stop and walk-to-idle speed changes blend plausibly for both characters. Emily is partly cropped in the v645 batch. Standing/crouched bank passes both-role directional, distance-phase and native action-priority checks. Root source timestamps and bounded recovery from packet gaps passed the recorded sprint trace. Terrain, equipment, visual acceptance and automatic session integration remain pending. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "integrated-sprint-corvo-v2.mp4",
    "integrated-sprint-emily-v2.mp4",
    "native-gait-batch-v645/host-gait-batch.mp4",
    "native-gait-batch-v645/host-observer.mp4",
    "native-gait-batch-v645/client-gait-batch.mp4",
    "native-gait-batch-v645/client-observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-gait-batch-v645/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ]
}
````

- [evidence: native-gait-batch-v645/result.json](<../../../../work/natural-moveset/native-gait-batch-v645/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: integrated-sprint-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-corvo-v2.mp4>)
- [in_game_recordings: integrated-sprint-emily-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-emily-v2.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-observer.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-observer.mp4>)
## turn_in_place_90_180

Visual sweep 1260: accepted (corvo accept, emily accept). In-place turns step the feet around a stationary root for both characters, armed and holstered. The ~4 s heading snaps come from the harness's per-trial setviewpos reset, not the presentation. Saved both-role armed and unarmed tests passed continuous left/right standing/crouched/standing turns and recovery with stationary native root. This is not full per-angle90/180 acceptance or fresh regression on the current build. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.

````json
{
  "id": "turn_in_place_90_180",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_stance_turns_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). In-place turns step the feet around a stationary root for both characters, armed and holstered. The ~4 s heading snaps come from the harness's per-trial setviewpos reset, not the presentation. Saved both-role armed and unarmed tests passed continuous left/right standing/crouched/standing turns and recovery with stationary native root. This is not full per-angle90/180 acceptance or fresh regression on the current build. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.",
  "evidence": [
    "turns/quality.json",
    "turns-chest/angular-timing-quality.json",
    "turns-timed/manifest.json",
    "turn-chest-previews/turn_right_90-60.png",
    "turn-stance-corvo-knee-v4/result.json",
    "turn-stance-corvo-unarmed-knee-v4/result.json",
    "turn-stance-emily-knee-v4/result.json",
    "turn-stance-emily-unarmed-knee-v4/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ],
  "title": "turn in place 90 180",
  "motions": [
    "sword_turnleft90",
    "sword_turnleft180",
    "sword_turnright90",
    "sword_turnright180",
    "mixamo_left_turn",
    "mixamo_right_turn"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "turn-corvo-v18-v2.mp4",
    "turn-corvo-unarmed-v18-v2.mp4",
    "turn-emily-v18-v2.mp4",
    "turn-emily-unarmed-v18-v2.mp4",
    "turn-stance-corvo-knee-v4.mp4",
    "turn-stance-corvo-unarmed-knee-v4.mp4",
    "turn-stance-emily-knee-v4.mp4",
    "turn-stance-emily-unarmed-knee-v4.mp4"
  ],
  "complete": false,
  "visual_accepted": false
}
````

- [evidence: turn-stance-corvo-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-corvo-knee-v4/result.json>)
- [evidence: turn-stance-corvo-unarmed-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-corvo-unarmed-knee-v4/result.json>)
- [evidence: turn-stance-emily-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-emily-knee-v4/result.json>)
- [evidence: turn-stance-emily-unarmed-knee-v4/result.json](<../../../../work/natural-moveset/turn-stance-emily-unarmed-knee-v4/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: turn-corvo-v18-v2.mp4](<../../../../work/natural-moveset/turn-corvo-v18-v2.mp4>)
- [in_game_recordings: turn-corvo-unarmed-v18-v2.mp4](<../../../../work/natural-moveset/turn-corvo-unarmed-v18-v2.mp4>)
- [in_game_recordings: turn-emily-v18-v2.mp4](<../../../../work/natural-moveset/turn-emily-v18-v2.mp4>)
- [in_game_recordings: turn-emily-unarmed-v18-v2.mp4](<../../../../work/natural-moveset/turn-emily-unarmed-v18-v2.mp4>)
- [in_game_recordings: turn-stance-corvo-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-knee-v4.mp4>)
- [in_game_recordings: turn-stance-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-knee-v4.mp4>)
- [in_game_recordings: turn-stance-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/turn-stance-emily-unarmed-knee-v4.mp4>)
## moving_turns

Visual sweep 1260: accepted (corvo accept, emily accept). Curved walk, run and sprint loops read with the stride following the curve, and sprint lean in the tight arcs past the camera, for both characters armed and holstered. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.

````json
{
  "id": "moving_turns",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_batched_locomotion_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "title": "moving turns",
  "motions": [
    "sword_turntorunleft45",
    "sword_turntorunleft90",
    "sword_turntorunleft135",
    "sword_turntorunleft180",
    "sword_turntorunright45",
    "sword_turntorunright90",
    "sword_turntorunright135",
    "sword_turntorunright180",
    "sword_turntowalkleft45",
    "sword_turntowalkleft90",
    "sword_turntowalkleft135",
    "sword_turntowalkleft180",
    "sword_turntowalkright45",
    "sword_turntowalkright90",
    "sword_turntowalkright135",
    "sword_turntowalkright180"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Curved walk, run and sprint loops read with the stride following the curve, and sprint lean in the tight arcs past the camera, for both characters armed and holstered. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "moving-turn-corvo-knee-v4b.mp4",
    "moving-turn-corvo-unarmed-knee-v4.mp4",
    "moving-turn-emily-knee-v4.mp4",
    "moving-turn-emily-unarmed-knee-v4.mp4",
    "native-gait-batch-v645/host-gait-batch.mp4",
    "native-gait-batch-v645/host-observer.mp4",
    "native-gait-batch-v645/client-gait-batch.mp4",
    "native-gait-batch-v645/client-observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-gait-batch-v645/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ]
}
````

- [evidence: native-gait-batch-v645/result.json](<../../../../work/natural-moveset/native-gait-batch-v645/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: moving-turn-corvo-knee-v4b.mp4](<../../../../work/natural-moveset/moving-turn-corvo-knee-v4b.mp4>)
- [in_game_recordings: moving-turn-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/moving-turn-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: moving-turn-emily-knee-v4.mp4](<../../../../work/natural-moveset/moving-turn-emily-knee-v4.mp4>)
- [in_game_recordings: moving-turn-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/moving-turn-emily-unarmed-knee-v4.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-observer.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-observer.mp4>)
## reverse_and_direction_changes

Visual sweep 1260: accepted (corvo accept, emily accept). Reversals and direction changes blend without pops for both. Emily's v645 backward and moving-turn segments partly leave the frame but the visible parts are intact. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.

````json
{
  "id": "reverse_and_direction_changes",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_batched_locomotion_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Reversals and direction changes blend without pops for both. Emily's v645 backward and moving-turn segments partly leave the frame but the visible parts are intact. Candidates require animation review and appropriate game-state/contact binding. 645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.",
  "title": "reverse and direction changes",
  "motions": [
    "sword_run_turnleft180_left",
    "sword_run_turnleft180_right",
    "sword_run_turnright180_left",
    "sword_run_turnright180_right",
    "walk_forward_left",
    "walk_left",
    "walk_back_left",
    "walk_back",
    "walk_back_right",
    "walk_right",
    "walk_forward_right",
    "run_forward_left",
    "run_left",
    "run_back_left",
    "run_back",
    "run_back_right",
    "run_right",
    "run_forward_right"
  ],
  "deliverable_kind": "animation",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-stance-corvo-v1.mp4",
    "integrated-stance-emily-v1.mp4",
    "integrated-sprint-corvo-v2.mp4",
    "integrated-sprint-emily-v2.mp4",
    "native-gait-batch-v645/host-gait-batch.mp4",
    "native-gait-batch-v645/host-observer.mp4",
    "native-gait-batch-v645/client-gait-batch.mp4",
    "native-gait-batch-v645/client-observer.mp4"
  ],
  "complete": false,
  "evidence": [
    "native-gait-batch-v645/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ]
}
````

- [evidence: native-gait-batch-v645/result.json](<../../../../work/natural-moveset/native-gait-batch-v645/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: integrated-stance-corvo-v1.mp4](<../../../../work/natural-moveset/integrated-stance-corvo-v1.mp4>)
- [in_game_recordings: integrated-stance-emily-v1.mp4](<../../../../work/natural-moveset/integrated-stance-emily-v1.mp4>)
- [in_game_recordings: integrated-sprint-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-corvo-v2.mp4>)
- [in_game_recordings: integrated-sprint-emily-v2.mp4](<../../../../work/natural-moveset/integrated-sprint-emily-v2.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/host-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/host-observer.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-gait-batch.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-gait-batch.mp4>)
- [in_game_recordings: native-gait-batch-v645/client-observer.mp4](<../../../../work/natural-moveset/native-gait-batch-v645/client-observer.mp4>)
## slopes_and_stairs

Visual sweep 1260: accepted (corvo accept, emily accept). Both characters step up and down the Safe Room staircase with height following the steps and no floating. They are small in frame, so fine foot contact is unjudged and other slopes are not covered. Saved both-role armed/unarmed Dunwall staircase tests passed up/down grade-selected cycles, native height following, stopped cadence and preserved health/equipment. Arbitrary slopes, collision surfaces, broader terrain and visual acceptance remain open. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.

````json
{
  "id": "slopes_and_stairs",
  "family": "locomotion",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_dunwall_stairs_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 90,
  "priority": 1,
  "title": "slopes and stairs",
  "motions": [
    "sword_run_slopes_down",
    "sword_run_slopes_up",
    "sword_run_stairs_down",
    "sword_run_stairs_up",
    "sword_walk_slopes_down",
    "sword_walk_slopes_up",
    "sword_walk_stairs_down",
    "sword_walk_stairs_up"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Both characters step up and down the Safe Room staircase with height following the steps and no floating. They are small in frame, so fine foot contact is unjudged and other slopes are not covered. Saved both-role armed/unarmed Dunwall staircase tests passed up/down grade-selected cycles, native height following, stopped cadence and preserved health/equipment. Arbitrary slopes, collision surfaces, broader terrain and visual acceptance remain open. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.",
  "phase_status": "work_remaining",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-feet-stairs-corvo-v5b.mp4",
    "native-feet-stairs-emily-v5.mp4",
    "terrain-corvo-v17-v2.mp4",
    "terrain-corvo-unarmed-v17-v2.mp4",
    "terrain-emily-v17-v2.mp4",
    "terrain-emily-unarmed-v17-v2.mp4",
    "terrain-corvo-knee-v4.mp4",
    "terrain-corvo-unarmed-knee-v4.mp4",
    "terrain-emily-knee-v4b.mp4",
    "terrain-emily-unarmed-knee-v4.mp4"
  ],
  "complete": false,
  "visual_accepted": false,
  "evidence": [
    "terrain-corvo-knee-v4/result.json",
    "terrain-corvo-unarmed-knee-v4/result.json",
    "terrain-emily-knee-v4b/result.json",
    "terrain-emily-unarmed-knee-v4/result.json",
    "../movement-integration/visual-sweep-v1260/locomotion_stance-review.json"
  ]
}
````

- [evidence: terrain-corvo-knee-v4/result.json](<../../../../work/natural-moveset/terrain-corvo-knee-v4/result.json>)
- [evidence: terrain-corvo-unarmed-knee-v4/result.json](<../../../../work/natural-moveset/terrain-corvo-unarmed-knee-v4/result.json>)
- [evidence: terrain-emily-knee-v4b/result.json](<../../../../work/natural-moveset/terrain-emily-knee-v4b/result.json>)
- [evidence: terrain-emily-unarmed-knee-v4/result.json](<../../../../work/natural-moveset/terrain-emily-unarmed-knee-v4/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/locomotion_stance-review.json](<../../../../work/movement-integration/visual-sweep-v1260/locomotion_stance-review.json>)
- [in_game_recordings: native-feet-stairs-corvo-v5b.mp4](<../../../../work/natural-moveset/native-feet-stairs-corvo-v5b.mp4>)
- [in_game_recordings: native-feet-stairs-emily-v5.mp4](<../../../../work/natural-moveset/native-feet-stairs-emily-v5.mp4>)
- [in_game_recordings: terrain-corvo-v17-v2.mp4](<../../../../work/natural-moveset/terrain-corvo-v17-v2.mp4>)
- [in_game_recordings: terrain-corvo-unarmed-v17-v2.mp4](<../../../../work/natural-moveset/terrain-corvo-unarmed-v17-v2.mp4>)
- [in_game_recordings: terrain-emily-v17-v2.mp4](<../../../../work/natural-moveset/terrain-emily-v17-v2.mp4>)
- [in_game_recordings: terrain-emily-unarmed-v17-v2.mp4](<../../../../work/natural-moveset/terrain-emily-unarmed-v17-v2.mp4>)
- [in_game_recordings: terrain-corvo-knee-v4.mp4](<../../../../work/natural-moveset/terrain-corvo-knee-v4.mp4>)
- [in_game_recordings: terrain-corvo-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/terrain-corvo-unarmed-knee-v4.mp4>)
- [in_game_recordings: terrain-emily-knee-v4b.mp4](<../../../../work/natural-moveset/terrain-emily-knee-v4b.mp4>)
- [in_game_recordings: terrain-emily-unarmed-knee-v4.mp4](<../../../../work/natural-moveset/terrain-emily-unarmed-knee-v4.mp4>)

