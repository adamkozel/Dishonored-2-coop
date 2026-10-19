# traversal: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-19T04:13:57.642960+02:00. All fields below retain the catalog wording.

## standing_jump_takeoff

Visual sweep 1260: accepted (corvo accept, emily accept). Both characters take off from a standing idle into a readable tucked jump and recover cleanly; the only issues are polish (no anticipation dip, Emily's stiff coat). Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.

````json
{
  "id": "standing_jump_takeoff",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Both characters take off from a standing idle into a readable tucked jump and recover cleanly; the only issues are polish (no anticipation dip, Emily's stiff coat). Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.",
  "evidence": [
    "priority-2/result.json",
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1321h/result.json"
  ],
  "title": "standing jump takeoff",
  "motions": [
    "mixamo_jumping_up",
    "standing_jump_sequence"
  ],
  "deliverable_kind": "animation",
  "runtime_priority": "Existing native action suppresses the walking/running bank; this does not validate new donor gameplay binding.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-corvo-v5.mp4",
    "integrated-air-emily-v5.mp4",
    "integrated-unarmed-air-corvo-v3.mp4",
    "integrated-unarmed-air-emily-v3.mp4",
    "native-feet-air-corvo-v5.mp4",
    "native-feet-air-emily-v5.mp4"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Standing jumps armed and holstered in both roles: each presents an air episode (air weight > .5) that lands and settles; the armed mix follows the sword.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-jumps",
          "client-jumps"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/host-jumps-observer.mp4",
        "verdict": "Corvo takes off from a planted idle, tucks, lands with a compression and recovers, armed and holstered."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: integrated-air-corvo-v5.mp4](<../../../../work/natural-moveset/integrated-air-corvo-v5.mp4>)
- [in_game_recordings: integrated-air-emily-v5.mp4](<../../../../work/natural-moveset/integrated-air-emily-v5.mp4>)
- [in_game_recordings: integrated-unarmed-air-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-corvo-v3.mp4>)
- [in_game_recordings: integrated-unarmed-air-emily-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-emily-v3.mp4>)
- [in_game_recordings: native-feet-air-corvo-v5.mp4](<../../../../work/natural-moveset/native-feet-air-corvo-v5.mp4>)
- [in_game_recordings: native-feet-air-emily-v5.mp4](<../../../../work/natural-moveset/native-feet-air-emily-v5.mp4>)
## running_jump_takeoff

Visual sweep 1260: accepted (corvo accept, emily accept). Running jumps read naturally for both characters, armed and unarmed, with a proper stride into the jump, tuck and landing recovery. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.

````json
{
  "id": "running_jump_takeoff",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "running jump takeoff",
  "motions": [
    "running_jump_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Running jumps read naturally for both characters, armed and unarmed, with a proper stride into the jump, tuck and landing recovery. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-corvo-v5.mp4",
    "integrated-air-emily-v5.mp4",
    "integrated-unarmed-air-corvo-v3.mp4",
    "integrated-unarmed-air-emily-v3.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1321h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Running and sprint (maximum-speed) jumps in both roles, presented from the run and sprint gaits with air episodes and moving landings.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-runjump",
          "client-runjump"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/client-runjump-observer.mp4",
        "verdict": "Emily runs into the jump, carries through the air toward the camera and keeps running."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: integrated-air-corvo-v5.mp4](<../../../../work/natural-moveset/integrated-air-corvo-v5.mp4>)
- [in_game_recordings: integrated-air-emily-v5.mp4](<../../../../work/natural-moveset/integrated-air-emily-v5.mp4>)
- [in_game_recordings: integrated-unarmed-air-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-corvo-v3.mp4>)
- [in_game_recordings: integrated-unarmed-air-emily-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-emily-v3.mp4>)
## crouch_jump_transition

Visual sweep 1260: accepted (corvo accept, emily accept). Crouch-to-jump-to-crouch transitions are readable for both characters; there is a brief upright moment on the landing before re-crouching (polish). Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.

````json
{
  "id": "crouch_jump_transition",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "crouch jump transition",
  "motions": [
    "crouch_jump_to_stand",
    "crouch_jump_to_crouch"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Crouch-to-jump-to-crouch transitions are readable for both characters; there is a brief upright moment on the landing before re-crouching (polish). Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-corvo-v5.mp4",
    "integrated-air-emily-v5.mp4",
    "integrated-unarmed-air-corvo-v3.mp4",
    "integrated-unarmed-air-emily-v3.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1321h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Crouched jumps in both roles: the take-off starts crouched, an air episode lands and settles, and the native landing returns to the crouch.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-jumps",
          "client-jumps"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/host-jumps-observer.mp4",
        "verdict": "Corvo jumps from the crouch, tucks and lands back into the crouch."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: integrated-air-corvo-v5.mp4](<../../../../work/natural-moveset/integrated-air-corvo-v5.mp4>)
- [in_game_recordings: integrated-air-emily-v5.mp4](<../../../../work/natural-moveset/integrated-air-emily-v5.mp4>)
- [in_game_recordings: integrated-unarmed-air-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-corvo-v3.mp4>)
- [in_game_recordings: integrated-unarmed-air-emily-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-emily-v3.mp4>)
## double_jump

Visual sweep 1260: accepted (corvo accept, emily accept). The second impulse reads as a distinct high tuck followed by a long fall and landing for both characters, armed and unarmed, stationary and moving. Confirmed native Agility second impulse (remaining 1 to 0, apex +2.5 m) drives a distinct whole-body tuck/extension from the same delayed Steam root. Two actual D2 recordings and22checks cover stationary/moving, drawn/holstered, single impulses, landing recovery, native equipment/first person and restoration of the temporary upgrade. All8contact sheets inspected. Crouched launch, midair actions, interruptions and final visual acceptance remain unfinished.

````json
{
  "id": "double_jump",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_double_jump_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "double jump",
  "motions": [
    "double_jump"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The second impulse reads as a distinct high tuck followed by a long fall and landing for both characters, armed and unarmed, stationary and moving. Confirmed native Agility second impulse (remaining 1 to 0, apex +2.5 m) drives a distinct whole-body tuck/extension from the same delayed Steam root. Two actual D2 recordings and22checks cover stationary/moving, drawn/holstered, single impulses, landing recovery, native equipment/first person and restoration of the temporary upgrade. All8contact sheets inspected. Crouched launch, midair actions, interruptions and final visual acceptance remain unfinished.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "double-jump-corvo-v21-v1.mp4",
    "double-jump-emily-v21-v1.mp4"
  ],
  "evidence": [
    "double-jump-corvo-v21-v1/result.json",
    "double-jump-emily-v21-v1/result.json",
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1333b/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1336-double-jump.json",
    "decision": "complete",
    "scope": "Native Agility double jumps standing armed, running on the open course and holstered in both roles: each second impulse is presented in the air (the partner second-jump serial advances once per jump, 3 of 3 in each role) inside an air episode that lands.",
    "takes": [
      {
        "take": "coverage-v1333b",
        "cases": [
          "host-doublejump",
          "client-doublejump"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1333b/host-doublejump-observer.mp4",
        "verdict": "Corvo springs from a crouched take-off, tucks, kicks into a second tuck near the top and comes down arms spread into a compressed crouched landing, then recovers."
      },
      {
        "video": "coverage-v1333b/client-doublejump-observer.mp4",
        "verdict": "Emily takes off, tucks with her coat flaring, tucks again for the second impulse and lands in a crouch before standing."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: double-jump-corvo-v21-v1/result.json](<../../../../work/natural-moveset/double-jump-corvo-v21-v1/result.json>)
- [evidence: double-jump-emily-v21-v1/result.json](<../../../../work/natural-moveset/double-jump-emily-v21-v1/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1333b/result.json](<../../../../work/natural-moveset/coverage-v1333b/result.json>)
- [in_game_recordings: double-jump-corvo-v21-v1.mp4](<../../../../work/natural-moveset/double-jump-corvo-v21-v1.mp4>)
- [in_game_recordings: double-jump-emily-v21-v1.mp4](<../../../../work/natural-moveset/double-jump-emily-v21-v1.mp4>)
## rising_airborne

Visual sweep 1260: accepted (corvo accept, emily accept). The ascent pose (liftoff extension into tuck) is coherent and tracks height for both characters. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.

````json
{
  "id": "rising_airborne",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The ascent pose (liftoff extension into tuck) is coherent and tracks height for both characters. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.",
  "evidence": [
    "priority-2/result.json",
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1321h/result.json"
  ],
  "title": "rising airborne",
  "motions": [
    "mixamo_jumping_up",
    "standing_jump_sequence"
  ],
  "deliverable_kind": "animation",
  "runtime_priority": "Existing native action suppresses the walking/running bank; this does not validate new donor gameplay binding.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-corvo-v5.mp4",
    "integrated-air-emily-v5.mp4",
    "integrated-unarmed-air-corvo-v3.mp4",
    "integrated-unarmed-air-emily-v3.mp4"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "The rising part of standing, crouched, holstered, running and sprint jumps and the start of the 6 m fall, presented as air episodes tracking native height in both roles.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-jumps",
          "host-runjump",
          "host-fall",
          "client-jumps",
          "client-runjump",
          "client-fall"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/host-jumps-observer.mp4",
        "verdict": "The ascent pose extends, then tucks with height."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: integrated-air-corvo-v5.mp4](<../../../../work/natural-moveset/integrated-air-corvo-v5.mp4>)
- [in_game_recordings: integrated-air-emily-v5.mp4](<../../../../work/natural-moveset/integrated-air-emily-v5.mp4>)
- [in_game_recordings: integrated-unarmed-air-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-corvo-v3.mp4>)
- [in_game_recordings: integrated-unarmed-air-emily-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-emily-v3.mp4>)
## fall_loop

Audit 1283: a 5-6 m fall in both roles shows the falling pose with arms out (coverage-v1283a), accepted functionally. Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). The short descent poses in ordinary jumps look correct, but no recording contains a long or hard fall, so the fall loop itself cannot be judged. The double-jump takes show a somewhat longer fall (~0.4 s) that also looks fine. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.

````json
{
  "id": "fall_loop",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_functional_both_roles_long_fall",
  "native_reference_family_count": 155,
  "priority": 2,
  "notes": "Audit 1283: a 5-6 m fall in both roles shows the falling pose with arms out (coverage-v1283a), accepted functionally. Visual sweep 1260: insufficient (corvo insufficient, emily insufficient). The short descent poses in ordinary jumps look correct, but no recording contains a long or hard fall, so the fall loop itself cannot be judged. The double-jump takes show a somewhat longer fall (~0.4 s) that also looks fine. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.",
  "evidence": [
    "priority-2/result.json",
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "../movement-integration/coverage-audit-v1283.json",
    "coverage-v1321h/result.json"
  ],
  "title": "fall loop",
  "motions": [
    "sword_fall_loop",
    "mixamo_falling_idle"
  ],
  "deliverable_kind": "animation",
  "runtime_priority": "Existing native action suppresses the walking/running bank; this does not validate new donor gameplay binding.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-corvo-v5.mp4",
    "integrated-air-emily-v5.mp4",
    "integrated-unarmed-air-corvo-v3.mp4",
    "integrated-unarmed-air-emily-v3.mp4"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "A 6 m fall in both roles: the partner presents the fall as an air episode of at least 3 samples that lands and settles.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-fall",
          "client-fall"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/host-fall-observer.mp4",
        "verdict": "Corvo drops from height in the falling pose and lands."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: ../movement-integration/coverage-audit-v1283.json](<../../../../work/movement-integration/coverage-audit-v1283.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: integrated-air-corvo-v5.mp4](<../../../../work/natural-moveset/integrated-air-corvo-v5.mp4>)
- [in_game_recordings: integrated-air-emily-v5.mp4](<../../../../work/natural-moveset/integrated-air-emily-v5.mp4>)
- [in_game_recordings: integrated-unarmed-air-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-corvo-v3.mp4>)
- [in_game_recordings: integrated-unarmed-air-emily-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-emily-v3.mp4>)
## soft_landing

Visual sweep 1260: accepted (corvo accept, emily accept). Landings compress and recover correctly for both characters, including after airborne combat. The only issue is Emily's stiff coat (polish). Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.

````json
{
  "id": "soft_landing",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "soft landing",
  "motions": [
    "sword_land_walk_out"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Landings compress and recover correctly for both characters, including after airborne combat. The only issue is Emily's stiff coat (polish). Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-corvo-v5.mp4",
    "integrated-air-emily-v5.mp4",
    "integrated-unarmed-air-corvo-v3.mp4",
    "integrated-unarmed-air-emily-v3.mp4",
    "integrated-air-combat-corvo-v2.mp4",
    "integrated-air-combat-emily-v2.mp4",
    "native-feet-air-corvo-v5.mp4",
    "native-feet-air-emily-v5.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1321h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Landings after standing, crouched and holstered jumps settle (speed and air weight back near zero) in both roles.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-jumps",
          "client-jumps"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/host-jumps-observer.mp4",
        "verdict": "Each landing compresses and recovers to a planted idle."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: integrated-air-corvo-v5.mp4](<../../../../work/natural-moveset/integrated-air-corvo-v5.mp4>)
- [in_game_recordings: integrated-air-emily-v5.mp4](<../../../../work/natural-moveset/integrated-air-emily-v5.mp4>)
- [in_game_recordings: integrated-unarmed-air-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-corvo-v3.mp4>)
- [in_game_recordings: integrated-unarmed-air-emily-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-emily-v3.mp4>)
- [in_game_recordings: integrated-air-combat-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-air-combat-corvo-v2.mp4>)
- [in_game_recordings: integrated-air-combat-emily-v2.mp4](<../../../../work/natural-moveset/integrated-air-combat-emily-v2.mp4>)
- [in_game_recordings: native-feet-air-corvo-v5.mp4](<../../../../work/natural-moveset/native-feet-air-corvo-v5.mp4>)
- [in_game_recordings: native-feet-air-emily-v5.mp4](<../../../../work/natural-moveset/native-feet-air-emily-v5.mp4>)
## hard_landing

Audit 1283: the landing after a 5-6 m fall shows a deep crouch and recovery in both roles (coverage-v1283a), accepted functionally. Native CatFall light body scene drives complete-body landing on both characters through Steam. Standing and direct crouched recovery, drawn/holstered, are recorded. Native input latch chooses crouch before exit, removing the rejected V22 stand-up dip. Strong/damaging roll, travelling recovery, arbitrary geometry and interruptions remain to complete.

````json
{
  "id": "hard_landing",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_light_landing_standing_and_crouched_verified_dunwall",
  "visual_validation": "accepted_functional_both_roles_long_fall_landing",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "hard landing",
  "motions": [
    "sword_landfail_walk_out",
    "mixamo_hard_landing",
    "armed_native_hard_landing",
    "empty_native_hard_landing",
    "armed_native_hard_landing_crouched",
    "empty_native_hard_landing_crouched"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1283: the landing after a 5-6 m fall shows a deep crouch and recovery in both roles (coverage-v1283a), accepted functionally. Native CatFall light body scene drives complete-body landing on both characters through Steam. Standing and direct crouched recovery, drawn/holstered, are recorded. Native input latch chooses crouch before exit, removing the rejected V22 stand-up dip. Strong/damaging roll, travelling recovery, arbitrary geometry and interruptions remain to complete.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "hard-landing-corvo-stand-v23-v5.mp4",
    "hard-landing-corvo-crouch-v23-v1.mp4",
    "hard-landing-emily-stand-v23-v5.mp4",
    "hard-landing-emily-crouch-v23-v1.mp4"
  ],
  "evidence": [
    "hard-landing-corvo-stand-v23-v5/result.json",
    "hard-landing-corvo-crouch-v23-v1/result.json",
    "hard-landing-emily-stand-v23-v5/result.json",
    "hard-landing-emily-crouch-v23-v1/result.json",
    "../movement-integration/coverage-audit-v1283.json",
    "coverage-v1321h/result.json",
    "mission-coverage-v1328q/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1344-traversal-2.json",
    "decision": "complete",
    "scope": "A damaging ~12 m fall onto the dock in both roles (health held): the source enters the native CatFall with the strong landing scene (cat_fall_strong, 1066 ms, bound to the body and both arms), the partner presents it as the hard landing (traversal variant 6, 15 samples) and walks on. The light landing after 5-6 m falls was accepted earlier (audit 1283).",
    "takes": [
      {
        "take": "mission-coverage-v1328q",
        "cases": [
          "host-hardfall",
          "client-hardfall"
        ]
      }
    ],
    "visual": [
      {
        "video": "mission-coverage-v1328q/host-hardfall-observer.mp4",
        "verdict": "Corvo drops onto the dock and lands in a deep crouch with one hand on the planks, holds it a moment, rises and walks on."
      },
      {
        "video": "mission-coverage-v1328q/client-hardfall-observer.mp4",
        "verdict": "Emily lands from the fall in the same braced crouch, pushes up and walks away along the dock."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: hard-landing-corvo-stand-v23-v5/result.json](<../../../../work/natural-moveset/hard-landing-corvo-stand-v23-v5/result.json>)
- [evidence: hard-landing-corvo-crouch-v23-v1/result.json](<../../../../work/natural-moveset/hard-landing-corvo-crouch-v23-v1/result.json>)
- [evidence: hard-landing-emily-stand-v23-v5/result.json](<../../../../work/natural-moveset/hard-landing-emily-stand-v23-v5/result.json>)
- [evidence: hard-landing-emily-crouch-v23-v1/result.json](<../../../../work/natural-moveset/hard-landing-emily-crouch-v23-v1/result.json>)
- [evidence: ../movement-integration/coverage-audit-v1283.json](<../../../../work/movement-integration/coverage-audit-v1283.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [evidence: mission-coverage-v1328q/result.json](<../../../../work/natural-moveset/mission-coverage-v1328q/result.json>)
- [in_game_recordings: hard-landing-corvo-stand-v23-v5.mp4](<../../../../work/natural-moveset/hard-landing-corvo-stand-v23-v5.mp4>)
- [in_game_recordings: hard-landing-corvo-crouch-v23-v1.mp4](<../../../../work/natural-moveset/hard-landing-corvo-crouch-v23-v1.mp4>)
- [in_game_recordings: hard-landing-emily-stand-v23-v5.mp4](<../../../../work/natural-moveset/hard-landing-emily-stand-v23-v5.mp4>)
- [in_game_recordings: hard-landing-emily-crouch-v23-v1.mp4](<../../../../work/natural-moveset/hard-landing-emily-crouch-v23-v1.mp4>)
## moving_landing

Visual sweep 1260: accepted (corvo accept, emily accept). Landings from running jumps compress and flow into a recovery stride for both characters. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.

````json
{
  "id": "moving_landing",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "moving landing",
  "motions": [
    "sword_land_walk_out",
    "mixamo_falling_to_roll"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). Landings from running jumps compress and flow into a recovery stride for both characters. Exact native Jumping/Falling states, delayed motion semantics and native start/apex heights drive the authored full-body bank. Both roles pass rise/fall/landing and slide interruption checks. Actual D2 ordinary-jump recordings show the complete body. Maximum-speed takeoff, double jump, hard falls, terrain contact, airborne combat and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-air-corvo-v5.mp4",
    "integrated-air-emily-v5.mp4",
    "integrated-unarmed-air-corvo-v3.mp4",
    "integrated-unarmed-air-emily-v3.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1321h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Landings after running and sprint jumps keep moving (speed > 1 within 0.8 s) in both roles.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-runjump",
          "client-runjump"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/client-runjump-observer.mp4",
        "verdict": "Emily lands from the run jump into a continuing stride."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: integrated-air-corvo-v5.mp4](<../../../../work/natural-moveset/integrated-air-corvo-v5.mp4>)
- [in_game_recordings: integrated-air-emily-v5.mp4](<../../../../work/natural-moveset/integrated-air-emily-v5.mp4>)
- [in_game_recordings: integrated-unarmed-air-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-corvo-v3.mp4>)
- [in_game_recordings: integrated-unarmed-air-emily-v3.mp4](<../../../../work/natural-moveset/integrated-unarmed-air-emily-v3.mp4>)
## vault_low_obstacle

Visual sweep 1260: accepted (corvo accept, emily accept). The desk vault is recognizable for both characters, armed and unarmed (approach, plant, swing over, land). The framing is distant and hazy and the landing is partly hidden by the desk. Native state9, exact player/owned-arm scene, obstacle geometry and distance phase drive both characters armed and unarmed over Steam. Eight repeated cases pass recovery, equipment, health and monotonic progress; four actual D2 recordings exist. Arbitrary support contacts, window/high variants and final visual acceptance remain incomplete.

````json
{
  "id": "vault_low_obstacle",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_isolated_Dunwall_native_vault",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "vault low obstacle",
  "motions": [
    "sword_mantle_vault_walk_out",
    "sword_vaultthrough_walk_out",
    "sword_vaultthrough_fall",
    "sword_vaultthrough_walkright_in",
    "sword_vaultraise_walk_out",
    "sword_vaultraise_walkleft_in",
    "sword_vaultraise_walkright_in",
    "sword_vaultthrough_fail_walk_out",
    "sword_vaultthrough_walkleft_in"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The desk vault is recognizable for both characters, armed and unarmed (approach, plant, swing over, land). The framing is distant and hazy and the landing is partly hidden by the desk. Native state9, exact player/owned-arm scene, obstacle geometry and distance phase drive both characters armed and unarmed over Steam. Eight repeated cases pass recovery, equipment, health and monotonic progress; four actual D2 recordings exist. Arbitrary support contacts, window/high variants and final visual acceptance remain incomplete.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-vault-corvo-v3.mp4",
    "integrated-vault-corvo-unarmed-v3.mp4",
    "integrated-vault-emily-v3.mp4",
    "integrated-vault-emily-unarmed-v3.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1326d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Native vaults (state 9) over the safe-room obstacle, armed and holstered, in both roles: the partner presents the vault (traversal variant 3) and recovers; the vault-exit clock reset is fixed (1214ci).",
    "takes": [
      {
        "take": "coverage-v1326d",
        "cases": [
          "host-vault",
          "client-vault"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1326d/host-vault-observer.mp4",
        "verdict": "Seen side-on, Corvo runs in, goes over the obstacle airborne with legs tucked and carries on."
      },
      {
        "video": "coverage-v1326d/client-vault-observer.mp4",
        "verdict": "Seen side-on, Emily vaults the obstacle the same way."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1326d/result.json](<../../../../work/natural-moveset/coverage-v1326d/result.json>)
- [in_game_recordings: integrated-vault-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-vault-corvo-v3.mp4>)
- [in_game_recordings: integrated-vault-corvo-unarmed-v3.mp4](<../../../../work/natural-moveset/integrated-vault-corvo-unarmed-v3.mp4>)
- [in_game_recordings: integrated-vault-emily-v3.mp4](<../../../../work/natural-moveset/integrated-vault-emily-v3.mp4>)
- [in_game_recordings: integrated-vault-emily-unarmed-v3.mp4](<../../../../work/natural-moveset/integrated-vault-emily-unarmed-v3.mp4>)
## climb_high_ledge

Visual sweep 1260: accepted (corvo accept, emily accept). The high canopy mantle reads as grab, hang, pull-up and stand or crouch for both characters, armed and unarmed. The scene is dark but the motion is clear after brightening. Native kind-2, 833 ms mantle scenes drive both characters over Steam with standing or crouched recovery, drawn or holstered swords. Four actual D2 recordings and 36 checks cover exact player/left-arm ownership, native timing, single events, recovery, equipment and preserved health/saves. The full-body donor keeps native bone lengths and root authority. Support contact is calibrated to the Dunwall canopy; other ledges, interruptions and close grip/contact visual acceptance remain unfinished.

````json
{
  "id": "climb_high_ledge",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_isolated_Dunwall_native_high_mantle",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "climb high ledge",
  "motions": [
    "sword_mantlehigh_walkleft_in",
    "sword_mantlehigh_walkright_in",
    "sword_mantlemiddle1_walkleft_in",
    "sword_mantlemiddle1_walkright_in",
    "sword_mantlemiddle2_walkleft_in",
    "sword_mantlemiddle2_walkright_in",
    "sword_mantlemiddle3_walkleft_in",
    "sword_mantlemiddle3_walkright_in",
    "mixamo_freehang_climb"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The high canopy mantle reads as grab, hang, pull-up and stand or crouch for both characters, armed and unarmed. The scene is dark but the motion is clear after brightening. Native kind-2, 833 ms mantle scenes drive both characters over Steam with standing or crouched recovery, drawn or holstered swords. Four actual D2 recordings and 36 checks cover exact player/left-arm ownership, native timing, single events, recovery, equipment and preserved health/saves. The full-body donor keeps native bone lengths and root authority. Support contact is calibrated to the Dunwall canopy; other ledges, interruptions and close grip/contact visual acceptance remain unfinished.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "high-mantle-corvo-v20-lit-v1.mp4",
    "high-mantle-corvo-crouched-v20-lit-v1.mp4",
    "high-mantle-emily-v20-lit-v1.mp4",
    "high-mantle-emily-crouched-v20-lit-v1.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1326d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Native high mantles (state 8, kind 2) standing and crouched in both roles: the partner presents variants 4 and 5 matching the native exit stance and recovers.",
    "takes": [
      {
        "take": "coverage-v1326d",
        "cases": [
          "host-highmantle",
          "client-highmantle"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1326c/client-highmantle-observer.mp4",
        "verdict": "Emily jumps, reaches up and pulls onto the canopy, then stands on top."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1326d/result.json](<../../../../work/natural-moveset/coverage-v1326d/result.json>)
- [in_game_recordings: high-mantle-corvo-v20-lit-v1.mp4](<../../../../work/natural-moveset/high-mantle-corvo-v20-lit-v1.mp4>)
- [in_game_recordings: high-mantle-corvo-crouched-v20-lit-v1.mp4](<../../../../work/natural-moveset/high-mantle-corvo-crouched-v20-lit-v1.mp4>)
- [in_game_recordings: high-mantle-emily-v20-lit-v1.mp4](<../../../../work/natural-moveset/high-mantle-emily-v20-lit-v1.mp4>)
- [in_game_recordings: high-mantle-emily-crouched-v20-lit-v1.mp4](<../../../../work/natural-moveset/high-mantle-emily-crouched-v20-lit-v1.mp4>)
## ledge_hang_and_pull_up

Visual sweep 1260: accepted (corvo accept, emily accept). The low desk mantle (standing and crouched exits) shows a readable grab and pull-up for both characters. A sustained hang is never exercised, and the backlit framing limits detail. Native low mantle with standing or crouched exit is bound on both roles, armed and unarmed. Exact owned scenes, native timing and short sword recovery pass runtime checks; actual D2 videos show both exits. Sustained hanging, higher ledges, vaults, arbitrary geometry contacts and visual acceptance remain incomplete.

````json
{
  "id": "ledge_hang_and_pull_up",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_isolated_Dunwall_low_mantle",
  "visual_validation": "accepted_sweep_1260_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "ledge hang and pull up",
  "motions": [
    "mixamo_freehang_climb"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept, emily accept). The low desk mantle (standing and crouched exits) shows a readable grab and pull-up for both characters. A sustained hang is never exercised, and the backlit framing limits detail. Native low mantle with standing or crouched exit is bound on both roles, armed and unarmed. Exact owned scenes, native timing and short sword recovery pass runtime checks; actual D2 videos show both exits. Sustained hanging, higher ledges, vaults, arbitrary geometry contacts and visual acceptance remain incomplete.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "integrated-low-mantle-corvo-v2.mp4",
    "integrated-low-mantle-emily-v2.mp4",
    "integrated-crouch-mantle-corvo-v3.mp4",
    "integrated-crouch-mantle-emily-v3.mp4"
  ],
  "evidence": [
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "coverage-v1326d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "The native game has no sustained ledge hang: its only climbing state is arkPlayerStateClimb on arkClimbable, and ledge grabs are the native mantle (confirmed static, d2-ghidra string search). The low and high mantles (grab, pull-up, standing or crouched exit) are presented in both roles.",
    "takes": [
      {
        "take": "coverage-v1326d",
        "cases": [
          "host-lowmantle",
          "host-highmantle",
          "client-lowmantle",
          "client-highmantle"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1326c/client-highmantle-observer.mp4",
        "verdict": "The grab and pull-up read as a reach and climb onto the ledge."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: coverage-v1326d/result.json](<../../../../work/natural-moveset/coverage-v1326d/result.json>)
- [in_game_recordings: integrated-low-mantle-corvo-v2.mp4](<../../../../work/natural-moveset/integrated-low-mantle-corvo-v2.mp4>)
- [in_game_recordings: integrated-low-mantle-emily-v2.mp4](<../../../../work/natural-moveset/integrated-low-mantle-emily-v2.mp4>)
- [in_game_recordings: integrated-crouch-mantle-corvo-v3.mp4](<../../../../work/natural-moveset/integrated-crouch-mantle-corvo-v3.mp4>)
- [in_game_recordings: integrated-crouch-mantle-emily-v3.mp4](<../../../../work/natural-moveset/integrated-crouch-mantle-emily-v3.mp4>)
## window_enter_exit

Candidates require animation review and appropriate game-state/contact binding. 1201/1206 (audit 1207): native probes on both catalogued window usables found no climbable window (Dunwall Tower: chest-height opening over a drop; EotW docks: blind toggle). Static census: no window-specific traversal state; window entry/exit is the generic native mantle (state 8 confined/low, partially bound in 651/656). Remaining: record that binding through a genuinely climbable window. Audit 1238 (#7, 1213l): native mantle through the climbable EotW window script_rich_small_01_sndctrl presented on the partner (traversal variant 2) in both roles; visual pending (#33). Audit 1241 (1213m): the same mantle passes again in both roles with the partner held through source gaps (native-pose time 29% -> 1% on the host-source side).

````json
{
  "id": "window_enter_exit",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_confined_low_mantle_through_climbable_eotw_window_both_roles",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "window enter exit",
  "motions": [
    "sword_vaultthrough_walk_out",
    "sword_vaultthrough_fall",
    "sword_vaultthrough_walkright_in",
    "sword_vaultthrough_fail_walk_out",
    "sword_vaultthrough_walkleft_in"
  ],
  "deliverable_kind": "animation",
  "notes": "Candidates require animation review and appropriate game-state/contact binding. 1201/1206 (audit 1207): native probes on both catalogued window usables found no climbable window (Dunwall Tower: chest-height opening over a drop; EotW docks: blind toggle). Static census: no window-specific traversal state; window entry/exit is the generic native mantle (state 8 confined/low, partially bound in 651/656). Remaining: record that binding through a genuinely climbable window. Audit 1238 (#7, 1213l): native mantle through the climbable EotW window script_rich_small_01_sndctrl presented on the partner (traversal variant 2) in both roles; visual pending (#33). Audit 1241 (1213m): the same mantle passes again in both roles with the partner held through source gaps (native-pose time 29% -> 1% on the host-source side).",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [],
  "evidence": [
    "../movement-integration/window-traversal-audit-v1207.json",
    "../movement-integration/window-traversal-audit-v1238.json",
    "../movement-integration/presentation-hold-audit-v1241.json",
    "window-traversal-v1235i/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1344-traversal-2.json",
    "decision": "complete",
    "scope": "Crossing the climbable Edge of the World window in both roles: the native mantle through the window (crossed from one side to the other) is presented on the partner (traversal weight 1, presentation weight 1 throughout).",
    "takes": [
      {
        "take": "window-traversal-v1235i",
        "cases": [
          "host-script_rich_small_01_sndctrl",
          "client-script_rich_small_01_sndctrl"
        ]
      }
    ],
    "visual": [
      {
        "video": "window-traversal-v1235i/cross-client-observed.mp4",
        "verdict": "Emily steps up to the window, plants her hands on the sill, swings through the opening and drops out of sight on the far side."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/window-traversal-audit-v1207.json](<../../../../work/movement-integration/window-traversal-audit-v1207.json>)
- [evidence: ../movement-integration/window-traversal-audit-v1238.json](<../../../../work/movement-integration/window-traversal-audit-v1238.json>)
- [evidence: ../movement-integration/presentation-hold-audit-v1241.json](<../../../../work/movement-integration/presentation-hold-audit-v1241.json>)
- [evidence: window-traversal-v1235i/result.json](<../../../../work/natural-moveset/window-traversal-v1235i/result.json>)
## slide_enter_loop_exit

Audit 1267: sprint-to-slide with crouched exit accepted for both characters (1267a); Corvo stand/jump exits accepted in sweep 1260. Visual sweep 1260: insufficient (corvo accept, emily insufficient). Corvo's slide entry, hold and both exits (recovery and jump interrupt) read correctly. No Emily slide recording is listed, so Emily cannot be judged. The folder does contain older integrated-slide-emily-v6 and integrated-slide-jump-emily-v3 recordings, which were not reviewed. Native Sliding selects authored full-body entry, hold and recovery; native jump interrupts into the authored air bank. Both roles pass state/priority checks. Collision-dependent exits, slopes, low ceilings, contact and final visual acceptance remain pending.

````json
{
  "id": "slide_enter_loop_exit",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "slide enter loop exit",
  "motions": [
    "slide_running_sequence",
    "slide_loop",
    "slide_enter",
    "slide_exit_to_run",
    "slide_to_crouch"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1267: sprint-to-slide with crouched exit accepted for both characters (1267a); Corvo stand/jump exits accepted in sweep 1260. Visual sweep 1260: insufficient (corvo accept, emily insufficient). Corvo's slide entry, hold and both exits (recovery and jump interrupt) read correctly. No Emily slide recording is listed, so Emily cannot be judged. The folder does contain older integrated-slide-emily-v6 and integrated-slide-jump-emily-v3 recordings, which were not reviewed. Native Sliding selects authored full-body entry, hold and recovery; native jump interrupts into the authored air bank. Both roles pass state/priority checks. Collision-dependent exits, slopes, low ceilings, contact and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "slide-corvo-crouch-v28-v8.mp4",
    "slide-corvo-crouch-v28-v8-source.mp4",
    "slide-corvo-stand-v28-v8.mp4",
    "slide-corvo-stand-v28-v8-source.mp4",
    "slide-corvo-jump-v28-v8.mp4",
    "slide-corvo-jump-v28-v8-source.mp4",
    "slide-corvo-empty-crouch-v28-v8.mp4",
    "slide-corvo-empty-crouch-v28-v8-source.mp4",
    "slide-corvo-empty-stand-v28-v8.mp4",
    "slide-corvo-empty-stand-v28-v8-source.mp4",
    "slide-corvo-empty-jump-v28-v9.mp4",
    "slide-corvo-empty-jump-v28-v9-source.mp4",
    "slide-emily-crouch-v28-v9.mp4",
    "slide-emily-crouch-v28-v9-source.mp4",
    "slide-emily-stand-v28-v9.mp4",
    "slide-emily-stand-v28-v9-source.mp4",
    "slide-emily-jump-v28-v9.mp4",
    "slide-emily-jump-v28-v9-source.mp4",
    "slide-emily-empty-crouch-v28-v9.mp4",
    "slide-emily-empty-crouch-v28-v9-source.mp4",
    "slide-emily-empty-stand-v28-v9.mp4",
    "slide-emily-empty-stand-v28-v9-source.mp4",
    "slide-emily-empty-jump-v28-v9.mp4",
    "slide-emily-empty-jump-v28-v9-source.mp4",
    "coverage-v1267a/client-slide-observer.mp4",
    "coverage-v1267a/host-slide-observer.mp4"
  ],
  "evidence": [
    "slide-corvo-crouch-v28-v8/result.json",
    "slide-corvo-stand-v28-v8/result.json",
    "slide-corvo-jump-v28-v8/result.json",
    "slide-corvo-empty-crouch-v28-v8/result.json",
    "slide-corvo-empty-stand-v28-v8/result.json",
    "slide-corvo-empty-jump-v28-v9/result.json",
    "slide-emily-crouch-v28-v9/result.json",
    "slide-emily-stand-v28-v9/result.json",
    "slide-emily-jump-v28-v9/result.json",
    "slide-emily-empty-crouch-v28-v9/result.json",
    "slide-emily-empty-stand-v28-v9/result.json",
    "slide-emily-empty-jump-v28-v9/result.json",
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "../movement-integration/sweep-defects-audit-v1267.json",
    "coverage-v1321h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Native slides from a sprint in both roles: entry, hold and recovery presented (slide weight > .8), with crouched, jump-out and stand exits.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-slide",
          "host-slideexit",
          "client-slide",
          "client-slideexit"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/client-slideexit-observer.mp4",
        "verdict": "Emily sprints, drops into the slide and jumps out of it; the second slide enters low."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: slide-corvo-crouch-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-crouch-v28-v8/result.json>)
- [evidence: slide-corvo-stand-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-stand-v28-v8/result.json>)
- [evidence: slide-corvo-jump-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-jump-v28-v8/result.json>)
- [evidence: slide-corvo-empty-crouch-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-empty-crouch-v28-v8/result.json>)
- [evidence: slide-corvo-empty-stand-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-empty-stand-v28-v8/result.json>)
- [evidence: slide-corvo-empty-jump-v28-v9/result.json](<../../../../work/natural-moveset/slide-corvo-empty-jump-v28-v9/result.json>)
- [evidence: slide-emily-crouch-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-crouch-v28-v9/result.json>)
- [evidence: slide-emily-stand-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-stand-v28-v9/result.json>)
- [evidence: slide-emily-jump-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-jump-v28-v9/result.json>)
- [evidence: slide-emily-empty-crouch-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-empty-crouch-v28-v9/result.json>)
- [evidence: slide-emily-empty-stand-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-empty-stand-v28-v9/result.json>)
- [evidence: slide-emily-empty-jump-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-empty-jump-v28-v9/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: ../movement-integration/sweep-defects-audit-v1267.json](<../../../../work/movement-integration/sweep-defects-audit-v1267.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: slide-corvo-crouch-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-crouch-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-crouch-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-crouch-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-stand-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-stand-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-stand-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-stand-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-jump-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-jump-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-jump-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-jump-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-empty-crouch-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-empty-crouch-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-empty-crouch-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-empty-crouch-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-empty-stand-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-empty-stand-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-empty-stand-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-empty-stand-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-empty-jump-v28-v9.mp4](<../../../../work/natural-moveset/slide-corvo-empty-jump-v28-v9.mp4>)
- [in_game_recordings: slide-corvo-empty-jump-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-corvo-empty-jump-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-crouch-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-crouch-v28-v9.mp4>)
- [in_game_recordings: slide-emily-crouch-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-crouch-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-stand-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-stand-v28-v9.mp4>)
- [in_game_recordings: slide-emily-stand-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-stand-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-jump-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-jump-v28-v9.mp4>)
- [in_game_recordings: slide-emily-jump-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-jump-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-empty-crouch-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-empty-crouch-v28-v9.mp4>)
- [in_game_recordings: slide-emily-empty-crouch-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-empty-crouch-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-empty-stand-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-empty-stand-v28-v9.mp4>)
- [in_game_recordings: slide-emily-empty-stand-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-empty-stand-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-empty-jump-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-empty-jump-v28-v9.mp4>)
- [in_game_recordings: slide-emily-empty-jump-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-empty-jump-v28-v9-source.mp4>)
- [in_game_recordings: coverage-v1267a/client-slide-observer.mp4](<../../../../work/natural-moveset/coverage-v1267a/client-slide-observer.mp4>)
- [in_game_recordings: coverage-v1267a/host-slide-observer.mp4](<../../../../work/natural-moveset/coverage-v1267a/host-slide-observer.mp4>)
## slide_to_crouch_or_stand

Audit 1267: crouched exits for both characters (1267a); Corvo stand and jump exits accepted in sweep 1260. Visual sweep 1260: insufficient (corvo accept, emily insufficient). Corvo's slide recovers correctly into both crouch and stand. No Emily recording is listed for this item, so Emily is insufficient. Native Sliding selects authored full-body entry, hold and recovery; native jump interrupts into the authored air bank. Both roles pass state/priority checks. Collision-dependent exits, slopes, low ceilings, contact and final visual acceptance remain pending.

````json
{
  "id": "slide_to_crouch_or_stand",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "isolated_Dunwall_both_roles",
  "visual_validation": "accepted_functional_both_roles",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "slide to crouch or stand",
  "motions": [
    "slide_exit_to_run",
    "slide_to_crouch"
  ],
  "deliverable_kind": "animation",
  "notes": "Audit 1267: crouched exits for both characters (1267a); Corvo stand and jump exits accepted in sweep 1260. Visual sweep 1260: insufficient (corvo accept, emily insufficient). Corvo's slide recovers correctly into both crouch and stand. No Emily recording is listed for this item, so Emily is insufficient. Native Sliding selects authored full-body entry, hold and recovery; native jump interrupts into the authored air bank. Both roles pass state/priority checks. Collision-dependent exits, slopes, low ceilings, contact and final visual acceptance remain pending.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "slide-corvo-crouch-v28-v8.mp4",
    "slide-corvo-crouch-v28-v8-source.mp4",
    "slide-corvo-stand-v28-v8.mp4",
    "slide-corvo-stand-v28-v8-source.mp4",
    "slide-corvo-jump-v28-v8.mp4",
    "slide-corvo-jump-v28-v8-source.mp4",
    "slide-corvo-empty-crouch-v28-v8.mp4",
    "slide-corvo-empty-crouch-v28-v8-source.mp4",
    "slide-corvo-empty-stand-v28-v8.mp4",
    "slide-corvo-empty-stand-v28-v8-source.mp4",
    "slide-corvo-empty-jump-v28-v9.mp4",
    "slide-corvo-empty-jump-v28-v9-source.mp4",
    "slide-emily-crouch-v28-v9.mp4",
    "slide-emily-crouch-v28-v9-source.mp4",
    "slide-emily-stand-v28-v9.mp4",
    "slide-emily-stand-v28-v9-source.mp4",
    "slide-emily-jump-v28-v9.mp4",
    "slide-emily-jump-v28-v9-source.mp4",
    "slide-emily-empty-crouch-v28-v9.mp4",
    "slide-emily-empty-crouch-v28-v9-source.mp4",
    "slide-emily-empty-stand-v28-v9.mp4",
    "slide-emily-empty-stand-v28-v9-source.mp4",
    "slide-emily-empty-jump-v28-v9.mp4",
    "slide-emily-empty-jump-v28-v9-source.mp4"
  ],
  "evidence": [
    "slide-corvo-crouch-v28-v8/result.json",
    "slide-corvo-stand-v28-v8/result.json",
    "slide-corvo-jump-v28-v8/result.json",
    "slide-corvo-empty-crouch-v28-v8/result.json",
    "slide-corvo-empty-stand-v28-v8/result.json",
    "slide-corvo-empty-jump-v28-v9/result.json",
    "slide-emily-crouch-v28-v9/result.json",
    "slide-emily-stand-v28-v9/result.json",
    "slide-emily-jump-v28-v9/result.json",
    "slide-emily-empty-crouch-v28-v9/result.json",
    "slide-emily-empty-stand-v28-v9/result.json",
    "slide-emily-empty-jump-v28-v9/result.json",
    "../movement-integration/visual-sweep-v1260/traversal-review.json",
    "../movement-integration/sweep-defects-audit-v1267.json",
    "coverage-v1321h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1331-traversal.json",
    "decision": "complete",
    "scope": "Slide exits to the crouch (the native default), to a jump and to standing (B once the slide has ended) in both roles; the partner ends standing.",
    "takes": [
      {
        "take": "coverage-v1321h",
        "cases": [
          "host-slide",
          "host-slideexit",
          "client-slide",
          "client-slideexit"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1321h/client-slideexit-observer.mp4",
        "verdict": "The slide recovers into a crouch, and the jump exit lands standing."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: slide-corvo-crouch-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-crouch-v28-v8/result.json>)
- [evidence: slide-corvo-stand-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-stand-v28-v8/result.json>)
- [evidence: slide-corvo-jump-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-jump-v28-v8/result.json>)
- [evidence: slide-corvo-empty-crouch-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-empty-crouch-v28-v8/result.json>)
- [evidence: slide-corvo-empty-stand-v28-v8/result.json](<../../../../work/natural-moveset/slide-corvo-empty-stand-v28-v8/result.json>)
- [evidence: slide-corvo-empty-jump-v28-v9/result.json](<../../../../work/natural-moveset/slide-corvo-empty-jump-v28-v9/result.json>)
- [evidence: slide-emily-crouch-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-crouch-v28-v9/result.json>)
- [evidence: slide-emily-stand-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-stand-v28-v9/result.json>)
- [evidence: slide-emily-jump-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-jump-v28-v9/result.json>)
- [evidence: slide-emily-empty-crouch-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-empty-crouch-v28-v9/result.json>)
- [evidence: slide-emily-empty-stand-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-empty-stand-v28-v9/result.json>)
- [evidence: slide-emily-empty-jump-v28-v9/result.json](<../../../../work/natural-moveset/slide-emily-empty-jump-v28-v9/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/traversal-review.json](<../../../../work/movement-integration/visual-sweep-v1260/traversal-review.json>)
- [evidence: ../movement-integration/sweep-defects-audit-v1267.json](<../../../../work/movement-integration/sweep-defects-audit-v1267.json>)
- [evidence: coverage-v1321h/result.json](<../../../../work/natural-moveset/coverage-v1321h/result.json>)
- [in_game_recordings: slide-corvo-crouch-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-crouch-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-crouch-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-crouch-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-stand-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-stand-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-stand-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-stand-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-jump-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-jump-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-jump-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-jump-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-empty-crouch-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-empty-crouch-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-empty-crouch-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-empty-crouch-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-empty-stand-v28-v8.mp4](<../../../../work/natural-moveset/slide-corvo-empty-stand-v28-v8.mp4>)
- [in_game_recordings: slide-corvo-empty-stand-v28-v8-source.mp4](<../../../../work/natural-moveset/slide-corvo-empty-stand-v28-v8-source.mp4>)
- [in_game_recordings: slide-corvo-empty-jump-v28-v9.mp4](<../../../../work/natural-moveset/slide-corvo-empty-jump-v28-v9.mp4>)
- [in_game_recordings: slide-corvo-empty-jump-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-corvo-empty-jump-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-crouch-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-crouch-v28-v9.mp4>)
- [in_game_recordings: slide-emily-crouch-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-crouch-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-stand-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-stand-v28-v9.mp4>)
- [in_game_recordings: slide-emily-stand-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-stand-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-jump-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-jump-v28-v9.mp4>)
- [in_game_recordings: slide-emily-jump-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-jump-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-empty-crouch-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-empty-crouch-v28-v9.mp4>)
- [in_game_recordings: slide-emily-empty-crouch-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-empty-crouch-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-empty-stand-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-empty-stand-v28-v9.mp4>)
- [in_game_recordings: slide-emily-empty-stand-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-empty-stand-v28-v9-source.mp4>)
- [in_game_recordings: slide-emily-empty-jump-v28-v9.mp4](<../../../../work/natural-moveset/slide-emily-empty-jump-v28-v9.mp4>)
- [in_game_recordings: slide-emily-empty-jump-v28-v9-source.mp4](<../../../../work/natural-moveset/slide-emily-empty-jump-v28-v9-source.mp4>)
## ladder_up_down_entry_exit

1041/1043/1046: native arkClimbable chain climbing in Edge of the World (ordinary interact -> alternate-mode state29 arkPlayerStateClimb) is sent as action state23 (protocol72); the receiver presents bank70 authored ladder-contact ascend/descend cycles advanced by replicated vertical distance. Both roles verified as source with observer-camera videos (Corvo 1043 on client, Emily 1046 on host: grab, climb, descent, release, landing). Chain hand-over-hand acting, entry/exit transitions and true ladders/top exits remain open.

````json
{
  "id": "ladder_up_down_entry_exit",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_chain_climb_both_roles_mission_map",
  "visual_validation": "bounded_actual_game_frames_inspected",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "ladder up down entry exit",
  "motions": [
    "mixamo_climbing_ladder",
    "mixamo_start_climbing_ladder",
    "mixamo_climbing_to_top",
    "ladder_ascend_contact",
    "ladder_descend_contact",
    "ladder_bottom_enter_contact",
    "ladder_bottom_exit_contact",
    "ladder_top_exit_sequence",
    "ladder_top_descent_entry_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "1041/1043/1046: native arkClimbable chain climbing in Edge of the World (ordinary interact -> alternate-mode state29 arkPlayerStateClimb) is sent as action state23 (protocol72); the receiver presents bank70 authored ladder-contact ascend/descend cycles advanced by replicated vertical distance. Both roles verified as source with observer-camera videos (Corvo 1043 on client, Emily 1046 on host: grab, climb, descent, release, landing). Chain hand-over-hand acting, entry/exit transitions and true ladders/top exits remain open.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-climb-eotw-v1041/chain1.mp4",
    "native-climb-eotw-v1041/chain2.mp4",
    "native-climb-eotw-v1041/chain3.mp4",
    "native-climb-eotw-v1041b/chain1.mp4",
    "native-climb-eotw-v1041b/chain2.mp4",
    "climb-coop-eotw-v1043/client.mp4",
    "climb-coop-eotw-v1043/host.mp4",
    "traversal-emily-v1046/climb-client.mp4",
    "traversal-emily-v1046/climb-host.mp4"
  ],
  "evidence": [
    "../movement-integration/climb-audit-v1044.json",
    "../movement-integration/traversal-parity-audit-v1050.json",
    "mission-coverage-v1328v/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1347-climb.json",
    "decision": "complete",
    "scope": "D2 has no climbable ladders: the only climbing state is arkPlayerStateClimb on arkClimbable (the chains), and the ladder resources are step-ladder props (confirmed static). Entry, ascent, descent and exit of that climb are presented in both roles (chain_rope_climb, the same takes).",
    "takes": [
      {
        "take": "mission-coverage-v1328v",
        "cases": [
          "host-climb",
          "host-climbtop",
          "client-climb",
          "client-climbtop"
        ]
      }
    ],
    "visual": [
      {
        "video": "mission-coverage-v1328v/client-climbtop-observer.mp4",
        "verdict": "Emily enters the climb from the street, ascends to the top and exits by letting go."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/climb-audit-v1044.json](<../../../../work/movement-integration/climb-audit-v1044.json>)
- [evidence: ../movement-integration/traversal-parity-audit-v1050.json](<../../../../work/movement-integration/traversal-parity-audit-v1050.json>)
- [evidence: mission-coverage-v1328v/result.json](<../../../../work/natural-moveset/mission-coverage-v1328v/result.json>)
- [in_game_recordings: native-climb-eotw-v1041/chain1.mp4](<../../../../work/natural-moveset/native-climb-eotw-v1041/chain1.mp4>)
- [in_game_recordings: native-climb-eotw-v1041/chain2.mp4](<../../../../work/natural-moveset/native-climb-eotw-v1041/chain2.mp4>)
- [in_game_recordings: native-climb-eotw-v1041/chain3.mp4](<../../../../work/natural-moveset/native-climb-eotw-v1041/chain3.mp4>)
- [in_game_recordings: native-climb-eotw-v1041b/chain1.mp4](<../../../../work/natural-moveset/native-climb-eotw-v1041b/chain1.mp4>)
- [in_game_recordings: native-climb-eotw-v1041b/chain2.mp4](<../../../../work/natural-moveset/native-climb-eotw-v1041b/chain2.mp4>)
- [in_game_recordings: climb-coop-eotw-v1043/client.mp4](<../../../../work/natural-moveset/climb-coop-eotw-v1043/client.mp4>)
- [in_game_recordings: climb-coop-eotw-v1043/host.mp4](<../../../../work/natural-moveset/climb-coop-eotw-v1043/host.mp4>)
- [in_game_recordings: traversal-emily-v1046/climb-client.mp4](<../../../../work/natural-moveset/traversal-emily-v1046/climb-client.mp4>)
- [in_game_recordings: traversal-emily-v1046/climb-host.mp4](<../../../../work/natural-moveset/traversal-emily-v1046/climb-host.mp4>)
## chain_rope_climb

659–661: original native chain definition resolves in both roles, but its isolated creation yields only Audio and AnimBase; PhysicsSystem/Climbable components absent.136 source samples never enter Climb29. Actual source video inspected; no climbing acceptance. Checkpoint fully removes fixture and restores both inventories, health and14 protected hashes. Dependency is documented; do not repeat this incomplete fixture.

````json
{
  "id": "chain_rope_climb",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "blocked_native_fixture_missing_components",
  "visual_validation": "not_accepted",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "chain rope climb",
  "motions": [
    "mixamo_climbing_a_rope"
  ],
  "deliverable_kind": "animation",
  "notes": "659–661: original native chain definition resolves in both roles, but its isolated creation yields only Audio and AnimBase; PhysicsSystem/Climbable components absent.136 source samples never enter Climb29. Actual source video inspected; no climbing acceptance. Checkpoint fully removes fixture and restores both inventories, health and14 protected hashes. Dependency is documented; do not repeat this incomplete fixture.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-climb-corvo-v661/source.mp4"
  ],
  "complete": true,
  "evidence": [
    "native-climb-corvo-v661/result.json",
    "mission-coverage-v1328v/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1347-climb.json",
    "decision": "complete",
    "scope": "Climbing the Edge of the World chain in both roles: grab, hand-over-hand ascent and descent cycles advanced by the replicated height (about 85-110 climb samples, 9-10 phases per case), climbing to the top and letting go back into the default mode.",
    "takes": [
      {
        "take": "mission-coverage-v1328v",
        "cases": [
          "host-climb",
          "host-climbtop",
          "client-climb",
          "client-climbtop"
        ]
      }
    ],
    "visual": [
      {
        "video": "mission-coverage-v1328v/host-climb-observer.mp4",
        "verdict": "Corvo grabs the chain at the street, pulls himself up hand over hand, then climbs back down and steps off."
      },
      {
        "video": "mission-coverage-v1328v/client-climbtop-observer.mp4",
        "verdict": "Emily climbs the chain all the way up, hangs at the top and lets go."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: native-climb-corvo-v661/result.json](<../../../../work/natural-moveset/native-climb-corvo-v661/result.json>)
- [evidence: mission-coverage-v1328v/result.json](<../../../../work/natural-moveset/mission-coverage-v1328v/result.json>)
- [in_game_recordings: native-climb-corvo-v661/source.mp4](<../../../../work/natural-moveset/native-climb-corvo-v661/source.mp4>)
## swim_idle_directions

1035/1038c-f/1046/1049: native swimming (Edge of the World, native campaign command) is arkPlayerStateSwimming in the alternate mode table, sent as action state22 (protocol71+); the receiver presents authored tread/stroke/up/down blended by replicated horizontal speed and vertical velocity, root and depth network-owned. Both roles verified as source (Corvo 1038c-f/1049, Emily 1046/1047/1048/1049). Overhead actual-game videos (1049, both roles) show tread, horizontal stroke and dive/surface on the remote body (bounded visual acceptance). Swim mantle/exit and polish remain open.

````json
{
  "id": "swim_idle_directions",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_swimming_both_roles_mission_map",
  "visual_validation": "bounded_actual_game_frames_inspected",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "swim idle directions",
  "motions": [
    "mixamo_swimming",
    "mixamo_treading_water",
    "water_forward_sequence",
    "water_back_sequence",
    "water_left_sequence",
    "water_right_sequence",
    "water_up_sequence",
    "water_down_sequence",
    "water_dive_sequence",
    "water_surface_sequence",
    "water_start_forward_sequence",
    "water_stop_forward_sequence",
    "water_ledge_hold_sequence",
    "water_ledge_exit_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "1035/1038c-f/1046/1049: native swimming (Edge of the World, native campaign command) is arkPlayerStateSwimming in the alternate mode table, sent as action state22 (protocol71+); the receiver presents authored tread/stroke/up/down blended by replicated horizontal speed and vertical velocity, root and depth network-owned. Both roles verified as source (Corvo 1038c-f/1049, Emily 1046/1047/1048/1049). Overhead actual-game videos (1049, both roles) show tread, horizontal stroke and dive/surface on the remote body (bounded visual acceptance). Swim mantle/exit and polish remain open.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-swim-v1035/host-swim.mp4",
    "swim-coop-eotw-v1038c/client.mp4",
    "swim-coop-eotw-v1038c/host.mp4",
    "swim-coop-eotw-v1038d/client.mp4",
    "swim-coop-eotw-v1038d/host.mp4",
    "swim-coop-eotw-v1038e/client.mp4",
    "swim-coop-eotw-v1038e/host.mp4",
    "swim-coop-eotw-v1038f/client.mp4",
    "swim-coop-eotw-v1038f/host.mp4",
    "traversal-emily-v1046/swim-client.mp4",
    "traversal-emily-v1046/swim-host.mp4",
    "traversal-emily-swimview-v1047/swim-client.mp4",
    "traversal-emily-swimview-v1047/swim-host.mp4",
    "traversal-emily-swimview-v1048/swim-client.mp4",
    "traversal-emily-swimview-v1048/swim-host.mp4",
    "traversal-emily-swimview-v1049/swim-client.mp4",
    "traversal-emily-swimview-v1049/swim-host.mp4",
    "traversal-corvo-swimview-v1049/swim-client.mp4",
    "traversal-corvo-swimview-v1049/swim-host.mp4"
  ],
  "evidence": [
    "../movement-integration/mission-swim-audit-v1040.json",
    "../movement-integration/traversal-parity-audit-v1050.json",
    "mission-coverage-v1328o/result.json",
    "mission-coverage-v1328h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1344-traversal-2.json",
    "decision": "complete",
    "scope": "Native swimming in both roles: forward, lateral and backward strokes, treading, sinking and rising are presented (about 184 samples per role, strokes 82-98 with 37-39 lateral, sink and rise episodes).",
    "takes": [
      {
        "take": "mission-coverage-v1328o",
        "cases": [
          "host-swim",
          "client-swim"
        ]
      },
      {
        "take": "mission-coverage-v1328h",
        "cases": [
          "host-swim",
          "client-swim"
        ]
      }
    ],
    "visual": [
      {
        "video": "mission-coverage-v1328o/client-swim-observer.mp4",
        "verdict": "From behind, Emily strokes away through the murky water, slips under and surfaces again, then treads water with her arms out."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/mission-swim-audit-v1040.json](<../../../../work/movement-integration/mission-swim-audit-v1040.json>)
- [evidence: ../movement-integration/traversal-parity-audit-v1050.json](<../../../../work/movement-integration/traversal-parity-audit-v1050.json>)
- [evidence: mission-coverage-v1328o/result.json](<../../../../work/natural-moveset/mission-coverage-v1328o/result.json>)
- [evidence: mission-coverage-v1328h/result.json](<../../../../work/natural-moveset/mission-coverage-v1328h/result.json>)
- [in_game_recordings: native-swim-v1035/host-swim.mp4](<../../../../work/natural-moveset/native-swim-v1035/host-swim.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038c/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038c/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038c/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038c/host.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038d/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038d/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038d/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038d/host.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038e/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038e/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038e/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038e/host.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038f/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038f/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038f/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038f/host.mp4>)
- [in_game_recordings: traversal-emily-v1046/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-v1046/swim-client.mp4>)
- [in_game_recordings: traversal-emily-v1046/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-v1046/swim-host.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1047/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1047/swim-client.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1047/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1047/swim-host.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1048/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1048/swim-client.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1048/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1048/swim-host.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1049/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1049/swim-client.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1049/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1049/swim-host.mp4>)
- [in_game_recordings: traversal-corvo-swimview-v1049/swim-client.mp4](<../../../../work/natural-moveset/traversal-corvo-swimview-v1049/swim-client.mp4>)
- [in_game_recordings: traversal-corvo-swimview-v1049/swim-host.mp4](<../../../../work/natural-moveset/traversal-corvo-swimview-v1049/swim-host.mp4>)
## dive_surface_water_exit

Dive/surface are vertical-velocity blends of native swimming (1038c-f, 1049 both roles). Water exits (1052-1057): steps hand over swimming->running (remote body rises and runs up the steps, 1052); ledge exits use the native high mantle (state8, mantle_idle/sneak, 833 ms) presented as traversal variant4/5 - candidate1056 raised the mantle rise bound to 4 m (swim-depth exits rise ~3.1 m, which retired the action stream on 1042) and adds a protocol73 hold so unclassified states no longer kill the stream. Both roles verified with observer videos (1057). Native arkPlayerStateSwimMantle not yet observed; wading/exit polish deferred.

````json
{
  "id": "dive_surface_water_exit",
  "family": "traversal",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_water_exit_both_roles_mission_map",
  "visual_validation": "bounded_actual_game_frames_inspected",
  "native_reference_family_count": 155,
  "priority": 2,
  "title": "dive surface water exit",
  "motions": [
    "mixamo_swimming_to_edge",
    "water_dive_sequence",
    "water_surface_sequence",
    "water_ledge_hold_sequence",
    "water_ledge_exit_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Dive/surface are vertical-velocity blends of native swimming (1038c-f, 1049 both roles). Water exits (1052-1057): steps hand over swimming->running (remote body rises and runs up the steps, 1052); ledge exits use the native high mantle (state8, mantle_idle/sneak, 833 ms) presented as traversal variant4/5 - candidate1056 raised the mantle rise bound to 4 m (swim-depth exits rise ~3.1 m, which retired the action stream on 1042) and adds a protocol73 hold so unclassified states no longer kill the stream. Both roles verified with observer videos (1057). Native arkPlayerStateSwimMantle not yet observed; wading/exit polish deferred.",
  "phase_status": "complete",
  "power_visual_requirements": [],
  "in_game_recordings": [
    "native-swim-v1035/host-swim.mp4",
    "swim-coop-eotw-v1038c/client.mp4",
    "swim-coop-eotw-v1038c/host.mp4",
    "swim-coop-eotw-v1038d/client.mp4",
    "swim-coop-eotw-v1038d/host.mp4",
    "swim-coop-eotw-v1038e/client.mp4",
    "swim-coop-eotw-v1038e/host.mp4",
    "swim-coop-eotw-v1038f/client.mp4",
    "swim-coop-eotw-v1038f/host.mp4",
    "traversal-emily-v1046/swim-client.mp4",
    "traversal-emily-v1046/swim-host.mp4",
    "traversal-emily-swimview-v1047/swim-client.mp4",
    "traversal-emily-swimview-v1047/swim-host.mp4",
    "traversal-emily-swimview-v1048/swim-client.mp4",
    "traversal-emily-swimview-v1048/swim-host.mp4",
    "traversal-emily-swimview-v1049/swim-client.mp4",
    "traversal-emily-swimview-v1049/swim-host.mp4",
    "traversal-corvo-swimview-v1049/swim-client.mp4",
    "traversal-corvo-swimview-v1049/swim-host.mp4",
    "swim-exit-corvo-v1051/exit-client.mp4",
    "swim-exit-corvo-v1051/exit-host.mp4",
    "swim-exit-corvo-v1052/exit-client.mp4",
    "swim-exit-corvo-v1052/exit-host.mp4",
    "swim-exit-boat-corvo-v1053/exit-client.mp4",
    "swim-exit-boat-corvo-v1053/exit-host.mp4",
    "swim-exit-boat-corvo-v1054/exit-client.mp4",
    "swim-exit-boat-corvo-v1054/exit-host.mp4",
    "swim-exit-boat-corvo-v1054c/exit-client.mp4",
    "swim-exit-boat-corvo-v1054c/exit-host.mp4",
    "swim-exit-deep-corvo-v1054d/exit-client.mp4",
    "swim-exit-deep-corvo-v1054d/exit-host.mp4",
    "swim-exit-deep-corvo-v1057/exit-client.mp4",
    "swim-exit-deep-corvo-v1057/exit-host.mp4",
    "swim-exit-deep-emily-v1057/exit-client.mp4",
    "swim-exit-deep-emily-v1057/exit-host.mp4"
  ],
  "evidence": [
    "../movement-integration/water-exit-audit-v1058.json",
    "mission-coverage-v1328h/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1344-traversal-2.json",
    "decision": "complete",
    "scope": "Swimming to the dock and climbing out in both roles: the partner swims, then presents the native ledge exit (traversal variants 1, 4) and stands on land; dives and surfacing are presented within swimming (sink and rise).",
    "takes": [
      {
        "take": "mission-coverage-v1328h",
        "cases": [
          "host-waterexit",
          "client-waterexit"
        ]
      }
    ],
    "visual": [
      {
        "video": "mission-coverage-v1328h/host-waterexit-observer.mp4",
        "verdict": "Corvo swims up to the dock edge, pulls himself up onto it in one movement, stands and steps forward."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/water-exit-audit-v1058.json](<../../../../work/movement-integration/water-exit-audit-v1058.json>)
- [evidence: mission-coverage-v1328h/result.json](<../../../../work/natural-moveset/mission-coverage-v1328h/result.json>)
- [in_game_recordings: native-swim-v1035/host-swim.mp4](<../../../../work/natural-moveset/native-swim-v1035/host-swim.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038c/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038c/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038c/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038c/host.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038d/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038d/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038d/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038d/host.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038e/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038e/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038e/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038e/host.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038f/client.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038f/client.mp4>)
- [in_game_recordings: swim-coop-eotw-v1038f/host.mp4](<../../../../work/natural-moveset/swim-coop-eotw-v1038f/host.mp4>)
- [in_game_recordings: traversal-emily-v1046/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-v1046/swim-client.mp4>)
- [in_game_recordings: traversal-emily-v1046/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-v1046/swim-host.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1047/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1047/swim-client.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1047/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1047/swim-host.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1048/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1048/swim-client.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1048/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1048/swim-host.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1049/swim-client.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1049/swim-client.mp4>)
- [in_game_recordings: traversal-emily-swimview-v1049/swim-host.mp4](<../../../../work/natural-moveset/traversal-emily-swimview-v1049/swim-host.mp4>)
- [in_game_recordings: traversal-corvo-swimview-v1049/swim-client.mp4](<../../../../work/natural-moveset/traversal-corvo-swimview-v1049/swim-client.mp4>)
- [in_game_recordings: traversal-corvo-swimview-v1049/swim-host.mp4](<../../../../work/natural-moveset/traversal-corvo-swimview-v1049/swim-host.mp4>)
- [in_game_recordings: swim-exit-corvo-v1051/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-corvo-v1051/exit-client.mp4>)
- [in_game_recordings: swim-exit-corvo-v1051/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-corvo-v1051/exit-host.mp4>)
- [in_game_recordings: swim-exit-corvo-v1052/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-corvo-v1052/exit-client.mp4>)
- [in_game_recordings: swim-exit-corvo-v1052/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-corvo-v1052/exit-host.mp4>)
- [in_game_recordings: swim-exit-boat-corvo-v1053/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-boat-corvo-v1053/exit-client.mp4>)
- [in_game_recordings: swim-exit-boat-corvo-v1053/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-boat-corvo-v1053/exit-host.mp4>)
- [in_game_recordings: swim-exit-boat-corvo-v1054/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-boat-corvo-v1054/exit-client.mp4>)
- [in_game_recordings: swim-exit-boat-corvo-v1054/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-boat-corvo-v1054/exit-host.mp4>)
- [in_game_recordings: swim-exit-boat-corvo-v1054c/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-boat-corvo-v1054c/exit-client.mp4>)
- [in_game_recordings: swim-exit-boat-corvo-v1054c/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-boat-corvo-v1054c/exit-host.mp4>)
- [in_game_recordings: swim-exit-deep-corvo-v1054d/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-deep-corvo-v1054d/exit-client.mp4>)
- [in_game_recordings: swim-exit-deep-corvo-v1054d/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-deep-corvo-v1054d/exit-host.mp4>)
- [in_game_recordings: swim-exit-deep-corvo-v1057/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-deep-corvo-v1057/exit-client.mp4>)
- [in_game_recordings: swim-exit-deep-corvo-v1057/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-deep-corvo-v1057/exit-host.mp4>)
- [in_game_recordings: swim-exit-deep-emily-v1057/exit-client.mp4](<../../../../work/natural-moveset/swim-exit-deep-emily-v1057/exit-client.mp4>)
- [in_game_recordings: swim-exit-deep-emily-v1057/exit-host.mp4](<../../../../work/natural-moveset/swim-exit-deep-emily-v1057/exit-host.mp4>)

