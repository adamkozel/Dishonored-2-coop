# powers: complete requirement records

Source [review-catalog.json](<../../../../work/natural-moveset/review-catalog.json>); captured 2026-10-24T20:40:37.938648+02:00. All fields below retain the catalog wording.

## corvo_blink_aim_cancel_release_arrive

Visual sweep 1260: accepted (corvo accept). Blink aim, native departure/arrival world FX, discrete arrival and the cancelled second aim all read correctly on the partner. Only a dark streak during the lab's reset back to the origin is visible, and it is not part of the power. Standing armed Dunwall native Blink phases drive whole-body motion over Steam. Actual travel uses native departure/arrival FX and a discrete foot-position transfer, without the temporary eye-height body jump. Eight checks cover cast/recovery, native cancellation, two effects, duplicate refusal and equipment/health. Other stances, upgrades, interruptions, broader lifecycle and final visual acceptance remain incomplete. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.

````json
{
  "id": "corvo_blink_aim_cancel_release_arrive",
  "family": "powers",
  "characters": [
    "corvo"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_Blink_body_and_world_FX",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "corvo blink aim cancel release arrive",
  "motions": [
    "sword_teleportation_right",
    "sword_teleportation_back",
    "sword_teleportation_front",
    "sword_teleportation_left",
    "power_blink_fire_in",
    "power_blink_fire_loop",
    "power_blink_fire_out",
    "power_blink_sequence",
    "power_blink_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept). Blink aim, native departure/arrival world FX, discrete arrival and the cancelled second aim all read correctly on the partner. Only a dark streak during the lab's reset back to the origin is visible, and it is not part of the power. Standing armed Dunwall native Blink phases drive whole-body motion over Steam. Actual travel uses native departure/arrival FX and a discrete foot-position transfer, without the temporary eye-height body jump. Eight checks cover cast/recovery, native cancellation, two effects, duplicate refusal and equipment/health. Other stances, upgrades, interruptions, broader lifecycle and final visual acceptance remain incomplete. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "blink",
      "character": "corvo",
      "requirements": [
        "corvo_blink_aim_cancel_release_arrive",
        "corvo_blink_pull_and_kick_variants"
      ],
      "needs": [
        "aim/cancel/release/arrival timing",
        "departure and arrival presentation if native counterpart effects exist",
        "teleport discontinuity rather than interpolated walking",
        "Blink Assault contact and recovery"
      ],
      "runtime_effects_verified": false,
      "partial_runtime_evidence": {
        "scope": "Standing armed Dunwall body and native world effects, cancellation and replay refusal.",
        "recording": "integrated-blink-fx-v3.mp4",
        "remaining": "Stance/movement/equipment/upgrades/dynamic targets, full lifecycle and final visual acceptance."
      }
    }
  ],
  "in_game_recordings": [
    "integrated-blink-fx-v1.mp4",
    "integrated-blink-fx-v3.mp4",
    "blink-knee-v4.mp4",
    "blink-regression-v26-v2.mp4"
  ],
  "native_effects_status": "partial_standing_armed_Dunwall_departure_arrival",
  "evidence": [
    "blink-regression-v26-v2/result.json",
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1332d/result.json",
    "coverage-v1350a/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1350-blink.json",
    "decision": "complete",
    "scope": "Blink in the Corvo source role: aim (variants 1, 2), release (3), the departure burst and arrival presented (1332c/d); a native X cancellation during the aim presents no release, the source stays put (0.0 m) and the partner returns to locomotion within about 0.3 s, and a following cast still releases and travels 4.8 m (1350a). Blink is Corvo-only; the Emily source role has no Blink.",
    "takes": [
      {
        "take": "coverage-v1350a",
        "cases": [
          "host-blinkcancel"
        ]
      },
      {
        "take": "coverage-v1332c",
        "cases": [
          "host-blink"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "host-blink"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1350a/host-blinkcancel-observer.mp4",
        "verdict": "Corvo throws his left arm out to aim, drops it again at the cancel and stays where he stands; a few seconds later he aims again and vanishes in a blue burst."
      },
      {
        "video": "coverage-v1332d/host-blink-observer.mp4",
        "verdict": "Corvo holds the aim with his arm out; a blue burst marks the departure as he vanishes forward."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: blink-regression-v26-v2/result.json](<../../../../work/natural-moveset/blink-regression-v26-v2/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [evidence: coverage-v1350a/result.json](<../../../../work/natural-moveset/coverage-v1350a/result.json>)
- [in_game_recordings: integrated-blink-fx-v1.mp4](<../../../../work/natural-moveset/integrated-blink-fx-v1.mp4>)
- [in_game_recordings: integrated-blink-fx-v3.mp4](<../../../../work/natural-moveset/integrated-blink-fx-v3.mp4>)
- [in_game_recordings: blink-knee-v4.mp4](<../../../../work/natural-moveset/blink-knee-v4.mp4>)
- [in_game_recordings: blink-regression-v26-v2.mp4](<../../../../work/natural-moveset/blink-regression-v26-v2.mp4>)
## corvo_blink_pull_and_kick_variants

Exact native assets confirm Blink Assault. Full-body kick and slash sequences are recorded. No Corvo pulling variant was confirmed; pulling remains under Emily Far Reach. Teleport/target timing, contact, knockdown and runtime binding are pending. Emily is a comparison adaptation. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.

````json
{
  "id": "corvo_blink_pull_and_kick_variants",
  "family": "powers",
  "characters": [
    "corvo"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_runtime_completion_audit",
  "visual_validation": "accepted_completion_audit",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "Corvo Blink Assault kick and slash",
  "motions": [
    "blink_assault_kick_sequence",
    "blink_assault_slash_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Exact native assets confirm Blink Assault. Full-body kick and slash sequences are recorded. No Corvo pulling variant was confirmed; pulling remains under Emily Far Reach. Teleport/target timing, contact, knockdown and runtime binding are pending. Emily is a comparison adaptation. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "blink",
      "character": "corvo",
      "requirements": [
        "corvo_blink_aim_cancel_release_arrive",
        "corvo_blink_pull_and_kick_variants"
      ],
      "needs": [
        "aim/cancel/release/arrival timing",
        "departure and arrival presentation if native counterpart effects exist",
        "teleport discontinuity rather than interpolated walking",
        "Blink Assault contact and recovery"
      ],
      "runtime_effects_verified": false,
      "partial_runtime_evidence": {
        "scope": "Standing armed Dunwall body and native world effects, cancellation and replay refusal.",
        "recording": "integrated-blink-fx-v3.mp4",
        "remaining": "Stance/movement/equipment/upgrades/dynamic targets, full lifecycle and final visual acceptance."
      }
    }
  ],
  "in_game_recordings": [],
  "native_effects_status": "not_verified",
  "completion": {
    "audit": "completion-audit-v1356-blink-assault.json",
    "decision": "complete",
    "scope": "Blink Assault in the Corvo source role: the blink (power variants 1-3, departure and arrival effects) followed by the native kick on arrival, presented on the partner as the kick (variant 17, 5-6 samples per take). Corvo-only (Blink).",
    "takes": [
      {
        "take": "coverage-v1359b",
        "cases": [
          "host-blinkkick"
        ]
      },
      {
        "take": "coverage-v1359a",
        "cases": [
          "host-blinkkick"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1359b/host-blinkkick-observer.mp4",
        "verdict": "Corvo bursts out of the blink right at the guard with his arms flung wide and one leg driving forward in the kick, then lands and straightens in the dispersing smoke."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true,
  "evidence": [
    "coverage-v1359b/result.json",
    "coverage-v1359a/result.json"
  ],
  "visual_validation_before": "not_accepted",
  "runtime_binding_before": "not_verified"
}
````

- [evidence: coverage-v1359b/result.json](<../../../../work/natural-moveset/coverage-v1359b/result.json>)
- [evidence: coverage-v1359a/result.json](<../../../../work/natural-moveset/coverage-v1359a/result.json>)
## corvo_dark_vision_cast_end

Native Dark Vision casts now cover ordinary jumps as well as tested ground stance/equipment/travel contexts. Exact native cast clock and airborne semantics travel together; native private reveal, sword state and landing recovery are preserved. Other contexts and lifecycle remain.

````json
{
  "id": "corvo_dark_vision_cast_end",
  "family": "powers",
  "characters": [
    "corvo"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_Dark_Vision_stance_equipment_and_travel_through_Steam",
  "visual_validation": "accepted_completion_audit",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "corvo dark vision cast end",
  "motions": [
    "power_dark_vision_fire",
    "power_dark_vision_sequence",
    "native_phase_dark_vision_fire",
    "native_darkvision_standing_empty",
    "native_darkvision_crouched_armed",
    "native_darkvision_crouched_empty"
  ],
  "deliverable_kind": "animation",
  "notes": "Native Dark Vision casts now cover ordinary jumps as well as tested ground stance/equipment/travel contexts. Exact native cast clock and airborne semantics travel together; native private reveal, sword state and landing recovery are preserved. Other contexts and lifecycle remain.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "dark_vision",
      "character": "both",
      "requirements": [
        "corvo_dark_vision_cast_end",
        "emily_dark_vision_cast_end"
      ],
      "needs": [
        "cast and end timing",
        "native viewer-specific reveal and screen effects",
        "confirmed world-visible effects only"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "darkvision-corvo-v25-v2.mp4",
    "darkvision-corvo-v25-v2-source.mp4",
    "darkvision-corvo-crouched-v26-v1.mp4",
    "darkvision-corvo-crouched-v26-v1-source.mp4",
    "darkvision-corvo-empty-v26-v1.mp4",
    "darkvision-corvo-empty-v26-v1-source.mp4",
    "darkvision-corvo-crouched-empty-v26-v1.mp4",
    "darkvision-corvo-crouched-empty-v26-v1-source.mp4",
    "darkvision-corvo-moving-v26-v3.mp4",
    "darkvision-corvo-moving-v26-v3-source.mp4",
    "darkvision-corvo-crouched-moving-v26-v1.mp4",
    "darkvision-corvo-crouched-moving-v26-v1-source.mp4",
    "darkvision-corvo-empty-moving-v26-v1.mp4",
    "darkvision-corvo-empty-moving-v26-v1-source.mp4",
    "darkvision-corvo-crouched-empty-moving-v26-v1.mp4",
    "darkvision-corvo-crouched-empty-moving-v26-v1-source.mp4",
    "darkvision-corvo-air-v26-v3.mp4",
    "darkvision-corvo-air-v26-v3-source.mp4",
    "darkvision-corvo-air-empty-v26-v1.mp4",
    "darkvision-corvo-air-empty-v26-v1-source.mp4",
    "darkvision-corvo-balanced-v28-v1.mp4",
    "darkvision-corvo-balanced-v28-v1-source.mp4"
  ],
  "native_effects_status": "native_caster_vision_on_off_and_viewer_scope_verified_in_ground_contexts",
  "evidence": [
    "darkvision-corvo-v25-v2/result.json",
    "darkvision-corvo-crouched-v26-v1/result.json",
    "darkvision-corvo-empty-v26-v1/result.json",
    "darkvision-corvo-crouched-empty-v26-v1/result.json",
    "darkvision-corvo-moving-v26-v3/result.json",
    "darkvision-corvo-crouched-moving-v26-v1/result.json",
    "darkvision-corvo-empty-moving-v26-v1/result.json",
    "darkvision-corvo-crouched-empty-moving-v26-v1/result.json",
    "darkvision-corvo-air-v26-v3/result.json",
    "darkvision-corvo-air-empty-v26-v1/result.json",
    "darkvision-corvo-balanced-v28-v1/result.json",
    "coverage-v1332c/result.json",
    "coverage-v1332d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1337-powers.json",
    "decision": "complete",
    "scope": "Corvo casts and ends Dark Vision (power variant 10) standing armed; the partner presents both gestures and recovers. Dark Vision has no world-visible effect: its reveal and screen effect stay with the caster (first-person scope).",
    "takes": [
      {
        "take": "coverage-v1332c",
        "cases": [
          "host-darkvision"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "host-darkvision"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1332d/host-darkvision-observer.mp4",
        "verdict": "Corvo raises his left hand to his eye to cast, holds the armed stance, and repeats the gesture to end it; no effect appears in the observer world."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true,
  "visual_validation_before": "bounded_actual_game_frames_inspected"
}
````

- [evidence: darkvision-corvo-v25-v2/result.json](<../../../../work/natural-moveset/darkvision-corvo-v25-v2/result.json>)
- [evidence: darkvision-corvo-crouched-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-crouched-v26-v1/result.json>)
- [evidence: darkvision-corvo-empty-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-empty-v26-v1/result.json>)
- [evidence: darkvision-corvo-crouched-empty-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-crouched-empty-v26-v1/result.json>)
- [evidence: darkvision-corvo-moving-v26-v3/result.json](<../../../../work/natural-moveset/darkvision-corvo-moving-v26-v3/result.json>)
- [evidence: darkvision-corvo-crouched-moving-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-crouched-moving-v26-v1/result.json>)
- [evidence: darkvision-corvo-empty-moving-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-empty-moving-v26-v1/result.json>)
- [evidence: darkvision-corvo-crouched-empty-moving-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-crouched-empty-moving-v26-v1/result.json>)
- [evidence: darkvision-corvo-air-v26-v3/result.json](<../../../../work/natural-moveset/darkvision-corvo-air-v26-v3/result.json>)
- [evidence: darkvision-corvo-air-empty-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-air-empty-v26-v1/result.json>)
- [evidence: darkvision-corvo-balanced-v28-v1/result.json](<../../../../work/natural-moveset/darkvision-corvo-balanced-v28-v1/result.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: darkvision-corvo-v25-v2.mp4](<../../../../work/natural-moveset/darkvision-corvo-v25-v2.mp4>)
- [in_game_recordings: darkvision-corvo-v25-v2-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-v25-v2-source.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-v26-v1.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-corvo-empty-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-empty-v26-v1.mp4>)
- [in_game_recordings: darkvision-corvo-empty-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-empty-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-empty-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-empty-v26-v1.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-empty-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-empty-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-corvo-moving-v26-v3.mp4](<../../../../work/natural-moveset/darkvision-corvo-moving-v26-v3.mp4>)
- [in_game_recordings: darkvision-corvo-moving-v26-v3-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-moving-v26-v3-source.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-moving-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-moving-v26-v1.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-moving-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-moving-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-corvo-empty-moving-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-empty-moving-v26-v1.mp4>)
- [in_game_recordings: darkvision-corvo-empty-moving-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-empty-moving-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-empty-moving-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-empty-moving-v26-v1.mp4>)
- [in_game_recordings: darkvision-corvo-crouched-empty-moving-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-crouched-empty-moving-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-corvo-air-v26-v3.mp4](<../../../../work/natural-moveset/darkvision-corvo-air-v26-v3.mp4>)
- [in_game_recordings: darkvision-corvo-air-v26-v3-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-air-v26-v3-source.mp4>)
- [in_game_recordings: darkvision-corvo-air-empty-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-air-empty-v26-v1.mp4>)
- [in_game_recordings: darkvision-corvo-air-empty-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-air-empty-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-corvo-balanced-v28-v1.mp4](<../../../../work/natural-moveset/darkvision-corvo-balanced-v28-v1.mp4>)
- [in_game_recordings: darkvision-corvo-balanced-v28-v1-source.mp4](<../../../../work/natural-moveset/darkvision-corvo-balanced-v28-v1-source.mp4>)
## corvo_possession_enter_loop_exit

Visual sweep 1260: accepted (corvo accept). The empty-target Possession entry and aim, and the recovery to idle, read naturally. The possessed loop and the exit with a real target are not recorded, so that is a coverage gap rather than a visual defect. Scope: body accepted for the empty-target aim and recovery only; possession of a real target is not recorded. 374 partner exclusion passes8;375 ordinary crouched guard lifecycle passes20 on same DLL/bank54. Other powers, contexts, reciprocal direct interactions and collision exclusion remain unfinished.

````json
{
  "id": "corvo_possession_enter_loop_exit",
  "family": "powers",
  "characters": [
    "corvo"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_empty_target_entry_aim_remote_body_verified_target_pending",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "corvo possession enter loop exit",
  "motions": [
    "power_possession_fire_in",
    "power_possession_fire_loop",
    "power_possession_fire_out",
    "power_possession_sequence",
    "power_possession_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept). The empty-target Possession entry and aim, and the recovery to idle, read naturally. The possessed loop and the exit with a real target are not recorded, so that is a coverage gap rather than a visual defect. Scope: body accepted for the empty-target aim and recovery only; possession of a real target is not recorded. 374 partner exclusion passes8;375 ordinary crouched guard lifecycle passes20 on same DLL/bank54. Other powers, contexts, reciprocal direct interactions and collision exclusion remain unfinished.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "possession",
      "character": "corvo",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "needs": [
        "body visibility and controlled-body handoff",
        "native enter/exit presentation",
        "reappearance and target restoration"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "possession-empty-v322.mp4",
    "possession-network-v323.mp4",
    "possession-network-v323-source.mp4",
    "possession-human-v328.mp4",
    "possession-release-network-v335.mp4",
    "possession-release-network-v335-source.mp4",
    "possession-visibility-network-v339.mp4",
    "possession-visibility-network-v339-source.mp4",
    "possession-visibility-return-v342.mp4",
    "possession-visibility-return-v342-source.mp4",
    "possession-exit-continuity-v343.mp4",
    "possession-exit-continuity-v343-source.mp4",
    "possession-receiver-stop-v344.mp4",
    "possession-receiver-stop-v344-source.mp4",
    "possession-exit-close-v345.mp4",
    "possession-exit-close-v345-source.mp4",
    "possession-receiver-pause-v347.mp4",
    "possession-receiver-pause-v347-source.mp4",
    "possession-source-pause-v347.mp4",
    "possession-source-pause-v347-source.mp4",
    "possession-fx-observe-v349.mp4",
    "possession-fx-observe-v349-source.mp4",
    "possession-scoped-fx-v350.mp4",
    "possession-scoped-fx-v350-source.mp4",
    "possession-target-fx-network-v351.mp4",
    "possession-target-fx-network-v351-source.mp4",
    "possession-target-fx-view-v352.mp4",
    "possession-target-fx-view-v352-source.mp4",
    "possession-target-fx-view-v354.mp4",
    "possession-target-fx-view-v354-source.mp4",
    "possession-target-fx-side-v355.mp4",
    "possession-target-fx-side-v355-source.mp4",
    "possession-target-fx-cancel-v358.mp4",
    "possession-target-fx-cancel-v358-source.mp4",
    "possession-target-fx-receiver-stop-v359.mp4",
    "possession-target-fx-receiver-stop-v359-source.mp4",
    "possession-target-fx-source-pause-v360.mp4",
    "possession-target-fx-source-pause-v360-source.mp4",
    "possession-target-fx-receiver-pause-v360.mp4",
    "possession-target-fx-receiver-pause-v360-source.mp4",
    "possession-target-fx-source-pause-v362.mp4",
    "possession-target-fx-source-pause-v362-source.mp4",
    "possession-crouch-observe-v361.mp4",
    "possession-holstered-observe-v361.mp4",
    "possession-crouch-holstered-observe-v361.mp4",
    "possession-crouch-network-v363.mp4",
    "possession-crouch-network-v363-source.mp4",
    "possession-holstered-network-v364.mp4",
    "possession-holstered-network-v364-source.mp4",
    "possession-holstered-network-v366.mp4",
    "possession-holstered-network-v366-source.mp4",
    "possession-holstered-network-v367.mp4",
    "possession-holstered-network-v367-source.mp4",
    "possession-holstered-network-v368.mp4",
    "possession-holstered-network-v368-source.mp4",
    "possession-holstered-network-v369.mp4",
    "possession-holstered-network-v369-source.mp4",
    "possession-crouch-holstered-network-v369.mp4",
    "possession-crouch-holstered-network-v369-source.mp4",
    "possession-crouch-holstered-network-v370.mp4",
    "possession-crouch-holstered-network-v370-source.mp4",
    "possession-crouch-target-network-v370.mp4",
    "possession-crouch-target-network-v370-source.mp4",
    "possession-crouch-target-network-v371.mp4",
    "possession-crouch-target-network-v371-source.mp4",
    "possession-crouch-target-network-v372.mp4",
    "possession-crouch-target-network-v372-source.mp4",
    "possession-partner-exclusion-v374.mp4",
    "possession-partner-guard-control-v375.mp4",
    "possession-partner-guard-control-v375-source.mp4"
  ],
  "native_effects_status": "not_verified",
  "native_integration": {
    "possession-empty-v322.mp4": {
      "video": "possession-empty-v322.mp4",
      "character": "corvo",
      "label": "Possession native source · empty target · v322",
      "alias": "native_possession_in",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-empty-v322/result.json",
      "visual_review": "possession-empty-v322/visual-review.json",
      "proxy_sha256": "8B612D3614CB878BF7EB19C9E89BBB8EC6E47C440DBF168D984B2039FF733620",
      "actual_capture_hz": 26.61795407098121,
      "video_sha256": "F243414FEFA42E1ADF43C0E17BD084DF67640A37E7A222F26B935E2FA0475C72",
      "checks_passed": true,
      "scope": "Native source Corvo Possession acquisition, controller input, raw scene clocks/state/equipment and candidate target/entity observations. AI held in the isolated Dunwall fixture; empty-target cast only; valid-target possession, remote movement/VFX/gameplay remain unverified. Checkpoint and protected save restoration mandatory."
    },
    "possession-network-v323.mp4": {
      "video": "possession-network-v323.mp4",
      "character": "corvo",
      "label": "Possession · remote full-body · empty target · v323",
      "alias": "native_possession_in",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-network-v323/result.json",
      "visual_review": "possession-network-v323/visual-review.json",
      "proxy_sha256": "EA78931D9D066973132F9949D3A57773F438D3C94733CD62058FDCF0A25D970C",
      "actual_capture_hz": 23.25,
      "video_sha256": "F3A8910F339B120C9FCD068A5B5376473999F461C23A6CB4332C2D631966D785",
      "checks_passed": true,
      "scope": "Standing armed native Corvo empty-target Possession entry333ms/aim200ms drive remote full-body bank51/315 through native Steam protocol48. Source scene retirement drives idle recovery; no invented successful-target release. Local first-person remains native; peer animation executes no transfer, VFX or damage event. Valid-target transfer, disappearance/reappearance, VFX, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "possession-network-v323-source.mp4": {
      "video": "possession-network-v323-source.mp4",
      "character": "corvo",
      "label": "Possession · native source · empty target · v323",
      "alias": "native_possession_in",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-network-v323/result.json",
      "visual_review": "possession-network-v323/visual-review.json",
      "proxy_sha256": "EA78931D9D066973132F9949D3A57773F438D3C94733CD62058FDCF0A25D970C",
      "actual_capture_hz": 26.0,
      "video_sha256": "6F6328F0493090AD2696069AD6713FE2E982E56A175D55CFCBDB38A91C6D1F3F",
      "checks_passed": true,
      "scope": "Standing armed native Corvo empty-target Possession entry333ms/aim200ms drive remote full-body bank51/315 through native Steam protocol48. Source scene retirement drives idle recovery; no invented successful-target release. Local first-person remains native; peer animation executes no transfer, VFX or damage event. Valid-target transfer, disappearance/reappearance, VFX, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "possession-human-v328.mp4": {
      "video": "possession-human-v328.mp4",
      "character": "corvo",
      "label": "Possession · native human host entry and exit · v328",
      "alias": "native_possession_in",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-human-v328/result.json",
      "visual_review": "possession-human-v328/visual-review.json",
      "proxy_sha256": "268AE6B6A44FD08F6F0947C9D63333F4E09B30DC03EBF3CA22931491A40479EB",
      "actual_capture_hz": 26.0625,
      "video_sha256": "A5CF744B3E2AC44FC0E1C2AE1FACBE47FD61FFE0CD524C62A5CB1E78B9E7813F",
      "checks_passed": true,
      "scope": "Source-only native human-target Possession lifecycle and deferred acquisition rollback. Remote presentation remains limited to empty-target entry/aim. See result and review for isolation retirement and unresolved networking/VFX."
    },
    "possession-release-network-v335.mp4": {
      "video": "possession-release-network-v335.mp4",
      "character": "corvo",
      "label": "Possession · remote full-body successful release · v335",
      "alias": "native_possession_out",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-release-network-v335/result.json",
      "visual_review": "possession-release-network-v335/visual-review.json",
      "proxy_sha256": "716FBF25D5EA93758CBD5F253C8E610408E6F5987547D11FC8B59447C69F6487",
      "actual_capture_hz": 25.196374622356494,
      "video_sha256": "53C566616B8FD4155882F284CA93F2388AD250811A9023B13736C42DFF8543D8",
      "checks_passed": true,
      "scope": "Peer shows Corvo fullbody raised left arm and native sword holster during successful release. Source shows target highlight/travel/host vision and native exit. Peer avatar moves out of view toward source host position; this does NOT establish correct disappearance or remote occupancy. Tutorial popup and dark body limit hand-detail judgment. Full native host visibility/return, replicated VFX and target gameplay remain incomplete. Captures have no audio."
    },
    "possession-release-network-v335-source.mp4": {
      "video": "possession-release-network-v335-source.mp4",
      "character": "corvo",
      "label": "Possession · native human host cast and exit · v335",
      "alias": "native_possession_out",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-release-network-v335/result.json",
      "visual_review": "possession-release-network-v335/visual-review.json",
      "proxy_sha256": "716FBF25D5EA93758CBD5F253C8E610408E6F5987547D11FC8B59447C69F6487",
      "actual_capture_hz": 25.454545454545453,
      "video_sha256": "793954B3A59093DFC2E5F3039CE17D7E60C89A65C3234EB8E0C9F6013C655323",
      "checks_passed": true,
      "scope": "Peer shows Corvo fullbody raised left arm and native sword holster during successful release. Source shows target highlight/travel/host vision and native exit. Peer avatar moves out of view toward source host position; this does NOT establish correct disappearance or remote occupancy. Tutorial popup and dark body limit hand-detail judgment. Full native host visibility/return, replicated VFX and target gameplay remain incomplete. Captures have no audio."
    },
    "possession-visibility-network-v339.mp4": {
      "video": "possession-visibility-network-v339.mp4",
      "character": "corvo",
      "label": "Possession · remote body hides during occupation · v339",
      "alias": "native_possession_occupied",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-visibility-network-v339/result.json",
      "visual_review": "possession-visibility-network-v339/visual-review.json",
      "proxy_sha256": "D21071A6DE663F017815B00F21F63BD467DD5C1E4AEADF8E209F6315F67FA718",
      "actual_capture_hz": 23.799392097264437,
      "video_sha256": "AC6369C538FB84710B86E1CAC79BC1C482CC663C7CABBEC0D77B604AFA41C9C9",
      "checks_passed": true,
      "scope": "Peer shows Corvo casting and holstering, then disappearance by3.8s and absence during occupied4.5/5.5s. Returned body position is occluded or outside the camera; exactly one native show/readback is established numerically, not visually. Source shows native host vision and normal exit. No audio. Remote NPC control, outcome authority, world VFX, exit movement continuity and interrupted visibility remain unverified."
    },
    "possession-visibility-network-v339-source.mp4": {
      "video": "possession-visibility-network-v339-source.mp4",
      "character": "corvo",
      "label": "Possession · native human occupation and exit · v339",
      "alias": "native_possession_occupied",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-visibility-network-v339/result.json",
      "visual_review": "possession-visibility-network-v339/visual-review.json",
      "proxy_sha256": "D21071A6DE663F017815B00F21F63BD467DD5C1E4AEADF8E209F6315F67FA718",
      "actual_capture_hz": 24.90909090909091,
      "video_sha256": "EBD91B2F25F94697CBD2E49E139607F999812D2EDC9928884C2EB9BBFB250B85",
      "checks_passed": true,
      "scope": "Peer shows Corvo casting and holstering, then disappearance by3.8s and absence during occupied4.5/5.5s. Returned body position is occluded or outside the camera; exactly one native show/readback is established numerically, not visually. Source shows native host vision and normal exit. No audio. Remote NPC control, outcome authority, world VFX, exit movement continuity and interrupted visibility remain unverified."
    },
    "possession-visibility-return-v342.mp4": {
      "video": "possession-visibility-return-v342.mp4",
      "character": "corvo",
      "label": "Possession · return-camera inspection v342 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-visibility-return-v342/result.json",
      "visual_review": "possession-visibility-return-v342/visual-review.json",
      "proxy_sha256": "D21071A6DE663F017815B00F21F63BD467DD5C1E4AEADF8E209F6315F67FA718",
      "actual_capture_hz": 24.454545454545453,
      "video_sha256": "DEF09684A4F07865B3CC001AB0B947CD4F182F201F8492A372559F6FD22E072E",
      "checks_passed": true,
      "scope": "Source shows native cast, travel, occupied vision and exit. Peer Corvo is visible at0.8s and disappears during occupation. After reframe the native guard occupies the same area and the pillar obscures the returned clone; a distinct visible Corvo return is not established. Native clone show/readback passes. Both semantic streams retire during the unhandled power_equip plus sword_equip exit; subsequent body playback is not functional in DLL338. This test verifies visibility ownership, not completed exit movement or remote NPC control. Recordings have no audio."
    },
    "possession-visibility-return-v342-source.mp4": {
      "video": "possession-visibility-return-v342-source.mp4",
      "character": "corvo",
      "label": "Possession · return-camera inspection v342 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-visibility-return-v342/result.json",
      "visual_review": "possession-visibility-return-v342/visual-review.json",
      "proxy_sha256": "D21071A6DE663F017815B00F21F63BD467DD5C1E4AEADF8E209F6315F67FA718",
      "actual_capture_hz": 25.440729483282674,
      "video_sha256": "E12E44BABF5E52BEFC5C4F26D2906600CEAD3A8CEC6BD8288329A78A47FDA7FB",
      "checks_passed": true,
      "scope": "Source shows native cast, travel, occupied vision and exit. Peer Corvo is visible at0.8s and disappears during occupation. After reframe the native guard occupies the same area and the pillar obscures the returned clone; a distinct visible Corvo return is not established. Native clone show/readback passes. Both semantic streams retire during the unhandled power_equip plus sword_equip exit; subsequent body playback is not functional in DLL338. This test verifies visibility ownership, not completed exit movement or remote NPC control. Recordings have no audio."
    },
    "possession-exit-continuity-v343.mp4": {
      "video": "possession-exit-continuity-v343.mp4",
      "character": "corvo",
      "label": "Possession · native exit continuity v343 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-exit-continuity-v343/result.json",
      "visual_review": "possession-exit-continuity-v343/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 23.11178247734139,
      "video_sha256": "9E27C5D42C1C14A21BB425A7AB4CF9FFFB11A8D38ECE8C80B7CED63F4F7CA5BE",
      "checks_passed": true,
      "scope": "Source clearly shows the native human target, cast highlight, occupied view, normal exit/equip and half-metre backward walk. The fixed observer floor is too far from this alternate guard endpoint: peer view mostly contains the chandelier and does not frame the remote body. Numerical evidence proves paired-scene ownership, uninterrupted semantic transport, remote equip and walking samples, but this recording is NOT visual acceptance of the remote return. First-person remains native; no audio. Next capture reuses the source's measured floor as the closer post-exit observer floor."
    },
    "possession-exit-continuity-v343-source.mp4": {
      "video": "possession-exit-continuity-v343-source.mp4",
      "character": "corvo",
      "label": "Possession · native exit continuity v343 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-exit-continuity-v343/result.json",
      "visual_review": "possession-exit-continuity-v343/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 25.818181818181817,
      "video_sha256": "0D865B41C8E9894EFC480DBC7D45DC3770273E44C07A67A4C5089F08FE6508D0",
      "checks_passed": true,
      "scope": "Source clearly shows the native human target, cast highlight, occupied view, normal exit/equip and half-metre backward walk. The fixed observer floor is too far from this alternate guard endpoint: peer view mostly contains the chandelier and does not frame the remote body. Numerical evidence proves paired-scene ownership, uninterrupted semantic transport, remote equip and walking samples, but this recording is NOT visual acceptance of the remote return. First-person remains native; no audio. Next capture reuses the source's measured floor as the closer post-exit observer floor."
    },
    "possession-receiver-stop-v344.mp4": {
      "video": "possession-receiver-stop-v344.mp4",
      "character": "corvo",
      "label": "Possession · receiver interruption v344 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-receiver-stop-v344/result.json",
      "visual_review": "possession-receiver-stop-v344/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 24.25531914893617,
      "video_sha256": "C82670139EF4BD73A010B8FEBBA6B897D3E21CEB01003C0793CC19EFDD442C86",
      "checks_passed": true,
      "scope": "The peer shows Corvo before cast, disappearance during occupation, then a clearly visible masked Corvo after camera reframe. The source continues native occupied vision through receiver-stop and exits by normal input. The remote clone and native guard overlap at the host return point; during deliberate stream shutdown the clone uses its restored native idle presentation. This confirms visible restoration under interruption, not continuous equipped movement, remote NPC control or polished return staging. Source first-person VFX stay local. No audio captured."
    },
    "possession-receiver-stop-v344-source.mp4": {
      "video": "possession-receiver-stop-v344-source.mp4",
      "character": "corvo",
      "label": "Possession · receiver interruption v344 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-receiver-stop-v344/result.json",
      "visual_review": "possession-receiver-stop-v344/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 24.90909090909091,
      "video_sha256": "098235A228B6FDAE86D35432EB95022CE46BBCA2C8B4597A242217F00E67F25B",
      "checks_passed": true,
      "scope": "The peer shows Corvo before cast, disappearance during occupation, then a clearly visible masked Corvo after camera reframe. The source continues native occupied vision through receiver-stop and exits by normal input. The remote clone and native guard overlap at the host return point; during deliberate stream shutdown the clone uses its restored native idle presentation. This confirms visible restoration under interruption, not continuous equipped movement, remote NPC control or polished return staging. Source first-person VFX stay local. No audio captured."
    },
    "possession-exit-close-v345.mp4": {
      "video": "possession-exit-close-v345.mp4",
      "character": "corvo",
      "label": "Possession · close exit and movement v345 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-exit-close-v345/result.json",
      "visual_review": "possession-exit-close-v345/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 23.818181818181817,
      "video_sha256": "C38E7E5FF2381F2CE9A6920BBE6C51463612256008057BCD6941E9DA61C078F5",
      "checks_passed": true,
      "scope": "Peer shows masked Corvo casting behind the guard, disappearing during occupation, and visibly returning at6.4s. The guard blocks much of the view and overlaps Corvo during the later half-metre backward step. Source shows untouched native cast/host vision/equip/exit. This supports visible return plus numerical movement continuity, but not a polished complete-body sequence or synchronized host NPC. No audio captured. Remote NPC authority, target VFX and shared outcomes remain unimplemented."
    },
    "possession-exit-close-v345-source.mp4": {
      "video": "possession-exit-close-v345-source.mp4",
      "character": "corvo",
      "label": "Possession · close exit and movement v345 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-exit-close-v345/result.json",
      "visual_review": "possession-exit-close-v345/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 25.727272727272727,
      "video_sha256": "3D21C9D94C43F565D29F5595ADD63DC2D3AD7BA41107586A955F59AEB19EFCA1",
      "checks_passed": true,
      "scope": "Peer shows masked Corvo casting behind the guard, disappearing during occupation, and visibly returning at6.4s. The guard blocks much of the view and overlaps Corvo during the later half-metre backward step. Source shows untouched native cast/host vision/equip/exit. This supports visible return plus numerical movement continuity, but not a polished complete-body sequence or synchronized host NPC. No audio captured. Remote NPC authority, target VFX and shared outcomes remain unimplemented."
    },
    "possession-receiver-pause-v347.mp4": {
      "video": "possession-receiver-pause-v347.mp4",
      "character": "corvo",
      "label": "Possession · native receiver pause v347 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-receiver-pause-v347/result.json",
      "visual_review": "possession-receiver-pause-v347/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 24.90909090909091,
      "video_sha256": "48D3A07E6831ACFDF2E93CA3C32204050AF8FD5D0EB14214770019691768978B",
      "checks_passed": true,
      "scope": "Peer shows the native Emily pause menu at5.1s and native resume by5.7s. Source remains in native occupied vision through that pause/resume and later exits by normal input. Remote return area remains obscured/overlapped with the native guard; exact visibility cleanup is supported by independent telemetry, not complete visual acceptance. No audio captured. Remote NPC authority, target VFX and shared outcomes remain unimplemented."
    },
    "possession-receiver-pause-v347-source.mp4": {
      "video": "possession-receiver-pause-v347-source.mp4",
      "character": "corvo",
      "label": "Possession · native receiver pause v347 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-receiver-pause-v347/result.json",
      "visual_review": "possession-receiver-pause-v347/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 25.196374622356494,
      "video_sha256": "52AF4D1B6397C01494645F33543852F27A1F5DC347A08B84E8E0BD6770D62482",
      "checks_passed": true,
      "scope": "Peer shows the native Emily pause menu at5.1s and native resume by5.7s. Source remains in native occupied vision through that pause/resume and later exits by normal input. Remote return area remains obscured/overlapped with the native guard; exact visibility cleanup is supported by independent telemetry, not complete visual acceptance. No audio captured. Remote NPC authority, target VFX and shared outcomes remain unimplemented."
    },
    "possession-source-pause-v347.mp4": {
      "video": "possession-source-pause-v347.mp4",
      "character": "corvo",
      "label": "Possession · native source pause v347 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-source-pause-v347/result.json",
      "visual_review": "possession-source-pause-v347/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 25.258358662613983,
      "video_sha256": "A1D3D44CEE9B2FE58204F2DF8C47B158D68B5B8728BEB1FC843145B77BE8A4F8",
      "checks_passed": true,
      "scope": "Source shows the native Corvo pause menu at5.1s, resumes occupied vision, then exits normally. Peer continues rendering and later camera reframe shows the host area. A distinct full-body restored Corvo is not clearly framed; native visibility readback and stream retirement are independently verified. This is lifecycle cleanup evidence, not finished host/world presentation. No audio captured. Remote NPC authority, target VFX and shared outcomes remain unimplemented."
    },
    "possession-source-pause-v347-source.mp4": {
      "video": "possession-source-pause-v347-source.mp4",
      "character": "corvo",
      "label": "Possession · native source pause v347 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-source-pause-v347/result.json",
      "visual_review": "possession-source-pause-v347/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 25.830815709969787,
      "video_sha256": "14BEE7A1245C3A63DA0918951A5C439A205D4317E75272B0DEC7CEF3626A2002",
      "checks_passed": true,
      "scope": "Source shows the native Corvo pause menu at5.1s, resumes occupied vision, then exits normally. Peer continues rendering and later camera reframe shows the host area. A distinct full-body restored Corvo is not clearly framed; native visibility readback and stream retirement are independently verified. This is lifecycle cleanup evidence, not finished host/world presentation. No audio captured. Remote NPC authority, target VFX and shared outcomes remain unimplemented."
    },
    "possession-fx-observe-v349.mp4": {
      "video": "possession-fx-observe-v349.mp4",
      "character": "corvo",
      "label": "Possession · native effect ownership v349 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-fx-observe-v349/result.json",
      "visual_review": "possession-fx-observe-v349/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 23.414634146341463,
      "video_sha256": "774F377C0840698D9CF8BC442E5CCACAC2639DDC3DE6460D66642F7138E9A6AE",
      "checks_passed": true,
      "scope": "Source shows green target particles during native cast, occupied vision and normal equipment recovery. Peer shows Corvo casting, disappearing and returning, but a native guard obscures and overlaps the return; late camera reframe primarily shows a guard. No remote target particles are implemented. No audio captured. This is ownership research and body lifecycle evidence, not finished NPC/world presentation."
    },
    "possession-fx-observe-v349-source.mp4": {
      "video": "possession-fx-observe-v349-source.mp4",
      "character": "corvo",
      "label": "Possession · native effect ownership v349 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-fx-observe-v349/result.json",
      "visual_review": "possession-fx-observe-v349/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 24.98480243161094,
      "video_sha256": "544E6C53E9B6B57DC63F8E55F00D2349846BB7C0768F438E060E59627F74A322",
      "checks_passed": true,
      "scope": "Source shows green target particles during native cast, occupied vision and normal equipment recovery. Peer shows Corvo casting, disappearing and returning, but a native guard obscures and overlaps the return; late camera reframe primarily shows a guard. No remote target particles are implemented. No audio captured. This is ownership research and body lifecycle evidence, not finished NPC/world presentation."
    },
    "possession-scoped-fx-v350.mp4": {
      "video": "possession-scoped-fx-v350.mp4",
      "character": "corvo",
      "label": "Possession · Scoped target-effect ownership · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-scoped-fx-v350/result.json",
      "visual_review": "possession-scoped-fx-v350/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 22.066869300911854,
      "video_sha256": "116192329E00D87C9FE373D7A16734A2506A3EB58CA66B1E62BD39A978FB05B6",
      "checks_passed": true,
      "scope": "Source retains native occupied vision and equipment recovery. Peer Corvo casts, disappears and returns, but the native guard occludes/overlaps the return. These recordings verify scoped ownership and body lifecycle, not acceptable complete NPC presentation. DLL343 contains no remote target particles. No audio captured."
    },
    "possession-scoped-fx-v350-source.mp4": {
      "video": "possession-scoped-fx-v350-source.mp4",
      "character": "corvo",
      "label": "Possession · Scoped target-effect ownership · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-scoped-fx-v350/result.json",
      "visual_review": "possession-scoped-fx-v350/visual-review.json",
      "proxy_sha256": "7AF4A9718092E25F17F8078882B175B52E50BACB8831D334DB4F5891258E002C",
      "actual_capture_hz": 24.90909090909091,
      "video_sha256": "4AC30E6F1BB719C3FF2C6A590B498DFA18CF28937F5DEB003A36D25FC08ABD17",
      "checks_passed": true,
      "scope": "Source retains native occupied vision and equipment recovery. Peer Corvo casts, disappears and returns, but the native guard occludes/overlaps the return. These recordings verify scoped ownership and body lifecycle, not acceptable complete NPC presentation. DLL343 contains no remote target particles. No audio captured."
    },
    "possession-target-fx-network-v351.mp4": {
      "video": "possession-target-fx-network-v351.mp4",
      "character": "corvo",
      "label": "Possession · 351 target particles · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-network-v351/result.json",
      "visual_review": "possession-target-fx-network-v351/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 24.290030211480364,
      "video_sha256": "F051EA298BB45AA76B307D36CCB3375AA046D701BE3735FADDDEFA8BB4326B3A",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Native source target has bright white/green head particles during aim, occupied vision and native exit. Peer Corvo casts, disappears and returns; target is outside the initial body-centered view. Ten received snapshots and one native FX start/stop establish lifecycle, not particle appearance or target alignment. No audio."
    },
    "possession-target-fx-network-v351-source.mp4": {
      "video": "possession-target-fx-network-v351-source.mp4",
      "character": "corvo",
      "label": "Possession · 351 target particles · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-network-v351/result.json",
      "visual_review": "possession-target-fx-network-v351/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 25.0,
      "video_sha256": "D53898F43303134D2BA6480F395D2E6D73D41C9342F14C076AEDB3AB798B4D87",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Native source target has bright white/green head particles during aim, occupied vision and native exit. Peer Corvo casts, disappears and returns; target is outside the initial body-centered view. Ten received snapshots and one native FX start/stop establish lifecycle, not particle appearance or target alignment. No audio."
    },
    "possession-target-fx-view-v352.mp4": {
      "video": "possession-target-fx-view-v352.mp4",
      "character": "corvo",
      "label": "Possession · 352 target particles · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-view-v352/result.json",
      "visual_review": "possession-target-fx-view-v352/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 24.437689969604865,
      "video_sha256": "2C218067363D74821CFADCF304BF09792AA61698A85B1A70A3159046435BBAD6",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Native source bright target particles and exit remain visible. Peer initial camera faces across the hall and misses the source target; fallback observer floor was beside the target and used atan2(0,0) before collision correction. Late view shows native guard. Numeric particle lifecycle passes but particle appearance/alignment is not demonstrated.353 excludes close target placement and recalculates yaw from actual floor coordinates. No audio."
    },
    "possession-target-fx-view-v352-source.mp4": {
      "video": "possession-target-fx-view-v352-source.mp4",
      "character": "corvo",
      "label": "Possession · 352 target particles · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-view-v352/result.json",
      "visual_review": "possession-target-fx-view-v352/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 26.0,
      "video_sha256": "F16A89863A23B8BB0A7D5CD33A639BFAD9953C18A2D3549431936C081E38835E",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Native source bright target particles and exit remain visible. Peer initial camera faces across the hall and misses the source target; fallback observer floor was beside the target and used atan2(0,0) before collision correction. Late view shows native guard. Numeric particle lifecycle passes but particle appearance/alignment is not demonstrated.353 excludes close target placement and recalculates yaw from actual floor coordinates. No audio."
    },
    "possession-target-fx-view-v354.mp4": {
      "video": "possession-target-fx-view-v354.mp4",
      "character": "corvo",
      "label": "Possession · 354 target particles · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-view-v354/result.json",
      "visual_review": "possession-target-fx-view-v354/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 20.060790273556233,
      "video_sha256": "4E780A15F60BFB311592D80A9021E55ABCADBB481FF5BB6D093161664722F5C5",
      "checks_passed": true,
      "scope": "Both source/peer sheets viewed. Source target glow and native cast/occupation/exit work; peer target region is obscured by nearby Corvo and an independently simulated guard.18 runtime checks pass but remote glow is not clearly demonstrated by these angles. No audio."
    },
    "possession-target-fx-view-v354-source.mp4": {
      "video": "possession-target-fx-view-v354-source.mp4",
      "character": "corvo",
      "label": "Possession · 354 target particles · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-view-v354/result.json",
      "visual_review": "possession-target-fx-view-v354/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 23.545454545454547,
      "video_sha256": "A58DF5F78321100E95B76764E880EEB573D383B14090AB490D0D397EAB0E398A",
      "checks_passed": true,
      "scope": "Both source/peer sheets viewed. Source target glow and native cast/occupation/exit work; peer target region is obscured by nearby Corvo and an independently simulated guard.18 runtime checks pass but remote glow is not clearly demonstrated by these angles. No audio."
    },
    "possession-target-fx-side-v355.mp4": {
      "video": "possession-target-fx-side-v355.mp4",
      "character": "corvo",
      "label": "Possession · 355 target particles · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-side-v355/result.json",
      "visual_review": "possession-target-fx-side-v355/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 19.09090909090909,
      "video_sha256": "8902C161D6B1A2AC04DB831388C5A044038D1F0FB1767DE60B5BA25F25767061",
      "checks_passed": true,
      "scope": "Both temporal sheets viewed. Peer side camera is blocked by a bookshelf pillar until the late reframe; source native target halo/streaks, entry and exit are visible.19 runtime checks include19 independent peer-native FX records with the exact streamed effect ID and Corvo clone owner. Particle appearance remains unproven in this take. No audio."
    },
    "possession-target-fx-side-v355-source.mp4": {
      "video": "possession-target-fx-side-v355-source.mp4",
      "character": "corvo",
      "label": "Possession · 355 target particles · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-side-v355/result.json",
      "visual_review": "possession-target-fx-side-v355/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 23.65558912386707,
      "video_sha256": "2C790E802D2772B358DDE001E1AD73F5F4F0D7D244BD0FCBEF6CF5794063B769",
      "checks_passed": true,
      "scope": "Both temporal sheets viewed. Peer side camera is blocked by a bookshelf pillar until the late reframe; source native target halo/streaks, entry and exit are visible.19 runtime checks include19 independent peer-native FX records with the exact streamed effect ID and Corvo clone owner. Particle appearance remains unproven in this take. No audio."
    },
    "possession-target-fx-cancel-v358.mp4": {
      "video": "possession-target-fx-cancel-v358.mp4",
      "character": "corvo",
      "label": "Possession · 358 target particles · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-cancel-v358/result.json",
      "visual_review": "possession-target-fx-cancel-v358/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 24.666666666666668,
      "video_sha256": "137CBDE4FF2A9BF6DE16BB62E39FB7061766397535A2F28ADAA7C1E15B312ACC",
      "checks_passed": true,
      "scope": "Both temporal sheets viewed at0.7,1,1.3,1.7,2.1,3 seconds. Peer native target halo and streaks appear at the guard head, then clear after native source cancellation. Source first-person aim/cancel remains native. This accepts the scoped stationary target-particle appearance/retirement only; independently simulated NPC alignment across movement and full Possession gameplay remain unfinished. No audio."
    },
    "possession-target-fx-cancel-v358-source.mp4": {
      "video": "possession-target-fx-cancel-v358-source.mp4",
      "character": "corvo",
      "label": "Possession · 358 target particles · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-cancel-v358/result.json",
      "visual_review": "possession-target-fx-cancel-v358/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 24.15041782729805,
      "video_sha256": "71C9B520EC7288DD2F3107EF78083E2050C4B14FC9327B42392445E35324D751",
      "checks_passed": true,
      "scope": "Both temporal sheets viewed at0.7,1,1.3,1.7,2.1,3 seconds. Peer native target halo and streaks appear at the guard head, then clear after native source cancellation. Source first-person aim/cancel remains native. This accepts the scoped stationary target-particle appearance/retirement only; independently simulated NPC alignment across movement and full Possession gameplay remain unfinished. No audio."
    },
    "possession-target-fx-receiver-stop-v359.mp4": {
      "video": "possession-target-fx-receiver-stop-v359.mp4",
      "character": "corvo",
      "label": "Possession · receiver-stop-v359 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-receiver-stop-v359/result.json",
      "visual_review": "possession-target-fx-receiver-stop-v359/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 25.92797783933518,
      "video_sha256": "53D5C4D2A7A341D2C4D9C8975870EF25D739A6D4A29CEBEDA41258B3D4105F37",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Peer halo is visible near1s and absent after receiver stream stop, while the source retains its native aim/halo. Four late packets are rejected after disarm. No implicit rearm. This verifies cosmetic retirement, not shared NPC simulation. No audio."
    },
    "possession-target-fx-receiver-stop-v359-source.mp4": {
      "video": "possession-target-fx-receiver-stop-v359-source.mp4",
      "character": "corvo",
      "label": "Possession · receiver-stop-v359 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-receiver-stop-v359/result.json",
      "visual_review": "possession-target-fx-receiver-stop-v359/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 25.738161559888578,
      "video_sha256": "135ACC5D8782EFCD443BB3979C6A44551CD66B7241B90EA7C6E89AC54C109B25",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Peer halo is visible near1s and absent after receiver stream stop, while the source retains its native aim/halo. Four late packets are rejected after disarm. No implicit rearm. This verifies cosmetic retirement, not shared NPC simulation. No audio."
    },
    "possession-target-fx-source-pause-v360.mp4": {
      "video": "possession-target-fx-source-pause-v360.mp4",
      "character": "corvo",
      "label": "Possession · source-pause-v360 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-source-pause-v360/result.json",
      "visual_review": "possession-target-fx-source-pause-v360/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 26.25,
      "video_sha256": "92DBD3E0CE08D22298DF19D8A8FFBF9E6ED9BFAFA0F989F5BB0E527F0CF0CB83",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Source native pause menu appears, peer halo retires, stream remains disarmed. The peer halo has no aligned guard in this take: native NPC positions are still independent. Cleanup incorrectly sent a new X+LT cast after the menu had already cancelled aim;360 waits for native host entry before normal exit and restores the checkpoint.362 removes that unnecessary input. No audio."
    },
    "possession-target-fx-source-pause-v360-source.mp4": {
      "video": "possession-target-fx-source-pause-v360-source.mp4",
      "character": "corvo",
      "label": "Possession · source-pause-v360 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-source-pause-v360/result.json",
      "visual_review": "possession-target-fx-source-pause-v360/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 25.75,
      "video_sha256": "EE65913DE2B97FC89DEE03AE4718A2E6D7B155595AAE075DD6D68830678D9F35",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Source native pause menu appears, peer halo retires, stream remains disarmed. The peer halo has no aligned guard in this take: native NPC positions are still independent. Cleanup incorrectly sent a new X+LT cast after the menu had already cancelled aim;360 waits for native host entry before normal exit and restores the checkpoint.362 removes that unnecessary input. No audio."
    },
    "possession-target-fx-receiver-pause-v360.mp4": {
      "video": "possession-target-fx-receiver-pause-v360.mp4",
      "character": "corvo",
      "label": "Possession · receiver-pause-v360 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-receiver-pause-v360/result.json",
      "visual_review": "possession-target-fx-receiver-pause-v360/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 25.429362880886426,
      "video_sha256": "57CC590BB19F13C664D0F2DC4D1FFE66795E3EFB42EF5638DA5195FB5FFBB4F2",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Emily native pause menu is visible around1.7/2.2s; peer halo is absent after resume while source native aim continues until cancellation. This verifies retirement across native pause/resume, not shared NPC outcomes or complete power presentation. No audio."
    },
    "possession-target-fx-receiver-pause-v360-source.mp4": {
      "video": "possession-target-fx-receiver-pause-v360-source.mp4",
      "character": "corvo",
      "label": "Possession · receiver-pause-v360 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-receiver-pause-v360/result.json",
      "visual_review": "possession-target-fx-receiver-pause-v360/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 26.833333333333332,
      "video_sha256": "A1BE7933372EBDB1BE17A9B3E472AFDAE44AC878767AF7ACE5D57304391DC0D6",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets. Emily native pause menu is visible around1.7/2.2s; peer halo is absent after resume while source native aim continues until cancellation. This verifies retirement across native pause/resume, not shared NPC outcomes or complete power presentation. No audio."
    },
    "possession-target-fx-source-pause-v362.mp4": {
      "video": "possession-target-fx-source-pause-v362.mp4",
      "character": "corvo",
      "label": "Possession · source-pause-v362 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-source-pause-v362/result.json",
      "visual_review": "possession-target-fx-source-pause-v362/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 26.5,
      "video_sha256": "1138071B516AE9C5052CAF50F15ADDD70A71428E71DD290B0A0AC9140D8E67BE",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets at0.7/1.1/1.7/2.2/3/5s. Peer halo/streaks appear over the guard and clear after Corvo opens the native pause menu. Source returns to normal native view without the cleanup-induced fresh cast of360. Exact before-cleanup state is default Running0 with no UsePower63, so cleanup sends neutral only. No implicit stream rearm. Bookshelf pillar obscures some body details; no audio."
    },
    "possession-target-fx-source-pause-v362-source.mp4": {
      "video": "possession-target-fx-source-pause-v362-source.mp4",
      "character": "corvo",
      "label": "Possession · source-pause-v362 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-target-fx-source-pause-v362/result.json",
      "visual_review": "possession-target-fx-source-pause-v362/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 26.333333333333332,
      "video_sha256": "539BDC6A91B6CF9A62D78A6138D2B05A74D62780EBCCCFF3114D06A062AF0241",
      "checks_passed": true,
      "scope": "Viewed both temporal sheets at0.7/1.1/1.7/2.2/3/5s. Peer halo/streaks appear over the guard and clear after Corvo opens the native pause menu. Source returns to normal native view without the cleanup-induced fresh cast of360. Exact before-cleanup state is default Running0 with no UsePower63, so cleanup sends neutral only. No implicit stream rearm. Bookshelf pillar obscures some body details; no audio."
    },
    "possession-crouch-observe-v361.mp4": {
      "video": "possession-crouch-observe-v361.mp4",
      "character": "corvo",
      "label": "Possession native context · crouched armed ·361",
      "alias": "native_possession_context",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-observe-v361/result.json",
      "visual_review": "possession-crouch-observe-v361/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 26.170212765957448,
      "video_sha256": "DDC1DCC99EDB82EBA29110B6B3D299600F283E7AFEE95D26D88D3129095026F8",
      "checks_passed": true,
      "scope": "Source retains lowered native camera/sword combat grip while left arm enters and holds Possession aim. Sneaking6+UsePower63 owns333ms entry/200ms aim. Old standing-only reader accepts0 of390; this is missing context coverage, not absent native animation. Source-only native evidence. No new remote presentation, NPC control or sound validation."
    },
    "possession-holstered-observe-v361.mp4": {
      "video": "possession-holstered-observe-v361.mp4",
      "character": "corvo",
      "label": "Possession native context · standing initially holstered ·361",
      "alias": "native_possession_context",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-observe-v361/result.json",
      "visual_review": "possession-holstered-observe-v361/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 25.987841945288753,
      "video_sha256": "4653BA761D52F3F2F0A5683831D9F2412A8152E0257E8FBE9C7C4D7133C7C41D",
      "checks_passed": true,
      "scope": "Source starts empty-handed, draws native sword while left arm begins Possession, then retains sword. Raw Running0+UsePower63+EquipChange53 overlaps sword_equip Scene_2/1066ms with the left-arm cast. Old reader accepts33 later aim samples and rejects the initial three-state overlap. Source-only native evidence. No new remote presentation, NPC control or sound validation."
    },
    "possession-crouch-holstered-observe-v361.mp4": {
      "video": "possession-crouch-holstered-observe-v361.mp4",
      "character": "corvo",
      "label": "Possession native context · crouched initially holstered ·361",
      "alias": "native_possession_context",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-holstered-observe-v361/result.json",
      "visual_review": "possession-crouch-holstered-observe-v361/visual-review.json",
      "proxy_sha256": "1DECA11B323146BA2A5403E0CE71CB641A104E789071253B606796B06640A2C7",
      "actual_capture_hz": 26.35258358662614,
      "video_sha256": "58F26D18BBD9D357FDD86AA670C2E1D63A07BDAE80A86BA4FBE03F05745D5E22",
      "checks_passed": true,
      "scope": "Source begins with lowered empty-hand view, casts with left hand and draws/turns sword into native crouch grip. Raw Sneaking6+UsePower63+EquipChange53 overlaps sword_equip_sneak Scene_2/1133ms with the left-arm cast. Old standing-only reader accepts0 of378. Source-only native evidence. No new remote presentation, NPC control or sound validation."
    },
    "possession-crouch-network-v363.mp4": {
      "video": "possession-crouch-network-v363.mp4",
      "character": "corvo",
      "label": "Possession · crouched entry/aim363 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-network-v363/result.json",
      "visual_review": "possession-crouch-network-v363/visual-review.json",
      "proxy_sha256": "7B781EA3E113D2216C4217FCA95EA932D7D148F9872B729D431AE8C46FA8CF43",
      "actual_capture_hz": 24.237995824634655,
      "video_sha256": "E4FD2A9A35A070AA2078C94786549F0212CFCE2E4234B23C8183EBD656746872",
      "checks_passed": true,
      "scope": "Peer Corvo remains crouched, shifts the whole body and raises the left casting arm while keeping the crouched sword guard, then returns to crouched idle. Source retains native camera, hand and sword animation. Both phases travel with native scene clocks; dark lighting and small character size limit fine grip/anatomy judgment. This is stationary empty-target coverage, not crouched human transfer, overlapping sword draw or complete power acceptance. No audio."
    },
    "possession-crouch-network-v363-source.mp4": {
      "video": "possession-crouch-network-v363-source.mp4",
      "character": "corvo",
      "label": "Possession · crouched entry/aim363 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-network-v363/result.json",
      "visual_review": "possession-crouch-network-v363/visual-review.json",
      "proxy_sha256": "7B781EA3E113D2216C4217FCA95EA932D7D148F9872B729D431AE8C46FA8CF43",
      "actual_capture_hz": 25.6875,
      "video_sha256": "135D20CE4F579E9B1FF07C4187292D37D1B8292311A468BB0770A3EBFE7F8229",
      "checks_passed": true,
      "scope": "Peer Corvo remains crouched, shifts the whole body and raises the left casting arm while keeping the crouched sword guard, then returns to crouched idle. Source retains native camera, hand and sword animation. Both phases travel with native scene clocks; dark lighting and small character size limit fine grip/anatomy judgment. This is stationary empty-target coverage, not crouched human transfer, overlapping sword draw or complete power acceptance. No audio."
    },
    "possession-holstered-network-v364.mp4": {
      "video": "possession-holstered-network-v364.mp4",
      "character": "corvo",
      "label": "Possession · FAILED initially holstered overlap364 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v364/result.json",
      "visual_review": "possession-holstered-network-v364/visual-review.json",
      "proxy_sha256": "878C0603E288884A4D581693D5BA048FFD2562A7E84C5A050A4DB3D2D9AF294C",
      "actual_capture_hz": 24.08713692946058,
      "video_sha256": "EA1F66C4F3383B737B91883618E025DA9C286A58F7127B9C632C8EDE64AA49A8",
      "checks_passed": false,
      "scope": "Source visibly draws sword while casting with left hand. Peer goes from empty-handed to a held sword and casting pose, without the required draw transition. Runtime correctly fails:21 independently owned native draw samples but0 overlapping peer draw samples. Source motionValid is false for Ground+UsePower63+EquipChange53; equipment playback requires valid root motion.366 adds the exact observed combination to the physical-motion classifier, leaving scene authority separate. Full checkpoint inventories/health and protected14 restore. No audio."
    },
    "possession-holstered-network-v364-source.mp4": {
      "video": "possession-holstered-network-v364-source.mp4",
      "character": "corvo",
      "label": "Possession · FAILED initially holstered overlap364 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v364/result.json",
      "visual_review": "possession-holstered-network-v364/visual-review.json",
      "proxy_sha256": "878C0603E288884A4D581693D5BA048FFD2562A7E84C5A050A4DB3D2D9AF294C",
      "actual_capture_hz": 26.452282157676347,
      "video_sha256": "D53D8265C0BD811FB3AE333F32828D1259929A1D2BB50916B8056C90696593D1",
      "checks_passed": false,
      "scope": "Source visibly draws sword while casting with left hand. Peer goes from empty-handed to a held sword and casting pose, without the required draw transition. Runtime correctly fails:21 independently owned native draw samples but0 overlapping peer draw samples. Source motionValid is false for Ground+UsePower63+EquipChange53; equipment playback requires valid root motion.366 adds the exact observed combination to the physical-motion classifier, leaving scene authority separate. Full checkpoint inventories/health and protected14 restore. No audio."
    },
    "possession-holstered-network-v366.mp4": {
      "video": "possession-holstered-network-v366.mp4",
      "character": "corvo",
      "label": "Possession · FAILED draw startup continuity366 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v366/result.json",
      "visual_review": "possession-holstered-network-v366/visual-review.json",
      "proxy_sha256": "FE411CE4FF7BEB1445EE4D6AF0FFCE23B8DC99FC19C11895CDB24D288B1E8AF4",
      "actual_capture_hz": 24.3125,
      "video_sha256": "C60BD8AD11E3F029AF2912CFA7EE6CCABFFFA5C9541E876111719DCC11DC483D",
      "checks_passed": false,
      "scope": "Peer now performs sword draw while casting and returns to armed idle. However the body briefly falls back to native pose at cast startup: motionValid is false for native Running0+Draw65+EquipChange53 before UsePower63 begins. Runtime fails continuity despite21 native and11 peer draw/cast overlap samples and correct sword contact transition. No clock rejections.367 adds the exact preparatory request combination and independently owned equipment scene. Dark medium-distance sheets do not establish grip or anatomy acceptance. Both checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-holstered-network-v366-source.mp4": {
      "video": "possession-holstered-network-v366-source.mp4",
      "character": "corvo",
      "label": "Possession · FAILED draw startup continuity366 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v366/result.json",
      "visual_review": "possession-holstered-network-v366/visual-review.json",
      "proxy_sha256": "FE411CE4FF7BEB1445EE4D6AF0FFCE23B8DC99FC19C11895CDB24D288B1E8AF4",
      "actual_capture_hz": 26.0625,
      "video_sha256": "06FF37861B858616593F919D8126A970152F00E7758EEAC9E0C18A10AA2E23FF",
      "checks_passed": false,
      "scope": "Peer now performs sword draw while casting and returns to armed idle. However the body briefly falls back to native pose at cast startup: motionValid is false for native Running0+Draw65+EquipChange53 before UsePower63 begins. Runtime fails continuity despite21 native and11 peer draw/cast overlap samples and correct sword contact transition. No clock rejections.367 adds the exact preparatory request combination and independently owned equipment scene. Dark medium-distance sheets do not establish grip or anatomy acceptance. Both checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-holstered-network-v367.mp4": {
      "video": "possession-holstered-network-v367.mp4",
      "character": "corvo",
      "label": "Possession · FAILED transient draw context367 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v367/result.json",
      "visual_review": "possession-holstered-network-v367/visual-review.json",
      "proxy_sha256": "8BAF74E4687DAACDE454EFF4DBF25DC004C8097F6EB8161418378F97525ADD13",
      "actual_capture_hz": 23.875,
      "video_sha256": "D4CC6F44C25620677500B05D91AE63E92B982FB0A97139CB856D8F902E2C6CF0",
      "checks_passed": false,
      "scope": "Native source draws sword and casts. Peer also draws/casts, but a brief startup physical refusal still drops body weight.21 native and12 peer overlapping draw samples prove contact and cast channels; one receiver eligibility refusal has invalid physical motion, no clock rejection. Slower external observations miss the offending native combination.368 adds a bounded32-record in-engine refusal diagnostic without relaxing physical or semantic validation. Dark medium-distance footage limits fine hand/anatomy judgment. Checkpoint health/full inventories and protected14 restore. No audio."
    },
    "possession-holstered-network-v367-source.mp4": {
      "video": "possession-holstered-network-v367-source.mp4",
      "character": "corvo",
      "label": "Possession · FAILED transient draw context367 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v367/result.json",
      "visual_review": "possession-holstered-network-v367/visual-review.json",
      "proxy_sha256": "8BAF74E4687DAACDE454EFF4DBF25DC004C8097F6EB8161418378F97525ADD13",
      "actual_capture_hz": 26.25,
      "video_sha256": "F8E7B961EBEAE186FCA645793F97181929226A28030C0D700601EBDF7E8B2979",
      "checks_passed": false,
      "scope": "Native source draws sword and casts. Peer also draws/casts, but a brief startup physical refusal still drops body weight.21 native and12 peer overlapping draw samples prove contact and cast channels; one receiver eligibility refusal has invalid physical motion, no clock rejection. Slower external observations miss the offending native combination.368 adds a bounded32-record in-engine refusal diagnostic without relaxing physical or semantic validation. Dark medium-distance footage limits fine hand/anatomy judgment. Checkpoint health/full inventories and protected14 restore. No audio."
    },
    "possession-holstered-network-v368.mp4": {
      "video": "possession-holstered-network-v368.mp4",
      "character": "corvo",
      "label": "Possession · FAILED external final-loop coverage368 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v368/result.json",
      "visual_review": "possession-holstered-network-v368/visual-review.json",
      "proxy_sha256": "13CB5C21F73E1AA14D8B7EEB9A11EBDC5FDDFAF920E9B3B974D030832448C963",
      "actual_capture_hz": 23.875,
      "video_sha256": "039B0CFE4465EE078D9199EC80AF67BFF4A37D41695047273BB3DD97A7A74A27",
      "checks_passed": false,
      "scope": "Standing body/draw/cast is continuous in all79 measured samples, with no source motion refusals or receiver clock rejections. Native and peer draw overlap/contact pass. Test fails because the external watcher misses final loop scene238, which is present in both sent and accepted histories. No claim that external observation saw it. Paired sheets show draw, cast, recovery; no body fallback apparent in sampled frames. Native local first-person remains unchanged. Dark medium-distance shots do not establish fine grip/anatomy acceptance. Both checkpoint full inventories/health and protected14 restored. No audio."
    },
    "possession-holstered-network-v368-source.mp4": {
      "video": "possession-holstered-network-v368-source.mp4",
      "character": "corvo",
      "label": "Possession · FAILED external final-loop coverage368 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v368/result.json",
      "visual_review": "possession-holstered-network-v368/visual-review.json",
      "proxy_sha256": "13CB5C21F73E1AA14D8B7EEB9A11EBDC5FDDFAF920E9B3B974D030832448C963",
      "actual_capture_hz": 25.6875,
      "video_sha256": "7F4FE4CA5F4500770A3712CBC8D3BFC6EDE5A8787316323A60634E2AA68C0704",
      "checks_passed": false,
      "scope": "Standing body/draw/cast is continuous in all79 measured samples, with no source motion refusals or receiver clock rejections. Native and peer draw overlap/contact pass. Test fails because the external watcher misses final loop scene238, which is present in both sent and accepted histories. No claim that external observation saw it. Paired sheets show draw, cast, recovery; no body fallback apparent in sampled frames. Native local first-person remains unchanged. Dark medium-distance shots do not establish fine grip/anatomy acceptance. Both checkpoint full inventories/health and protected14 restored. No audio."
    },
    "possession-holstered-network-v369.mp4": {
      "video": "possession-holstered-network-v369.mp4",
      "character": "corvo",
      "label": "Possession · standing draw/cast369 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v369/result.json",
      "visual_review": "possession-holstered-network-v369/visual-review.json",
      "proxy_sha256": "13CB5C21F73E1AA14D8B7EEB9A11EBDC5FDDFAF920E9B3B974D030832448C963",
      "actual_capture_hz": 21.01871101871102,
      "video_sha256": "03C1FE514365A9196803F1CA308E3D3098EA46A183B8E6AAF956CFF5DB18C6D8",
      "checks_passed": true,
      "scope": "Standing complete-body draw/cast and armed idle recovery visible.13 checks pass;70 continuity samples have weight1 and no rejected clocks; no source-thread motion refusal. Every independent scene matches source and peer histories, with no source-only tail this take. Native local first-person remains unchanged. Dark medium-distance shots do not establish fine grip/anatomy acceptance. Both checkpoint full inventories/health and protected14 restored. No audio."
    },
    "possession-holstered-network-v369-source.mp4": {
      "video": "possession-holstered-network-v369-source.mp4",
      "character": "corvo",
      "label": "Possession · standing draw/cast369 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-holstered-network-v369/result.json",
      "visual_review": "possession-holstered-network-v369/visual-review.json",
      "proxy_sha256": "13CB5C21F73E1AA14D8B7EEB9A11EBDC5FDDFAF920E9B3B974D030832448C963",
      "actual_capture_hz": 23.13929313929314,
      "video_sha256": "909B0A2A18F6EA27E05387A6DCBFE8F674880D52CABAFE152C2C985FD2D710A6",
      "checks_passed": true,
      "scope": "Standing complete-body draw/cast and armed idle recovery visible.13 checks pass;70 continuity samples have weight1 and no rejected clocks; no source-thread motion refusal. Every independent scene matches source and peer histories, with no source-only tail this take. Native local first-person remains unchanged. Dark medium-distance shots do not establish fine grip/anatomy acceptance. Both checkpoint full inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-holstered-network-v369.mp4": {
      "video": "possession-crouch-holstered-network-v369.mp4",
      "character": "corvo",
      "label": "Possession · crouched draw/cast369 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-holstered-network-v369/result.json",
      "visual_review": "possession-crouch-holstered-network-v369/visual-review.json",
      "proxy_sha256": "13CB5C21F73E1AA14D8B7EEB9A11EBDC5FDDFAF920E9B3B974D030832448C963",
      "actual_capture_hz": 23.84937238493724,
      "video_sha256": "46285EE0CF1E739A5770012EDDBA9D557CDA773AE3089326F0208A9F805AD6F9",
      "checks_passed": true,
      "scope": "Peer stays crouched, draws the sword while left hand casts, and returns to crouched armed idle.13 checks pass;80 continuity samples have weight1 and no rejected clocks.34 native and18 peer overlapping draw samples cross contact timing. Source-thread diagnostic catches one four-state Sneaking6+Draw65+EquipChange53+UsePower63 refusal which did not appear at receiver cadence;370 addresses this exact observed combination. A nearby ordinary guard is visible only in the source view; no transfer occurs and mana stays unchanged. Native local first-person remains unchanged. Dark medium-distance shots do not establish fine grip/anatomy acceptance. Both checkpoint full inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-holstered-network-v369-source.mp4": {
      "video": "possession-crouch-holstered-network-v369-source.mp4",
      "character": "corvo",
      "label": "Possession · crouched draw/cast369 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-holstered-network-v369/result.json",
      "visual_review": "possession-crouch-holstered-network-v369/visual-review.json",
      "proxy_sha256": "13CB5C21F73E1AA14D8B7EEB9A11EBDC5FDDFAF920E9B3B974D030832448C963",
      "actual_capture_hz": 24.5625,
      "video_sha256": "DF0FFC6CA2429B79D52EE4D6E7CEFF8C54D0ED2328693C51F73EBC080D4B7222",
      "checks_passed": true,
      "scope": "Peer stays crouched, draws the sword while left hand casts, and returns to crouched armed idle.13 checks pass;80 continuity samples have weight1 and no rejected clocks.34 native and18 peer overlapping draw samples cross contact timing. Source-thread diagnostic catches one four-state Sneaking6+Draw65+EquipChange53+UsePower63 refusal which did not appear at receiver cadence;370 addresses this exact observed combination. A nearby ordinary guard is visible only in the source view; no transfer occurs and mana stays unchanged. Native local first-person remains unchanged. Dark medium-distance shots do not establish fine grip/anatomy acceptance. Both checkpoint full inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-holstered-network-v370.mp4": {
      "video": "possession-crouch-holstered-network-v370.mp4",
      "character": "corvo",
      "label": "Possession · crouched draw/cast370 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-holstered-network-v370/result.json",
      "visual_review": "possession-crouch-holstered-network-v370/visual-review.json",
      "proxy_sha256": "25067A86D573A1A6987415EF23AF5FBA854E60BBEEDBDBFB4FF889606EC59D5F",
      "actual_capture_hz": 24.288702928870293,
      "video_sha256": "9D0454A9313F2C89AA2C972EC5740FE469C4CD7AB08D997C3955DF7577F03FC5",
      "checks_passed": true,
      "scope": "Viewed paired temporal game sheets at1/2.1/2.3/2.6/3/4.5s. Corvo remains crouched, extends the left casting arm while the right arm draws the sword, and returns to armed crouch. Body weight1 in79 samples, no receiver clock rejections and no source-thread motion refusals. Native first-person remains unchanged. The remote body is dark and medium-distance; fine grip/anatomy acceptance remains open. Both complete checkpoint inventories/health and protected14 hashes restored. No audio."
    },
    "possession-crouch-holstered-network-v370-source.mp4": {
      "video": "possession-crouch-holstered-network-v370-source.mp4",
      "character": "corvo",
      "label": "Possession · crouched draw/cast370 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-holstered-network-v370/result.json",
      "visual_review": "possession-crouch-holstered-network-v370/visual-review.json",
      "proxy_sha256": "25067A86D573A1A6987415EF23AF5FBA854E60BBEEDBDBFB4FF889606EC59D5F",
      "actual_capture_hz": 25.678496868475992,
      "video_sha256": "6178A31A7F5C4A4EFE496DB2061AD9A05155651B8C4A9DA94C676D338A558CE3",
      "checks_passed": true,
      "scope": "Viewed paired temporal game sheets at1/2.1/2.3/2.6/3/4.5s. Corvo remains crouched, extends the left casting arm while the right arm draws the sword, and returns to armed crouch. Body weight1 in79 samples, no receiver clock rejections and no source-thread motion refusals. Native first-person remains unchanged. The remote body is dark and medium-distance; fine grip/anatomy acceptance remains open. Both complete checkpoint inventories/health and protected14 hashes restored. No audio."
    },
    "possession-crouch-target-network-v370.mp4": {
      "video": "possession-crouch-target-network-v370.mp4",
      "character": "corvo",
      "label": "Possession · FAILED crouched transfer370 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-target-network-v370/result.json",
      "visual_review": "possession-crouch-target-network-v370/visual-review.json",
      "proxy_sha256": "25067A86D573A1A6987415EF23AF5FBA854E60BBEEDBDBFB4FF889606EC59D5F",
      "actual_capture_hz": 25.636363636363637,
      "video_sha256": "1DE726F69FAE2A1C5509423F9B67F9FBF2976E92BB9EE3BD143B591C657A0393",
      "checks_passed": false,
      "scope": "FAILED: standing-only harness never reframes after actual crouched native exit. Source view shows crouched casting, native human transfer/occupation and return to crouch. Peer target halo/streaks appear then retire; caster lies outside usable frame, so no body-release visual acceptance. Raw clocks and strict reader demonstrate an actual standing-only release/context gap as well. Both complete checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-target-network-v370-source.mp4": {
      "video": "possession-crouch-target-network-v370-source.mp4",
      "character": "corvo",
      "label": "Possession · FAILED crouched transfer370 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-target-network-v370/result.json",
      "visual_review": "possession-crouch-target-network-v370/visual-review.json",
      "proxy_sha256": "25067A86D573A1A6987415EF23AF5FBA854E60BBEEDBDBFB4FF889606EC59D5F",
      "actual_capture_hz": 27.181818181818183,
      "video_sha256": "622CD56C902DE181005D053E4D3D424AEFA439FBF84181DAF15AD9E6CBBECE42",
      "checks_passed": false,
      "scope": "FAILED: standing-only harness never reframes after actual crouched native exit. Source view shows crouched casting, native human transfer/occupation and return to crouch. Peer target halo/streaks appear then retire; caster lies outside usable frame, so no body-release visual acceptance. Raw clocks and strict reader demonstrate an actual standing-only release/context gap as well. Both complete checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-target-network-v371.mp4": {
      "video": "possession-crouch-target-network-v371.mp4",
      "character": "corvo",
      "label": "Possession · FAILED final stance assertion371 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-target-network-v371/result.json",
      "visual_review": "possession-crouch-target-network-v371/visual-review.json",
      "proxy_sha256": "1197F87B1109C474BE89E96C82C9C900DE537D04E5A4BE5CBE8337C8AD570550",
      "actual_capture_hz": 24.0,
      "video_sha256": "AC806B664D4F48CDE4385108137357244EDECE8072AFB5D2D73E8BD23356E9C1",
      "checks_passed": false,
      "scope": "FAILED final harness assertion still requires standing despite observed crouched recovery.17 checks pass before failure, including owned release, Steam presentation, visibility, particles and recovered walking. Source native transfer/exit and peer particles viewed; close caster poorly framed until exit camera reframe. No complete visual acceptance. Both complete checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-target-network-v371-source.mp4": {
      "video": "possession-crouch-target-network-v371-source.mp4",
      "character": "corvo",
      "label": "Possession · FAILED final stance assertion371 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-target-network-v371/result.json",
      "visual_review": "possession-crouch-target-network-v371/visual-review.json",
      "proxy_sha256": "1197F87B1109C474BE89E96C82C9C900DE537D04E5A4BE5CBE8337C8AD570550",
      "actual_capture_hz": 25.363636363636363,
      "video_sha256": "999C2B2AC6CE72EA27D4F08643B8CCEE4A4D82616A111CD2C5C4AFDFD83776A6",
      "checks_passed": false,
      "scope": "FAILED final harness assertion still requires standing despite observed crouched recovery.17 checks pass before failure, including owned release, Steam presentation, visibility, particles and recovered walking. Source native transfer/exit and peer particles viewed; close caster poorly framed until exit camera reframe. No complete visual acceptance. Both complete checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-target-network-v372.mp4": {
      "video": "possession-crouch-target-network-v372.mp4",
      "character": "corvo",
      "label": "Possession · crouched transfer372 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-target-network-v372/result.json",
      "visual_review": "possession-crouch-target-network-v372/visual-review.json",
      "proxy_sha256": "1197F87B1109C474BE89E96C82C9C900DE537D04E5A4BE5CBE8337C8AD570550",
      "actual_capture_hz": 24.072948328267476,
      "video_sha256": "1AB22A100DF59AAB3DCC171054068633AD73DC8CA425EFFBD2E568023CD531E3",
      "checks_passed": true,
      "scope": "PASS20. Paired native game views show crouched caster body/left-arm gesture, empty-hand release, disappearance, target halo/streaks, reappearance and resumed crouched movement.30 native and7 peer crouched cast samples; release stance1.2 native dual-owned exit observations,7 remote draw samples,4 walking samples and0.354737m source displacement. Close view crops the legs during release; no complete anatomy/fine grip acceptance. Native first-person unchanged; NPC control/outcomes still not replicated. Both complete checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-crouch-target-network-v372-source.mp4": {
      "video": "possession-crouch-target-network-v372-source.mp4",
      "character": "corvo",
      "label": "Possession · crouched transfer372 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-crouch-target-network-v372/result.json",
      "visual_review": "possession-crouch-target-network-v372/visual-review.json",
      "proxy_sha256": "1197F87B1109C474BE89E96C82C9C900DE537D04E5A4BE5CBE8337C8AD570550",
      "actual_capture_hz": 25.818181818181817,
      "video_sha256": "6BD546A2BC9A633CD10CA0FD672A6FA45939348EA454DDFEA6A1F7219F785C65",
      "checks_passed": true,
      "scope": "PASS20. Paired native game views show crouched caster body/left-arm gesture, empty-hand release, disappearance, target halo/streaks, reappearance and resumed crouched movement.30 native and7 peer crouched cast samples; release stance1.2 native dual-owned exit observations,7 remote draw samples,4 walking samples and0.354737m source displacement. Close view crops the legs during release; no complete anatomy/fine grip acceptance. Native first-person unchanged; NPC control/outcomes still not replicated. Both complete checkpoint inventories/health and protected14 restored. No audio."
    },
    "possession-partner-exclusion-v374.mp4": {
      "video": "possession-partner-exclusion-v374.mp4",
      "character": "corvo",
      "label": "Possession · partner excluded374 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-partner-exclusion-v374/result.json",
      "visual_review": "possession-partner-exclusion-v374/visual-review.json",
      "proxy_sha256": "AD06C1A322AD45C7A511A1D74AB1058156F87DC527041F36CB82C894D6500BF2",
      "actual_capture_hz": 25.272727272727273,
      "video_sha256": "481FBA2DAF7B79B8BD6A7652DC4D79EF1623A29E3975C5AC7E918F9D10919061",
      "checks_passed": true,
      "scope": "Native Corvo input aims at remote Emily then releases. Viewed source temporal sheet at1/2/3/4/5/8s: native hands animate, Emily remains standing without target halo or transfer.228 exact partner selections blocked,0 identity refusals,mana100 unchanged,health/inventory restored. This is source-only targeting exclusion evidence, not a complete movement or partner-exclusion claim."
    },
    "possession-partner-guard-control-v375.mp4": {
      "video": "possession-partner-guard-control-v375.mp4",
      "character": "corvo",
      "label": "Possession · guard control375 · peer",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-partner-guard-control-v375/result.json",
      "visual_review": "possession-partner-guard-control-v375/visual-review.json",
      "proxy_sha256": "AD06C1A322AD45C7A511A1D74AB1058156F87DC527041F36CB82C894D6500BF2",
      "actual_capture_hz": 24.0,
      "video_sha256": "B3EBECBBBC27E9EB855D94FD8372BC7832DD5B19CC637FA4068A7369B46AF457",
      "checks_passed": true,
      "scope": "Ordinary guard positive control on374 DLL passes20. Viewed paired sheets at2.9/3.5/3.8/4.4/7.6/8.4s: native transfer/occupied tint/exit intact, remote Corvo disappears and returns, target particles play. Stair post obscures most caster gesture; no full visual acceptance. Scoped native source NPC possession remains local; shared NPC authority unimplemented. Complete checkpoint inventory/health and14 protected hashes restore."
    },
    "possession-partner-guard-control-v375-source.mp4": {
      "video": "possession-partner-guard-control-v375-source.mp4",
      "character": "corvo",
      "label": "Possession · guard control375 · source",
      "alias": "native_possession_lifecycle",
      "requirements": [
        "corvo_possession_enter_loop_exit"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "possession-partner-guard-control-v375/result.json",
      "visual_review": "possession-partner-guard-control-v375/visual-review.json",
      "proxy_sha256": "AD06C1A322AD45C7A511A1D74AB1058156F87DC527041F36CB82C894D6500BF2",
      "actual_capture_hz": 26.181818181818183,
      "video_sha256": "A247D549AC93EC5338BB0D36FE7EDBEA3F313703C5BEB439C94DD3DA70524142",
      "checks_passed": true,
      "scope": "Ordinary guard positive control on374 DLL passes20. Viewed paired sheets at2.9/3.5/3.8/4.4/7.6/8.4s: native transfer/occupied tint/exit intact, remote Corvo disappears and returns, target particles play. Stair post obscures most caster gesture; no full visual acceptance. Scoped native source NPC possession remains local; shared NPC authority unimplemented. Complete checkpoint inventory/health and14 protected hashes restore."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1332f/result.json",
    "coverage-v1332d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1341-possession.json",
    "decision": "complete",
    "scope": "Corvo possesses a human guard and leaves: the partner presents the cast (power variants 26-28), hides Corvo's body while he occupies the guard (one hide), and shows it again where he leaves the host (one show, original visibility restored).",
    "takes": [
      {
        "take": "coverage-v1332f",
        "cases": [
          "host-possesshuman"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "host-possession"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1332f/host-possesshuman-observer.mp4",
        "verdict": "Corvo thrusts his arm toward the guard and vanishes; for about three seconds only the guard is seen, then Corvo reappears beside him and stands."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332f/result.json](<../../../../work/natural-moveset/coverage-v1332f/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: possession-empty-v322.mp4](<../../../../work/natural-moveset/possession-empty-v322.mp4>)
- [in_game_recordings: possession-network-v323.mp4](<../../../../work/natural-moveset/possession-network-v323.mp4>)
- [in_game_recordings: possession-network-v323-source.mp4](<../../../../work/natural-moveset/possession-network-v323-source.mp4>)
- [in_game_recordings: possession-human-v328.mp4](<../../../../work/natural-moveset/possession-human-v328.mp4>)
- [in_game_recordings: possession-release-network-v335.mp4](<../../../../work/natural-moveset/possession-release-network-v335.mp4>)
- [in_game_recordings: possession-release-network-v335-source.mp4](<../../../../work/natural-moveset/possession-release-network-v335-source.mp4>)
- [in_game_recordings: possession-visibility-network-v339.mp4](<../../../../work/natural-moveset/possession-visibility-network-v339.mp4>)
- [in_game_recordings: possession-visibility-network-v339-source.mp4](<../../../../work/natural-moveset/possession-visibility-network-v339-source.mp4>)
- [in_game_recordings: possession-visibility-return-v342.mp4](<../../../../work/natural-moveset/possession-visibility-return-v342.mp4>)
- [in_game_recordings: possession-visibility-return-v342-source.mp4](<../../../../work/natural-moveset/possession-visibility-return-v342-source.mp4>)
- [in_game_recordings: possession-exit-continuity-v343.mp4](<../../../../work/natural-moveset/possession-exit-continuity-v343.mp4>)
- [in_game_recordings: possession-exit-continuity-v343-source.mp4](<../../../../work/natural-moveset/possession-exit-continuity-v343-source.mp4>)
- [in_game_recordings: possession-receiver-stop-v344.mp4](<../../../../work/natural-moveset/possession-receiver-stop-v344.mp4>)
- [in_game_recordings: possession-receiver-stop-v344-source.mp4](<../../../../work/natural-moveset/possession-receiver-stop-v344-source.mp4>)
- [in_game_recordings: possession-exit-close-v345.mp4](<../../../../work/natural-moveset/possession-exit-close-v345.mp4>)
- [in_game_recordings: possession-exit-close-v345-source.mp4](<../../../../work/natural-moveset/possession-exit-close-v345-source.mp4>)
- [in_game_recordings: possession-receiver-pause-v347.mp4](<../../../../work/natural-moveset/possession-receiver-pause-v347.mp4>)
- [in_game_recordings: possession-receiver-pause-v347-source.mp4](<../../../../work/natural-moveset/possession-receiver-pause-v347-source.mp4>)
- [in_game_recordings: possession-source-pause-v347.mp4](<../../../../work/natural-moveset/possession-source-pause-v347.mp4>)
- [in_game_recordings: possession-source-pause-v347-source.mp4](<../../../../work/natural-moveset/possession-source-pause-v347-source.mp4>)
- [in_game_recordings: possession-fx-observe-v349.mp4](<../../../../work/natural-moveset/possession-fx-observe-v349.mp4>)
- [in_game_recordings: possession-fx-observe-v349-source.mp4](<../../../../work/natural-moveset/possession-fx-observe-v349-source.mp4>)
- [in_game_recordings: possession-scoped-fx-v350.mp4](<../../../../work/natural-moveset/possession-scoped-fx-v350.mp4>)
- [in_game_recordings: possession-scoped-fx-v350-source.mp4](<../../../../work/natural-moveset/possession-scoped-fx-v350-source.mp4>)
- [in_game_recordings: possession-target-fx-network-v351.mp4](<../../../../work/natural-moveset/possession-target-fx-network-v351.mp4>)
- [in_game_recordings: possession-target-fx-network-v351-source.mp4](<../../../../work/natural-moveset/possession-target-fx-network-v351-source.mp4>)
- [in_game_recordings: possession-target-fx-view-v352.mp4](<../../../../work/natural-moveset/possession-target-fx-view-v352.mp4>)
- [in_game_recordings: possession-target-fx-view-v352-source.mp4](<../../../../work/natural-moveset/possession-target-fx-view-v352-source.mp4>)
- [in_game_recordings: possession-target-fx-view-v354.mp4](<../../../../work/natural-moveset/possession-target-fx-view-v354.mp4>)
- [in_game_recordings: possession-target-fx-view-v354-source.mp4](<../../../../work/natural-moveset/possession-target-fx-view-v354-source.mp4>)
- [in_game_recordings: possession-target-fx-side-v355.mp4](<../../../../work/natural-moveset/possession-target-fx-side-v355.mp4>)
- [in_game_recordings: possession-target-fx-side-v355-source.mp4](<../../../../work/natural-moveset/possession-target-fx-side-v355-source.mp4>)
- [in_game_recordings: possession-target-fx-cancel-v358.mp4](<../../../../work/natural-moveset/possession-target-fx-cancel-v358.mp4>)
- [in_game_recordings: possession-target-fx-cancel-v358-source.mp4](<../../../../work/natural-moveset/possession-target-fx-cancel-v358-source.mp4>)
- [in_game_recordings: possession-target-fx-receiver-stop-v359.mp4](<../../../../work/natural-moveset/possession-target-fx-receiver-stop-v359.mp4>)
- [in_game_recordings: possession-target-fx-receiver-stop-v359-source.mp4](<../../../../work/natural-moveset/possession-target-fx-receiver-stop-v359-source.mp4>)
- [in_game_recordings: possession-target-fx-source-pause-v360.mp4](<../../../../work/natural-moveset/possession-target-fx-source-pause-v360.mp4>)
- [in_game_recordings: possession-target-fx-source-pause-v360-source.mp4](<../../../../work/natural-moveset/possession-target-fx-source-pause-v360-source.mp4>)
- [in_game_recordings: possession-target-fx-receiver-pause-v360.mp4](<../../../../work/natural-moveset/possession-target-fx-receiver-pause-v360.mp4>)
- [in_game_recordings: possession-target-fx-receiver-pause-v360-source.mp4](<../../../../work/natural-moveset/possession-target-fx-receiver-pause-v360-source.mp4>)
- [in_game_recordings: possession-target-fx-source-pause-v362.mp4](<../../../../work/natural-moveset/possession-target-fx-source-pause-v362.mp4>)
- [in_game_recordings: possession-target-fx-source-pause-v362-source.mp4](<../../../../work/natural-moveset/possession-target-fx-source-pause-v362-source.mp4>)
- [in_game_recordings: possession-crouch-observe-v361.mp4](<../../../../work/natural-moveset/possession-crouch-observe-v361.mp4>)
- [in_game_recordings: possession-holstered-observe-v361.mp4](<../../../../work/natural-moveset/possession-holstered-observe-v361.mp4>)
- [in_game_recordings: possession-crouch-holstered-observe-v361.mp4](<../../../../work/natural-moveset/possession-crouch-holstered-observe-v361.mp4>)
- [in_game_recordings: possession-crouch-network-v363.mp4](<../../../../work/natural-moveset/possession-crouch-network-v363.mp4>)
- [in_game_recordings: possession-crouch-network-v363-source.mp4](<../../../../work/natural-moveset/possession-crouch-network-v363-source.mp4>)
- [in_game_recordings: possession-holstered-network-v364.mp4](<../../../../work/natural-moveset/possession-holstered-network-v364.mp4>)
- [in_game_recordings: possession-holstered-network-v364-source.mp4](<../../../../work/natural-moveset/possession-holstered-network-v364-source.mp4>)
- [in_game_recordings: possession-holstered-network-v366.mp4](<../../../../work/natural-moveset/possession-holstered-network-v366.mp4>)
- [in_game_recordings: possession-holstered-network-v366-source.mp4](<../../../../work/natural-moveset/possession-holstered-network-v366-source.mp4>)
- [in_game_recordings: possession-holstered-network-v367.mp4](<../../../../work/natural-moveset/possession-holstered-network-v367.mp4>)
- [in_game_recordings: possession-holstered-network-v367-source.mp4](<../../../../work/natural-moveset/possession-holstered-network-v367-source.mp4>)
- [in_game_recordings: possession-holstered-network-v368.mp4](<../../../../work/natural-moveset/possession-holstered-network-v368.mp4>)
- [in_game_recordings: possession-holstered-network-v368-source.mp4](<../../../../work/natural-moveset/possession-holstered-network-v368-source.mp4>)
- [in_game_recordings: possession-holstered-network-v369.mp4](<../../../../work/natural-moveset/possession-holstered-network-v369.mp4>)
- [in_game_recordings: possession-holstered-network-v369-source.mp4](<../../../../work/natural-moveset/possession-holstered-network-v369-source.mp4>)
- [in_game_recordings: possession-crouch-holstered-network-v369.mp4](<../../../../work/natural-moveset/possession-crouch-holstered-network-v369.mp4>)
- [in_game_recordings: possession-crouch-holstered-network-v369-source.mp4](<../../../../work/natural-moveset/possession-crouch-holstered-network-v369-source.mp4>)
- [in_game_recordings: possession-crouch-holstered-network-v370.mp4](<../../../../work/natural-moveset/possession-crouch-holstered-network-v370.mp4>)
- [in_game_recordings: possession-crouch-holstered-network-v370-source.mp4](<../../../../work/natural-moveset/possession-crouch-holstered-network-v370-source.mp4>)
- [in_game_recordings: possession-crouch-target-network-v370.mp4](<../../../../work/natural-moveset/possession-crouch-target-network-v370.mp4>)
- [in_game_recordings: possession-crouch-target-network-v370-source.mp4](<../../../../work/natural-moveset/possession-crouch-target-network-v370-source.mp4>)
- [in_game_recordings: possession-crouch-target-network-v371.mp4](<../../../../work/natural-moveset/possession-crouch-target-network-v371.mp4>)
- [in_game_recordings: possession-crouch-target-network-v371-source.mp4](<../../../../work/natural-moveset/possession-crouch-target-network-v371-source.mp4>)
- [in_game_recordings: possession-crouch-target-network-v372.mp4](<../../../../work/natural-moveset/possession-crouch-target-network-v372.mp4>)
- [in_game_recordings: possession-crouch-target-network-v372-source.mp4](<../../../../work/natural-moveset/possession-crouch-target-network-v372-source.mp4>)
- [in_game_recordings: possession-partner-exclusion-v374.mp4](<../../../../work/natural-moveset/possession-partner-exclusion-v374.mp4>)
- [in_game_recordings: possession-partner-guard-control-v375.mp4](<../../../../work/natural-moveset/possession-partner-guard-control-v375.mp4>)
- [in_game_recordings: possession-partner-guard-control-v375-source.mp4](<../../../../work/natural-moveset/possession-partner-guard-control-v375-source.mp4>)
## corvo_bend_time_cast_end

Visual sweep 1260: accepted (corvo accept). Standing and crouched Bend Time casts and releases are recognizable and recover cleanly. The crouched release pose is a little generic, and no world time effect is shown (out of scope). Native Corvo Bend Time start/release use exact native scene clocks over Steam. v42 corrects crouched shoulders; v44 retains airborne casting and tests same-world pause recovery with a temporary visibility gap. v45 adds the observed 333ms out-of-mana response. Named takes remain scoped; other contexts, interruptions, continuous visual acceptance and world-time-dependent effects remain open. Asset-only hold stays unbound; viewer blur is diagnostic only.

````json
{
  "id": "corvo_bend_time_cast_end",
  "family": "powers",
  "characters": [
    "corvo"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_Bend_Time_stance_equipment_air_and_pause_through_Steam",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "corvo bend time cast end",
  "motions": [
    "power_bend_time_fire_in",
    "power_bend_time_fire_loop",
    "power_bend_time_fire_out",
    "power_bend_time_sequence",
    "power_bend_time_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept). Standing and crouched Bend Time casts and releases are recognizable and recover cleanly. The crouched release pose is a little generic, and no world time effect is shown (out of scope). Native Corvo Bend Time start/release use exact native scene clocks over Steam. v42 corrects crouched shoulders; v44 retains airborne casting and tests same-world pause recovery with a temporary visibility gap. v45 adds the observed 333ms out-of-mana response. Named takes remain scoped; other contexts, interruptions, continuous visual acceptance and world-time-dependent effects remain open. Asset-only hold stays unbound; viewer blur is diagnostic only.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "bend_time",
      "character": "corvo",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "needs": [
        "native cast effects",
        "shared world-time state and coherent playback clocks",
        "end/interruption cleanup"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "native-bendtime-corvo-v39.mp4",
    "bendtime-corvo-body-v40-v1.mp4",
    "bendtime-corvo-body-v40-v1-source.mp4",
    "bendtime-corvo-body-v40-v2.mp4",
    "bendtime-corvo-body-v40-v2-source.mp4",
    "failure-fx-receiver-stop-v51.mp4",
    "failure-fx-receiver-stop-v51-source.mp4",
    "failure-fx-source-resume-v51.mp4",
    "failure-fx-source-resume-v51-source.mp4",
    "failure-fx-receiver-resume-v51.mp4",
    "failure-fx-receiver-resume-v51-source.mp4",
    "bendtime-corvo-crouch-guard-moving-v50.mp4",
    "bendtime-corvo-crouch-guard-moving-v50-source.mp4",
    "bendtime-corvo-crouch-guard-v50.mp4",
    "bendtime-corvo-crouch-guard-v50-source.mp4",
    "bendtime-corvo-crouch-clearance-baseline-v49.mp4",
    "bendtime-corvo-crouch-clearance-baseline-v49-source.mp4",
    "bendtime-corvo-crouch-clearance-candidate-v49.mp4",
    "bendtime-corvo-crouch-clearance-candidate-v49-source.mp4",
    "outofmana-corvo-standing-armed-fx-v48.mp4",
    "outofmana-corvo-standing-armed-fx-v48-source.mp4",
    "outofmana-corvo-crouched-armed-fx-v48.mp4",
    "outofmana-corvo-crouched-armed-fx-v48-source.mp4",
    "outofmana-corvo-standing-empty-fx-v48.mp4",
    "outofmana-corvo-standing-empty-fx-v48-source.mp4",
    "outofmana-corvo-crouched-empty-fx-v48.mp4",
    "outofmana-corvo-crouched-empty-fx-v48-source.mp4",
    "outofmana-corvo-standing-armed-v46.mp4",
    "outofmana-corvo-standing-armed-v46-source.mp4",
    "outofmana-corvo-standing-empty-v46.mp4",
    "outofmana-corvo-standing-empty-v46-source.mp4",
    "outofmana-corvo-crouched-armed-v46.mp4",
    "outofmana-corvo-crouched-armed-v46-source.mp4",
    "outofmana-corvo-crouched-empty-v46.mp4",
    "outofmana-corvo-crouched-empty-v46-source.mp4",
    "native-bendtime-corvo-no-mana-crouched-armed-v45.mp4",
    "native-bendtime-corvo-no-mana-standing-empty-v45.mp4",
    "native-bendtime-corvo-no-mana-crouched-empty-v45.mp4",
    "outofmana-corvo-standing-armed-front-v45.mp4",
    "outofmana-corvo-standing-armed-front-v45-source.mp4",
    "bendtime-corvo-crouched-armed-moving-v45.mp4",
    "bendtime-corvo-crouched-armed-moving-v45-source.mp4",
    "bendtime-corvo-air-empty-moving-v45.mp4",
    "bendtime-corvo-air-empty-moving-v45-source.mp4",
    "outofmana-corvo-standing-armed-v45-v1.mp4",
    "outofmana-corvo-standing-armed-v45-v1-source.mp4",
    "bendtime-corvo-air-v44-v1.mp4",
    "bendtime-corvo-air-v44-v1-source.mp4",
    "bendtime-receiver-stop-v44.mp4",
    "bendtime-receiver-stop-v44-source.mp4",
    "bendtime-source-pause-v44.mp4",
    "bendtime-source-pause-v44-source.mp4",
    "bendtime-receiver-pause-v44.mp4",
    "bendtime-receiver-pause-v44-source.mp4",
    "bendtime-source-resume-v44.mp4",
    "bendtime-source-resume-v44-source.mp4",
    "bendtime-receiver-resume-v44.mp4",
    "bendtime-receiver-resume-v44-source.mp4",
    "bendtime-receiver-resume-v44-v2.mp4",
    "bendtime-receiver-resume-v44-v2-source.mp4",
    "native-bendtime-corvo-no-mana-v44.mp4",
    "native-bendtime-corvo-no-mana-v44-v2.mp4",
    "native-bendtime-corvo-air-v43.mp4",
    "bendtime-corvo-air-v43-v1.mp4",
    "bendtime-corvo-air-v43-v1-source.mp4",
    "bendtime-corvo-crouched-empty-v42.mp4",
    "bendtime-corvo-crouched-empty-v42-source.mp4",
    "bendtime-corvo-standing-empty-moving-v42.mp4",
    "bendtime-corvo-standing-empty-moving-v42-source.mp4",
    "bendtime-corvo-crouched-armed-v41.mp4",
    "bendtime-corvo-crouched-armed-v41-source.mp4",
    "bendtime-corvo-crouched-empty-v41.mp4",
    "bendtime-corvo-crouched-empty-v41-source.mp4",
    "bendtime-corvo-standing-empty-v41.mp4",
    "bendtime-corvo-standing-empty-v41-source.mp4",
    "bendtime-corvo-crouched-armed-v42.mp4",
    "bendtime-corvo-crouched-armed-v42-source.mp4",
    "bendtime-corvo-crouched-armed-v42-v3.mp4",
    "bendtime-corvo-crouched-armed-v42-v3-source.mp4",
    "bendtime-viewer-fx-v41-v1.mp4"
  ],
  "native_effects_status": "not_verified",
  "native_integration": {
    "native-bendtime-corvo-v39.mp4": {
      "video": "native-bendtime-corvo-v39.mp4",
      "character": "corvo",
      "label": "Corvo · Bend Time · source reference",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-v39/result.json",
      "visual_review": "native-bendtime-corvo-v39/visual-review.json",
      "proxy_sha256": "60B99A0397B6C6E2652A132B239A9D9B6622206C170159F3CCC00CDAC5D3DD6D",
      "bank_sha256": null,
      "actual_capture_hz": 26.0,
      "video_sha256": "BF79AC1D5BA830E01B2DF148F4E7DA6C2CD4241578E742B857D755CAE28A8393",
      "checks_passed": true,
      "scope": "Native BendTime investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "bendtime-corvo-body-v40-v1.mp4": {
      "video": "bendtime-corvo-body-v40-v1.mp4",
      "character": "corvo",
      "label": "Corvo · Bend Time · paired body · sampling check failed",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-body-v40-v1/result.json",
      "visual_review": "bendtime-corvo-body-v40-v1/visual-review.json",
      "proxy_sha256": "CD3FB5F65FEB36951BFDCE98A7D51BC4F1914E01AA9C31287B4BDEC5BC92F337",
      "bank_sha256": "4A41D289364CBC0E91A39463AF572832CC840EC36AE33AACD5A564B0508FFC43",
      "actual_capture_hz": 27.083333333333332,
      "video_sha256": "CB6CC45D53DBF1196D2EAD72E3146E7E4963930F1DF62B9F7F776A392B4C83C8",
      "checks_passed": false,
      "scope": "Standing armed Corvo BendTime body integration only. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-body-v40-v1-source.mp4": {
      "video": "bendtime-corvo-body-v40-v1-source.mp4",
      "character": "corvo",
      "label": "Corvo · Bend Time · source reference · sampling check failed",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-body-v40-v1/result.json",
      "visual_review": null,
      "proxy_sha256": "CD3FB5F65FEB36951BFDCE98A7D51BC4F1914E01AA9C31287B4BDEC5BC92F337",
      "bank_sha256": "4A41D289364CBC0E91A39463AF572832CC840EC36AE33AACD5A564B0508FFC43",
      "actual_capture_hz": 23.166666666666668,
      "video_sha256": "A1ADACFF3ED047ACAAAC97480BB0D4C22D47EEB62C78ACD5C166318A571B2119",
      "checks_passed": false,
      "scope": "Standing armed Corvo BendTime body integration only. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-body-v40-v2.mp4": {
      "video": "bendtime-corvo-body-v40-v2.mp4",
      "character": "corvo",
      "label": "Corvo · Bend Time · paired body",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-body-v40-v2/result.json",
      "visual_review": "bendtime-corvo-body-v40-v2/visual-review.json",
      "proxy_sha256": "CD3FB5F65FEB36951BFDCE98A7D51BC4F1914E01AA9C31287B4BDEC5BC92F337",
      "bank_sha256": "4A41D289364CBC0E91A39463AF572832CC840EC36AE33AACD5A564B0508FFC43",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "929AEDAEC2062178FE9EC8BE110D8C7652D7DE594F66CBBFD8452C89F7D84675",
      "checks_passed": true,
      "scope": "Standing armed Corvo BendTime body integration only. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-body-v40-v2-source.mp4": {
      "video": "bendtime-corvo-body-v40-v2-source.mp4",
      "character": "corvo",
      "label": "Corvo · Bend Time · source reference",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-body-v40-v2/result.json",
      "visual_review": null,
      "proxy_sha256": "CD3FB5F65FEB36951BFDCE98A7D51BC4F1914E01AA9C31287B4BDEC5BC92F337",
      "bank_sha256": "4A41D289364CBC0E91A39463AF572832CC840EC36AE33AACD5A564B0508FFC43",
      "actual_capture_hz": 25.166666666666668,
      "video_sha256": "4D61E47CAA1F8A3BF97BC39D3DCD18B3E640DD08C70AFCB4D0204BE915D229B2",
      "checks_passed": true,
      "scope": "Standing armed Corvo BendTime body integration only. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-armed-v41.mp4": {
      "video": "bendtime-corvo-crouched-armed-v41.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-v41",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-v41/result.json",
      "visual_review": "bendtime-corvo-crouched-armed-v41/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "B35ABA09D046C9C42B5C60C3BB676995D3853E004ECAAE4344FDBB38E12CAA1F",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "2B273A31EFDD60EC40831EAB216E84F9F43F3DD0CBF001D5B40744D5B9450CFE",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-armed-v41-source.mp4": {
      "video": "bendtime-corvo-crouched-armed-v41-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-v41 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-v41/result.json",
      "visual_review": null,
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "B35ABA09D046C9C42B5C60C3BB676995D3853E004ECAAE4344FDBB38E12CAA1F",
      "actual_capture_hz": 24.515235457063714,
      "video_sha256": "E0404B36D9F6B0C0773FD5F862AAFFFE815CCF1CDD75B555B0F2501EE1BB2D97",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-empty-v41.mp4": {
      "video": "bendtime-corvo-crouched-empty-v41.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-empty-v41",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-empty-v41/result.json",
      "visual_review": "bendtime-corvo-crouched-empty-v41/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "B35ABA09D046C9C42B5C60C3BB676995D3853E004ECAAE4344FDBB38E12CAA1F",
      "actual_capture_hz": 27.257617728531855,
      "video_sha256": "7E8EDD2865FEFD48B25DAE2CB6AB99D4935CED5D4F8D2F5179D305881EAE2736",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-empty-v41-source.mp4": {
      "video": "bendtime-corvo-crouched-empty-v41-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-empty-v41 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-empty-v41/result.json",
      "visual_review": null,
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "B35ABA09D046C9C42B5C60C3BB676995D3853E004ECAAE4344FDBB38E12CAA1F",
      "actual_capture_hz": 24.666666666666668,
      "video_sha256": "19D5345D1E1021B874E3B87BF4A06277440AF22666FD4FF338A5D98EC2919D91",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-standing-empty-v41.mp4": {
      "video": "bendtime-corvo-standing-empty-v41.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-standing-empty-v41 · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-standing-empty-v41/result.json",
      "visual_review": null,
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "B35ABA09D046C9C42B5C60C3BB676995D3853E004ECAAE4344FDBB38E12CAA1F",
      "actual_capture_hz": 27.833333333333332,
      "video_sha256": "8D3D432D4F78A343FCF51468DC9E1E7337E41C01020CF78BFBE1A5A3FBCB85A7",
      "checks_passed": false,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-standing-empty-v41-source.mp4": {
      "video": "bendtime-corvo-standing-empty-v41-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-standing-empty-v41 · source · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-standing-empty-v41/result.json",
      "visual_review": "bendtime-corvo-standing-empty-v41/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "B35ABA09D046C9C42B5C60C3BB676995D3853E004ECAAE4344FDBB38E12CAA1F",
      "actual_capture_hz": 25.666666666666668,
      "video_sha256": "756BC503603FCE11AF8672C032227E3AD3F4FA2197182C6A60003AC5B56F368A",
      "checks_passed": false,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-armed-v42.mp4": {
      "video": "bendtime-corvo-crouched-armed-v42.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-v42 · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-v42/result.json",
      "visual_review": "bendtime-corvo-crouched-armed-v42/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "4A79CFA2A156A453B1616043F0435D7989CD943584D30A75A03595C9742989DC",
      "checks_passed": false,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-armed-v42-source.mp4": {
      "video": "bendtime-corvo-crouched-armed-v42-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-v42 · source · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-v42/result.json",
      "visual_review": null,
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 23.833333333333332,
      "video_sha256": "143A84DA46DFC68C90C55723E8165E81233BA16640D3E82C3A03CA0B8ADE4D61",
      "checks_passed": false,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-armed-v42-v3.mp4": {
      "video": "bendtime-corvo-crouched-armed-v42-v3.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-v42-v3",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-v42-v3/result.json",
      "visual_review": "bendtime-corvo-crouched-armed-v42-v3/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "ACC175BB1E93F853821797E934657ABE39579021DA4EFADEDC6F122BE639CF4B",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-armed-v42-v3-source.mp4": {
      "video": "bendtime-corvo-crouched-armed-v42-v3-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-v42-v3 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-v42-v3/result.json",
      "visual_review": null,
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 24.349030470914126,
      "video_sha256": "D83BDE983712AC409BFB38614E279C78AC7FAB3822508D65412D499F6CFF4D00",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-viewer-fx-v41-v1.mp4": {
      "video": "bendtime-viewer-fx-v41-v1.mp4",
      "character": "corvo",
      "label": "Bend Time · viewer-fx-v41-v1",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-viewer-fx-v41-v1/result.json",
      "visual_review": "bendtime-viewer-fx-v41-v1/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": null,
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "F534E35716BDCCB7DEBE4A3EBC854449513F290E225AB6A8B47AE0E5D5B9F89A",
      "checks_passed": true,
      "scope": "Isolated manual native Bend Time viewer particle probe. Original screen-particle FX, 1600ms lease or explicit cancellation, owned clone record. No network FX replication, render-factor override, power activation or world time changes. Visual feasibility only; not complete native Bend Time appearance."
    },
    "bendtime-corvo-crouched-empty-v42.mp4": {
      "video": "bendtime-corvo-crouched-empty-v42.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-empty-v42",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-empty-v42/result.json",
      "visual_review": "bendtime-corvo-crouched-empty-v42/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.083333333333332,
      "video_sha256": "5F8710563277A3BDB0F0F486ECBA69666089549CCD35A7DF3647C0F29CEB24CE",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-empty-v42-source.mp4": {
      "video": "bendtime-corvo-crouched-empty-v42-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-empty-v42 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-empty-v42/result.json",
      "visual_review": null,
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 23.784530386740332,
      "video_sha256": "F56EBAD8E42E08DD38746EF8E950D0568DB9265A4F9F4AC36E20A4DF7259D98E",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-standing-empty-moving-v42.mp4": {
      "video": "bendtime-corvo-standing-empty-moving-v42.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-standing-empty-moving-v42",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-standing-empty-moving-v42/result.json",
      "visual_review": "bendtime-corvo-standing-empty-moving-v42/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.916666666666668,
      "video_sha256": "68BD897C5BDB0F0D2ACDB2DCF64768C4107A6866861BCED4F29D1BD2E7C470CE",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-standing-empty-moving-v42-source.mp4": {
      "video": "bendtime-corvo-standing-empty-moving-v42-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-standing-empty-moving-v42 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-standing-empty-moving-v42/result.json",
      "visual_review": null,
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 24.666666666666668,
      "video_sha256": "2AD5684221DACC50CE39EEFAD97E0B1650BAFBDE33429B2B511BB3FDF2340305",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "native-bendtime-corvo-air-v43.mp4": {
      "video": "native-bendtime-corvo-air-v43.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-air-v43",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-air-v43/result.json",
      "visual_review": "native-bendtime-corvo-air-v43/visual-review.json",
      "proxy_sha256": "459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7",
      "bank_sha256": null,
      "actual_capture_hz": 26.166666666666668,
      "video_sha256": "D49F0567A042C4DF33332D3A476B2A64E5AD1AE395AE25381AEEEAB696F16C10",
      "checks_passed": true,
      "scope": "Native airborne BendTime investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "bendtime-corvo-air-v43-v1.mp4": {
      "video": "bendtime-corvo-air-v43-v1.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-air-v43-v1 · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-air-v43-v1/result.json",
      "visual_review": "bendtime-corvo-air-v43-v1/visual-review.json",
      "proxy_sha256": "B4845ABC910D51DB2C1C79F47BA6D39E3A00AC6427EF0E90EC42CA96291B3699",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.57660167130919,
      "video_sha256": "B1E8CD284A413C12258A00463388306A2BEFACE5EA8C9AEBE34F28199D991E54",
      "checks_passed": false,
      "scope": "Corvo Bend Time airborne cast, landing and equipment integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-air-v43-v1-source.mp4": {
      "video": "bendtime-corvo-air-v43-v1-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-air-v43-v1 · source · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-air-v43-v1/result.json",
      "visual_review": null,
      "proxy_sha256": "B4845ABC910D51DB2C1C79F47BA6D39E3A00AC6427EF0E90EC42CA96291B3699",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.323119777158773,
      "video_sha256": "A9381ECD24F4753F6F0CF968B173FB9D04377DA1F94D76BCB867B93517C98A96",
      "checks_passed": false,
      "scope": "Corvo Bend Time airborne cast, landing and equipment integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-air-v44-v1.mp4": {
      "video": "bendtime-corvo-air-v44-v1.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-air-v44-v1",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-air-v44-v1/result.json",
      "visual_review": "bendtime-corvo-air-v44-v1/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.083333333333332,
      "video_sha256": "4B551E741C146063E07A77B39B2958C2A7E024FA8D9099E5D1DCC75293119ABE",
      "checks_passed": true,
      "scope": "Corvo Bend Time airborne cast, landing and equipment integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-air-v44-v1-source.mp4": {
      "video": "bendtime-corvo-air-v44-v1-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-air-v44-v1 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-air-v44-v1/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 23.916666666666668,
      "video_sha256": "2B407FC5E24EF66378F9AEC3F21E523C2201BBC1698D656184AB4BF783802AAF",
      "checks_passed": true,
      "scope": "Corvo Bend Time airborne cast, landing and equipment integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-receiver-stop-v44.mp4": {
      "video": "bendtime-receiver-stop-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-stop-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-stop-v44/result.json",
      "visual_review": "bendtime-receiver-stop-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.38888888888889,
      "video_sha256": "203FE37BA8BC3F1F76E0E74E3D850E2821293F88680EF34E8C8EADF042B58561",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-stop-v44-source.mp4": {
      "video": "bendtime-receiver-stop-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-stop-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-stop-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.34011090573013,
      "video_sha256": "BB3D5F34FCA0AD9628C52AD4B7DD08D2E1CA9FB00667F0A10EC5B7CDC359FCCD",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-pause-v44.mp4": {
      "video": "bendtime-source-pause-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · source-pause-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-pause-v44/result.json",
      "visual_review": "bendtime-source-pause-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "903BE6034DE9951FD1CCF03B2DD6AD5DDEA89E040B96B768FFB1DE0E44CF7A79",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-pause-v44-source.mp4": {
      "video": "bendtime-source-pause-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · source-pause-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-pause-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.22222222222222,
      "video_sha256": "9A32ADFC7CD055B30F0F4E9396AFBF6A4EEA4F455656B3CCCCC5E75A249A8E4F",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-pause-v44.mp4": {
      "video": "bendtime-receiver-pause-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-pause-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-pause-v44/result.json",
      "visual_review": "bendtime-receiver-pause-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.44916820702403,
      "video_sha256": "610B1ECF5F1D331A492BB57773C0804BE28EEF3F86A566612951936307011700",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-pause-v44-source.mp4": {
      "video": "bendtime-receiver-pause-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-pause-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-pause-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.217068645640076,
      "video_sha256": "A8A66FBB4CC056DE4C2464A99A25CD589F1746AC93AD49F31567E5391E5894F0",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-resume-v44.mp4": {
      "video": "bendtime-source-resume-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · source-resume-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-resume-v44/result.json",
      "visual_review": "bendtime-source-resume-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.72222222222222,
      "video_sha256": "6FC7E4B54CFDFC1B75F819C8F1AFA43A58FCD02753158ED637785D886476DFF3",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-resume-v44-source.mp4": {
      "video": "bendtime-source-resume-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · source-resume-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-resume-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.22222222222222,
      "video_sha256": "091307A958980F854FA4B990ED3F0683C838C5562ED65D401AB95862A11C65DA",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44.mp4": {
      "video": "bendtime-receiver-resume-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44 · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44/result.json",
      "visual_review": "bendtime-receiver-resume-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.22222222222222,
      "video_sha256": "5A27735C82275C1EB96A8F2B4C7D81B24E691E2215777C44E3521EFB61C83E57",
      "checks_passed": false,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44-source.mp4": {
      "video": "bendtime-receiver-resume-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44 · source · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 25.78558225508318,
      "video_sha256": "1BDF823F63EAAB4DFF3CF253E414B27A28680F67CAF43659E79AA9A5F03F3F5A",
      "checks_passed": false,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44-v2.mp4": {
      "video": "bendtime-receiver-resume-v44-v2.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44-v2",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44-v2/result.json",
      "visual_review": "bendtime-receiver-resume-v44-v2/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.38888888888889,
      "video_sha256": "BEF1A23891BDD93864A06860466BC8DF8B38F5538D3550A3E618C00AC28FD9DC",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44-v2-source.mp4": {
      "video": "bendtime-receiver-resume-v44-v2-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44-v2 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44-v2/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.950092421441774,
      "video_sha256": "D6D6E2EFF460B32889450791C3FA93D38C4D4CB4470F601C28C3DDD793BEBF7D",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "native-bendtime-corvo-no-mana-v44.mp4": {
      "video": "native-bendtime-corvo-no-mana-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-v44 · failed check",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-v44/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": null,
      "actual_capture_hz": 26.125,
      "video_sha256": "7BF586EBADD27D542C9FAC85510DBA90F07566E93495FF81316C2CF9040C9568",
      "checks_passed": false,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "native-bendtime-corvo-no-mana-v44-v2.mp4": {
      "video": "native-bendtime-corvo-no-mana-v44-v2.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-v44-v2",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-v44-v2/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-v44-v2/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": null,
      "actual_capture_hz": 26.0,
      "video_sha256": "7333427C4C8747CAAD8FC1C8944759D5C0A04AF0ABF83826BD8A8463B31CB530",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "outofmana-corvo-standing-armed-v45-v1.mp4": {
      "video": "outofmana-corvo-standing-armed-v45-v1.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v45-v1",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v45-v1/result.json",
      "visual_review": "outofmana-corvo-standing-armed-v45-v1/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 27.5,
      "video_sha256": "CA2B3C3BBA0BB71E7C5B8A80E6671173A3AE5E02FD70B42B88FFD985F7FF07C3",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-v45-v1-source.mp4": {
      "video": "outofmana-corvo-standing-armed-v45-v1-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v45-v1 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v45-v1/result.json",
      "visual_review": null,
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 23.064066852367688,
      "video_sha256": "8211A0509C538B31E26CE22584F97EB2375CEED3DC5837BD4A50775EEBF44482",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "bendtime-corvo-air-empty-moving-v45.mp4": {
      "video": "bendtime-corvo-air-empty-moving-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-air-empty-moving-v45",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-air-empty-moving-v45/result.json",
      "visual_review": "bendtime-corvo-air-empty-moving-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "89D3D2725892B6C287FE574FE0395ECFD005DB35152576FA7B64ACB668D30CC6",
      "checks_passed": true,
      "scope": "Corvo Bend Time airborne cast, landing and equipment integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-air-empty-moving-v45-source.mp4": {
      "video": "bendtime-corvo-air-empty-moving-v45-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-air-empty-moving-v45 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-air-empty-moving-v45/result.json",
      "visual_review": null,
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 24.833333333333332,
      "video_sha256": "2FEC87776B3FC3B5378BE6826A8DF2D1215D1A4FF68FCDBF240940A7EA00003D",
      "checks_passed": true,
      "scope": "Corvo Bend Time airborne cast, landing and equipment integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "outofmana-corvo-standing-armed-front-v45.mp4": {
      "video": "outofmana-corvo-standing-armed-front-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-front-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-front-v45/result.json",
      "visual_review": "outofmana-corvo-standing-armed-front-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 27.57660167130919,
      "video_sha256": "940D056A4CF1A013AA0101944301E7F3338DE5CCD3261277FA5AF5FE8FE9950D",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-front-v45-source.mp4": {
      "video": "outofmana-corvo-standing-armed-front-v45-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-front-v45 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-front-v45/result.json",
      "visual_review": null,
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 22.29050279329609,
      "video_sha256": "3BC97F3F1DAC504E0978F34BA467E03DD19D76141B985B7623AFDCD60140A933",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "bendtime-corvo-crouched-armed-moving-v45.mp4": {
      "video": "bendtime-corvo-crouched-armed-moving-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-moving-v45",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-moving-v45/result.json",
      "visual_review": "bendtime-corvo-crouched-armed-moving-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 28.077994428969358,
      "video_sha256": "6ABEC3F62BBBA4DC6C836D142527B6D55910C1EE8FA9B8ED1624BE7954D86515",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouched-armed-moving-v45-source.mp4": {
      "video": "bendtime-corvo-crouched-armed-moving-v45-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouched-armed-moving-v45 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouched-armed-moving-v45/result.json",
      "visual_review": null,
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 24.166666666666668,
      "video_sha256": "909F38BD8C2EC2A650D324C5E34C6884697DFE3C866FEB96048AB1937783964D",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "native-bendtime-corvo-no-mana-standing-empty-v45.mp4": {
      "video": "native-bendtime-corvo-no-mana-standing-empty-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-standing-empty-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-standing-empty-v45/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-standing-empty-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": null,
      "actual_capture_hz": 25.905292479108635,
      "video_sha256": "B4947B3394622D33D8FAE4844EABA829C3033057331994053BC8E74A673134E9",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "native-bendtime-corvo-no-mana-crouched-empty-v45.mp4": {
      "video": "native-bendtime-corvo-no-mana-crouched-empty-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-crouched-empty-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-crouched-empty-v45/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-crouched-empty-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": null,
      "actual_capture_hz": 26.083333333333332,
      "video_sha256": "CFB522FC1F8F19BEB52919BC8AC9B6315952087DC76279D8BD2E0E42FED23707",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "outofmana-corvo-standing-armed-v46.mp4": {
      "video": "outofmana-corvo-standing-armed-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v46/result.json",
      "visual_review": "outofmana-corvo-standing-armed-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.333333333333332,
      "video_sha256": "5F2E568D34908BFF2EA94A7F0B4737F56287C5D81C72AAE220B033AE0BA81AF4",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-v46-source.mp4": {
      "video": "outofmana-corvo-standing-armed-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.083333333333332,
      "video_sha256": "5D76489440394EB427F5DD6894B61CEC136BEBB71F44FA5C32FD0DC61AF41C25",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-v46.mp4": {
      "video": "outofmana-corvo-standing-empty-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-v46/result.json",
      "visual_review": "outofmana-corvo-standing-empty-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "FB217C93125A8EFF652CAC02229885867BDD65D49FABB95E3C787B345DA6033E",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-v46-source.mp4": {
      "video": "outofmana-corvo-standing-empty-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.75,
      "video_sha256": "10A1550541D571F5D5D98530D73AAAC580CD01CC0A08610A5B5345E66813C651",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-v46.mp4": {
      "video": "outofmana-corvo-crouched-armed-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-v46/result.json",
      "visual_review": "outofmana-corvo-crouched-armed-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.74373259052925,
      "video_sha256": "22DC69024D5B05788396012A72C3896529587645C3693CAB32BCC52EFEDA65C3",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-v46-source.mp4": {
      "video": "outofmana-corvo-crouched-armed-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.583333333333332,
      "video_sha256": "004F8A556FC29C7A6E1CA367CCCF1A1D16639AABC98D5E9523536DA12E3A3E3B",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-v46.mp4": {
      "video": "outofmana-corvo-crouched-empty-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-v46/result.json",
      "visual_review": "outofmana-corvo-crouched-empty-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.24233983286908,
      "video_sha256": "0C86365C5EB85EF4B92542E5026E1B89EED5D6DE1B2FD44E040B8FB50401BB76",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-v46-source.mp4": {
      "video": "outofmana-corvo-crouched-empty-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.651810584958216,
      "video_sha256": "B666B412B92B3DCAD2AD93E64568E180B1A73191EA8398524CCA2EBCD9C4E435",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "native-bendtime-corvo-no-mana-crouched-armed-v45.mp4": {
      "video": "native-bendtime-corvo-no-mana-crouched-armed-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-crouched-armed-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-crouched-armed-v45/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-crouched-armed-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": null,
      "actual_capture_hz": 26.083333333333332,
      "video_sha256": "FE38AE6FA392739619CE7514DD4000E232F5BB27F285726B5841EAA7C9051209",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "outofmana-hand-fx-corvo-v47.mp4": {
      "video": "outofmana-hand-fx-corvo-v47.mp4",
      "character": "corvo",
      "label": "Native failure hand FX · corvo · manual",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-hand-fx-corvo-v47/result.json",
      "visual_review": "outofmana-hand-fx-corvo-v47/visual-review.json",
      "proxy_sha256": "5957834D38E30B3DFD3DD57FE2BEFFCFF55898AF4D0468A8A4839B8E167F6109",
      "bank_sha256": null,
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "71323C7A62E62914570372190B9D3DF1E7DC309733C1130FB16754814FBBE2E3",
      "checks_passed": true,
      "scope": "Isolated manual native out-of-mana hand particle probe on the role-specific remote avatar. Exact native FX/particles, owned left wrist transform, finite 1600ms lease and cancellation. No automatic network trigger, first-person callback, power activation, resource write or damage. Wrist-centred adaptation of the native tattoo socket; placement/scale and camera effects require actual visual review."
    },
    "outofmana-hand-fx-emily-v47.mp4": {
      "video": "outofmana-hand-fx-emily-v47.mp4",
      "character": "emily",
      "label": "Native failure hand FX · emily · manual",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-hand-fx-emily-v47/result.json",
      "visual_review": "outofmana-hand-fx-emily-v47/visual-review.json",
      "proxy_sha256": "5957834D38E30B3DFD3DD57FE2BEFFCFF55898AF4D0468A8A4839B8E167F6109",
      "bank_sha256": null,
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "F86C4EB3F6521F98D07C2F3AA7751D55DE234CED98190AE498DAD2CA8B04DEEC",
      "checks_passed": true,
      "scope": "Isolated manual native out-of-mana hand particle probe on the role-specific remote avatar. Exact native FX/particles, owned left wrist transform, finite 1600ms lease and cancellation. No automatic network trigger, first-person callback, power activation, resource write or damage. Wrist-centred adaptation of the native tattoo socket; placement/scale and camera effects require actual visual review."
    },
    "outofmana-corvo-standing-armed-fx-v48.mp4": {
      "video": "outofmana-corvo-standing-armed-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-fx-v48/result.json",
      "visual_review": "outofmana-corvo-standing-armed-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.333333333333332,
      "video_sha256": "29CF40DC881FA9755E664329DB1BCDA91DC9B0A7A4BB1577614C0D03C580DB4F",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-fx-v48-source.mp4": {
      "video": "outofmana-corvo-standing-armed-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.568245125348188,
      "video_sha256": "5098EA60E060C0627510FDFB0FEA05C13DDAB87666319AA5612317043EEF084F",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-fx-v48.mp4": {
      "video": "outofmana-corvo-crouched-armed-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-fx-v48/result.json",
      "visual_review": "outofmana-corvo-crouched-armed-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.416666666666668,
      "video_sha256": "CDE13F2614BF013EB585FAAA84CB5DF949492938CD7F197F065C56D49D382782",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-fx-v48-source.mp4": {
      "video": "outofmana-corvo-crouched-armed-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 23.102493074792243,
      "video_sha256": "10111547C333BA2B33227ABD68B902D8759DA160AE8E70E41F83193E038C022C",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-fx-v48.mp4": {
      "video": "outofmana-corvo-standing-empty-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-fx-v48/result.json",
      "visual_review": "outofmana-corvo-standing-empty-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.5,
      "video_sha256": "D5FCC2D7FD32247AEFC078489015EA2EF1B5DF63FC18DF53D94509F9C42D7A19",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-fx-v48-source.mp4": {
      "video": "outofmana-corvo-standing-empty-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.25,
      "video_sha256": "2B7D1FB74E09941F987DA5FAE44F531B43951AB32BBC73CCE5F056D946F22440",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-fx-v48.mp4": {
      "video": "outofmana-corvo-crouched-empty-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-fx-v48/result.json",
      "visual_review": "outofmana-corvo-crouched-empty-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.166666666666668,
      "video_sha256": "F0EEDD86982A62D54FB60CBBFBCD19ACAD2FA9B4D119C66DD361DFE6D8B6477E",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-fx-v48-source.mp4": {
      "video": "outofmana-corvo-crouched-empty-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.333333333333332,
      "video_sha256": "BDE378B84B2BEEBCE20EB14593CE35E2DDC46A22BCCD1F1ABE866FD546490145",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "bendtime-corvo-crouch-clearance-baseline-v49.mp4": {
      "video": "bendtime-corvo-crouch-clearance-baseline-v49.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-clearance-baseline-v49",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-clearance-baseline-v49/result.json",
      "visual_review": "bendtime-corvo-crouch-clearance-baseline-v49/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.409470752089135,
      "video_sha256": "D1081AE5F3F1111851DC184B23F3B9105539299B0BDEC3DADC9ECE03D39C5888",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouch-clearance-baseline-v49-source.mp4": {
      "video": "bendtime-corvo-crouch-clearance-baseline-v49-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-clearance-baseline-v49 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-clearance-baseline-v49/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.083333333333332,
      "video_sha256": "48D7FABA6A5D0DE1C7A37E0BA20E1997FDC078AD43C3ED9B7CDAD88F15C0FB6E",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouch-clearance-candidate-v49.mp4": {
      "video": "bendtime-corvo-crouch-clearance-candidate-v49.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-clearance-candidate-v49",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-clearance-candidate-v49/result.json",
      "visual_review": "bendtime-corvo-crouch-clearance-candidate-v49/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "4B3387254E18F7AD23E6F0FF0CB7ECB7C63C646D67916CDB1E2B4738FE2FCCA2",
      "actual_capture_hz": 28.328690807799443,
      "video_sha256": "58E062C9C0B65715CAE3131C124257BA60574ABF78C1992A4B73F7E1463C01D1",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouch-clearance-candidate-v49-source.mp4": {
      "video": "bendtime-corvo-crouch-clearance-candidate-v49-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-clearance-candidate-v49 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-clearance-candidate-v49/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "4B3387254E18F7AD23E6F0FF0CB7ECB7C63C646D67916CDB1E2B4738FE2FCCA2",
      "actual_capture_hz": 23.26869806094183,
      "video_sha256": "BCDB9094AAAE70CE7AF1A41359C2476391C02D6AE315A437314EB8FAEDDCE70A",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "failure-fx-receiver-stop-v51.mp4": {
      "video": "failure-fx-receiver-stop-v51.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-stop-v51",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-stop-v51/result.json",
      "visual_review": "failure-fx-receiver-stop-v51/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.646076794657763,
      "video_sha256": "862D28466121E21E8FA85E4B3A1A2935FD633CFD5B68508C536C0FB8AE22254E",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-receiver-stop-v51-source.mp4": {
      "video": "failure-fx-receiver-stop-v51-source.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-stop-v51 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-stop-v51/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 26.1,
      "video_sha256": "704580F6BC709E502E54268AD47C76B5DFDCD87CB4A066740F85F7F214792511",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-source-resume-v51.mp4": {
      "video": "failure-fx-source-resume-v51.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-source-resume-v51",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-source-resume-v51/result.json",
      "visual_review": "failure-fx-source-resume-v51/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.15,
      "video_sha256": "16820A0FCCE5BE319D742804E4F2D57D4E6F7D20F94BE21EFDD9DDFDEE140047",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-source-resume-v51-source.mp4": {
      "video": "failure-fx-source-resume-v51-source.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-source-resume-v51 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-source-resume-v51/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 25.45,
      "video_sha256": "392173FFD5212BEF63BEB2F52B9BCCAC2929A379F7E2D2F273F373B47EF7FF6D",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-receiver-resume-v51.mp4": {
      "video": "failure-fx-receiver-resume-v51.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-resume-v51",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-resume-v51/result.json",
      "visual_review": "failure-fx-receiver-resume-v51/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.6,
      "video_sha256": "CBA0E3CCD59F85DC767B2B9F27AA23DF68581D78D48DE179A7AC0A2C5099CA19",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-receiver-resume-v51-source.mp4": {
      "video": "failure-fx-receiver-resume-v51-source.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-resume-v51 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-resume-v51/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 26.056572379367722,
      "video_sha256": "81C4DEB60A69F80BAE6C3318E2141D526F3D0DAA3D06352763A1F6B37450DA46",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "bendtime-corvo-crouch-guard-moving-v50.mp4": {
      "video": "bendtime-corvo-crouch-guard-moving-v50.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-guard-moving-v50",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-guard-moving-v50/result.json",
      "visual_review": "bendtime-corvo-crouch-guard-moving-v50/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "B46EDFD62F878A406F1DBB5C739BD78AE3A82D226F4098F71222FB0A14795863",
      "actual_capture_hz": 27.25,
      "video_sha256": "55C00201BF6F1F2C1F184A8626B3429331A52C16BB7EEBD7B90D4184D4DD3F25",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouch-guard-moving-v50-source.mp4": {
      "video": "bendtime-corvo-crouch-guard-moving-v50-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-guard-moving-v50 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-guard-moving-v50/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "B46EDFD62F878A406F1DBB5C739BD78AE3A82D226F4098F71222FB0A14795863",
      "actual_capture_hz": 23.5,
      "video_sha256": "5E55AAC141288C58F8FD2CC38C9007B1CFF06C18CDDD21F1CF07910FA3373FB0",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouch-guard-v50.mp4": {
      "video": "bendtime-corvo-crouch-guard-v50.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-guard-v50",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-guard-v50/result.json",
      "visual_review": "bendtime-corvo-crouch-guard-v50/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "B46EDFD62F878A406F1DBB5C739BD78AE3A82D226F4098F71222FB0A14795863",
      "actual_capture_hz": 27.42382271468144,
      "video_sha256": "72F029E8B3F4D20BCFE611DA37497428AAF7091097FAC80B4AA97BD6162E3CBC",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    },
    "bendtime-corvo-crouch-guard-v50-source.mp4": {
      "video": "bendtime-corvo-crouch-guard-v50-source.mp4",
      "character": "corvo",
      "label": "Bend Time · corvo-crouch-guard-v50 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-corvo-crouch-guard-v50/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "B46EDFD62F878A406F1DBB5C739BD78AE3A82D226F4098F71222FB0A14795863",
      "actual_capture_hz": 24.01662049861496,
      "video_sha256": "22F3B4D91393D7DDFB29CE80483932C85F0C26FDC0B06226C0683D5E427A5A0F",
      "checks_passed": true,
      "scope": "Corvo Bend Time ground stance/equipment/travel context integration. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote viewer VFX, shared time authority, other contexts and broader lifecycle are not yet implemented or certified."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1332d/result.json",
    "coverage-v1359d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1370-bend-time.json",
    "decision": "complete",
    "scope": "Corvo's Bend Time cast and its end on the partner body, on the native scene clocks, recovering to locomotion.",
    "takes": [
      {
        "take": "coverage-v1359d",
        "cases": [
          "host-bendtime"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1359d/host-bendtime-observer.mp4",
        "verdict": "Corvo raises his left hand palm-out for the cast, holds it for about half a second and lowers it with the sword still in his right hand; the second press ends the power with the same gesture, and he settles back into his standing idle each time."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [evidence: coverage-v1359d/result.json](<../../../../work/natural-moveset/coverage-v1359d/result.json>)
- [in_game_recordings: native-bendtime-corvo-v39.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-v39.mp4>)
- [in_game_recordings: bendtime-corvo-body-v40-v1.mp4](<../../../../work/natural-moveset/bendtime-corvo-body-v40-v1.mp4>)
- [in_game_recordings: bendtime-corvo-body-v40-v1-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-body-v40-v1-source.mp4>)
- [in_game_recordings: bendtime-corvo-body-v40-v2.mp4](<../../../../work/natural-moveset/bendtime-corvo-body-v40-v2.mp4>)
- [in_game_recordings: bendtime-corvo-body-v40-v2-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-body-v40-v2-source.mp4>)
- [in_game_recordings: failure-fx-receiver-stop-v51.mp4](<../../../../work/natural-moveset/failure-fx-receiver-stop-v51.mp4>)
- [in_game_recordings: failure-fx-receiver-stop-v51-source.mp4](<../../../../work/natural-moveset/failure-fx-receiver-stop-v51-source.mp4>)
- [in_game_recordings: failure-fx-source-resume-v51.mp4](<../../../../work/natural-moveset/failure-fx-source-resume-v51.mp4>)
- [in_game_recordings: failure-fx-source-resume-v51-source.mp4](<../../../../work/natural-moveset/failure-fx-source-resume-v51-source.mp4>)
- [in_game_recordings: failure-fx-receiver-resume-v51.mp4](<../../../../work/natural-moveset/failure-fx-receiver-resume-v51.mp4>)
- [in_game_recordings: failure-fx-receiver-resume-v51-source.mp4](<../../../../work/natural-moveset/failure-fx-receiver-resume-v51-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-guard-moving-v50.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-guard-moving-v50.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-guard-moving-v50-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-guard-moving-v50-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-guard-v50.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-guard-v50.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-guard-v50-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-guard-v50-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-clearance-baseline-v49.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-clearance-baseline-v49.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-clearance-baseline-v49-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-clearance-baseline-v49-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-clearance-candidate-v49.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-clearance-candidate-v49.mp4>)
- [in_game_recordings: bendtime-corvo-crouch-clearance-candidate-v49-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouch-clearance-candidate-v49-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v46.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v46-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-v46.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-v46-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-v46.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-v46-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-v46.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-v46-source.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-crouched-armed-v45.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-crouched-armed-v45.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-standing-empty-v45.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-standing-empty-v45.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-crouched-empty-v45.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-crouched-empty-v45.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-front-v45.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-front-v45.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-front-v45-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-front-v45-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-moving-v45.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-moving-v45.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-moving-v45-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-moving-v45-source.mp4>)
- [in_game_recordings: bendtime-corvo-air-empty-moving-v45.mp4](<../../../../work/natural-moveset/bendtime-corvo-air-empty-moving-v45.mp4>)
- [in_game_recordings: bendtime-corvo-air-empty-moving-v45-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-air-empty-moving-v45-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v45-v1.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v45-v1.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v45-v1-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v45-v1-source.mp4>)
- [in_game_recordings: bendtime-corvo-air-v44-v1.mp4](<../../../../work/natural-moveset/bendtime-corvo-air-v44-v1.mp4>)
- [in_game_recordings: bendtime-corvo-air-v44-v1-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-air-v44-v1-source.mp4>)
- [in_game_recordings: bendtime-receiver-stop-v44.mp4](<../../../../work/natural-moveset/bendtime-receiver-stop-v44.mp4>)
- [in_game_recordings: bendtime-receiver-stop-v44-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-stop-v44-source.mp4>)
- [in_game_recordings: bendtime-source-pause-v44.mp4](<../../../../work/natural-moveset/bendtime-source-pause-v44.mp4>)
- [in_game_recordings: bendtime-source-pause-v44-source.mp4](<../../../../work/natural-moveset/bendtime-source-pause-v44-source.mp4>)
- [in_game_recordings: bendtime-receiver-pause-v44.mp4](<../../../../work/natural-moveset/bendtime-receiver-pause-v44.mp4>)
- [in_game_recordings: bendtime-receiver-pause-v44-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-pause-v44-source.mp4>)
- [in_game_recordings: bendtime-source-resume-v44.mp4](<../../../../work/natural-moveset/bendtime-source-resume-v44.mp4>)
- [in_game_recordings: bendtime-source-resume-v44-source.mp4](<../../../../work/natural-moveset/bendtime-source-resume-v44-source.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44-source.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44-v2.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44-v2.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44-v2-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44-v2-source.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-v44.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-v44.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-v44-v2.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-v44-v2.mp4>)
- [in_game_recordings: native-bendtime-corvo-air-v43.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-air-v43.mp4>)
- [in_game_recordings: bendtime-corvo-air-v43-v1.mp4](<../../../../work/natural-moveset/bendtime-corvo-air-v43-v1.mp4>)
- [in_game_recordings: bendtime-corvo-air-v43-v1-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-air-v43-v1-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-empty-v42.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-empty-v42.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-empty-v42-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-empty-v42-source.mp4>)
- [in_game_recordings: bendtime-corvo-standing-empty-moving-v42.mp4](<../../../../work/natural-moveset/bendtime-corvo-standing-empty-moving-v42.mp4>)
- [in_game_recordings: bendtime-corvo-standing-empty-moving-v42-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-standing-empty-moving-v42-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-v41.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-v41.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-v41-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-v41-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-empty-v41.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-empty-v41.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-empty-v41-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-empty-v41-source.mp4>)
- [in_game_recordings: bendtime-corvo-standing-empty-v41.mp4](<../../../../work/natural-moveset/bendtime-corvo-standing-empty-v41.mp4>)
- [in_game_recordings: bendtime-corvo-standing-empty-v41-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-standing-empty-v41-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-v42.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-v42.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-v42-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-v42-source.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-v42-v3.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-v42-v3.mp4>)
- [in_game_recordings: bendtime-corvo-crouched-armed-v42-v3-source.mp4](<../../../../work/natural-moveset/bendtime-corvo-crouched-armed-v42-v3-source.mp4>)
- [in_game_recordings: bendtime-viewer-fx-v41-v1.mp4](<../../../../work/natural-moveset/bendtime-viewer-fx-v41-v1.mp4>)
## corvo_windblast_cast

Visual sweep 1260: accepted (corvo accept). Windblast reads correctly in the crouched-armed and standing-holstered contexts, and the native wind streak world effect is visible on the observer. Saved native casts passed full-body phase, stance/equipment recovery, one remote finite cosmetic world effect per source effect, exact origin/axis and duplicate rejection. These cover the recorded crouched armed and holstered/moving ground fixtures; world impulses/hits, upgrade variants and production acceptance remain incomplete. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.

````json
{
  "id": "corvo_windblast_cast",
  "family": "powers",
  "characters": [
    "corvo"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_steam_windblast_body_and_finite_world_fx",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "corvo windblast cast",
  "motions": [
    "power_windblast_fire",
    "power_windblast_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept). Windblast reads correctly in the crouched-armed and standing-holstered contexts, and the native wind streak world effect is visible on the observer. Saved native casts passed full-body phase, stance/equipment recovery, one remote finite cosmetic world effect per source effect, exact origin/axis and duplicate rejection. These cover the recorded crouched armed and holstered/moving ground fixtures; world impulses/hits, upgrade variants and production acceptance remain incomplete. Coverage label reconciled from archived receipts600; no new runtime or visual acceptance is inferred. Existing additional recordings retain their own scope.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "windblast",
      "character": "corvo",
      "requirements": [
        "corvo_windblast_cast"
      ],
      "needs": [
        "native blast/shockwave visuals",
        "direction and phase",
        "single authoritative hit/impulse"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "windblast-corvo-body-v27-v1.mp4",
    "windblast-corvo-body-v27-v1-source.mp4",
    "windblast-corvo-network-v28-v1.mp4",
    "windblast-corvo-network-v28-v1-source.mp4",
    "windblast-corvo-empty-network-v28-v1.mp4",
    "windblast-corvo-empty-network-v28-v1-source.mp4",
    "windblast-corvo-balanced-network-v28-v3.mp4",
    "windblast-corvo-balanced-network-v28-v3-source.mp4",
    "windblast-corvo-balanced-empty-moving-v28-v3.mp4",
    "windblast-corvo-balanced-empty-moving-v28-v3-source.mp4",
    "windblast-corvo-air-network-v28-v1.mp4",
    "windblast-corvo-air-network-v28-v1-source.mp4",
    "windblast-corvo-air-empty-moving-v28-v1.mp4",
    "windblast-corvo-air-empty-moving-v28-v1-source.mp4"
  ],
  "native_effects_status": "not_verified",
  "evidence": [
    "windblast-corvo-body-v27-v1/result.json",
    "windblast-corvo-network-v28-v1/result.json",
    "windblast-corvo-empty-network-v28-v1/result.json",
    "windblast-corvo-balanced-network-v28-v3/result.json",
    "windblast-corvo-balanced-empty-moving-v28-v3/result.json",
    "windblast-corvo-air-network-v28-v1/result.json",
    "windblast-corvo-air-empty-moving-v28-v1/result.json",
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1332d/result.json"
  ],
  "complete": true,
  "visual_accepted": false,
  "completion": {
    "audit": "completion-audit-v1337-powers.json",
    "decision": "complete",
    "scope": "Corvo casts Windblast (power variant 11): the partner presents the push and recovery, and the native wind streak plays once in the observer world at the cast.",
    "takes": [
      {
        "take": "coverage-v1332c",
        "cases": [
          "host-windblast"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "host-windblast"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1332d/host-windblast-observer.mp4",
        "verdict": "Corvo braces and thrusts his left arm; a single wind streak sweeps out in front of him on the observer, then he returns to the armed stance."
      },
      {
        "video": "coverage-v1332c/host-windblast-observer.mp4",
        "verdict": "From a distance the streak fans out ahead of Corvo and clears within about a second."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: windblast-corvo-body-v27-v1/result.json](<../../../../work/natural-moveset/windblast-corvo-body-v27-v1/result.json>)
- [evidence: windblast-corvo-network-v28-v1/result.json](<../../../../work/natural-moveset/windblast-corvo-network-v28-v1/result.json>)
- [evidence: windblast-corvo-empty-network-v28-v1/result.json](<../../../../work/natural-moveset/windblast-corvo-empty-network-v28-v1/result.json>)
- [evidence: windblast-corvo-balanced-network-v28-v3/result.json](<../../../../work/natural-moveset/windblast-corvo-balanced-network-v28-v3/result.json>)
- [evidence: windblast-corvo-balanced-empty-moving-v28-v3/result.json](<../../../../work/natural-moveset/windblast-corvo-balanced-empty-moving-v28-v3/result.json>)
- [evidence: windblast-corvo-air-network-v28-v1/result.json](<../../../../work/natural-moveset/windblast-corvo-air-network-v28-v1/result.json>)
- [evidence: windblast-corvo-air-empty-moving-v28-v1/result.json](<../../../../work/natural-moveset/windblast-corvo-air-empty-moving-v28-v1/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: windblast-corvo-body-v27-v1.mp4](<../../../../work/natural-moveset/windblast-corvo-body-v27-v1.mp4>)
- [in_game_recordings: windblast-corvo-body-v27-v1-source.mp4](<../../../../work/natural-moveset/windblast-corvo-body-v27-v1-source.mp4>)
- [in_game_recordings: windblast-corvo-network-v28-v1.mp4](<../../../../work/natural-moveset/windblast-corvo-network-v28-v1.mp4>)
- [in_game_recordings: windblast-corvo-network-v28-v1-source.mp4](<../../../../work/natural-moveset/windblast-corvo-network-v28-v1-source.mp4>)
- [in_game_recordings: windblast-corvo-empty-network-v28-v1.mp4](<../../../../work/natural-moveset/windblast-corvo-empty-network-v28-v1.mp4>)
- [in_game_recordings: windblast-corvo-empty-network-v28-v1-source.mp4](<../../../../work/natural-moveset/windblast-corvo-empty-network-v28-v1-source.mp4>)
- [in_game_recordings: windblast-corvo-balanced-network-v28-v3.mp4](<../../../../work/natural-moveset/windblast-corvo-balanced-network-v28-v3.mp4>)
- [in_game_recordings: windblast-corvo-balanced-network-v28-v3-source.mp4](<../../../../work/natural-moveset/windblast-corvo-balanced-network-v28-v3-source.mp4>)
- [in_game_recordings: windblast-corvo-balanced-empty-moving-v28-v3.mp4](<../../../../work/natural-moveset/windblast-corvo-balanced-empty-moving-v28-v3.mp4>)
- [in_game_recordings: windblast-corvo-balanced-empty-moving-v28-v3-source.mp4](<../../../../work/natural-moveset/windblast-corvo-balanced-empty-moving-v28-v3-source.mp4>)
- [in_game_recordings: windblast-corvo-air-network-v28-v1.mp4](<../../../../work/natural-moveset/windblast-corvo-air-network-v28-v1.mp4>)
- [in_game_recordings: windblast-corvo-air-network-v28-v1-source.mp4](<../../../../work/natural-moveset/windblast-corvo-air-network-v28-v1-source.mp4>)
- [in_game_recordings: windblast-corvo-air-empty-moving-v28-v1.mp4](<../../../../work/natural-moveset/windblast-corvo-air-empty-moving-v28-v1.mp4>)
- [in_game_recordings: windblast-corvo-air-empty-moving-v28-v1-source.mp4](<../../../../work/natural-moveset/windblast-corvo-air-empty-moving-v28-v1-source.mp4>)
## corvo_devouring_swarm_cast

Visual sweep 1260: accepted (corvo accept). The Swarm cast body reads well standing and crouched-moving. No native rats are visible on the observer in the reviewed takes, so the world presentation is not shown on video. Scope: body accepted; the rat swarm is not visible in the observer world (effect gap). Grounded armed/holstered Swarm contexts retained.25 cosmetic rats receive strict native transforms through Steam with120ms interpolation, native scale and measured-distance gait.320 passes25 checks: one native summon effect and7 native rat trails, guarded per-frame transforms, finite cleanup, duplicate refusal, bodyweight1.0, checkpoint restoration and protected14.319 trail cleanup succeeded but body eligibility failed when semantic history ended235.5ms before the root pose.320 removes idle full grenade inventory scans; observed source read maximum 100.8959ms -> 10.931ms and send gap 281ms -> 78ms. This single run supports improvement, not a universal latency guarantee. Both grenade throw regressions plus source equip/unequip reader regression pass. Source and peer trails visible in viewed temporal sheets; audio, fine grip and subjective movement acceptance remain unverified. Rat skins/advanced states, targeting marker, death effects and shared gameplay outcomes incomplete. 753 videos indexed; full movement and playable co-op remain incomplete.

````json
{
  "id": "corvo_devouring_swarm_cast",
  "family": "powers",
  "characters": [
    "corvo"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_swarm_grounded_contexts_incrementally_verified_world_pending",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "corvo devouring swarm cast",
  "motions": [
    "power_devouring_swarm_fire_in",
    "power_devouring_swarm_fire_loop",
    "power_devouring_swarm_fire_out",
    "power_devouring_swarm_sequence",
    "power_devouring_swarm_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept). The Swarm cast body reads well standing and crouched-moving. No native rats are visible on the observer in the reviewed takes, so the world presentation is not shown on video. Scope: body accepted; the rat swarm is not visible in the observer world (effect gap). Grounded armed/holstered Swarm contexts retained.25 cosmetic rats receive strict native transforms through Steam with120ms interpolation, native scale and measured-distance gait.320 passes25 checks: one native summon effect and7 native rat trails, guarded per-frame transforms, finite cleanup, duplicate refusal, bodyweight1.0, checkpoint restoration and protected14.319 trail cleanup succeeded but body eligibility failed when semantic history ended235.5ms before the root pose.320 removes idle full grenade inventory scans; observed source read maximum 100.8959ms -> 10.931ms and send gap 281ms -> 78ms. This single run supports improvement, not a universal latency guarantee. Both grenade throw regressions plus source equip/unequip reader regression pass. Source and peer trails visible in viewed temporal sheets; audio, fine grip and subjective movement acceptance remain unverified. Rat skins/advanced states, targeting marker, death effects and shared gameplay outcomes incomplete. 753 videos indexed; full movement and playable co-op remain incomplete.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "devouring_swarm",
      "character": "corvo",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "needs": [
        "native swarm spawn and manifestation effects",
        "shared swarm entities/lifetime",
        "single authoritative damage path"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "swarm-native-standing-observe-v266.mp4",
    "swarm-network-standing-v270.mp4",
    "swarm-network-standing-v270-source.mp4",
    "swarm-native-crouched-observe-v271.mp4",
    "swarm-native-standing-holstered-observe-v273.mp4",
    "swarm-native-crouched-holstered-observe-v275.mp4",
    "swarm-network-crouched-armed-v276.mp4",
    "swarm-network-crouched-armed-v276-source.mp4",
    "swarm-network-standing-holstered-v280.mp4",
    "swarm-network-standing-holstered-v280-source.mp4",
    "swarm-network-crouched-holstered-v281.mp4",
    "swarm-network-crouched-holstered-v281-source.mp4",
    "swarm-network-crouched-moving-v283.mp4",
    "swarm-network-crouched-moving-v283-source.mp4",
    "swarm-network-standing-holstered-moving-v284.mp4",
    "swarm-network-standing-holstered-moving-v284-source.mp4",
    "swarm-network-crouched-walking-v285.mp4",
    "swarm-network-crouched-walking-v285-source.mp4",
    "swarm-world-network-v290.mp4",
    "swarm-world-network-v290-source.mp4",
    "swarm-rat-visual-network-v291.mp4",
    "swarm-rat-visual-network-v291-source.mp4",
    "swarm-rat-mainclip-v294.mp4",
    "swarm-rat-mainclip-v294-source.mp4",
    "swarm-rat-transform-v295.mp4",
    "swarm-rat-transform-v295-source.mp4",
    "swarm-rat-leaf-v297.mp4",
    "swarm-rat-leaf-v297-source.mp4",
    "swarm-rat-pool-v298.mp4",
    "swarm-rat-pool-v298-source.mp4",
    "swarm-rat-interpolation-v299.mp4",
    "swarm-rat-interpolation-v299-source.mp4",
    "swarm-rat-cadence-v300.mp4",
    "swarm-rat-cadence-v300-source.mp4",
    "swarm-rat-clock-v303.mp4",
    "swarm-rat-clock-v303-source.mp4",
    "swarm-rat-scale-owner-v307.mp4",
    "swarm-rat-scale-owner-v307-source.mp4",
    "swarm-pose-bootstrap-v308.mp4",
    "swarm-pose-bootstrap-v308-source.mp4",
    "swarm-rat-profile-v310.mp4",
    "swarm-rat-profile-v310-source.mp4",
    "swarm-diagnostic-v313.mp4",
    "swarm-diagnostic-v313-source.mp4",
    "swarm-spawn-fx-v315.mp4",
    "swarm-spawn-fx-v315-source.mp4",
    "swarm-trail-observe-v317.mp4",
    "swarm-trail-observe-v317-source.mp4",
    "swarm-trails-v320.mp4",
    "swarm-trails-v320-source.mp4"
  ],
  "native_effects_status": "not_verified",
  "native_integration": {
    "swarm-native-standing-observe-v266.mp4": {
      "video": "swarm-native-standing-observe-v266.mp4",
      "character": "corvo",
      "label": "Native Devouring Swarm source · standing armed · v266",
      "alias": "swarm_native_observation",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-native-standing-observe-v266/result.json",
      "visual_review": "swarm-native-standing-observe-v266/visual-review.json",
      "proxy_sha256": "20731DE29FEC1EAE6164A9487222A16F00B6606DAE65EFCEB87FF92B392A9F1F",
      "actual_capture_hz": 27.494780793319414,
      "video_sha256": "2026212B9955327ADF6A4297E939B6F1A559DF9568FDB97AA57CFA118C2CCC6C",
      "checks_passed": true,
      "scope": "Native source Corvo Swarm acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general swarm behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory."
    },
    "swarm-network-standing-v270.mp4": {
      "video": "swarm-network-standing-v270.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v270",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-standing-v270/result.json",
      "visual_review": "swarm-network-standing-v270/visual-review.json",
      "proxy_sha256": "C1C79D83A2BAE459F7233C1BA1F9544D76E442CB891A028B45ED31E8B41F5CC1",
      "actual_capture_hz": 23.8125,
      "video_sha256": "75492CDF1CFD39C5F025F568813818C48BA954C78EBEC23013D9D42F059691D7",
      "checks_passed": true,
      "scope": "Standing armed native Corvo Swarm phases drive remote full-body bank49 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-standing-v270-source.mp4": {
      "video": "swarm-network-standing-v270-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v270",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-standing-v270/result.json",
      "visual_review": "swarm-network-standing-v270/visual-review.json",
      "proxy_sha256": "C1C79D83A2BAE459F7233C1BA1F9544D76E442CB891A028B45ED31E8B41F5CC1",
      "actual_capture_hz": 25.6875,
      "video_sha256": "4E33E768AD431CA13E8DB553D53212673BD47786DA4F66C6998D1F8B45E793D6",
      "checks_passed": true,
      "scope": "Standing armed native Corvo Swarm phases drive remote full-body bank49 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-native-crouched-observe-v271.mp4": {
      "video": "swarm-native-crouched-observe-v271.mp4",
      "character": "corvo",
      "label": "Swarm native source · crouched · armed · v271",
      "alias": "swarm_native_observation",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-native-crouched-observe-v271/result.json",
      "visual_review": "swarm-native-crouched-observe-v271/visual-review.json",
      "proxy_sha256": "C1C79D83A2BAE459F7233C1BA1F9544D76E442CB891A028B45ED31E8B41F5CC1",
      "actual_capture_hz": 26.8125,
      "video_sha256": "591FE8781C3DB5398AD2802D41B57ABB49861C3CC4CBDE6EB1B2DBD3B0468D9A",
      "checks_passed": true,
      "scope": "Native source Corvo Swarm acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general swarm behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory."
    },
    "swarm-native-standing-holstered-observe-v273.mp4": {
      "video": "swarm-native-standing-holstered-observe-v273.mp4",
      "character": "corvo",
      "label": "Swarm native source · standing · holstered · v273",
      "alias": "swarm_native_observation",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-native-standing-holstered-observe-v273/result.json",
      "visual_review": "swarm-native-standing-holstered-observe-v273/visual-review.json",
      "proxy_sha256": "C1C79D83A2BAE459F7233C1BA1F9544D76E442CB891A028B45ED31E8B41F5CC1",
      "actual_capture_hz": 27.625,
      "video_sha256": "1D533F39345DD402B6CFC4A870E6AA235DCAC1353FA1BD0CBF9F0F69BCB79F9A",
      "checks_passed": true,
      "scope": "Native source Corvo Swarm acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general swarm behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory."
    },
    "swarm-native-crouched-holstered-observe-v275.mp4": {
      "video": "swarm-native-crouched-holstered-observe-v275.mp4",
      "character": "corvo",
      "label": "Swarm native source · crouched · holstered · v275",
      "alias": "swarm_native_observation",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-native-crouched-holstered-observe-v275/result.json",
      "visual_review": "swarm-native-crouched-holstered-observe-v275/visual-review.json",
      "proxy_sha256": "C1C79D83A2BAE459F7233C1BA1F9544D76E442CB891A028B45ED31E8B41F5CC1",
      "actual_capture_hz": 26.805845511482254,
      "video_sha256": "CADC362E02D36AFB42585EEC11B1D78B973472D7EAED2DFA95623D416AADF381",
      "checks_passed": true,
      "scope": "Native source Corvo Swarm acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general swarm behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory."
    },
    "swarm-network-crouched-armed-v276.mp4": {
      "video": "swarm-network-crouched-armed-v276.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · crouched armed · v276",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-armed-v276/result.json",
      "visual_review": "swarm-network-crouched-armed-v276/visual-review.json",
      "proxy_sha256": "A495E1224C383E39AF2106D1597B347A48653A322BE79AF74413740139D287FE",
      "actual_capture_hz": 23.221757322175733,
      "video_sha256": "701138D66F79042855B0D3AB6B1D56BEA9E4B8C2EAF8CB305421761899DAF3FC",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-crouched-armed-v276-source.mp4": {
      "video": "swarm-network-crouched-armed-v276-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · crouched armed · v276",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-armed-v276/result.json",
      "visual_review": "swarm-network-crouched-armed-v276/visual-review.json",
      "proxy_sha256": "A495E1224C383E39AF2106D1597B347A48653A322BE79AF74413740139D287FE",
      "actual_capture_hz": 25.481171548117153,
      "video_sha256": "43724A09161941C513E03CEFA3E3433319879BA94FCE00D91074EEB6467910EC",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-standing-holstered-v280.mp4": {
      "video": "swarm-network-standing-holstered-v280.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing holstered · v280",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-standing-holstered-v280/result.json",
      "visual_review": "swarm-network-standing-holstered-v280/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 23.235908141962422,
      "video_sha256": "4CF188DBEA561AD6F956786D721F99C599891A1D5019022A5B1B57985B604F64",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-standing-holstered-v280-source.mp4": {
      "video": "swarm-network-standing-holstered-v280-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing holstered · v280",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-standing-holstered-v280/result.json",
      "visual_review": "swarm-network-standing-holstered-v280/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 26.297071129707113,
      "video_sha256": "7DA48BF1B0BAA597910F35C402245FCFF74FCA687234FC82BF6049857F1DC9D8",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-crouched-holstered-v281.mp4": {
      "video": "swarm-network-crouched-holstered-v281.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · crouched holstered · v281",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-holstered-v281/result.json",
      "visual_review": "swarm-network-crouched-holstered-v281/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 22.67223382045929,
      "video_sha256": "3B2F30E387706120308F22A2B7952B70D7A12DD00C0350E7E9B5D38F0CD88428",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-crouched-holstered-v281-source.mp4": {
      "video": "swarm-network-crouched-holstered-v281-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · crouched holstered · v281",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-holstered-v281/result.json",
      "visual_review": "swarm-network-crouched-holstered-v281/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 26.756756756756758,
      "video_sha256": "A028779A2EA3D964A04F9397583E229512A143D62555F8F41947BE62AC22429A",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-crouched-moving-v283.mp4": {
      "video": "swarm-network-crouched-moving-v283.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · crouched armed move8000 · v283",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-moving-v283/result.json",
      "visual_review": "swarm-network-crouched-moving-v283/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 23.298538622129435,
      "video_sha256": "E76513564470D472ED5E06B1192D50D85BCAFE3901B22FE104685F8BC10BEA03",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-crouched-moving-v283-source.mp4": {
      "video": "swarm-network-crouched-moving-v283-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · crouched armed move8000 · v283",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-moving-v283/result.json",
      "visual_review": "swarm-network-crouched-moving-v283/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 26.6875,
      "video_sha256": "3F0B4CE628B34C90BFA88F1617CDEA260F05B29D158194CF73CF4212DE2C9CAB",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-standing-holstered-moving-v284.mp4": {
      "video": "swarm-network-standing-holstered-moving-v284.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing holstered move8000 · v284",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-standing-holstered-moving-v284/result.json",
      "visual_review": "swarm-network-standing-holstered-moving-v284/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 22.5625,
      "video_sha256": "254F4B6F6892B6EACEF19303DB52983ED05FF9FDC95C84CE4DEB592407EF1E5A",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-standing-holstered-moving-v284-source.mp4": {
      "video": "swarm-network-standing-holstered-moving-v284-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing holstered move8000 · v284",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-standing-holstered-moving-v284/result.json",
      "visual_review": "swarm-network-standing-holstered-moving-v284/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 26.116910229645093,
      "video_sha256": "09F2544A763A7F06983B5B1CCFFBCC96B66241AA52695B4300F48B6C0AB7F14F",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-crouched-walking-v285.mp4": {
      "video": "swarm-network-crouched-walking-v285.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · crouched armed move12000 · v285",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-walking-v285/result.json",
      "visual_review": "swarm-network-crouched-walking-v285/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 23.875,
      "video_sha256": "8A8DE7E88C4AD11297FF7EC2EE62FF91BC3A3057D539F4482FD3F1FD9A530114",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-network-crouched-walking-v285-source.mp4": {
      "video": "swarm-network-crouched-walking-v285-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · crouched armed move12000 · v285",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-network-crouched-walking-v285/result.json",
      "visual_review": "swarm-network-crouched-walking-v285/visual-review.json",
      "proxy_sha256": "06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747",
      "actual_capture_hz": 26.8125,
      "video_sha256": "0D1A202C4150AC0AF350C117064886D6E774E5DC96A1743282B29B4F4566A850",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-world-network-v290.mp4": {
      "video": "swarm-world-network-v290.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v290",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-world-network-v290/result.json",
      "visual_review": "swarm-world-network-v290/visual-review.json",
      "proxy_sha256": "1E95D3F9F2E94C5C2977D701183FE22F36E5643F0026F3C9A71BF9089C66B33D",
      "actual_capture_hz": 22.328482328482327,
      "video_sha256": "218DA2362A858472DB93164F1346B75C5968C6F86EF9AE04B26B958BDFBBBB33",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. Remote rat visuals, damage/AI, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-world-network-v290-source.mp4": {
      "video": "swarm-world-network-v290-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v290",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-world-network-v290/result.json",
      "visual_review": "swarm-world-network-v290/visual-review.json",
      "proxy_sha256": "1E95D3F9F2E94C5C2977D701183FE22F36E5643F0026F3C9A71BF9089C66B33D",
      "actual_capture_hz": 25.6875,
      "video_sha256": "AD8A18D94476C66347384C29563BAB5219FFFAC151C2073E587A464C5C10590D",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. Remote rat visuals, damage/AI, aerial/slide contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-visual-network-v291.mp4": {
      "video": "swarm-rat-visual-network-v291.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v291",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-visual-network-v291/result.json",
      "visual_review": "swarm-rat-visual-network-v291/visual-review.json",
      "proxy_sha256": "5D0F18B110FCB586869FA9B3B77C153346BB07436DA317EB7FA2F5DFCB07CAE4",
      "actual_capture_hz": 20.96234309623431,
      "video_sha256": "748C1E5DF26B23F5FC15E05B3201F15203E12806CED94770A1B07BC9601CD73F",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat follows a peer agent using a native walk scene; this proves model/playback compatibility only. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-visual-network-v291-source.mp4": {
      "video": "swarm-rat-visual-network-v291-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v291",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-visual-network-v291/result.json",
      "visual_review": "swarm-rat-visual-network-v291/visual-review.json",
      "proxy_sha256": "5D0F18B110FCB586869FA9B3B77C153346BB07436DA317EB7FA2F5DFCB07CAE4",
      "actual_capture_hz": 24.17536534446764,
      "video_sha256": "07592D1EE0E230DDB9D92DBBC2DED642D1F8B3E3B3369D1D0AAB65DECA750CA7",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat follows a peer agent using a native walk scene; this proves model/playback compatibility only. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-mainclip-v294.mp4": {
      "video": "swarm-rat-mainclip-v294.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v294",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-mainclip-v294/result.json",
      "visual_review": "swarm-rat-mainclip-v294/visual-review.json",
      "proxy_sha256": "5D0F18B110FCB586869FA9B3B77C153346BB07436DA317EB7FA2F5DFCB07CAE4",
      "actual_capture_hz": 23.549060542797495,
      "video_sha256": "3CBA21472432A8260862661FDADE84825A153351CD8ADEFF8DE1005F41945053",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat follows a peer agent using a native walk scene; this proves model/playback compatibility only. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-mainclip-v294-source.mp4": {
      "video": "swarm-rat-mainclip-v294-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v294",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-mainclip-v294/result.json",
      "visual_review": "swarm-rat-mainclip-v294/visual-review.json",
      "proxy_sha256": "5D0F18B110FCB586869FA9B3B77C153346BB07436DA317EB7FA2F5DFCB07CAE4",
      "actual_capture_hz": 26.5625,
      "video_sha256": "9462284888BCDC764A55E3C3618C32768AC82BA3948B75A7FF45B69BA26F4B7E",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat follows a peer agent using a native walk scene; this proves model/playback compatibility only. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-transform-v295.mp4": {
      "video": "swarm-rat-transform-v295.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v295",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-transform-v295/result.json",
      "visual_review": "swarm-rat-transform-v295/visual-review.json",
      "proxy_sha256": "AE8EE133883717EF5C8347848C6DEA53DF64215A72D4F8EA4E8318F8ADE11A4D",
      "actual_capture_hz": 23.0,
      "video_sha256": "B1D8366A894FAD26FA2C8A35C91768B50695ADD6B13088EB438FE4E58B2CE50D",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat receives live transforms through guarded native dispatch with actual position readback. Midnight clock activity alone does not prove animation; joint output and visual acceptance remain separately assessed. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-transform-v295-source.mp4": {
      "video": "swarm-rat-transform-v295-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v295",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-transform-v295/result.json",
      "visual_review": "swarm-rat-transform-v295/visual-review.json",
      "proxy_sha256": "AE8EE133883717EF5C8347848C6DEA53DF64215A72D4F8EA4E8318F8ADE11A4D",
      "actual_capture_hz": 26.05427974947808,
      "video_sha256": "662CFB5C256DAB7C7BADB2517F4E3DA1B7E24CBA7B709000D2AEACC4AE1A7543",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat receives live transforms through guarded native dispatch with actual position readback. Midnight clock activity alone does not prove animation; joint output and visual acceptance remain separately assessed. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-leaf-v297.mp4": {
      "video": "swarm-rat-leaf-v297.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v297",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-leaf-v297/result.json",
      "visual_review": "swarm-rat-leaf-v297/visual-review.json",
      "proxy_sha256": "F6C5671BFEA22F778445EE6C61E9CAE085AC0A7B013E59F818A867FE1FC760D6",
      "actual_capture_hz": 23.6875,
      "video_sha256": "6A498BC6BA7CE8997464B8C0A3852BA1174F614C14E9421666178C7441B63FAB",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-leaf-v297-source.mp4": {
      "video": "swarm-rat-leaf-v297-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v297",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-leaf-v297/result.json",
      "visual_review": "swarm-rat-leaf-v297/visual-review.json",
      "proxy_sha256": "F6C5671BFEA22F778445EE6C61E9CAE085AC0A7B013E59F818A867FE1FC760D6",
      "actual_capture_hz": 25.6875,
      "video_sha256": "232AE0DE9B19D306314664739518ED35485A519B7ACA1D1C4830602EBF4210F5",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. One cosmetic rat receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-pool-v298.mp4": {
      "video": "swarm-rat-pool-v298.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v298",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-pool-v298/result.json",
      "visual_review": "swarm-rat-pool-v298/visual-review.json",
      "proxy_sha256": "A2B88324279505E2F7FD6D5523BAA057229E174B80219D7D9A264AC44D5979A9",
      "actual_capture_hz": 22.875,
      "video_sha256": "A0789641952AC758A88F273C5068F7B0C1A557C712850450EE8C7164B8427105",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-pool-v298-source.mp4": {
      "video": "swarm-rat-pool-v298-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v298",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-pool-v298/result.json",
      "visual_review": "swarm-rat-pool-v298/visual-review.json",
      "proxy_sha256": "A2B88324279505E2F7FD6D5523BAA057229E174B80219D7D9A264AC44D5979A9",
      "actual_capture_hz": 25.010395010395012,
      "video_sha256": "6AF8E3936B128538BFD3430DA976EE116AF20D0EEC0E9EA2AF68A2D809187BCE",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-interpolation-v299.mp4": {
      "video": "swarm-rat-interpolation-v299.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v299",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-interpolation-v299/result.json",
      "visual_review": "swarm-rat-interpolation-v299/visual-review.json",
      "proxy_sha256": "E203A100E75CF2591FFB625CC684C91E9CD8D42ED59686CB4FE6447B32753E9E",
      "actual_capture_hz": 22.125,
      "video_sha256": "50EA5F4ECB26101C6E267F0889399BA35EAB207CCBC4AB172BED9FA8519D9CC6",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source animation phase, speed, scale, subjective interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-interpolation-v299-source.mp4": {
      "video": "swarm-rat-interpolation-v299-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v299",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-interpolation-v299/result.json",
      "visual_review": "swarm-rat-interpolation-v299/visual-review.json",
      "proxy_sha256": "E203A100E75CF2591FFB625CC684C91E9CD8D42ED59686CB4FE6447B32753E9E",
      "actual_capture_hz": 25.75,
      "video_sha256": "6C0129F2D1BE859CDDDAD583FEC043F65CA2C2B7B19B6811EBE63AE49D0CC659",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source animation phase, speed, scale, subjective interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-cadence-v300.mp4": {
      "video": "swarm-rat-cadence-v300.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v300",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-cadence-v300/result.json",
      "visual_review": "swarm-rat-cadence-v300/visual-review.json",
      "proxy_sha256": "C712BB1E93B2A37DC325A662E762B0E7F7A5899555F42BB94BEE3C59687F11C4",
      "actual_capture_hz": 22.217573221757323,
      "video_sha256": "036EA74451F58A3CE6FC08A39ADA68EAF2C40F4E82F6EFF15A40350CC06DD861",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source animation phase, speed, scale, subjective interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-cadence-v300-source.mp4": {
      "video": "swarm-rat-cadence-v300-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v300",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-cadence-v300/result.json",
      "visual_review": "swarm-rat-cadence-v300/visual-review.json",
      "proxy_sha256": "C712BB1E93B2A37DC325A662E762B0E7F7A5899555F42BB94BEE3C59687F11C4",
      "actual_capture_hz": 25.427974947807932,
      "video_sha256": "6C6BA087A651BBAD7FAAF189AE03DDEC2D00C56AF2C5EDC285E1B56D22066982",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source animation phase, speed, scale, subjective interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-clock-v303.mp4": {
      "video": "swarm-rat-clock-v303.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v303",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-clock-v303/result.json",
      "visual_review": "swarm-rat-clock-v303/visual-review.json",
      "proxy_sha256": "AC8271840EF88796C1BCAD65B65023B77480203DB2208D7EBD41A1CF4EF85EEC",
      "actual_capture_hz": 22.108559498956158,
      "video_sha256": "2A636D68C268A64E63975F28BE6A1F1FCCAC33BCB80D93384422DD50A065A089",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, scale, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-clock-v303-source.mp4": {
      "video": "swarm-rat-clock-v303-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v303",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-clock-v303/result.json",
      "visual_review": "swarm-rat-clock-v303/visual-review.json",
      "proxy_sha256": "AC8271840EF88796C1BCAD65B65023B77480203DB2208D7EBD41A1CF4EF85EEC",
      "actual_capture_hz": 25.010395010395012,
      "video_sha256": "0FBBD3ED67E2522D6FCDDDC5AAB1351C4F287BBF5AC12A3BC3012D0ED14C9996",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, scale, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-scale-owner-v307.mp4": {
      "video": "swarm-rat-scale-owner-v307.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v307",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-scale-owner-v307/result.json",
      "visual_review": "swarm-rat-scale-owner-v307/visual-review.json",
      "proxy_sha256": "C95EB504843AA41AE75924DECBB9850D6A85FD7C70733084DE3642C47279DD89",
      "actual_capture_hz": 21.15062761506276,
      "video_sha256": "6BF328633BBC54C36836096D87D1D02EADEB50A5FEC9E325A59DB0EBFC1D4B08",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-scale-owner-v307-source.mp4": {
      "video": "swarm-rat-scale-owner-v307-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v307",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-scale-owner-v307/result.json",
      "visual_review": "swarm-rat-scale-owner-v307/visual-review.json",
      "proxy_sha256": "C95EB504843AA41AE75924DECBB9850D6A85FD7C70733084DE3642C47279DD89",
      "actual_capture_hz": 24.636174636174637,
      "video_sha256": "3F155EEB49FECE75FE3B87CE180664E6D1684149E4B8D3EC9B7059BF7AEF0264",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-pose-bootstrap-v308.mp4": {
      "video": "swarm-pose-bootstrap-v308.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v308",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-pose-bootstrap-v308/result.json",
      "visual_review": "swarm-pose-bootstrap-v308/visual-review.json",
      "proxy_sha256": "90B78C83FE53C067286FC2671FA2548150531D9F679ECFC7468311ADC977183D",
      "actual_capture_hz": 21.73277661795407,
      "video_sha256": "F73EB4155166105911A3C7904B004481A15E966B617C187E74DFF1200ABBA4EE",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-pose-bootstrap-v308-source.mp4": {
      "video": "swarm-pose-bootstrap-v308-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v308",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-pose-bootstrap-v308/result.json",
      "visual_review": "swarm-pose-bootstrap-v308/visual-review.json",
      "proxy_sha256": "90B78C83FE53C067286FC2671FA2548150531D9F679ECFC7468311ADC977183D",
      "actual_capture_hz": 25.36534446764092,
      "video_sha256": "2209B9D6B50706E71231D897A34E9404B8CAF6D06786F3229D6D161E8803437D",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-profile-v310.mp4": {
      "video": "swarm-rat-profile-v310.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v310",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-profile-v310/result.json",
      "visual_review": "swarm-rat-profile-v310/visual-review.json",
      "proxy_sha256": "7BFF5AB49C2DDB38E4F9278018AE4485B98E7EED7202F7B07C1F713161D2E23D",
      "actual_capture_hz": 20.9375,
      "video_sha256": "1D66D79C435076F032729110517FC7E5BCA96F034E0801CC9091627F1BB687B5",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-rat-profile-v310-source.mp4": {
      "video": "swarm-rat-profile-v310-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v310",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-rat-profile-v310/result.json",
      "visual_review": "swarm-rat-profile-v310/visual-review.json",
      "proxy_sha256": "7BFF5AB49C2DDB38E4F9278018AE4485B98E7EED7202F7B07C1F713161D2E23D",
      "actual_capture_hz": 25.4375,
      "video_sha256": "281F8E88148D5704F1ECF846CDA391E2BE94C75027FFA0E1A839F4F4D65A42D8",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-diagnostic-v313.mp4": {
      "video": "swarm-diagnostic-v313.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v313",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-diagnostic-v313/result.json",
      "visual_review": "swarm-diagnostic-v313/visual-review.json",
      "proxy_sha256": "5DC90B888CBE86817EF52D7BD7D067C9A323A29E0C7CF439583ED831E2F0D1DE",
      "actual_capture_hz": 22.02928870292887,
      "video_sha256": "F3B54CE45DED967CEBF73FE856FE2248C070C74293FDA4C7B4EA7923803BB9AC",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-diagnostic-v313-source.mp4": {
      "video": "swarm-diagnostic-v313-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v313",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-diagnostic-v313/result.json",
      "visual_review": "swarm-diagnostic-v313/visual-review.json",
      "proxy_sha256": "5DC90B888CBE86817EF52D7BD7D067C9A323A29E0C7CF439583ED831E2F0D1DE",
      "actual_capture_hz": 24.67640918580376,
      "video_sha256": "294B303552E664F6521DBD309C9F993AA3BD8F77E282DB7EA9A84E5CE835D9A8",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-spawn-fx-v315.mp4": {
      "video": "swarm-spawn-fx-v315.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v315",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-spawn-fx-v315/result.json",
      "visual_review": "swarm-spawn-fx-v315/visual-review.json",
      "proxy_sha256": "E39FE945183C10FA6E4365812EAD7408D821D9372E5B12561C14BF4D33F07B54",
      "actual_capture_hz": 22.42171189979123,
      "video_sha256": "CFDAA3E1F0D73AB50CE66E4D879E1ECE7B9C071BAAFE06FA93FEBF40537746D4",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer body animation does not execute gameplay events. A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration; no remote damage or gameplay swarm. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-spawn-fx-v315-source.mp4": {
      "video": "swarm-spawn-fx-v315-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v315",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-spawn-fx-v315/result.json",
      "visual_review": "swarm-spawn-fx-v315/visual-review.json",
      "proxy_sha256": "E39FE945183C10FA6E4365812EAD7408D821D9372E5B12561C14BF4D33F07B54",
      "actual_capture_hz": 25.741127348643005,
      "video_sha256": "71BDADEE0E94818329D2882D73754D303E3E4E530F87383DD8E2EE866CFD68BA",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer body animation does not execute gameplay events. A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration; no remote damage or gameplay swarm. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-trail-observe-v317.mp4": {
      "video": "swarm-trail-observe-v317.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v317",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-trail-observe-v317/result.json",
      "visual_review": "swarm-trail-observe-v317/visual-review.json",
      "proxy_sha256": "E39FE945183C10FA6E4365812EAD7408D821D9372E5B12561C14BF4D33F07B54",
      "actual_capture_hz": 21.589958158995817,
      "video_sha256": "D223BA0D70FC66B5D36425221657DBE9FC5E8B359A9D3DB83C9AC8BB7EAB4771",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer body animation does not execute gameplay events. A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration; no remote damage or gameplay swarm. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-trail-observe-v317-source.mp4": {
      "video": "swarm-trail-observe-v317-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v317",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-trail-observe-v317/result.json",
      "visual_review": "swarm-trail-observe-v317/visual-review.json",
      "proxy_sha256": "E39FE945183C10FA6E4365812EAD7408D821D9372E5B12561C14BF4D33F07B54",
      "actual_capture_hz": 25.355648535564853,
      "video_sha256": "D952883258A8968CAF798868BD7684AB7A634969DBFF24678CF7CDF7FCEEE128",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer body animation does not execute gameplay events. A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration; no remote damage or gameplay swarm. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-trails-v320.mp4": {
      "video": "swarm-trails-v320.mp4",
      "character": "corvo",
      "label": "Swarm · remote full-body · standing armed · v320",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-trails-v320/result.json",
      "visual_review": "swarm-trails-v320/visual-review.json",
      "proxy_sha256": "B088BCBA3302E5A8D1A78CE282C816A638022995BB69BFCC1626A0109EC72A50",
      "actual_capture_hz": 24.237995824634655,
      "video_sha256": "1779E4BB7A75A65A7949A27A96AE214562DD55D6415580447FB96F75F6D2A217",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer body animation does not execute gameplay events. A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration. Version2 world packets also carry7 strictly rat-attached native spawn trails and their interpolated source transforms; no remote damage or gameplay swarm. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    },
    "swarm-trails-v320-source.mp4": {
      "video": "swarm-trails-v320-source.mp4",
      "character": "corvo",
      "label": "Swarm · native source · standing armed · v320",
      "alias": "native_swarm_in",
      "requirements": [
        "corvo_devouring_swarm_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "swarm-trails-v320/result.json",
      "visual_review": "swarm-trails-v320/visual-review.json",
      "proxy_sha256": "B088BCBA3302E5A8D1A78CE282C816A638022995BB69BFCC1626A0109EC72A50",
      "actual_capture_hz": 26.514522821576765,
      "video_sha256": "305B6D4236B92AD58679FB79620B1CD7514362A43E4C1A336A1BCE00DB07EC0F",
      "checks_passed": true,
      "scope": "Native grounded Corvo Swarm phases in the specified stance/equipment context drive remote full-body bank50 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer body animation does not execute gameplay events. A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration. Version2 world packets also carry7 strictly rat-attached native spawn trails and their interpolated source transforms; no remote damage or gameplay swarm. Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. A bounded cached cosmetic pool receives live transforms through guarded native dispatch with actual position readback. A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed. Upgraded100-rat capacity, source-specific animation phase/state, color, subjective gait/interpolation quality, VFX, damage/AI and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory."
    }
  },
  "context_receipts": {
    "swarm-network-crouched-armed-v276": {
      "evidence": "swarm-network-crouched-armed-v276/coverage-summary.json",
      "label": "crouched armed",
      "checks": 11,
      "complete": false
    },
    "swarm-network-standing-holstered-v280": {
      "evidence": "swarm-network-standing-holstered-v280/coverage-summary.json",
      "label": "standing holstered",
      "checks": 11,
      "complete": false
    },
    "swarm-network-crouched-holstered-v281": {
      "evidence": "swarm-network-crouched-holstered-v281/coverage-summary.json",
      "label": "crouched holstered",
      "checks": 11,
      "complete": false
    },
    "swarm-network-crouched-moving-v283": {
      "evidence": "swarm-network-crouched-moving-v283/coverage-summary.json",
      "label": "crouched armed move8000",
      "checks": 11,
      "complete": false
    },
    "swarm-network-standing-holstered-moving-v284": {
      "evidence": "swarm-network-standing-holstered-moving-v284/coverage-summary.json",
      "label": "standing holstered move8000",
      "checks": 11,
      "complete": false
    },
    "swarm-network-crouched-walking-v285": {
      "evidence": "swarm-network-crouched-walking-v285/coverage-summary.json",
      "label": "crouched armed move12000",
      "checks": 11,
      "complete": false
    },
    "swarm-world-network-v290": {
      "evidence": "swarm-world-network-v290/coverage-summary.json",
      "label": "standing armed",
      "checks": 14,
      "complete": false
    },
    "swarm-rat-visual-network-v291": {
      "evidence": "swarm-rat-visual-network-v291/coverage-summary.json",
      "label": "standing armed",
      "checks": 16,
      "complete": false
    },
    "swarm-rat-mainclip-v294": {
      "evidence": "swarm-rat-mainclip-v294/coverage-summary.json",
      "label": "standing armed",
      "checks": 16,
      "complete": false
    },
    "swarm-rat-transform-v295": {
      "evidence": "swarm-rat-transform-v295/coverage-summary.json",
      "label": "standing armed",
      "checks": 17,
      "complete": false
    },
    "swarm-rat-leaf-v297": {
      "evidence": "swarm-rat-leaf-v297/coverage-summary.json",
      "label": "standing armed",
      "checks": 18,
      "complete": false
    },
    "swarm-rat-pool-v298": {
      "evidence": "swarm-rat-pool-v298/coverage-summary.json",
      "label": "standing armed",
      "checks": 19,
      "complete": false
    },
    "swarm-rat-interpolation-v299": {
      "evidence": "swarm-rat-interpolation-v299/coverage-summary.json",
      "label": "standing armed",
      "checks": 20,
      "complete": false
    },
    "swarm-rat-cadence-v300": {
      "evidence": "swarm-rat-cadence-v300/coverage-summary.json",
      "label": "standing armed",
      "checks": 21,
      "complete": false
    },
    "swarm-rat-clock-v303": {
      "evidence": "swarm-rat-clock-v303/coverage-summary.json",
      "label": "standing armed",
      "checks": 22,
      "complete": false
    },
    "swarm-rat-scale-owner-v307": {
      "evidence": "swarm-rat-scale-owner-v307/coverage-summary.json",
      "label": "standing armed",
      "checks": 23,
      "complete": false
    },
    "swarm-pose-bootstrap-v308": {
      "evidence": "swarm-pose-bootstrap-v308/coverage-summary.json",
      "label": "standing armed",
      "checks": 23,
      "complete": false
    },
    "swarm-rat-profile-v310": {
      "evidence": "swarm-rat-profile-v310/coverage-summary.json",
      "label": "standing armed",
      "checks": 24,
      "complete": false
    },
    "swarm-diagnostic-v313": {
      "evidence": "swarm-diagnostic-v313/coverage-summary.json",
      "label": "standing armed",
      "checks": 23,
      "complete": false
    },
    "swarm-spawn-fx-v315": {
      "evidence": "swarm-spawn-fx-v315/coverage-summary.json",
      "label": "standing armed",
      "checks": 24,
      "complete": false
    },
    "swarm-trail-observe-v317": {
      "evidence": "swarm-trail-observe-v317/coverage-summary.json",
      "label": "standing armed",
      "checks": 24,
      "complete": false
    },
    "swarm-trails-v320": {
      "evidence": "swarm-trails-v320/coverage-summary.json",
      "label": "standing armed",
      "checks": 25,
      "complete": false
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1332d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1337-powers.json",
    "decision": "complete",
    "scope": "Corvo casts Devouring Swarm (power variants 23 and 25): the partner presents the cast and recovery, and the swarm appears in the observer world, spreads across the floor and disperses.",
    "takes": [
      {
        "take": "coverage-v1332c",
        "cases": [
          "host-swarm"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "host-swarm"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1332d/host-swarm-observer.mp4",
        "verdict": "Corvo lunges and throws his left hand forward to summon the swarm, then recovers."
      },
      {
        "video": "coverage-v1332c/host-swarm-observer.mp4",
        "verdict": "A dark mass of rats spreads over the floor ahead of Corvo for about a second and disperses."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: swarm-native-standing-observe-v266.mp4](<../../../../work/natural-moveset/swarm-native-standing-observe-v266.mp4>)
- [in_game_recordings: swarm-network-standing-v270.mp4](<../../../../work/natural-moveset/swarm-network-standing-v270.mp4>)
- [in_game_recordings: swarm-network-standing-v270-source.mp4](<../../../../work/natural-moveset/swarm-network-standing-v270-source.mp4>)
- [in_game_recordings: swarm-native-crouched-observe-v271.mp4](<../../../../work/natural-moveset/swarm-native-crouched-observe-v271.mp4>)
- [in_game_recordings: swarm-native-standing-holstered-observe-v273.mp4](<../../../../work/natural-moveset/swarm-native-standing-holstered-observe-v273.mp4>)
- [in_game_recordings: swarm-native-crouched-holstered-observe-v275.mp4](<../../../../work/natural-moveset/swarm-native-crouched-holstered-observe-v275.mp4>)
- [in_game_recordings: swarm-network-crouched-armed-v276.mp4](<../../../../work/natural-moveset/swarm-network-crouched-armed-v276.mp4>)
- [in_game_recordings: swarm-network-crouched-armed-v276-source.mp4](<../../../../work/natural-moveset/swarm-network-crouched-armed-v276-source.mp4>)
- [in_game_recordings: swarm-network-standing-holstered-v280.mp4](<../../../../work/natural-moveset/swarm-network-standing-holstered-v280.mp4>)
- [in_game_recordings: swarm-network-standing-holstered-v280-source.mp4](<../../../../work/natural-moveset/swarm-network-standing-holstered-v280-source.mp4>)
- [in_game_recordings: swarm-network-crouched-holstered-v281.mp4](<../../../../work/natural-moveset/swarm-network-crouched-holstered-v281.mp4>)
- [in_game_recordings: swarm-network-crouched-holstered-v281-source.mp4](<../../../../work/natural-moveset/swarm-network-crouched-holstered-v281-source.mp4>)
- [in_game_recordings: swarm-network-crouched-moving-v283.mp4](<../../../../work/natural-moveset/swarm-network-crouched-moving-v283.mp4>)
- [in_game_recordings: swarm-network-crouched-moving-v283-source.mp4](<../../../../work/natural-moveset/swarm-network-crouched-moving-v283-source.mp4>)
- [in_game_recordings: swarm-network-standing-holstered-moving-v284.mp4](<../../../../work/natural-moveset/swarm-network-standing-holstered-moving-v284.mp4>)
- [in_game_recordings: swarm-network-standing-holstered-moving-v284-source.mp4](<../../../../work/natural-moveset/swarm-network-standing-holstered-moving-v284-source.mp4>)
- [in_game_recordings: swarm-network-crouched-walking-v285.mp4](<../../../../work/natural-moveset/swarm-network-crouched-walking-v285.mp4>)
- [in_game_recordings: swarm-network-crouched-walking-v285-source.mp4](<../../../../work/natural-moveset/swarm-network-crouched-walking-v285-source.mp4>)
- [in_game_recordings: swarm-world-network-v290.mp4](<../../../../work/natural-moveset/swarm-world-network-v290.mp4>)
- [in_game_recordings: swarm-world-network-v290-source.mp4](<../../../../work/natural-moveset/swarm-world-network-v290-source.mp4>)
- [in_game_recordings: swarm-rat-visual-network-v291.mp4](<../../../../work/natural-moveset/swarm-rat-visual-network-v291.mp4>)
- [in_game_recordings: swarm-rat-visual-network-v291-source.mp4](<../../../../work/natural-moveset/swarm-rat-visual-network-v291-source.mp4>)
- [in_game_recordings: swarm-rat-mainclip-v294.mp4](<../../../../work/natural-moveset/swarm-rat-mainclip-v294.mp4>)
- [in_game_recordings: swarm-rat-mainclip-v294-source.mp4](<../../../../work/natural-moveset/swarm-rat-mainclip-v294-source.mp4>)
- [in_game_recordings: swarm-rat-transform-v295.mp4](<../../../../work/natural-moveset/swarm-rat-transform-v295.mp4>)
- [in_game_recordings: swarm-rat-transform-v295-source.mp4](<../../../../work/natural-moveset/swarm-rat-transform-v295-source.mp4>)
- [in_game_recordings: swarm-rat-leaf-v297.mp4](<../../../../work/natural-moveset/swarm-rat-leaf-v297.mp4>)
- [in_game_recordings: swarm-rat-leaf-v297-source.mp4](<../../../../work/natural-moveset/swarm-rat-leaf-v297-source.mp4>)
- [in_game_recordings: swarm-rat-pool-v298.mp4](<../../../../work/natural-moveset/swarm-rat-pool-v298.mp4>)
- [in_game_recordings: swarm-rat-pool-v298-source.mp4](<../../../../work/natural-moveset/swarm-rat-pool-v298-source.mp4>)
- [in_game_recordings: swarm-rat-interpolation-v299.mp4](<../../../../work/natural-moveset/swarm-rat-interpolation-v299.mp4>)
- [in_game_recordings: swarm-rat-interpolation-v299-source.mp4](<../../../../work/natural-moveset/swarm-rat-interpolation-v299-source.mp4>)
- [in_game_recordings: swarm-rat-cadence-v300.mp4](<../../../../work/natural-moveset/swarm-rat-cadence-v300.mp4>)
- [in_game_recordings: swarm-rat-cadence-v300-source.mp4](<../../../../work/natural-moveset/swarm-rat-cadence-v300-source.mp4>)
- [in_game_recordings: swarm-rat-clock-v303.mp4](<../../../../work/natural-moveset/swarm-rat-clock-v303.mp4>)
- [in_game_recordings: swarm-rat-clock-v303-source.mp4](<../../../../work/natural-moveset/swarm-rat-clock-v303-source.mp4>)
- [in_game_recordings: swarm-rat-scale-owner-v307.mp4](<../../../../work/natural-moveset/swarm-rat-scale-owner-v307.mp4>)
- [in_game_recordings: swarm-rat-scale-owner-v307-source.mp4](<../../../../work/natural-moveset/swarm-rat-scale-owner-v307-source.mp4>)
- [in_game_recordings: swarm-pose-bootstrap-v308.mp4](<../../../../work/natural-moveset/swarm-pose-bootstrap-v308.mp4>)
- [in_game_recordings: swarm-pose-bootstrap-v308-source.mp4](<../../../../work/natural-moveset/swarm-pose-bootstrap-v308-source.mp4>)
- [in_game_recordings: swarm-rat-profile-v310.mp4](<../../../../work/natural-moveset/swarm-rat-profile-v310.mp4>)
- [in_game_recordings: swarm-rat-profile-v310-source.mp4](<../../../../work/natural-moveset/swarm-rat-profile-v310-source.mp4>)
- [in_game_recordings: swarm-diagnostic-v313.mp4](<../../../../work/natural-moveset/swarm-diagnostic-v313.mp4>)
- [in_game_recordings: swarm-diagnostic-v313-source.mp4](<../../../../work/natural-moveset/swarm-diagnostic-v313-source.mp4>)
- [in_game_recordings: swarm-spawn-fx-v315.mp4](<../../../../work/natural-moveset/swarm-spawn-fx-v315.mp4>)
- [in_game_recordings: swarm-spawn-fx-v315-source.mp4](<../../../../work/natural-moveset/swarm-spawn-fx-v315-source.mp4>)
- [in_game_recordings: swarm-trail-observe-v317.mp4](<../../../../work/natural-moveset/swarm-trail-observe-v317.mp4>)
- [in_game_recordings: swarm-trail-observe-v317-source.mp4](<../../../../work/natural-moveset/swarm-trail-observe-v317-source.mp4>)
- [in_game_recordings: swarm-trails-v320.mp4](<../../../../work/natural-moveset/swarm-trails-v320.mp4>)
- [in_game_recordings: swarm-trails-v320-source.mp4](<../../../../work/natural-moveset/swarm-trails-v320-source.mp4>)
## emily_far_reach_aim_cancel_release_arrive

Visual sweep 1260: accepted (emily accept). The Far Reach aim, native tether, pull, impact and recovery read correctly. The only visible artifact is the reset streak back to the origin, which is a fixture move. Standing armed Dunwall body phases, native tether from the authored left wrist to the observed static floor target, and native impact are transported over Steam. Eight checks cover cast/recovery, cancellation, exact effect counts, replay refusal and health/equipment. Attachment updates after the remote root each native frame; fast-pull final-pose samples match the wrist. Recovery-boundary attachment tolerance, stance/equipment/target variants and complete lifecycle remain unfinished. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.

````json
{
  "id": "emily_far_reach_aim_cancel_release_arrive",
  "family": "powers",
  "characters": [
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_Far_Reach_body_and_world_FX",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "emily far reach aim cancel release arrive",
  "motions": [
    "power_far_reach_fire_in",
    "power_far_reach_fire_in_loop",
    "power_far_reach_fire_out",
    "power_far_reach_fire_out_loop",
    "power_far_reach_pull_in",
    "power_far_reach_pull_loop",
    "power_far_reach_pull_out",
    "power_far_reach_sequence",
    "power_far_reach_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (emily accept). The Far Reach aim, native tether, pull, impact and recovery read correctly. The only visible artifact is the reset streak back to the origin, which is a fixture move. Standing armed Dunwall body phases, native tether from the authored left wrist to the observed static floor target, and native impact are transported over Steam. Eight checks cover cast/recovery, cancellation, exact effect counts, replay refusal and health/equipment. Attachment updates after the remote root each native frame; fast-pull final-pose samples match the wrist. Recovery-boundary attachment tolerance, stance/equipment/target variants and complete lifecycle remain unfinished. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "far_reach",
      "character": "emily",
      "requirements": [
        "emily_far_reach_aim_cancel_release_arrive",
        "emily_far_reach_pull_variants"
      ],
      "needs": [
        "native tendril and attachment endpoints",
        "pull/travel/arrival phases",
        "target ownership and cleanup"
      ],
      "runtime_effects_verified": false,
      "partial_runtime_evidence": {
        "scope": "Standing armed Dunwall body and native world effects, cancellation and replay refusal.",
        "recording": "integrated-grapple-fx-v3.mp4",
        "remaining": "Stance/movement/equipment/upgrades/dynamic targets, full lifecycle and final visual acceptance."
      }
    }
  ],
  "in_game_recordings": [
    "integrated-power-emily-v3.mp4",
    "integrated-grapple-fx-v3.mp4",
    "grapple-knee-v4.mp4",
    "grapple-regression-v26-v2.mp4"
  ],
  "native_effects_status": "partial_standing_armed_Dunwall_tether_impact",
  "evidence": [
    "grapple-regression-v26-v2/result.json",
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1353a/result.json",
    "coverage-v1350a/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1351-far-reach.json",
    "decision": "complete",
    "scope": "Far Reach in the Emily source role: cast-in and aim loop (variants 4, 5), release (6) and the pull-in (7) are presented; a native X cancellation during the aim presents no release, the source stays put (0.0 m) and the partner returns to locomotion, and a following cast releases and travels 9.2 m to its arrival (1353a); a long reach travels 9.6 m down the lane with the tether at her hand (1350a). Far Reach is Emily-only; the Corvo source role has Blink instead.",
    "takes": [
      {
        "take": "coverage-v1353a",
        "cases": [
          "client-farreachcancel"
        ]
      },
      {
        "take": "coverage-v1350a",
        "cases": [
          "client-farreachfar"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1350a/client-farreachfar-observer.mp4",
        "verdict": "From behind, Emily reaches her left hand down the lane, is pulled off her feet with the tether light at her hand, flies about ten metres and lands at the far end."
      },
      {
        "video": "coverage-v1353a/client-farreachcancel-observer.mp4",
        "verdict": "Emily reaches out with her left hand and holds it, lowers it again without moving when the aim is cancelled, then reaches once more and is pulled away down the lane; the scene is dim but the poses read."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: grapple-regression-v26-v2/result.json](<../../../../work/natural-moveset/grapple-regression-v26-v2/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1353a/result.json](<../../../../work/natural-moveset/coverage-v1353a/result.json>)
- [evidence: coverage-v1350a/result.json](<../../../../work/natural-moveset/coverage-v1350a/result.json>)
- [in_game_recordings: integrated-power-emily-v3.mp4](<../../../../work/natural-moveset/integrated-power-emily-v3.mp4>)
- [in_game_recordings: integrated-grapple-fx-v3.mp4](<../../../../work/natural-moveset/integrated-grapple-fx-v3.mp4>)
- [in_game_recordings: grapple-knee-v4.mp4](<../../../../work/natural-moveset/grapple-knee-v4.mp4>)
- [in_game_recordings: grapple-regression-v26-v2.mp4](<../../../../work/natural-moveset/grapple-regression-v26-v2.mp4>)
## emily_far_reach_pull_variants

Full-body cast studies with native caster acting and D2 first-person phase timing/finger articulation. Wrists follow the body freely. Effects, target contact, character availability, gameplay binding and visual acceptance remain pending. Pull gesture only; object/enemy attachment, weight, trajectories and variants are not demonstrated. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.

````json
{
  "id": "emily_far_reach_pull_variants",
  "family": "powers",
  "characters": [
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_runtime_completion_audit",
  "visual_validation": "accepted_completion_audit",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "emily far reach pull variants",
  "motions": [
    "power_far_reach_pull_in",
    "power_far_reach_pull_loop",
    "power_far_reach_pull_out"
  ],
  "deliverable_kind": "animation",
  "notes": "Full-body cast studies with native caster acting and D2 first-person phase timing/finger articulation. Wrists follow the body freely. Effects, target contact, character availability, gameplay binding and visual acceptance remain pending. Pull gesture only; object/enemy attachment, weight, trajectories and variants are not demonstrated. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "far_reach",
      "character": "emily",
      "requirements": [
        "emily_far_reach_aim_cancel_release_arrive",
        "emily_far_reach_pull_variants"
      ],
      "needs": [
        "native tendril and attachment endpoints",
        "pull/travel/arrival phases",
        "target ownership and cleanup"
      ],
      "runtime_effects_verified": false,
      "partial_runtime_evidence": {
        "scope": "Standing armed Dunwall body and native world effects, cancellation and replay refusal.",
        "recording": "integrated-grapple-fx-v3.mp4",
        "remaining": "Stance/movement/equipment/upgrades/dynamic targets, full lifecycle and final visual acceptance."
      }
    }
  ],
  "in_game_recordings": [],
  "native_effects_status": "not_verified",
  "completion": {
    "audit": "completion-audit-v1479-far-reach-pull.json",
    "decision": "complete",
    "scope": "Emily: the Far Reach pull (aim, release, pullIn, the loop when native, pullOut) presented on the partner from the native pull state and scenes, after the native lock onto a target. The pulled object's physics and the body pull are stage 4 world authority (#115).",
    "takes": [
      {
        "take": "object-carry-v1471b",
        "cases": [
          "client-farreachbottle"
        ]
      }
    ],
    "visual": [
      {
        "video": "object-carry-v1471b/client-farreachbottle-observer.mp4",
        "verdict": "Corvo's window: Emily raises her left arm and holds the Far Reach aim in a braced stance, the tendril lashes out from her hand, then she draws the arm back sharply with her body leaning into the pull and recovers to her stance. Natural full-body pull. Accepted (the pulled bottle moves only in her world, #115)."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true,
  "evidence": [
    "object-carry-v1471b/result.json"
  ],
  "visual_validation_before": "not_accepted",
  "runtime_binding_before": "not_verified"
}
````

- [evidence: object-carry-v1471b/result.json](<../../../../work/natural-moveset/object-carry-v1471b/result.json>)
## emily_dark_vision_cast_end

Native Dark Vision casts now cover ordinary jumps as well as tested ground stance/equipment/travel contexts. Exact native cast clock and airborne semantics travel together; native private reveal, sword state and landing recovery are preserved. Other contexts and lifecycle remain.

````json
{
  "id": "emily_dark_vision_cast_end",
  "family": "powers",
  "characters": [
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_Dark_Vision_stance_equipment_and_travel_through_Steam",
  "visual_validation": "accepted_completion_audit",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "emily dark vision cast end",
  "motions": [
    "power_dark_vision_fire",
    "power_dark_vision_sequence",
    "native_phase_dark_vision_fire",
    "native_darkvision_standing_empty",
    "native_darkvision_crouched_armed",
    "native_darkvision_crouched_empty"
  ],
  "deliverable_kind": "animation",
  "notes": "Native Dark Vision casts now cover ordinary jumps as well as tested ground stance/equipment/travel contexts. Exact native cast clock and airborne semantics travel together; native private reveal, sword state and landing recovery are preserved. Other contexts and lifecycle remain.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "dark_vision",
      "character": "both",
      "requirements": [
        "corvo_dark_vision_cast_end",
        "emily_dark_vision_cast_end"
      ],
      "needs": [
        "cast and end timing",
        "native viewer-specific reveal and screen effects",
        "confirmed world-visible effects only"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "darkvision-emily-v25-v2.mp4",
    "darkvision-emily-v25-v2-source.mp4",
    "darkvision-emily-crouched-v26-v1.mp4",
    "darkvision-emily-crouched-v26-v1-source.mp4",
    "darkvision-emily-empty-v26-v1.mp4",
    "darkvision-emily-empty-v26-v1-source.mp4",
    "darkvision-emily-crouched-empty-v26-v2.mp4",
    "darkvision-emily-crouched-empty-v26-v2-source.mp4",
    "darkvision-emily-moving-v26-v1.mp4",
    "darkvision-emily-moving-v26-v1-source.mp4",
    "darkvision-emily-crouched-moving-v26-v1.mp4",
    "darkvision-emily-crouched-moving-v26-v1-source.mp4",
    "darkvision-emily-empty-moving-v26-v1.mp4",
    "darkvision-emily-empty-moving-v26-v1-source.mp4",
    "darkvision-emily-crouched-empty-moving-v26-v1.mp4",
    "darkvision-emily-crouched-empty-moving-v26-v1-source.mp4",
    "darkvision-emily-air-v26-v2.mp4",
    "darkvision-emily-air-v26-v2-source.mp4",
    "darkvision-emily-air-empty-v26-v1.mp4",
    "darkvision-emily-air-empty-v26-v1-source.mp4",
    "darkvision-emily-balanced-empty-moving-v28-v1.mp4",
    "darkvision-emily-balanced-empty-moving-v28-v1-source.mp4"
  ],
  "native_effects_status": "native_caster_vision_on_off_and_viewer_scope_verified_in_ground_contexts",
  "evidence": [
    "darkvision-emily-v25-v2/result.json",
    "darkvision-emily-crouched-v26-v1/result.json",
    "darkvision-emily-empty-v26-v1/result.json",
    "darkvision-emily-crouched-empty-v26-v2/result.json",
    "darkvision-emily-moving-v26-v1/result.json",
    "darkvision-emily-crouched-moving-v26-v1/result.json",
    "darkvision-emily-empty-moving-v26-v1/result.json",
    "darkvision-emily-crouched-empty-moving-v26-v1/result.json",
    "darkvision-emily-air-v26-v2/result.json",
    "darkvision-emily-air-empty-v26-v1/result.json",
    "darkvision-emily-balanced-empty-moving-v28-v1/result.json",
    "coverage-v1332c/result.json",
    "coverage-v1332d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1337-powers.json",
    "decision": "complete",
    "scope": "Emily casts and ends Dark Vision (power variant 10) standing armed; the partner presents both gestures and recovers. The reveal and screen effect stay with the caster.",
    "takes": [
      {
        "take": "coverage-v1332c",
        "cases": [
          "client-darkvision"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "client-darkvision"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1332d/client-darkvision-observer.mp4",
        "verdict": "Emily lifts her hand to her face to cast and again to end Dark Vision, sword held low, then settles."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true,
  "visual_validation_before": "bounded_actual_game_frames_inspected"
}
````

- [evidence: darkvision-emily-v25-v2/result.json](<../../../../work/natural-moveset/darkvision-emily-v25-v2/result.json>)
- [evidence: darkvision-emily-crouched-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-crouched-v26-v1/result.json>)
- [evidence: darkvision-emily-empty-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-empty-v26-v1/result.json>)
- [evidence: darkvision-emily-crouched-empty-v26-v2/result.json](<../../../../work/natural-moveset/darkvision-emily-crouched-empty-v26-v2/result.json>)
- [evidence: darkvision-emily-moving-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-moving-v26-v1/result.json>)
- [evidence: darkvision-emily-crouched-moving-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-crouched-moving-v26-v1/result.json>)
- [evidence: darkvision-emily-empty-moving-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-empty-moving-v26-v1/result.json>)
- [evidence: darkvision-emily-crouched-empty-moving-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-crouched-empty-moving-v26-v1/result.json>)
- [evidence: darkvision-emily-air-v26-v2/result.json](<../../../../work/natural-moveset/darkvision-emily-air-v26-v2/result.json>)
- [evidence: darkvision-emily-air-empty-v26-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-air-empty-v26-v1/result.json>)
- [evidence: darkvision-emily-balanced-empty-moving-v28-v1/result.json](<../../../../work/natural-moveset/darkvision-emily-balanced-empty-moving-v28-v1/result.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: darkvision-emily-v25-v2.mp4](<../../../../work/natural-moveset/darkvision-emily-v25-v2.mp4>)
- [in_game_recordings: darkvision-emily-v25-v2-source.mp4](<../../../../work/natural-moveset/darkvision-emily-v25-v2-source.mp4>)
- [in_game_recordings: darkvision-emily-crouched-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-v26-v1.mp4>)
- [in_game_recordings: darkvision-emily-crouched-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-emily-empty-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-empty-v26-v1.mp4>)
- [in_game_recordings: darkvision-emily-empty-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-empty-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-emily-crouched-empty-v26-v2.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-empty-v26-v2.mp4>)
- [in_game_recordings: darkvision-emily-crouched-empty-v26-v2-source.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-empty-v26-v2-source.mp4>)
- [in_game_recordings: darkvision-emily-moving-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-moving-v26-v1.mp4>)
- [in_game_recordings: darkvision-emily-moving-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-moving-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-emily-crouched-moving-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-moving-v26-v1.mp4>)
- [in_game_recordings: darkvision-emily-crouched-moving-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-moving-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-emily-empty-moving-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-empty-moving-v26-v1.mp4>)
- [in_game_recordings: darkvision-emily-empty-moving-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-empty-moving-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-emily-crouched-empty-moving-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-empty-moving-v26-v1.mp4>)
- [in_game_recordings: darkvision-emily-crouched-empty-moving-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-crouched-empty-moving-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-emily-air-v26-v2.mp4](<../../../../work/natural-moveset/darkvision-emily-air-v26-v2.mp4>)
- [in_game_recordings: darkvision-emily-air-v26-v2-source.mp4](<../../../../work/natural-moveset/darkvision-emily-air-v26-v2-source.mp4>)
- [in_game_recordings: darkvision-emily-air-empty-v26-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-air-empty-v26-v1.mp4>)
- [in_game_recordings: darkvision-emily-air-empty-v26-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-air-empty-v26-v1-source.mp4>)
- [in_game_recordings: darkvision-emily-balanced-empty-moving-v28-v1.mp4](<../../../../work/natural-moveset/darkvision-emily-balanced-empty-moving-v28-v1.mp4>)
- [in_game_recordings: darkvision-emily-balanced-empty-moving-v28-v1-source.mp4](<../../../../work/natural-moveset/darkvision-emily-balanced-empty-moving-v28-v1-source.mp4>)
## emily_domino_mark_link_cancel

Visual sweep 1260: accepted (emily accept). The Domino point-and-link gesture and its recovery read correctly, and the native mark effect appears on the target. Framing in v218 is cramped at the screen edge. Domino scoped body/marks, partner exclusion, beams, aim cancellation, target deselection and standing final recovery remain preserved. Paired armed crouchv250/v256 verifies all ordinary/final gestures, continuous root, target removal and standing recovery; railing/darkness limit fine visual acceptance. Native holsteredv253 confirms ordinary phases with sword remaining unequipped in standing and crouch. Standing source passes; crouched source fails inventory audit because native X pickpockets a nearby key (checkpoint/protected14 restore). Candidate258 validates the same equipment identity across reads and enables bank48 empty-hand contexts, but its first paired run exposed a remaining source/wire armed-only gate. Candidate261 shares that equipment-context rule between sender and receiver and uses semantic protocol45/336bytes. Paired tests look away before holstering. Native v263 standing/crouched holstered final observations each pass7 checks; candidate264 accepts their exact400ms scene and grounded recovery with equipment/ownership guards retained. Bank48/301 and protocol45 remain unchanged. Completed paired contexts: domino-context-crouched-holstered-final-v264 PASS; domino-context-crouched-holstered-moving-v263 PASS; domino-context-crouched-holstered-target-v262 PASS; domino-context-crouched-holstered-v261 PASS; domino-context-standing-holstered-final-v264 PASS; domino-context-standing-holstered-v258 FAIL: Native transport/presentation retired: native_state_stale; domino-context-standing-holstered-v261 PASS. Broader air/slide/interruptions, shared NPC authority and gameplay outcomes remain unfinished. No complete-power or final visual acceptance.

````json
{
  "id": "emily_domino_mark_link_cancel",
  "family": "powers",
  "characters": [
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_domino_standing_armed_body_and_mark_partial_verified",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "emily domino mark link cancel",
  "motions": [
    "power_domino_select_in",
    "power_domino_select_loop",
    "power_domino_fire",
    "power_domino_select_out",
    "power_domino_sequence",
    "power_domino_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (emily accept). The Domino point-and-link gesture and its recovery read correctly, and the native mark effect appears on the target. Framing in v218 is cramped at the screen edge. Domino scoped body/marks, partner exclusion, beams, aim cancellation, target deselection and standing final recovery remain preserved. Paired armed crouchv250/v256 verifies all ordinary/final gestures, continuous root, target removal and standing recovery; railing/darkness limit fine visual acceptance. Native holsteredv253 confirms ordinary phases with sword remaining unequipped in standing and crouch. Standing source passes; crouched source fails inventory audit because native X pickpockets a nearby key (checkpoint/protected14 restore). Candidate258 validates the same equipment identity across reads and enables bank48 empty-hand contexts, but its first paired run exposed a remaining source/wire armed-only gate. Candidate261 shares that equipment-context rule between sender and receiver and uses semantic protocol45/336bytes. Paired tests look away before holstering. Native v263 standing/crouched holstered final observations each pass7 checks; candidate264 accepts their exact400ms scene and grounded recovery with equipment/ownership guards retained. Bank48/301 and protocol45 remain unchanged. Completed paired contexts: domino-context-crouched-holstered-final-v264 PASS; domino-context-crouched-holstered-moving-v263 PASS; domino-context-crouched-holstered-target-v262 PASS; domino-context-crouched-holstered-v261 PASS; domino-context-standing-holstered-final-v264 PASS; domino-context-standing-holstered-v258 FAIL: Native transport/presentation retired: native_state_stale; domino-context-standing-holstered-v261 PASS. Broader air/slide/interruptions, shared NPC authority and gameplay outcomes remain unfinished. No complete-power or final visual acceptance.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "domino",
      "character": "emily",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "needs": [
        "native marks and link effects",
        "target identity and link updates",
        "removal/target-death cleanup"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "domino-native-source-v187.mp4",
    "domino-native-target-v191.mp4",
    "domino-native-reader-v190.mp4",
    "domino-network-emily-v194.mp4",
    "domino-network-emily-v194-source.mp4",
    "domino-network-emily-v197.mp4",
    "domino-network-emily-v197-source.mp4",
    "domino-network-source-v198.mp4",
    "domino-target-reader-v200.mp4",
    "domino-target-scoped-v202.mp4",
    "domino-world-network-v204.mp4",
    "domino-world-network-v204-source.mp4",
    "domino-world-network-v206.mp4",
    "domino-world-network-v206-source.mp4",
    "domino-mark-retirement-v208.mp4",
    "domino-mark-retirement-v208-source.mp4",
    "domino-mark-fade-v209.mp4",
    "domino-mark-fade-v209-source.mp4",
    "domino-partner-picker-v212.mp4",
    "domino-guard-regression-v213.mp4",
    "domino-guard-regression-v213-source.mp4",
    "domino-two-target-source-v214.mp4",
    "domino-guard-grounded-v216.mp4",
    "domino-guard-grounded-v216-source.mp4",
    "domino-link-reader-v217.mp4",
    "domino-guard-feedback-v218.mp4",
    "domino-guard-feedback-v218-source.mp4",
    "domino-link-patrol-v219.mp4",
    "domino-link-clear-view-v221.mp4",
    "domino-beam-network-v223.mp4",
    "domino-beam-network-v223-source.mp4",
    "domino-beam-view-v225.mp4",
    "domino-beam-view-v225-source.mp4",
    "domino-native-cancel-v226.mp4",
    "domino-native-cancel-v226-source.mp4",
    "domino-native-cancel-v227.mp4",
    "domino-native-cancel-v227-source.mp4",
    "domino-native-cancel-v228.mp4",
    "domino-native-cancel-v228-source.mp4",
    "domino-native-cancel-v230.mp4",
    "domino-native-cancel-v230-source.mp4",
    "domino-native-cancel-v231.mp4",
    "domino-native-cancel-v231-source.mp4",
    "domino-native-cancel-v232.mp4",
    "domino-native-cancel-v232-source.mp4",
    "domino-native-cancel-v233.mp4",
    "domino-native-cancel-v233-source.mp4",
    "domino-native-deselect-v234.mp4",
    "domino-native-deselect-v234-source.mp4",
    "domino-native-deselect-v235.mp4",
    "domino-native-deselect-v235-source.mp4",
    "domino-final-body-v238.mp4",
    "domino-final-body-v238-source.mp4",
    "domino-final-body-v239.mp4",
    "domino-final-body-v239-source.mp4",
    "domino-final-recovery-v241.mp4",
    "domino-final-recovery-v241-source.mp4",
    "domino-crouch-observe-v245.mp4",
    "domino-final-recovery-v244.mp4",
    "domino-final-recovery-v244-source.mp4",
    "domino-crouch-network-v246.mp4",
    "domino-crouch-network-v246-source.mp4",
    "domino-crouch-selection-observe-v249.mp4",
    "domino-crouch-network-v250.mp4",
    "domino-crouch-network-v250-source.mp4",
    "domino-crouch-final-observe-v251.mp4",
    "domino-crouch-final-network-v256.mp4",
    "domino-crouch-final-network-v256-source.mp4",
    "domino-holstered-standing-observe-v253.mp4",
    "domino-holstered-crouched-observe-v253.mp4",
    "domino-context-standing-holstered-v258.mp4",
    "domino-context-standing-holstered-v258-source.mp4",
    "domino-context-standing-holstered-v261.mp4",
    "domino-context-standing-holstered-v261-source.mp4",
    "domino-context-crouched-holstered-v261.mp4",
    "domino-context-crouched-holstered-v261-source.mp4",
    "domino-context-crouched-holstered-target-v262.mp4",
    "domino-context-crouched-holstered-target-v262-source.mp4",
    "domino-context-crouched-holstered-moving-v263.mp4",
    "domino-context-crouched-holstered-moving-v263-source.mp4",
    "domino-holstered-crouched-final-observe-v263.mp4",
    "domino-holstered-standing-final-observe-v263.mp4",
    "domino-context-crouched-holstered-final-v264.mp4",
    "domino-context-crouched-holstered-final-v264-source.mp4",
    "domino-context-standing-holstered-final-v264.mp4",
    "domino-context-standing-holstered-final-v264-source.mp4"
  ],
  "native_effects_status": "not_verified",
  "native_integration": {
    "domino-native-source-v187.mp4": {
      "video": "domino-native-source-v187.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-native-source-v187",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-source-v187/result.json",
      "visual_review": "domino-native-source-v187/visual-review.json",
      "proxy_sha256": "22FF6B6DEE8D06D18FAD1C22E6B1D2AE8968F36064C2E2F345DB19CE4919632A",
      "actual_capture_hz": 27.5,
      "video_sha256": "3BCE9D3CE01FA4F792A4049AEB93F6A57D67BC4EC9B6E666B62052929468E0A9",
      "checks_passed": true,
      "scope": "Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-target-v191.mp4": {
      "video": "domino-native-target-v191.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-native-target-v191",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-target-v191/result.json",
      "visual_review": "domino-native-target-v191/visual-review.json",
      "proxy_sha256": "22FF6B6DEE8D06D18FAD1C22E6B1D2AE8968F36064C2E2F345DB19CE4919632A",
      "actual_capture_hz": 27.3125,
      "video_sha256": "B0059FBA45A7FAF6E93BC6B4C1859086A01B17DD2302BF22678AA7FABAF550DB",
      "checks_passed": true,
      "scope": "Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-reader-v190.mp4": {
      "video": "domino-native-reader-v190.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-native-reader-v190",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-reader-v190/result.json",
      "visual_review": "domino-native-reader-v190/visual-review.json",
      "proxy_sha256": "22FF6B6DEE8D06D18FAD1C22E6B1D2AE8968F36064C2E2F345DB19CE4919632A",
      "actual_capture_hz": 26.86847599164927,
      "video_sha256": "AF786D721640730F50EC48DF094A9B2287EEAB530845DE28EF1BA83C4BC3B7B9",
      "checks_passed": false,
      "scope": "Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-network-emily-v194.mp4": {
      "video": "domino-network-emily-v194.mp4",
      "character": "emily",
      "label": "Domino Steam body presentation · domino-network-emily-v194",
      "alias": "domino_native_body_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-network-emily-v194/result.json",
      "visual_review": "domino-network-emily-v194/visual-review.json",
      "proxy_sha256": "41D3D69BB54B2390D3A31495C5CF0D558C9DC329CBAD4E780C20EA67BB370CBA",
      "actual_capture_hz": 26.1875,
      "video_sha256": "3BA3378E8D5E8E8A954EE81CB4E25830A8EE6B2E079A42DE1A37143963A83A5E",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote target marks/links, AI and shared outcomes remain pending; receiver plays only body poses. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-network-emily-v194-source.mp4": {
      "video": "domino-network-emily-v194-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-network-emily-v194",
      "alias": "domino_native_body_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-network-emily-v194/result.json",
      "visual_review": "domino-network-emily-v194/visual-review.json",
      "proxy_sha256": "41D3D69BB54B2390D3A31495C5CF0D558C9DC329CBAD4E780C20EA67BB370CBA",
      "actual_capture_hz": 25.6875,
      "video_sha256": "0C47554E23880640CD5BF0F20282309DEC984497DED2F75174C22047C1FC65CA",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote target marks/links, AI and shared outcomes remain pending; receiver plays only body poses. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-network-emily-v197.mp4": {
      "video": "domino-network-emily-v197.mp4",
      "character": "emily",
      "label": "Domino Steam body presentation · domino-network-emily-v197",
      "alias": "domino_native_body_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-network-emily-v197/result.json",
      "visual_review": "domino-network-emily-v197/visual-review.json",
      "proxy_sha256": "41D3D69BB54B2390D3A31495C5CF0D558C9DC329CBAD4E780C20EA67BB370CBA",
      "actual_capture_hz": 26.3125,
      "video_sha256": "3B6C3ED8BD7D6CB00C65A3A8FF7B06FA6DF8E8A0C4C99CD9AA0047A807EB7D9B",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote target marks/links, AI and shared outcomes remain pending; receiver plays only body poses. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-network-emily-v197-source.mp4": {
      "video": "domino-network-emily-v197-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-network-emily-v197",
      "alias": "domino_native_body_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-network-emily-v197/result.json",
      "visual_review": "domino-network-emily-v197/visual-review.json",
      "proxy_sha256": "41D3D69BB54B2390D3A31495C5CF0D558C9DC329CBAD4E780C20EA67BB370CBA",
      "actual_capture_hz": 25.9375,
      "video_sha256": "87590C8AEABDB4B8234591F5184651E600871097ABF70B0786B0E03977E6CDD7",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote target marks/links, AI and shared outcomes remain pending; receiver plays only body poses. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-network-source-v198.mp4": {
      "video": "domino-network-source-v198.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-network-source-v198",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-network-source-v198/result.json",
      "visual_review": "domino-network-source-v198/visual-review.json",
      "proxy_sha256": "41D3D69BB54B2390D3A31495C5CF0D558C9DC329CBAD4E780C20EA67BB370CBA",
      "actual_capture_hz": 27.4375,
      "video_sha256": "D9F6608CEDB05C2576D4ABBB0F3E9986E09415D8DB00DA82476EBFFCA23150E3",
      "checks_passed": true,
      "scope": "Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-target-reader-v200.mp4": {
      "video": "domino-target-reader-v200.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-target-reader-v200",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-target-reader-v200/result.json",
      "visual_review": "domino-target-reader-v200/visual-review.json",
      "proxy_sha256": "41D3D69BB54B2390D3A31495C5CF0D558C9DC329CBAD4E780C20EA67BB370CBA",
      "actual_capture_hz": 27.3125,
      "video_sha256": "050AAB6237240109D1533933082A140AEFA8000E93A0B1A834E27C0CC7095F32",
      "checks_passed": true,
      "scope": "Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-target-scoped-v202.mp4": {
      "video": "domino-target-scoped-v202.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-target-scoped-v202",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-target-scoped-v202/result.json",
      "visual_review": "domino-target-scoped-v202/visual-review.json",
      "proxy_sha256": "41D3D69BB54B2390D3A31495C5CF0D558C9DC329CBAD4E780C20EA67BB370CBA",
      "actual_capture_hz": 26.805845511482254,
      "video_sha256": "795BB24D3385F7C1FCC39664DA2471277E036037CF94C7462B75731136B96133",
      "checks_passed": true,
      "scope": "Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-world-network-v204.mp4": {
      "video": "domino-world-network-v204.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-world-network-v204",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-world-network-v204/result.json",
      "visual_review": "domino-world-network-v204/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 26.25,
      "video_sha256": "6F9F579EB4CF5C473E1A3BA2C205DCB15D05E47793FE3F06879A73E81F6C1A5A",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-world-network-v204-source.mp4": {
      "video": "domino-world-network-v204-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-world-network-v204",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-world-network-v204/result.json",
      "visual_review": "domino-world-network-v204/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 26.0625,
      "video_sha256": "890F9F97260EF236732AA6080C09E93DE34BE51F420DD2711D184BC4F1A0E8A8",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-world-network-v206.mp4": {
      "video": "domino-world-network-v206.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-world-network-v206",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-world-network-v206/result.json",
      "visual_review": "domino-world-network-v206/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 25.9375,
      "video_sha256": "3B7C7725B50B264979F60A1AB13D167F6BABAD2D02AC8B6F1743B1CDF469FA02",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-world-network-v206-source.mp4": {
      "video": "domino-world-network-v206-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-world-network-v206",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-world-network-v206/result.json",
      "visual_review": "domino-world-network-v206/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 25.794979079497907,
      "video_sha256": "4FB3E9E5EF943D5BE67342D21EAE398DB056786B8A483064E27ABB10014C6233",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-mark-retirement-v208.mp4": {
      "video": "domino-mark-retirement-v208.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-mark-retirement-v208",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-mark-retirement-v208/result.json",
      "visual_review": "domino-mark-retirement-v208/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 26.6875,
      "video_sha256": "F6069FA651311E0D7DF40D5712F15AD3FA805537EE96310445FEDC269FA5E34F",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-mark-retirement-v208-source.mp4": {
      "video": "domino-mark-retirement-v208-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-mark-retirement-v208",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-mark-retirement-v208/result.json",
      "visual_review": "domino-mark-retirement-v208/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 26.17954070981211,
      "video_sha256": "E560748A8871E1245BBA6715F88B96E660AB94D735CD54CA82C3B875974FCFEA",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-mark-fade-v209.mp4": {
      "video": "domino-mark-fade-v209.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-mark-fade-v209",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-mark-fade-v209/result.json",
      "visual_review": "domino-mark-fade-v209/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 26.917510853835022,
      "video_sha256": "80C6010812B7C2CB9E58CA83DF5731D4BBE5F10C602990447B0CA29E5C732877",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-mark-fade-v209-source.mp4": {
      "video": "domino-mark-fade-v209-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-mark-fade-v209",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-mark-fade-v209/result.json",
      "visual_review": "domino-mark-fade-v209/visual-review.json",
      "proxy_sha256": "C60AC9EC08219A6132AE382FB5CF4E5FBABB6A94231985BC08A854848CD345B9",
      "actual_capture_hz": 26.956521739130434,
      "video_sha256": "A463A2F69EB1847B74CD1CC971E7E1924F2E96EB956D7541BA58EFCCE4F53272",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-partner-picker-v212.mp4": {
      "video": "domino-partner-picker-v212.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-partner-picker-v212",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-partner-picker-v212/result.json",
      "visual_review": "domino-partner-picker-v212/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 27.3125,
      "video_sha256": "1402A487C25D4DAF962600A8A841D6893DCE98AB066F8104F084B2898F6B92D2",
      "checks_passed": true,
      "scope": "Bounded native Domino partner aim exclusion; no claim about linked damage or general NPC synchronization. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-guard-regression-v213.mp4": {
      "video": "domino-guard-regression-v213.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-guard-regression-v213",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-guard-regression-v213/result.json",
      "visual_review": "domino-guard-regression-v213/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 27.34397677793904,
      "video_sha256": "7F3C7FC9635A20367FEEFFBDA1DF08C3676B2F05EEFA12E340021DC547261C2C",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-guard-regression-v213-source.mp4": {
      "video": "domino-guard-regression-v213-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-guard-regression-v213",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-guard-regression-v213/result.json",
      "visual_review": "domino-guard-regression-v213/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 26.95210449927431,
      "video_sha256": "F42F7CA94C7EBD817291F07C25AFE366A6109A8EA8FAFA05C5A672B73BF6C60C",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-two-target-source-v214.mp4": {
      "video": "domino-two-target-source-v214.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-two-target-source-v214",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-two-target-source-v214/result.json",
      "visual_review": "domino-two-target-source-v214/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 26.297071129707113,
      "video_sha256": "1A1F773A16A2D852ED787D9233D784AEEA5B14832F70D5B22BFE426A3688A5D5",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Raw connection records are not yet trusted for replication. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-guard-grounded-v216.mp4": {
      "video": "domino-guard-grounded-v216.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-guard-grounded-v216",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-guard-grounded-v216/result.json",
      "visual_review": "domino-guard-grounded-v216/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 26.608695652173914,
      "video_sha256": "FC473AC3A32E6885D854A2C79213D6B259CB20EE4B881963BAEDC10C53F4711F",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-guard-grounded-v216-source.mp4": {
      "video": "domino-guard-grounded-v216-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-guard-grounded-v216",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-guard-grounded-v216/result.json",
      "visual_review": "domino-guard-grounded-v216/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 25.869565217391305,
      "video_sha256": "B98914A944A99F1BB9B7F252D6BBC32DDB8683C299215FF453857725CB59390B",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-link-reader-v217.mp4": {
      "video": "domino-link-reader-v217.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-link-reader-v217",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-link-reader-v217/result.json",
      "visual_review": "domino-link-reader-v217/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 27.125,
      "video_sha256": "A0572AFE18F4A4D719D3E86E77813931A40625578ACBA9A8678E6382E8AA4EF0",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-guard-feedback-v218.mp4": {
      "video": "domino-guard-feedback-v218.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-guard-feedback-v218",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-guard-feedback-v218/result.json",
      "visual_review": "domino-guard-feedback-v218/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 27.126269956458636,
      "video_sha256": "6BDDD9F52DA6A7638745059843A7C60F3F322E96CE575742585CFA57E555EB3C",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-guard-feedback-v218-source.mp4": {
      "video": "domino-guard-feedback-v218-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-guard-feedback-v218",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-guard-feedback-v218/result.json",
      "visual_review": "domino-guard-feedback-v218/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 26.565217391304348,
      "video_sha256": "21BF1E2BF6FF953851CB617CB4E40DF3B4384836EA776162B7D0630797B18383",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized. No claim of complete Domino support, NPC attachment accuracy or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-link-patrol-v219.mp4": {
      "video": "domino-link-patrol-v219.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-link-patrol-v219",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-link-patrol-v219/result.json",
      "visual_review": "domino-link-patrol-v219/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 26.304801670146137,
      "video_sha256": "1D478E472EF0A03B496441D1F8238A74899C80A302E16C462EAE051C32211CF8",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-link-clear-view-v221.mp4": {
      "video": "domino-link-clear-view-v221.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-link-clear-view-v221",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-link-clear-view-v221/result.json",
      "visual_review": "domino-link-clear-view-v221/visual-review.json",
      "proxy_sha256": "FF70EFECF5E4ED56421856A930BB454DF500F9547ECF326BF2A0D05E5E7B6F95",
      "actual_capture_hz": 27.4375,
      "video_sha256": "25FDA3C51F6C96FEEE790C5F7E4DF1649FD838A6B89A7CB6A8A7D9E0E1C4282D",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-beam-network-v223.mp4": {
      "video": "domino-beam-network-v223.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-beam-network-v223",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-beam-network-v223/result.json",
      "visual_review": "domino-beam-network-v223/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.966666666666665,
      "video_sha256": "AF645ED880284238E5214E71FBF1E46EFC5C45969033EFB716866AF3F89858B5",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. NPC alignment, AI, damage/outcomes and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-beam-network-v223-source.mp4": {
      "video": "domino-beam-network-v223-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-beam-network-v223",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-beam-network-v223/result.json",
      "visual_review": "domino-beam-network-v223/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 25.966666666666665,
      "video_sha256": "C4DA664380E758548E62CA285F773DC9B2B108B7C02D503C2743F0562938C466",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. NPC alignment, AI, damage/outcomes and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-beam-view-v225.mp4": {
      "video": "domino-beam-view-v225.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-beam-view-v225",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-beam-view-v225/result.json",
      "visual_review": "domino-beam-view-v225/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 27.133333333333333,
      "video_sha256": "65196FC8882C99F6862A0FE040551AFBE9B75B21D21C9B9EF9A90FEBE253371C",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. NPC alignment, AI, damage/outcomes and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-beam-view-v225-source.mp4": {
      "video": "domino-beam-view-v225-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-beam-view-v225",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-beam-view-v225/result.json",
      "visual_review": "domino-beam-view-v225/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.833333333333332,
      "video_sha256": "23D157B07EDCC73075DE36129F3CDBF154BCD5FE9C2298CFA84D9732F4F8A533",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. NPC alignment, AI, damage/outcomes and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v226.mp4": {
      "video": "domino-native-cancel-v226.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-cancel-v226",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v226/result.json",
      "visual_review": "domino-native-cancel-v226/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 27.325,
      "video_sha256": "B64005BEC0DB6B8CBF6A8E8A9D4782102BB2F46D7DA9CE8D2D795B495E2E58C0",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v226-source.mp4": {
      "video": "domino-native-cancel-v226-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-cancel-v226",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v226/result.json",
      "visual_review": "domino-native-cancel-v226/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 28.05,
      "video_sha256": "FF10A768BDF669D030540697DCD75E4E90CF5F257C3E77FE962761E00F6FB467",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v227.mp4": {
      "video": "domino-native-cancel-v227.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-cancel-v227",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v227/result.json",
      "visual_review": "domino-native-cancel-v227/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.775,
      "video_sha256": "B55E93BFF3B12357894E427E48C516BAF46BEFB578F4F19C0EF3D8D748F955CC",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v227-source.mp4": {
      "video": "domino-native-cancel-v227-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-cancel-v227",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v227/result.json",
      "visual_review": "domino-native-cancel-v227/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.9,
      "video_sha256": "67A6284988E92F96A7461689632302874CA035E29266E89A5FDA2E1CB1C101EB",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v228.mp4": {
      "video": "domino-native-cancel-v228.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-cancel-v228",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v228/result.json",
      "visual_review": "domino-native-cancel-v228/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.4,
      "video_sha256": "B5C0D39DCC1C99260A065DFC4637BDFA08D98AA35BB592A34BE5CDDB8D054433",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v228-source.mp4": {
      "video": "domino-native-cancel-v228-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-cancel-v228",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v228/result.json",
      "visual_review": "domino-native-cancel-v228/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.85,
      "video_sha256": "870C18EE4575EC73F5F32D9C7998D9D2B3B90E2FB65674EA60E7956BA9D3F4B1",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v230.mp4": {
      "video": "domino-native-cancel-v230.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-cancel-v230",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v230/result.json",
      "visual_review": "domino-native-cancel-v230/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 27.25,
      "video_sha256": "BC749957ACEB8C6D8F42BABC5A8B3AD3E27FF0ACC7A470D64B1955DC84BD10F5",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v230-source.mp4": {
      "video": "domino-native-cancel-v230-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-cancel-v230",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v230/result.json",
      "visual_review": "domino-native-cancel-v230/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 27.0,
      "video_sha256": "FF6CC611619D6A8E6FEEF6A9A7EFF219ED33376C34AD63E11DE8FC2BB5D3E02A",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v231.mp4": {
      "video": "domino-native-cancel-v231.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-cancel-v231",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v231/result.json",
      "visual_review": "domino-native-cancel-v231/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.825,
      "video_sha256": "3C7850242FC17A99A09ACB835C899A272C90F3137D0F7B9BA37D9F5D429B9F2E",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v231-source.mp4": {
      "video": "domino-native-cancel-v231-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-cancel-v231",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v231/result.json",
      "visual_review": "domino-native-cancel-v231/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.675,
      "video_sha256": "43DF1B4F047812CBDE8A7B1D8F11311B942201568EBC3C585C06A791F47D80F6",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v232.mp4": {
      "video": "domino-native-cancel-v232.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-cancel-v232",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v232/result.json",
      "visual_review": "domino-native-cancel-v232/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.9,
      "video_sha256": "24E76B520D0D153280A4EBC62BA79740EE4A4BA47E07362D5286E23F4CF668C3",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v232-source.mp4": {
      "video": "domino-native-cancel-v232-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-cancel-v232",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v232/result.json",
      "visual_review": "domino-native-cancel-v232/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.625,
      "video_sha256": "F55BF004CF3CBC08EF0201F00D33274382BB479074B41954C6EFA3B4A02E5729",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native linked-target cancellation is additionally tested with controller X while LT is held; raw post-link cancellation scenes are observations. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v233.mp4": {
      "video": "domino-native-cancel-v233.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-cancel-v233",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v233/result.json",
      "visual_review": "domino-native-cancel-v233/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.0,
      "video_sha256": "938A3571D2C186A759AE5E602123B710FE1941758B63F02CA40977F83A3CFC1B",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native X aim cancellation is verified to preserve established native links, with the peer matching that state and recovering body to idle. Independent peer presentation stop is separately tested; native target deselection remains pending. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-cancel-v233-source.mp4": {
      "video": "domino-native-cancel-v233-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-cancel-v233",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-cancel-v233/result.json",
      "visual_review": "domino-native-cancel-v233/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 25.75,
      "video_sha256": "DD4B0846843E78CE934CB4FF2078875405179CBDABBDA01A7D671950E83E9E23",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native X aim cancellation is verified to preserve established native links, with the peer matching that state and recovering body to idle. Independent peer presentation stop is separately tested; native target deselection remains pending. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-deselect-v234.mp4": {
      "video": "domino-native-deselect-v234.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-deselect-v234",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-deselect-v234/result.json",
      "visual_review": "domino-native-deselect-v234/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 27.12255798686733,
      "video_sha256": "47CC26CB24CED0375786E1E1CF5A090E18B6DBDB9DEE13962E6D8E655547089C",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. Native cast subscenes are retained for separate body coverage analysis; this test does not claim unobserved phase animation support. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-deselect-v234-source.mp4": {
      "video": "domino-native-deselect-v234-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-deselect-v234",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-deselect-v234/result.json",
      "visual_review": "domino-native-deselect-v234/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 27.04749555701438,
      "video_sha256": "92836A7C18C188475910BCD9D72ACD102080CFDC71BB3CC3B2548DDF9729775F",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. Native cast subscenes are retained for separate body coverage analysis; this test does not claim unobserved phase animation support. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-deselect-v235.mp4": {
      "video": "domino-native-deselect-v235.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-native-deselect-v235",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-deselect-v235/result.json",
      "visual_review": "domino-native-deselect-v235/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.575,
      "video_sha256": "2D78BBFA2FC2C0D90148EED583E60CC233F6D41A7E9D1561DE8C61D850B1EE19",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. Native cast subscenes are retained for separate body coverage analysis; this test does not claim unobserved phase animation support. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-native-deselect-v235-source.mp4": {
      "video": "domino-native-deselect-v235-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-native-deselect-v235",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-native-deselect-v235/result.json",
      "visual_review": "domino-native-deselect-v235/visual-review.json",
      "proxy_sha256": "0A5B3285071B33D69B300ECBFE1527E7FF7642F37D20BAD4293E6B72A4B5ABD2",
      "actual_capture_hz": 26.7,
      "video_sha256": "AFBDFD4B914F78072B3E8B3972C154E3B102FB3EA88E2083A15FD1EADCEB638D",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. Native cast subscenes are retained for separate body coverage analysis; this test does not claim unobserved phase animation support. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-body-v238.mp4": {
      "video": "domino-final-body-v238.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-final-body-v238",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-body-v238/result.json",
      "visual_review": "domino-final-body-v238/visual-review.json",
      "proxy_sha256": "C208F6DFB490D7900854BF53A1704F4E42149BC4EC96D1AE5A2842A8A9FEEDB5",
      "actual_capture_hz": 27.275,
      "video_sha256": "D460F9E334D50D03B3B80C4D4D035109FEEA7758DCDCD3A032F28BDC8245B8C3",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-body-v238-source.mp4": {
      "video": "domino-final-body-v238-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-final-body-v238",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-body-v238/result.json",
      "visual_review": "domino-final-body-v238/visual-review.json",
      "proxy_sha256": "C208F6DFB490D7900854BF53A1704F4E42149BC4EC96D1AE5A2842A8A9FEEDB5",
      "actual_capture_hz": 27.425,
      "video_sha256": "E5028CE668488C63F47D82360CE5551100455B0471E307EE3764DD94C76DE6AF",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-body-v239.mp4": {
      "video": "domino-final-body-v239.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-final-body-v239",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-body-v239/result.json",
      "visual_review": "domino-final-body-v239/visual-review.json",
      "proxy_sha256": "C208F6DFB490D7900854BF53A1704F4E42149BC4EC96D1AE5A2842A8A9FEEDB5",
      "actual_capture_hz": 27.25,
      "video_sha256": "9CFB7BCF0EA0002B5FA0E3ECB9D44A651B9B3C8D68F5DB68A460F7250479C186",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-body-v239-source.mp4": {
      "video": "domino-final-body-v239-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-final-body-v239",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-body-v239/result.json",
      "visual_review": "domino-final-body-v239/visual-review.json",
      "proxy_sha256": "C208F6DFB490D7900854BF53A1704F4E42149BC4EC96D1AE5A2842A8A9FEEDB5",
      "actual_capture_hz": 26.65,
      "video_sha256": "830D5D64DF0971953B891BC368BF365E6304E72469C01F231C7739D378877CCA",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-recovery-v241.mp4": {
      "video": "domino-final-recovery-v241.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-final-recovery-v241",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-recovery-v241/result.json",
      "visual_review": "domino-final-recovery-v241/visual-review.json",
      "proxy_sha256": "40102FC99C4310DFEB769B7AE9236C13FC961452C6D73E09F82BBE2108902111",
      "actual_capture_hz": 27.925,
      "video_sha256": "DC9E4A496B4EB5122A7B4F0057F9750EBF6FD53D857FF32046381D4B7346CF3B",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-recovery-v241-source.mp4": {
      "video": "domino-final-recovery-v241-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-final-recovery-v241",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-recovery-v241/result.json",
      "visual_review": "domino-final-recovery-v241/visual-review.json",
      "proxy_sha256": "40102FC99C4310DFEB769B7AE9236C13FC961452C6D73E09F82BBE2108902111",
      "actual_capture_hz": 28.325,
      "video_sha256": "EDFA3E23F0422380AD7D88FF280A6A0A0DB842E8EE006423EAA9BE75C044003C",
      "checks_passed": false,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-crouch-observe-v245.mp4": {
      "video": "domino-crouch-observe-v245.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-crouch-observe-v245",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-observe-v245/result.json",
      "visual_review": "domino-crouch-observe-v245/visual-review.json",
      "proxy_sha256": "40102FC99C4310DFEB769B7AE9236C13FC961452C6D73E09F82BBE2108902111",
      "actual_capture_hz": 27.25,
      "video_sha256": "CEC090B3D5441FB900767C5C5EE61F72B3BFFA6123B6AE940FA2A11EFC5FD537",
      "checks_passed": true,
      "scope": "Crouched native Domino observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-recovery-v244.mp4": {
      "video": "domino-final-recovery-v244.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-final-recovery-v244",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-recovery-v244/result.json",
      "visual_review": "domino-final-recovery-v244/visual-review.json",
      "proxy_sha256": "40102FC99C4310DFEB769B7AE9236C13FC961452C6D73E09F82BBE2108902111",
      "actual_capture_hz": 26.425,
      "video_sha256": "9D795F1CCAC33E176A10129E0E9300DD5B30EBEA52B53B25FD2EB4DDD897B0F9",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-final-recovery-v244-source.mp4": {
      "video": "domino-final-recovery-v244-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-final-recovery-v244",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-final-recovery-v244/result.json",
      "visual_review": "domino-final-recovery-v244/visual-review.json",
      "proxy_sha256": "40102FC99C4310DFEB769B7AE9236C13FC961452C6D73E09F82BBE2108902111",
      "actual_capture_hz": 26.925,
      "video_sha256": "3469002F8E94AE14B1D440CDBC6ABD8BF55D0D251B2BA7D3E1C05D592BE196C6",
      "checks_passed": true,
      "scope": "Two-target native Domino link feasibility on existing upper/lower tutorial guards. Strict read-only connection identity and beam endpoints are required; no replication or gameplay-outcome claim. Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. After native casts and source placement, persistent native marks/beam traverse native Steam. Body cast integration remains separately verified byv218. Native target deselection is tested with LT on an already linked guard, requiring source and peer to retain only the other target and preserve its mark incarnation. The observed400ms Cast_final_Select maps to the existing full-body selection clip because native scene resources use the identical native hand clip/duration; exact state72, arm/equipment identity and paired body gesture are verified. Fire, slide and other unobserved phases remain unsupported. NPC alignment, AI, damage/outcomes, unobserved body phases and complete-power acceptance remain pending. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-crouch-network-v246.mp4": {
      "video": "domino-crouch-network-v246.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-crouch-network-v246",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-network-v246/result.json",
      "visual_review": "domino-crouch-network-v246/visual-review.json",
      "proxy_sha256": "D4646A3336F8274581DF37767C7F805544AA7DEABED915688D81398F55B41A30",
      "actual_capture_hz": 25.866388308977037,
      "video_sha256": "19DB3DC6D8AD44305DB2E82EE90880E6DBE3FE81A6846658027F26CBE2D7B808",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and crouched empty-target controller casting drive three observed armed Emily complete-body crouched phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. This take exercises no target marks or links. Their earlier scoped presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-crouch-network-v246-source.mp4": {
      "video": "domino-crouch-network-v246-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-crouch-network-v246",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-network-v246/result.json",
      "visual_review": "domino-crouch-network-v246/visual-review.json",
      "proxy_sha256": "D4646A3336F8274581DF37767C7F805544AA7DEABED915688D81398F55B41A30",
      "actual_capture_hz": 26.0625,
      "video_sha256": "F701222135B7681826AEF474061C2471BB6791500643264A94295BE4EE8F1F7D",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and crouched empty-target controller casting drive three observed armed Emily complete-body crouched phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. This take exercises no target marks or links. Their earlier scoped presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-crouch-selection-observe-v249.mp4": {
      "video": "domino-crouch-selection-observe-v249.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-crouch-selection-observe-v249",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-selection-observe-v249/result.json",
      "visual_review": "domino-crouch-selection-observe-v249/visual-review.json",
      "proxy_sha256": "D4646A3336F8274581DF37767C7F805544AA7DEABED915688D81398F55B41A30",
      "actual_capture_hz": 27.3125,
      "video_sha256": "C898D0E7DA4BDE32777ED402ADDD2F746F240D74C747E5C3882E294ED76F7B74",
      "checks_passed": true,
      "scope": "Targeted crouched native Domino selection observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-crouch-network-v250.mp4": {
      "video": "domino-crouch-network-v250.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-crouch-network-v250",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-network-v250/result.json",
      "visual_review": "domino-crouch-network-v250/visual-review.json",
      "proxy_sha256": "7A9D569C92CE4FC1C1D9ACF9F3F6B481EA24ECDF2371D2C642222BDF93088411",
      "actual_capture_hz": 26.86847599164927,
      "video_sha256": "0E8F230754BF6145D89A402D23F0A4C3097842CB4F7F675726188D5AC9E37DBE",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and crouched controller casting drives observed armed Emily complete-body crouched phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. This take permits native target selection and records its effects. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-crouch-network-v250-source.mp4": {
      "video": "domino-crouch-network-v250-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-crouch-network-v250",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-network-v250/result.json",
      "visual_review": "domino-crouch-network-v250/visual-review.json",
      "proxy_sha256": "7A9D569C92CE4FC1C1D9ACF9F3F6B481EA24ECDF2371D2C642222BDF93088411",
      "actual_capture_hz": 26.555323590814197,
      "video_sha256": "50ED045AA314EC77F91496B869E8489B9565184E24CBA1A434EC3740F521829C",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and crouched controller casting drives observed armed Emily complete-body crouched phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. This take permits native target selection and records its effects. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-crouch-final-observe-v251.mp4": {
      "video": "domino-crouch-final-observe-v251.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-crouch-final-observe-v251",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-final-observe-v251/result.json",
      "visual_review": "domino-crouch-final-observe-v251/visual-review.json",
      "proxy_sha256": "7A9D569C92CE4FC1C1D9ACF9F3F6B481EA24ECDF2371D2C642222BDF93088411",
      "actual_capture_hz": 27.55741127348643,
      "video_sha256": "CD13DA130541E66AE072683EFB8BACB1C1B4C8AF4CA42EB61899B419A02F5663",
      "checks_passed": true,
      "scope": "Targeted crouched native Domino final-selection observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-crouch-final-network-v256.mp4": {
      "video": "domino-crouch-final-network-v256.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-crouch-final-network-v256",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-final-network-v256/result.json",
      "visual_review": "domino-crouch-final-network-v256/visual-review.json",
      "proxy_sha256": "8AC3107982810EC3C2AD8D854FFF3B36DED8107E34D2487DDE7051CD0BBB91C9",
      "actual_capture_hz": 26.0,
      "video_sha256": "95FBD316D58762AE6ECEE27DB089D7A90E3F64E217CFE2E138D85BE9B3B7A8AE",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and crouched controller casting drives observed armed Emily complete-body crouched phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-crouch-final-network-v256-source.mp4": {
      "video": "domino-crouch-final-network-v256-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-crouch-final-network-v256",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-crouch-final-network-v256/result.json",
      "visual_review": "domino-crouch-final-network-v256/visual-review.json",
      "proxy_sha256": "8AC3107982810EC3C2AD8D854FFF3B36DED8107E34D2487DDE7051CD0BBB91C9",
      "actual_capture_hz": 25.77777777777778,
      "video_sha256": "D410F5DECD504C86CE8498E0057A48CBC47449E4C85C1A6DC8F728635FC61627",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and crouched controller casting drives observed armed Emily complete-body crouched phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-holstered-standing-observe-v253.mp4": {
      "video": "domino-holstered-standing-observe-v253.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-holstered-standing-observe-v253",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-holstered-standing-observe-v253/result.json",
      "visual_review": "domino-holstered-standing-observe-v253/visual-review.json",
      "proxy_sha256": "8AC3107982810EC3C2AD8D854FFF3B36DED8107E34D2487DDE7051CD0BBB91C9",
      "actual_capture_hz": 26.993736951983298,
      "video_sha256": "49CC6F6E2460B8DE4915CC7DACF12143C6A39B8FF69A1314AF03D6E1D08DA6FA",
      "checks_passed": true,
      "scope": "Targeted native Domino holster and cast observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-holstered-crouched-observe-v253.mp4": {
      "video": "domino-holstered-crouched-observe-v253.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-holstered-crouched-observe-v253",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-holstered-crouched-observe-v253/result.json",
      "visual_review": "domino-holstered-crouched-observe-v253/visual-review.json",
      "proxy_sha256": "8AC3107982810EC3C2AD8D854FFF3B36DED8107E34D2487DDE7051CD0BBB91C9",
      "actual_capture_hz": 27.3125,
      "video_sha256": "3A669265EF6643D526ADA6388042BBC37DFC01A4178A6DC116A49BD486D471F6",
      "checks_passed": false,
      "scope": "Targeted native Domino holster and cast observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-context-standing-holstered-v258.mp4": {
      "video": "domino-context-standing-holstered-v258.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-context-standing-holstered-v258",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-standing-holstered-v258/result.json",
      "visual_review": "domino-context-standing-holstered-v258/visual-review.json",
      "proxy_sha256": "726A819E5EFC61CC5C8858940ECA41AC1B82B5AD6EC79A66747DB227E9B10E83",
      "actual_capture_hz": 27.75,
      "video_sha256": "E077098590BE8697772DDD5EB2176AEC693BA98F910CC347F963B16CEE08EAC5",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-standing-holstered-v258-source.mp4": {
      "video": "domino-context-standing-holstered-v258-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-context-standing-holstered-v258",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-standing-holstered-v258/result.json",
      "visual_review": "domino-context-standing-holstered-v258/visual-review.json",
      "proxy_sha256": "726A819E5EFC61CC5C8858940ECA41AC1B82B5AD6EC79A66747DB227E9B10E83",
      "actual_capture_hz": 27.4375,
      "video_sha256": "2F1D4EFC32654FAF4FC0DDB097C3142C9F1CC4B054ED34C132B3024CB17AC5BC",
      "checks_passed": false,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-standing-holstered-v261.mp4": {
      "video": "domino-context-standing-holstered-v261.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-context-standing-holstered-v261",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-standing-holstered-v261/result.json",
      "visual_review": "domino-context-standing-holstered-v261/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 25.875,
      "video_sha256": "701E583743E1B22DF7ED30C71A840BA179ECF399269CAC5AB0F3BAB4C07EF04F",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-standing-holstered-v261-source.mp4": {
      "video": "domino-context-standing-holstered-v261-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-context-standing-holstered-v261",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-standing-holstered-v261/result.json",
      "visual_review": "domino-context-standing-holstered-v261/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 26.25,
      "video_sha256": "CE36F540684A929A7987131F9ADFEFCF2D33FA3D9683612F3D7A9D182EC7A964",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-crouched-holstered-v261.mp4": {
      "video": "domino-context-crouched-holstered-v261.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-context-crouched-holstered-v261",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-v261/result.json",
      "visual_review": "domino-context-crouched-holstered-v261/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 25.8125,
      "video_sha256": "98FBEBCA93AE8D2EFABF1AF5911DC0036796EB62623111AB13F85BCA27398318",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-crouched-holstered-v261-source.mp4": {
      "video": "domino-context-crouched-holstered-v261-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-context-crouched-holstered-v261",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-v261/result.json",
      "visual_review": "domino-context-crouched-holstered-v261/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 26.4375,
      "video_sha256": "D5206F65100E7C86696D4FE22DEA307BFC8774E8894C6B973527DAB2710DF538",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-crouched-holstered-target-v262.mp4": {
      "video": "domino-context-crouched-holstered-target-v262.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-context-crouched-holstered-target-v262",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-target-v262/result.json",
      "visual_review": "domino-context-crouched-holstered-target-v262/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 26.8125,
      "video_sha256": "8EE7E49E5330A7BA2C118489F5DC479A351050B0CC7029DBE024639D949D76DF",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. This targeted take requires the ordinary selection phase and native mark; it places the source on a verified floor near a settled guard and holsters while looking upward. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-crouched-holstered-target-v262-source.mp4": {
      "video": "domino-context-crouched-holstered-target-v262-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-context-crouched-holstered-target-v262",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-target-v262/result.json",
      "visual_review": "domino-context-crouched-holstered-target-v262/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 26.86847599164927,
      "video_sha256": "C13A1851FFA6E19B691002E52A79865F09708181F33D709182FFCFA8DB24A300",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. This targeted take requires the ordinary selection phase and native mark; it places the source on a verified floor near a settled guard and holsters while looking upward. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-crouched-holstered-moving-v263.mp4": {
      "video": "domino-context-crouched-holstered-moving-v263.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-context-crouched-holstered-moving-v263",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-moving-v263/result.json",
      "visual_review": "domino-context-crouched-holstered-moving-v263/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 24.9375,
      "video_sha256": "29A06C4646EEC39C08031EE5206D5D735ED7E7D2A3ADA643C49CC3ECD6608058",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-crouched-holstered-moving-v263-source.mp4": {
      "video": "domino-context-crouched-holstered-moving-v263-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-context-crouched-holstered-moving-v263",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-moving-v263/result.json",
      "visual_review": "domino-context-crouched-holstered-moving-v263/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 25.55323590814196,
      "video_sha256": "20D3CC5A166C0D71CE57412CF394217F0B749CF8311D88017475A85C77C9704D",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives observed Emily complete-body stance/equipment phases then standing recovery through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-holstered-crouched-final-observe-v263.mp4": {
      "video": "domino-holstered-crouched-final-observe-v263.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-holstered-crouched-final-observe-v263",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-holstered-crouched-final-observe-v263/result.json",
      "visual_review": "domino-holstered-crouched-final-observe-v263/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 26.5,
      "video_sha256": "ADC3CDA66F3F8DBDBDDA17A4453C8DA5D843E6D19EF131AF91F5C8478EE676AE",
      "checks_passed": true,
      "scope": "Targeted holstered native Domino final-selection observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-holstered-standing-final-observe-v263.mp4": {
      "video": "domino-holstered-standing-final-observe-v263.mp4",
      "character": "emily",
      "label": "Native Domino source observation · domino-holstered-standing-final-observe-v263",
      "alias": "domino_native_observation",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-holstered-standing-final-observe-v263/result.json",
      "visual_review": "domino-holstered-standing-final-observe-v263/visual-review.json",
      "proxy_sha256": "C2ABD4788DA464A0A8DA274C16CB03CCFCFF216680216A88F796017DC270AD8B",
      "actual_capture_hz": 27.375,
      "video_sha256": "FC6E11796224CF551F488B7CFA833988F82447ACD57A941DA1B064787F26A7CC",
      "checks_passed": true,
      "scope": "Targeted holstered native Domino final-selection observation (no remote binding): exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required."
    },
    "domino-context-crouched-holstered-final-v264.mp4": {
      "video": "domino-context-crouched-holstered-final-v264.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-context-crouched-holstered-final-v264",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-final-v264/result.json",
      "visual_review": "domino-context-crouched-holstered-final-v264/visual-review.json",
      "proxy_sha256": "144672DAAA03A3ACEB942F6920D38F96055593D19A6CED7786679C07CB064800",
      "actual_capture_hz": 25.38888888888889,
      "video_sha256": "CAD6C34194F29C4597D5FB6021C47AA6998B7A01C060D70F3A06A84763F2B330",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives the explicit Emily stance/equipment phases then standing recovery through native Steam, with source scene timing and unchanged equipment state. One native scene incarnation changes subscenes. This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-crouched-holstered-final-v264-source.mp4": {
      "video": "domino-context-crouched-holstered-final-v264-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-context-crouched-holstered-final-v264",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-crouched-holstered-final-v264/result.json",
      "visual_review": "domino-context-crouched-holstered-final-v264/visual-review.json",
      "proxy_sha256": "144672DAAA03A3ACEB942F6920D38F96055593D19A6CED7786679C07CB064800",
      "actual_capture_hz": 26.5,
      "video_sha256": "A755B257343D52B0A2C2C4CE96506790DA12DAA17215FD49ABE9864221A34E00",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives the explicit Emily stance/equipment phases then standing recovery through native Steam, with source scene timing and unchanged equipment state. One native scene incarnation changes subscenes. This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-standing-holstered-final-v264.mp4": {
      "video": "domino-context-standing-holstered-final-v264.mp4",
      "character": "emily",
      "label": "Domino Steam body and mark presentation · domino-context-standing-holstered-final-v264",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-standing-holstered-final-v264/result.json",
      "visual_review": "domino-context-standing-holstered-final-v264/visual-review.json",
      "proxy_sha256": "144672DAAA03A3ACEB942F6920D38F96055593D19A6CED7786679C07CB064800",
      "actual_capture_hz": 26.055555555555557,
      "video_sha256": "A8805438F23F97FD6DFE5A7ED241DB145E602D00F7EECF65ED12317A96CB7069",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives the explicit Emily stance/equipment phases then standing recovery through native Steam, with source scene timing and unchanged equipment state. One native scene incarnation changes subscenes. This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "domino-context-standing-holstered-final-v264-source.mp4": {
      "video": "domino-context-standing-holstered-final-v264-source.mp4",
      "character": "emily",
      "label": "Domino native source · domino-context-standing-holstered-final-v264",
      "alias": "domino_native_mark_network",
      "requirements": [
        "emily_domino_mark_link_cancel"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "domino-context-standing-holstered-final-v264/result.json",
      "visual_review": "domino-context-standing-holstered-final-v264/visual-review.json",
      "proxy_sha256": "144672DAAA03A3ACEB942F6920D38F96055593D19A6CED7786679C07CB064800",
      "actual_capture_hz": 26.77777777777778,
      "video_sha256": "C10519E298BDB22D3FE685685B51622DD8C3BD4E421F1FC01220D95930C08440",
      "checks_passed": true,
      "scope": "Native source Domino acquisition and native controller casting drives the explicit Emily stance/equipment phases then standing recovery through native Steam, with source scene timing and unchanged equipment state. One native scene incarnation changes subscenes. This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement. Earlier scoped mark/beam presentation proofs remain separate; NPC AI and shared outcomes are unfinished. No claim of complete Domino support or visual acceptance. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1354a/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1352-domino.json",
    "decision": "complete",
    "scope": "Domino in the Emily source role in product mode: the cast, select and release gestures (variants 19-22), a mark on each of two real guards and the native link beam between them are presented on the host and held between casts (17 samples with both marks), and a native X cancellation of a third aim adds no mark. Marks and beam are cosmetic native FX on the partner presentation; the partner never links, damages or spends mana.",
    "takes": [
      {
        "take": "coverage-v1354a",
        "cases": [
          "client-dominolink"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1354a/client-dominolink-observer.mp4",
        "verdict": "On the upper floor Emily throws her hand toward the guard and a golden mark flares on him; downstairs she casts at the second guard and a bright golden link beam runs up to the first, staying in place while she aims once more and lowers her hand."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1354a/result.json](<../../../../work/natural-moveset/coverage-v1354a/result.json>)
- [in_game_recordings: domino-native-source-v187.mp4](<../../../../work/natural-moveset/domino-native-source-v187.mp4>)
- [in_game_recordings: domino-native-target-v191.mp4](<../../../../work/natural-moveset/domino-native-target-v191.mp4>)
- [in_game_recordings: domino-native-reader-v190.mp4](<../../../../work/natural-moveset/domino-native-reader-v190.mp4>)
- [in_game_recordings: domino-network-emily-v194.mp4](<../../../../work/natural-moveset/domino-network-emily-v194.mp4>)
- [in_game_recordings: domino-network-emily-v194-source.mp4](<../../../../work/natural-moveset/domino-network-emily-v194-source.mp4>)
- [in_game_recordings: domino-network-emily-v197.mp4](<../../../../work/natural-moveset/domino-network-emily-v197.mp4>)
- [in_game_recordings: domino-network-emily-v197-source.mp4](<../../../../work/natural-moveset/domino-network-emily-v197-source.mp4>)
- [in_game_recordings: domino-network-source-v198.mp4](<../../../../work/natural-moveset/domino-network-source-v198.mp4>)
- [in_game_recordings: domino-target-reader-v200.mp4](<../../../../work/natural-moveset/domino-target-reader-v200.mp4>)
- [in_game_recordings: domino-target-scoped-v202.mp4](<../../../../work/natural-moveset/domino-target-scoped-v202.mp4>)
- [in_game_recordings: domino-world-network-v204.mp4](<../../../../work/natural-moveset/domino-world-network-v204.mp4>)
- [in_game_recordings: domino-world-network-v204-source.mp4](<../../../../work/natural-moveset/domino-world-network-v204-source.mp4>)
- [in_game_recordings: domino-world-network-v206.mp4](<../../../../work/natural-moveset/domino-world-network-v206.mp4>)
- [in_game_recordings: domino-world-network-v206-source.mp4](<../../../../work/natural-moveset/domino-world-network-v206-source.mp4>)
- [in_game_recordings: domino-mark-retirement-v208.mp4](<../../../../work/natural-moveset/domino-mark-retirement-v208.mp4>)
- [in_game_recordings: domino-mark-retirement-v208-source.mp4](<../../../../work/natural-moveset/domino-mark-retirement-v208-source.mp4>)
- [in_game_recordings: domino-mark-fade-v209.mp4](<../../../../work/natural-moveset/domino-mark-fade-v209.mp4>)
- [in_game_recordings: domino-mark-fade-v209-source.mp4](<../../../../work/natural-moveset/domino-mark-fade-v209-source.mp4>)
- [in_game_recordings: domino-partner-picker-v212.mp4](<../../../../work/natural-moveset/domino-partner-picker-v212.mp4>)
- [in_game_recordings: domino-guard-regression-v213.mp4](<../../../../work/natural-moveset/domino-guard-regression-v213.mp4>)
- [in_game_recordings: domino-guard-regression-v213-source.mp4](<../../../../work/natural-moveset/domino-guard-regression-v213-source.mp4>)
- [in_game_recordings: domino-two-target-source-v214.mp4](<../../../../work/natural-moveset/domino-two-target-source-v214.mp4>)
- [in_game_recordings: domino-guard-grounded-v216.mp4](<../../../../work/natural-moveset/domino-guard-grounded-v216.mp4>)
- [in_game_recordings: domino-guard-grounded-v216-source.mp4](<../../../../work/natural-moveset/domino-guard-grounded-v216-source.mp4>)
- [in_game_recordings: domino-link-reader-v217.mp4](<../../../../work/natural-moveset/domino-link-reader-v217.mp4>)
- [in_game_recordings: domino-guard-feedback-v218.mp4](<../../../../work/natural-moveset/domino-guard-feedback-v218.mp4>)
- [in_game_recordings: domino-guard-feedback-v218-source.mp4](<../../../../work/natural-moveset/domino-guard-feedback-v218-source.mp4>)
- [in_game_recordings: domino-link-patrol-v219.mp4](<../../../../work/natural-moveset/domino-link-patrol-v219.mp4>)
- [in_game_recordings: domino-link-clear-view-v221.mp4](<../../../../work/natural-moveset/domino-link-clear-view-v221.mp4>)
- [in_game_recordings: domino-beam-network-v223.mp4](<../../../../work/natural-moveset/domino-beam-network-v223.mp4>)
- [in_game_recordings: domino-beam-network-v223-source.mp4](<../../../../work/natural-moveset/domino-beam-network-v223-source.mp4>)
- [in_game_recordings: domino-beam-view-v225.mp4](<../../../../work/natural-moveset/domino-beam-view-v225.mp4>)
- [in_game_recordings: domino-beam-view-v225-source.mp4](<../../../../work/natural-moveset/domino-beam-view-v225-source.mp4>)
- [in_game_recordings: domino-native-cancel-v226.mp4](<../../../../work/natural-moveset/domino-native-cancel-v226.mp4>)
- [in_game_recordings: domino-native-cancel-v226-source.mp4](<../../../../work/natural-moveset/domino-native-cancel-v226-source.mp4>)
- [in_game_recordings: domino-native-cancel-v227.mp4](<../../../../work/natural-moveset/domino-native-cancel-v227.mp4>)
- [in_game_recordings: domino-native-cancel-v227-source.mp4](<../../../../work/natural-moveset/domino-native-cancel-v227-source.mp4>)
- [in_game_recordings: domino-native-cancel-v228.mp4](<../../../../work/natural-moveset/domino-native-cancel-v228.mp4>)
- [in_game_recordings: domino-native-cancel-v228-source.mp4](<../../../../work/natural-moveset/domino-native-cancel-v228-source.mp4>)
- [in_game_recordings: domino-native-cancel-v230.mp4](<../../../../work/natural-moveset/domino-native-cancel-v230.mp4>)
- [in_game_recordings: domino-native-cancel-v230-source.mp4](<../../../../work/natural-moveset/domino-native-cancel-v230-source.mp4>)
- [in_game_recordings: domino-native-cancel-v231.mp4](<../../../../work/natural-moveset/domino-native-cancel-v231.mp4>)
- [in_game_recordings: domino-native-cancel-v231-source.mp4](<../../../../work/natural-moveset/domino-native-cancel-v231-source.mp4>)
- [in_game_recordings: domino-native-cancel-v232.mp4](<../../../../work/natural-moveset/domino-native-cancel-v232.mp4>)
- [in_game_recordings: domino-native-cancel-v232-source.mp4](<../../../../work/natural-moveset/domino-native-cancel-v232-source.mp4>)
- [in_game_recordings: domino-native-cancel-v233.mp4](<../../../../work/natural-moveset/domino-native-cancel-v233.mp4>)
- [in_game_recordings: domino-native-cancel-v233-source.mp4](<../../../../work/natural-moveset/domino-native-cancel-v233-source.mp4>)
- [in_game_recordings: domino-native-deselect-v234.mp4](<../../../../work/natural-moveset/domino-native-deselect-v234.mp4>)
- [in_game_recordings: domino-native-deselect-v234-source.mp4](<../../../../work/natural-moveset/domino-native-deselect-v234-source.mp4>)
- [in_game_recordings: domino-native-deselect-v235.mp4](<../../../../work/natural-moveset/domino-native-deselect-v235.mp4>)
- [in_game_recordings: domino-native-deselect-v235-source.mp4](<../../../../work/natural-moveset/domino-native-deselect-v235-source.mp4>)
- [in_game_recordings: domino-final-body-v238.mp4](<../../../../work/natural-moveset/domino-final-body-v238.mp4>)
- [in_game_recordings: domino-final-body-v238-source.mp4](<../../../../work/natural-moveset/domino-final-body-v238-source.mp4>)
- [in_game_recordings: domino-final-body-v239.mp4](<../../../../work/natural-moveset/domino-final-body-v239.mp4>)
- [in_game_recordings: domino-final-body-v239-source.mp4](<../../../../work/natural-moveset/domino-final-body-v239-source.mp4>)
- [in_game_recordings: domino-final-recovery-v241.mp4](<../../../../work/natural-moveset/domino-final-recovery-v241.mp4>)
- [in_game_recordings: domino-final-recovery-v241-source.mp4](<../../../../work/natural-moveset/domino-final-recovery-v241-source.mp4>)
- [in_game_recordings: domino-crouch-observe-v245.mp4](<../../../../work/natural-moveset/domino-crouch-observe-v245.mp4>)
- [in_game_recordings: domino-final-recovery-v244.mp4](<../../../../work/natural-moveset/domino-final-recovery-v244.mp4>)
- [in_game_recordings: domino-final-recovery-v244-source.mp4](<../../../../work/natural-moveset/domino-final-recovery-v244-source.mp4>)
- [in_game_recordings: domino-crouch-network-v246.mp4](<../../../../work/natural-moveset/domino-crouch-network-v246.mp4>)
- [in_game_recordings: domino-crouch-network-v246-source.mp4](<../../../../work/natural-moveset/domino-crouch-network-v246-source.mp4>)
- [in_game_recordings: domino-crouch-selection-observe-v249.mp4](<../../../../work/natural-moveset/domino-crouch-selection-observe-v249.mp4>)
- [in_game_recordings: domino-crouch-network-v250.mp4](<../../../../work/natural-moveset/domino-crouch-network-v250.mp4>)
- [in_game_recordings: domino-crouch-network-v250-source.mp4](<../../../../work/natural-moveset/domino-crouch-network-v250-source.mp4>)
- [in_game_recordings: domino-crouch-final-observe-v251.mp4](<../../../../work/natural-moveset/domino-crouch-final-observe-v251.mp4>)
- [in_game_recordings: domino-crouch-final-network-v256.mp4](<../../../../work/natural-moveset/domino-crouch-final-network-v256.mp4>)
- [in_game_recordings: domino-crouch-final-network-v256-source.mp4](<../../../../work/natural-moveset/domino-crouch-final-network-v256-source.mp4>)
- [in_game_recordings: domino-holstered-standing-observe-v253.mp4](<../../../../work/natural-moveset/domino-holstered-standing-observe-v253.mp4>)
- [in_game_recordings: domino-holstered-crouched-observe-v253.mp4](<../../../../work/natural-moveset/domino-holstered-crouched-observe-v253.mp4>)
- [in_game_recordings: domino-context-standing-holstered-v258.mp4](<../../../../work/natural-moveset/domino-context-standing-holstered-v258.mp4>)
- [in_game_recordings: domino-context-standing-holstered-v258-source.mp4](<../../../../work/natural-moveset/domino-context-standing-holstered-v258-source.mp4>)
- [in_game_recordings: domino-context-standing-holstered-v261.mp4](<../../../../work/natural-moveset/domino-context-standing-holstered-v261.mp4>)
- [in_game_recordings: domino-context-standing-holstered-v261-source.mp4](<../../../../work/natural-moveset/domino-context-standing-holstered-v261-source.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-v261.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-v261.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-v261-source.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-v261-source.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-target-v262.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-target-v262.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-target-v262-source.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-target-v262-source.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-moving-v263.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-moving-v263.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-moving-v263-source.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-moving-v263-source.mp4>)
- [in_game_recordings: domino-holstered-crouched-final-observe-v263.mp4](<../../../../work/natural-moveset/domino-holstered-crouched-final-observe-v263.mp4>)
- [in_game_recordings: domino-holstered-standing-final-observe-v263.mp4](<../../../../work/natural-moveset/domino-holstered-standing-final-observe-v263.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-final-v264.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-final-v264.mp4>)
- [in_game_recordings: domino-context-crouched-holstered-final-v264-source.mp4](<../../../../work/natural-moveset/domino-context-crouched-holstered-final-v264-source.mp4>)
- [in_game_recordings: domino-context-standing-holstered-final-v264.mp4](<../../../../work/natural-moveset/domino-context-standing-holstered-final-v264.mp4>)
- [in_game_recordings: domino-context-standing-holstered-final-v264-source.mp4](<../../../../work/natural-moveset/domino-context-standing-holstered-final-v264-source.mp4>)
## emily_doppelganger_cast

Native493/494 show standing/crouched Emily keeps the sword sheathed throughout the466/1200/666ms Doppelganger phases. 495 appends six complete-body hands-free phase clips to bank57/331, retaining all325 previous payloads; semantic53 accepts the observed native equipment context. 496/497 pass remote body/clone/FX and standing-recovery checks. Standing/crouched armed evidence491 and continuous clone/material-reuse evidence485/486/488 remain preserved. Airborne/cancellation/upgrades/multiple clones, shared outcomes and visual acceptance remain open; whole movement/playable co-op incomplete. Native cancellation505: confirmed X-with-LT cancels with no source activation/mana/clone scene and no remote clone/FX;503 standing armed and504 crouched holstered recover full body. Wrong-input499/500 and array-assertion502 failures are preserved. Native air, other interruptions, upgrades/multiple clones, late join and shared gameplay remain unfinished.

````json
{
  "id": "emily_doppelganger_cast",
  "family": "powers",
  "characters": [
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "grounded_armed_and_holstered_scoped",
  "visual_validation": "accepted_completion_audit",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "emily doppelganger cast",
  "motions": [
    "power_doppelganger_fire_in",
    "power_doppelganger_fire_loop",
    "power_doppelganger_fire_out",
    "power_doppelganger_sequence",
    "power_doppelganger_cancel",
    "native_doppelganger_in",
    "native_doppelganger_in_crouched",
    "native_doppelganger_loop",
    "native_doppelganger_loop_crouched",
    "native_doppelganger_out",
    "native_doppelganger_out_crouched",
    "native_doppelganger_in_standing_empty",
    "native_doppelganger_in_crouched_empty",
    "native_doppelganger_loop_standing_empty",
    "native_doppelganger_loop_crouched_empty",
    "native_doppelganger_out_standing_empty",
    "native_doppelganger_out_crouched_empty"
  ],
  "deliverable_kind": "animation",
  "notes": "Native493/494 show standing/crouched Emily keeps the sword sheathed throughout the466/1200/666ms Doppelganger phases. 495 appends six complete-body hands-free phase clips to bank57/331, retaining all325 previous payloads; semantic53 accepts the observed native equipment context. 496/497 pass remote body/clone/FX and standing-recovery checks. Standing/crouched armed evidence491 and continuous clone/material-reuse evidence485/486/488 remain preserved. Airborne/cancellation/upgrades/multiple clones, shared outcomes and visual acceptance remain open; whole movement/playable co-op incomplete. Native cancellation505: confirmed X-with-LT cancels with no source activation/mana/clone scene and no remote clone/FX;503 standing armed and504 crouched holstered recover full body. Wrong-input499/500 and array-assertion502 failures are preserved. Native air, other interruptions, upgrades/multiple clones, late join and shared gameplay remain unfinished.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "doppelganger",
      "character": "emily",
      "requirements": [
        "emily_doppelganger_cast"
      ],
      "needs": [
        "native summon/expiry presentation",
        "independent summoned clone identity distinct from the remote player avatar",
        "single clone lifecycle and outcomes"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "native-doppelganger-source-v406b.mp4",
    "doppelganger-network-v408.mp4",
    "doppelganger-network-v408-source.mp4",
    "doppelganger-network-v409.mp4",
    "doppelganger-network-v409-source.mp4",
    "doppelganger-network-v411.mp4",
    "doppelganger-network-v411-source.mp4",
    "doppelganger-world-ownership-v415.mp4",
    "doppelganger-world-ownership-v417.mp4",
    "doppelganger-stream-v419.mp4",
    "doppelganger-stream-v419-source.mp4",
    "doppelganger-rig-v422.mp4",
    "doppelganger-visual-v424.mp4",
    "doppelganger-visual-v424-source.mp4",
    "doppelganger-visual-v426.mp4",
    "doppelganger-visual-v426-source.mp4",
    "doppelganger-visual-v428.mp4",
    "doppelganger-visual-v428-source.mp4",
    "doppelganger-visual-v430.mp4",
    "doppelganger-visual-v430-source.mp4",
    "doppelganger-fx-v433.mp4",
    "doppelganger-fx-v433-source.mp4",
    "doppelganger-fx-v435.mp4",
    "doppelganger-fx-v435-source.mp4",
    "doppelganger-midnight-v437.mp4",
    "doppelganger-midnight-v437-source.mp4",
    "midnight-fx-v439.mp4",
    "midnight-fx-v439-source.mp4",
    "midnight-fx-v440.mp4",
    "midnight-fx-v440-source.mp4",
    "midnight-fx-v443.mp4",
    "midnight-fx-v443-source.mp4",
    "midnight-fx-v446.mp4",
    "midnight-fx-v446-source.mp4",
    "doppelganger-fx-v450.mp4",
    "doppelganger-fx-v450-source.mp4",
    "doppelganger-fx-v453.mp4",
    "doppelganger-fx-v453-source.mp4",
    "doppelganger-fx-v455.mp4",
    "doppelganger-fx-v455-source.mp4",
    "doppelganger-fx-v457.mp4",
    "doppelganger-fx-v457-source.mp4",
    "doppelganger-fx-v460.mp4",
    "doppelganger-fx-v460-source.mp4",
    "doppelganger-fx-v463.mp4",
    "doppelganger-fx-v463-source.mp4",
    "doppelganger-fx-v465.mp4",
    "doppelganger-fx-v465-source.mp4",
    "doppelganger-fx-v469.mp4",
    "doppelganger-fx-v469-source.mp4",
    "doppelganger-fx-v470.mp4",
    "doppelganger-fx-v470-source.mp4",
    "doppelganger-fx-v473.mp4",
    "doppelganger-fx-v473-source.mp4",
    "doppelganger-fx-v476.mp4",
    "doppelganger-fx-v476-source.mp4",
    "doppelganger-fx-v478.mp4",
    "doppelganger-fx-v478-source.mp4",
    "doppelganger-fx-v480.mp4",
    "doppelganger-fx-v480-source.mp4",
    "doppelganger-fx-v482.mp4",
    "doppelganger-fx-v482-source.mp4",
    "doppelganger-fx-v485.mp4",
    "doppelganger-fx-v485-source.mp4",
    "doppelganger-fx-v486.mp4",
    "doppelganger-fx-v486-source.mp4",
    "doppelganger-fx-v488.mp4",
    "doppelganger-fx-v488-source.mp4",
    "native-doppelganger-crouch-v489.mp4",
    "doppelganger-crouch-v491.mp4",
    "doppelganger-crouch-v491-source.mp4",
    "native-doppelganger-holstered-standing-v493.mp4",
    "native-doppelganger-holstered-crouched-v494.mp4",
    "doppelganger-holstered-standing-v496.mp4",
    "doppelganger-holstered-standing-v496-source.mp4",
    "doppelganger-holstered-crouched-v497.mp4",
    "doppelganger-holstered-crouched-v497-source.mp4",
    "native-doppelganger-cancel-standing-v499.mp4",
    "native-doppelganger-cancel-standing-v500.mp4",
    "native-doppelganger-cancel-standing-v501.mp4",
    "doppelganger-cancel-standing-v502.mp4",
    "doppelganger-cancel-standing-v502-source.mp4",
    "doppelganger-cancel-standing-v503.mp4",
    "doppelganger-cancel-standing-v503-source.mp4",
    "doppelganger-cancel-crouched-holstered-v504.mp4",
    "doppelganger-cancel-crouched-holstered-v504-source.mp4",
    "native-doppelganger-air-release-v506.mp4",
    "native-doppelganger-air-entry-holstered-v507.mp4",
    "doppelganger-air-release-armed-v509.mp4",
    "doppelganger-air-release-armed-v509-source.mp4",
    "doppelganger-air-landing-trace-v513.mp4",
    "doppelganger-air-landing-trace-v513-source.mp4",
    "doppelganger-air-entry-holstered-v510.mp4",
    "doppelganger-air-entry-holstered-v510-source.mp4"
  ],
  "native_effects_status": "scoped_native_summon_and_disappearance_verified",
  "complete": true,
  "native_effects_evidence": [
    "doppelganger-fx-v485/result.json",
    "doppelganger-fx-v486/result.json",
    "doppelganger-fx-v488/visual-review.json"
  ],
  "completion": {
    "audit": "completion-audit-v1339-emily-clone-spirit.json",
    "decision": "complete",
    "scope": "Emily casts Doppelganger (power variants 29-31): the partner presents the cast, the native summon effect, a separate clone beside her that moves off on its own, and its retirement.",
    "takes": [
      {
        "take": "coverage-v1332e",
        "cases": [
          "client-doppelganger"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "client-doppelganger"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1332e/client-doppelganger-observer.mp4",
        "verdict": "A bright summon burst rises beside Emily; a second Emily appears next to her, runs off, and is gone about two seconds later while Emily stands armed."
      }
    ],
    "lifecycle": "See method."
  },
  "evidence": [
    "coverage-v1332c/result.json",
    "coverage-v1332e/result.json",
    "coverage-v1332d/result.json"
  ],
  "visual_validation_before": "not_accepted"
}
````

- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332e/result.json](<../../../../work/natural-moveset/coverage-v1332e/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: native-doppelganger-source-v406b.mp4](<../../../../work/natural-moveset/native-doppelganger-source-v406b.mp4>)
- [in_game_recordings: doppelganger-network-v408.mp4](<../../../../work/natural-moveset/doppelganger-network-v408.mp4>)
- [in_game_recordings: doppelganger-network-v408-source.mp4](<../../../../work/natural-moveset/doppelganger-network-v408-source.mp4>)
- [in_game_recordings: doppelganger-network-v409.mp4](<../../../../work/natural-moveset/doppelganger-network-v409.mp4>)
- [in_game_recordings: doppelganger-network-v409-source.mp4](<../../../../work/natural-moveset/doppelganger-network-v409-source.mp4>)
- [in_game_recordings: doppelganger-network-v411.mp4](<../../../../work/natural-moveset/doppelganger-network-v411.mp4>)
- [in_game_recordings: doppelganger-network-v411-source.mp4](<../../../../work/natural-moveset/doppelganger-network-v411-source.mp4>)
- [in_game_recordings: doppelganger-world-ownership-v415.mp4](<../../../../work/natural-moveset/doppelganger-world-ownership-v415.mp4>)
- [in_game_recordings: doppelganger-world-ownership-v417.mp4](<../../../../work/natural-moveset/doppelganger-world-ownership-v417.mp4>)
- [in_game_recordings: doppelganger-stream-v419.mp4](<../../../../work/natural-moveset/doppelganger-stream-v419.mp4>)
- [in_game_recordings: doppelganger-stream-v419-source.mp4](<../../../../work/natural-moveset/doppelganger-stream-v419-source.mp4>)
- [in_game_recordings: doppelganger-rig-v422.mp4](<../../../../work/natural-moveset/doppelganger-rig-v422.mp4>)
- [in_game_recordings: doppelganger-visual-v424.mp4](<../../../../work/natural-moveset/doppelganger-visual-v424.mp4>)
- [in_game_recordings: doppelganger-visual-v424-source.mp4](<../../../../work/natural-moveset/doppelganger-visual-v424-source.mp4>)
- [in_game_recordings: doppelganger-visual-v426.mp4](<../../../../work/natural-moveset/doppelganger-visual-v426.mp4>)
- [in_game_recordings: doppelganger-visual-v426-source.mp4](<../../../../work/natural-moveset/doppelganger-visual-v426-source.mp4>)
- [in_game_recordings: doppelganger-visual-v428.mp4](<../../../../work/natural-moveset/doppelganger-visual-v428.mp4>)
- [in_game_recordings: doppelganger-visual-v428-source.mp4](<../../../../work/natural-moveset/doppelganger-visual-v428-source.mp4>)
- [in_game_recordings: doppelganger-visual-v430.mp4](<../../../../work/natural-moveset/doppelganger-visual-v430.mp4>)
- [in_game_recordings: doppelganger-visual-v430-source.mp4](<../../../../work/natural-moveset/doppelganger-visual-v430-source.mp4>)
- [in_game_recordings: doppelganger-fx-v433.mp4](<../../../../work/natural-moveset/doppelganger-fx-v433.mp4>)
- [in_game_recordings: doppelganger-fx-v433-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v433-source.mp4>)
- [in_game_recordings: doppelganger-fx-v435.mp4](<../../../../work/natural-moveset/doppelganger-fx-v435.mp4>)
- [in_game_recordings: doppelganger-fx-v435-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v435-source.mp4>)
- [in_game_recordings: doppelganger-midnight-v437.mp4](<../../../../work/natural-moveset/doppelganger-midnight-v437.mp4>)
- [in_game_recordings: doppelganger-midnight-v437-source.mp4](<../../../../work/natural-moveset/doppelganger-midnight-v437-source.mp4>)
- [in_game_recordings: midnight-fx-v439.mp4](<../../../../work/natural-moveset/midnight-fx-v439.mp4>)
- [in_game_recordings: midnight-fx-v439-source.mp4](<../../../../work/natural-moveset/midnight-fx-v439-source.mp4>)
- [in_game_recordings: midnight-fx-v440.mp4](<../../../../work/natural-moveset/midnight-fx-v440.mp4>)
- [in_game_recordings: midnight-fx-v440-source.mp4](<../../../../work/natural-moveset/midnight-fx-v440-source.mp4>)
- [in_game_recordings: midnight-fx-v443.mp4](<../../../../work/natural-moveset/midnight-fx-v443.mp4>)
- [in_game_recordings: midnight-fx-v443-source.mp4](<../../../../work/natural-moveset/midnight-fx-v443-source.mp4>)
- [in_game_recordings: midnight-fx-v446.mp4](<../../../../work/natural-moveset/midnight-fx-v446.mp4>)
- [in_game_recordings: midnight-fx-v446-source.mp4](<../../../../work/natural-moveset/midnight-fx-v446-source.mp4>)
- [in_game_recordings: doppelganger-fx-v450.mp4](<../../../../work/natural-moveset/doppelganger-fx-v450.mp4>)
- [in_game_recordings: doppelganger-fx-v450-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v450-source.mp4>)
- [in_game_recordings: doppelganger-fx-v453.mp4](<../../../../work/natural-moveset/doppelganger-fx-v453.mp4>)
- [in_game_recordings: doppelganger-fx-v453-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v453-source.mp4>)
- [in_game_recordings: doppelganger-fx-v455.mp4](<../../../../work/natural-moveset/doppelganger-fx-v455.mp4>)
- [in_game_recordings: doppelganger-fx-v455-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v455-source.mp4>)
- [in_game_recordings: doppelganger-fx-v457.mp4](<../../../../work/natural-moveset/doppelganger-fx-v457.mp4>)
- [in_game_recordings: doppelganger-fx-v457-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v457-source.mp4>)
- [in_game_recordings: doppelganger-fx-v460.mp4](<../../../../work/natural-moveset/doppelganger-fx-v460.mp4>)
- [in_game_recordings: doppelganger-fx-v460-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v460-source.mp4>)
- [in_game_recordings: doppelganger-fx-v463.mp4](<../../../../work/natural-moveset/doppelganger-fx-v463.mp4>)
- [in_game_recordings: doppelganger-fx-v463-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v463-source.mp4>)
- [in_game_recordings: doppelganger-fx-v465.mp4](<../../../../work/natural-moveset/doppelganger-fx-v465.mp4>)
- [in_game_recordings: doppelganger-fx-v465-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v465-source.mp4>)
- [in_game_recordings: doppelganger-fx-v469.mp4](<../../../../work/natural-moveset/doppelganger-fx-v469.mp4>)
- [in_game_recordings: doppelganger-fx-v469-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v469-source.mp4>)
- [in_game_recordings: doppelganger-fx-v470.mp4](<../../../../work/natural-moveset/doppelganger-fx-v470.mp4>)
- [in_game_recordings: doppelganger-fx-v470-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v470-source.mp4>)
- [in_game_recordings: doppelganger-fx-v473.mp4](<../../../../work/natural-moveset/doppelganger-fx-v473.mp4>)
- [in_game_recordings: doppelganger-fx-v473-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v473-source.mp4>)
- [in_game_recordings: doppelganger-fx-v476.mp4](<../../../../work/natural-moveset/doppelganger-fx-v476.mp4>)
- [in_game_recordings: doppelganger-fx-v476-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v476-source.mp4>)
- [in_game_recordings: doppelganger-fx-v478.mp4](<../../../../work/natural-moveset/doppelganger-fx-v478.mp4>)
- [in_game_recordings: doppelganger-fx-v478-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v478-source.mp4>)
- [in_game_recordings: doppelganger-fx-v480.mp4](<../../../../work/natural-moveset/doppelganger-fx-v480.mp4>)
- [in_game_recordings: doppelganger-fx-v480-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v480-source.mp4>)
- [in_game_recordings: doppelganger-fx-v482.mp4](<../../../../work/natural-moveset/doppelganger-fx-v482.mp4>)
- [in_game_recordings: doppelganger-fx-v482-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v482-source.mp4>)
- [in_game_recordings: doppelganger-fx-v485.mp4](<../../../../work/natural-moveset/doppelganger-fx-v485.mp4>)
- [in_game_recordings: doppelganger-fx-v485-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v485-source.mp4>)
- [in_game_recordings: doppelganger-fx-v486.mp4](<../../../../work/natural-moveset/doppelganger-fx-v486.mp4>)
- [in_game_recordings: doppelganger-fx-v486-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v486-source.mp4>)
- [in_game_recordings: doppelganger-fx-v488.mp4](<../../../../work/natural-moveset/doppelganger-fx-v488.mp4>)
- [in_game_recordings: doppelganger-fx-v488-source.mp4](<../../../../work/natural-moveset/doppelganger-fx-v488-source.mp4>)
- [in_game_recordings: native-doppelganger-crouch-v489.mp4](<../../../../work/natural-moveset/native-doppelganger-crouch-v489.mp4>)
- [in_game_recordings: doppelganger-crouch-v491.mp4](<../../../../work/natural-moveset/doppelganger-crouch-v491.mp4>)
- [in_game_recordings: doppelganger-crouch-v491-source.mp4](<../../../../work/natural-moveset/doppelganger-crouch-v491-source.mp4>)
- [in_game_recordings: native-doppelganger-holstered-standing-v493.mp4](<../../../../work/natural-moveset/native-doppelganger-holstered-standing-v493.mp4>)
- [in_game_recordings: native-doppelganger-holstered-crouched-v494.mp4](<../../../../work/natural-moveset/native-doppelganger-holstered-crouched-v494.mp4>)
- [in_game_recordings: doppelganger-holstered-standing-v496.mp4](<../../../../work/natural-moveset/doppelganger-holstered-standing-v496.mp4>)
- [in_game_recordings: doppelganger-holstered-standing-v496-source.mp4](<../../../../work/natural-moveset/doppelganger-holstered-standing-v496-source.mp4>)
- [in_game_recordings: doppelganger-holstered-crouched-v497.mp4](<../../../../work/natural-moveset/doppelganger-holstered-crouched-v497.mp4>)
- [in_game_recordings: doppelganger-holstered-crouched-v497-source.mp4](<../../../../work/natural-moveset/doppelganger-holstered-crouched-v497-source.mp4>)
- [in_game_recordings: native-doppelganger-cancel-standing-v499.mp4](<../../../../work/natural-moveset/native-doppelganger-cancel-standing-v499.mp4>)
- [in_game_recordings: native-doppelganger-cancel-standing-v500.mp4](<../../../../work/natural-moveset/native-doppelganger-cancel-standing-v500.mp4>)
- [in_game_recordings: native-doppelganger-cancel-standing-v501.mp4](<../../../../work/natural-moveset/native-doppelganger-cancel-standing-v501.mp4>)
- [in_game_recordings: doppelganger-cancel-standing-v502.mp4](<../../../../work/natural-moveset/doppelganger-cancel-standing-v502.mp4>)
- [in_game_recordings: doppelganger-cancel-standing-v502-source.mp4](<../../../../work/natural-moveset/doppelganger-cancel-standing-v502-source.mp4>)
- [in_game_recordings: doppelganger-cancel-standing-v503.mp4](<../../../../work/natural-moveset/doppelganger-cancel-standing-v503.mp4>)
- [in_game_recordings: doppelganger-cancel-standing-v503-source.mp4](<../../../../work/natural-moveset/doppelganger-cancel-standing-v503-source.mp4>)
- [in_game_recordings: doppelganger-cancel-crouched-holstered-v504.mp4](<../../../../work/natural-moveset/doppelganger-cancel-crouched-holstered-v504.mp4>)
- [in_game_recordings: doppelganger-cancel-crouched-holstered-v504-source.mp4](<../../../../work/natural-moveset/doppelganger-cancel-crouched-holstered-v504-source.mp4>)
- [in_game_recordings: native-doppelganger-air-release-v506.mp4](<../../../../work/natural-moveset/native-doppelganger-air-release-v506.mp4>)
- [in_game_recordings: native-doppelganger-air-entry-holstered-v507.mp4](<../../../../work/natural-moveset/native-doppelganger-air-entry-holstered-v507.mp4>)
- [in_game_recordings: doppelganger-air-release-armed-v509.mp4](<../../../../work/natural-moveset/doppelganger-air-release-armed-v509.mp4>)
- [in_game_recordings: doppelganger-air-release-armed-v509-source.mp4](<../../../../work/natural-moveset/doppelganger-air-release-armed-v509-source.mp4>)
- [in_game_recordings: doppelganger-air-landing-trace-v513.mp4](<../../../../work/natural-moveset/doppelganger-air-landing-trace-v513.mp4>)
- [in_game_recordings: doppelganger-air-landing-trace-v513-source.mp4](<../../../../work/natural-moveset/doppelganger-air-landing-trace-v513-source.mp4>)
- [in_game_recordings: doppelganger-air-entry-holstered-v510.mp4](<../../../../work/natural-moveset/doppelganger-air-entry-holstered-v510.mp4>)
- [in_game_recordings: doppelganger-air-entry-holstered-v510-source.mp4](<../../../../work/natural-moveset/doppelganger-air-entry-holstered-v510-source.mp4>)
## emily_mesmerize_cast

Visual sweep 1260: accepted (emily accept). The Mesmerize cast body reads naturally, and the native spirit world effect appears and clears on the observer. Native Emily enter/aim/release clocks drive the remote complete body through Steam. Committed source world states drive one cosmetic model, native scenes and owned particles. Individual takes specify cancellation, expiry and recovery coverage. Latest inspected passing take mesmerize-empty-air-release-network-v181: Both actual-game sheets inspected. Empty-hand native aim switches to airborne release; peer retains the hidden sword, changing cast-arm and leg motion, landing and native standing recovery. All17 scoped checks pass;45 continuity samples have weight1.0 and zero rejections/dropouts. Three source and three peer airborne release samples;1.4857m peak rise returns to the same floor. Native empty-target world case creates no invented peer links. Effects retire; both checkpoint restorations and protected14 pass. Broader visual acceptance, directions and shared NPC outcomes remain open. Authoritative NPC outcomes, untested stance/direction and interruption contexts, and full visual acceptance remain incomplete. Individual failed takes and their reasons are retained.

````json
{
  "id": "emily_mesmerize_cast",
  "family": "powers",
  "characters": [
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_observed_mesmerize_body_and_cosmetic_world_contexts_partial_verified",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "emily mesmerize cast",
  "motions": [
    "power_mesmerize_fire_in",
    "power_mesmerize_fire_loop",
    "power_mesmerize_fire_out",
    "power_mesmerize_sequence",
    "power_mesmerize_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (emily accept). The Mesmerize cast body reads naturally, and the native spirit world effect appears and clears on the observer. Native Emily enter/aim/release clocks drive the remote complete body through Steam. Committed source world states drive one cosmetic model, native scenes and owned particles. Individual takes specify cancellation, expiry and recovery coverage. Latest inspected passing take mesmerize-empty-air-release-network-v181: Both actual-game sheets inspected. Empty-hand native aim switches to airborne release; peer retains the hidden sword, changing cast-arm and leg motion, landing and native standing recovery. All17 scoped checks pass;45 continuity samples have weight1.0 and zero rejections/dropouts. Three source and three peer airborne release samples;1.4857m peak rise returns to the same floor. Native empty-target world case creates no invented peer links. Effects retire; both checkpoint restorations and protected14 pass. Broader visual acceptance, directions and shared NPC outcomes remain open. Authoritative NPC outcomes, untested stance/direction and interruption contexts, and full visual acceptance remain incomplete. Individual failed takes and their reasons are retained.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "mesmerize",
      "character": "emily",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "needs": [
        "native manifestation entity/effects",
        "targets and duration",
        "dismissal/interruption cleanup"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "mesmerize-native-emily-v70.mp4",
    "mesmerize-native-emily-v71.mp4",
    "mesmerize-owned-scenes-emily-v72.mp4",
    "mesmerize-network-emily-v73.mp4",
    "mesmerize-network-emily-v73-source.mp4",
    "mesmerize-world-emily-v75.mp4",
    "mesmerize-network-emily-v74.mp4",
    "mesmerize-network-emily-v74-source.mp4",
    "mesmerize-visual-model-v76c.mp4",
    "mesmerize-visual-model-v76d.mp4",
    "mesmerize-world-network-emily-v78.mp4",
    "mesmerize-world-network-emily-v78-source.mp4",
    "mesmerize-world-network-emily-v79.mp4",
    "mesmerize-world-network-emily-v79-source.mp4",
    "mesmerize-world-network-emily-v80.mp4",
    "mesmerize-world-network-emily-v80-source.mp4",
    "mesmerize-world-network-emily-v81.mp4",
    "mesmerize-world-network-emily-v81-source.mp4",
    "mesmerize-interrupt-source-v82.mp4",
    "mesmerize-interrupt-source-v82-source.mp4",
    "mesmerize-interrupt-receiver-v82.mp4",
    "mesmerize-interrupt-receiver-v82-source.mp4",
    "mesmerize-moving-forward-v83.mp4",
    "mesmerize-moving-forward-v83-source.mp4",
    "mesmerize-moving-eyes-v84.mp4",
    "mesmerize-moving-eyes-v84-source.mp4",
    "mesmerize-eye-network-v85.mp4",
    "mesmerize-eye-network-v85-source.mp4",
    "mesmerize-eye-network-v86.mp4",
    "mesmerize-eye-network-v86-source.mp4",
    "mesmerize-eye-moving-v87.mp4",
    "mesmerize-eye-moving-v87-source.mp4",
    "mesmerize-eye-source-pause-v88.mp4",
    "mesmerize-eye-source-pause-v88-source.mp4",
    "mesmerize-targets-receiver-pause-v89.mp4",
    "mesmerize-targets-receiver-pause-v89-source.mp4",
    "mesmerize-targets-receiver-pause-v90.mp4",
    "mesmerize-targets-receiver-pause-v90-source.mp4",
    "mesmerize-partner-exclusion-v91.mp4",
    "mesmerize-partner-exclusion-v91-source.mp4",
    "mesmerize-partner-lifetime-v93.mp4",
    "mesmerize-partner-lifetime-v93-source.mp4",
    "mesmerize-partner-current-owner-v94.mp4",
    "mesmerize-partner-current-owner-v94-source.mp4",
    "mesmerize-partner-current-owner-v95.mp4",
    "mesmerize-partner-current-owner-v95-source.mp4",
    "mesmerize-npc-positive-v99.mp4",
    "mesmerize-npc-positive-v99-source.mp4",
    "mesmerize-scoped-reader-v105.mp4",
    "mesmerize-scoped-reader-v105-source.mp4",
    "mesmerize-target-links-v107.mp4",
    "mesmerize-target-links-v107-source.mp4",
    "mesmerize-links-receiver-pause-v109.mp4",
    "mesmerize-links-receiver-pause-v109-source.mp4",
    "mesmerize-links-ai-receiver-pause-v113.mp4",
    "mesmerize-links-ai-receiver-pause-v113-source.mp4",
    "mesmerize-links-fixture-receiver-pause-v116.mp4",
    "mesmerize-links-fixture-receiver-pause-v116-source.mp4",
    "mesmerize-links-fixture-source-pause-v117.mp4",
    "mesmerize-links-fixture-source-pause-v117-source.mp4",
    "mesmerize-active-resume-receiver-v120.mp4",
    "mesmerize-active-resume-receiver-v120-source.mp4",
    "mesmerize-active-resume-source-v121.mp4",
    "mesmerize-active-resume-source-v121-source.mp4",
    "mesmerize-aim-cancel-emily-v123.mp4",
    "mesmerize-aim-cancel-emily-v123-source.mp4",
    "mesmerize-natural-expiry-emily-v125.mp4",
    "mesmerize-natural-expiry-emily-v125-source.mp4",
    "mesmerize-natural-expiry-emily-v129.mp4",
    "mesmerize-natural-expiry-emily-v129-source.mp4",
    "mesmerize-crouch-source-observe-v127.mp4",
    "mesmerize-crouch-fast-observe-v131.mp4",
    "mesmerize-crouch-network-emily-v134.mp4",
    "mesmerize-crouch-network-emily-v134-source.mp4",
    "mesmerize-crouch-cancel-emily-v136.mp4",
    "mesmerize-crouch-cancel-emily-v136-source.mp4",
    "mesmerize-crouch-network-emily-v138.mp4",
    "mesmerize-crouch-network-emily-v138-source.mp4",
    "mesmerize-crouch-moving-emily-v140.mp4",
    "mesmerize-crouch-moving-emily-v140-source.mp4",
    "mesmerize-holstered-source-v142.mp4",
    "mesmerize-holstered-crouch-source-v144.mp4",
    "mesmerize-air-source-v151.mp4",
    "mesmerize-empty-standing-emily-v156.mp4",
    "mesmerize-empty-standing-emily-v156-source.mp4",
    "mesmerize-empty-crouched-emily-v158.mp4",
    "mesmerize-empty-crouched-emily-v158-source.mp4",
    "mesmerize-crouched-moving-emily-v159.mp4",
    "mesmerize-crouched-moving-emily-v159-source.mp4",
    "mesmerize-empty-moving-emily-v160.mp4",
    "mesmerize-empty-moving-emily-v160-source.mp4",
    "mesmerize-air-release-source-v161.mp4",
    "mesmerize-air-entry-source-v163.mp4",
    "mesmerize-air-entry-network-emily-v166.mp4",
    "mesmerize-air-entry-network-emily-v166-source.mp4",
    "mesmerize-air-entry-network-emily-v168.mp4",
    "mesmerize-air-entry-network-emily-v168-source.mp4",
    "mesmerize-air-entry-network-emily-v171.mp4",
    "mesmerize-air-entry-network-emily-v171-source.mp4",
    "mesmerize-air-release-network-emily-v173.mp4",
    "mesmerize-air-release-network-emily-v173-source.mp4",
    "mesmerize-empty-air-entry-source-v175.mp4",
    "mesmerize-empty-air-release-source-v177.mp4",
    "mesmerize-empty-air-entry-network-v179.mp4",
    "mesmerize-empty-air-entry-network-v179-source.mp4",
    "mesmerize-empty-air-release-network-v181.mp4",
    "mesmerize-empty-air-release-network-v181-source.mp4"
  ],
  "native_effects_status": "not_verified",
  "native_integration": {
    "mesmerize-native-emily-v70.mp4": {
      "video": "mesmerize-native-emily-v70.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-native-emily-v70",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-native-emily-v70/result.json",
      "visual_review": "mesmerize-native-emily-v70/visual-review.json",
      "proxy_sha256": "1E25A8A6E73E5E922C5E6BAE3214F3F0E5ED31D3D8271FBF1F09179171AD7F2D",
      "actual_capture_hz": 26.625,
      "video_sha256": "B1F59E20BB68C8FBE95F29F596B095BBF8AA4804B4E0BA9AB9D9C3C7CB53CE94",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition, controller casting, scenes and effects only. No remote body, VFX, AI or gameplay outcome replication. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-native-emily-v71.mp4": {
      "video": "mesmerize-native-emily-v71.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-native-emily-v71",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-native-emily-v71/result.json",
      "visual_review": "mesmerize-native-emily-v71/visual-review.json",
      "proxy_sha256": "1E25A8A6E73E5E922C5E6BAE3214F3F0E5ED31D3D8271FBF1F09179171AD7F2D",
      "actual_capture_hz": 27.0625,
      "video_sha256": "44BC688BFD1C44894E37FC68CDB6B67D4D701434AC43E02F2D19706A46B438FD",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition, controller casting, scenes and effects only. No remote body, VFX, AI or gameplay outcome replication. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-owned-scenes-emily-v72.mp4": {
      "video": "mesmerize-owned-scenes-emily-v72.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-owned-scenes-emily-v72",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-owned-scenes-emily-v72/result.json",
      "visual_review": "mesmerize-owned-scenes-emily-v72/visual-review.json",
      "proxy_sha256": "1E25A8A6E73E5E922C5E6BAE3214F3F0E5ED31D3D8271FBF1F09179171AD7F2D",
      "actual_capture_hz": 27.5,
      "video_sha256": "5A610D828D251EC0614B03528F63E83AE886DF68F7140032969BD2CB4298E46D",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition, controller casting, scenes and effects only. No remote body, VFX, AI or gameplay outcome replication. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-network-emily-v73.mp4": {
      "video": "mesmerize-network-emily-v73.mp4",
      "character": "emily",
      "label": "Mesmerize Steam remote body · mesmerize-network-emily-v73",
      "alias": "mesmerize_native_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-network-emily-v73/result.json",
      "visual_review": "mesmerize-network-emily-v73/visual-review.json",
      "proxy_sha256": "1AF1B4D81619C1E2E51BD5C95C45D82F4EF410FB708D238C6006F867E2EBD915",
      "actual_capture_hz": 26.5625,
      "video_sha256": "6AD91F7538BC4CB80143DFA9658462E64A718980DCC83C0BAACA6FA8FBAFFF20",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition, controller casting, scenes and effects only. Standing armed Emily body phases replicate through native Steam. Remote world VFX, AI and gameplay outcomes remain pending; source-only world effects intentionally retained. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-network-emily-v73-source.mp4": {
      "video": "mesmerize-network-emily-v73-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-network-emily-v73",
      "alias": "mesmerize_native_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-network-emily-v73/result.json",
      "visual_review": "mesmerize-network-emily-v73/visual-review.json",
      "proxy_sha256": "1AF1B4D81619C1E2E51BD5C95C45D82F4EF410FB708D238C6006F867E2EBD915",
      "actual_capture_hz": 26.8125,
      "video_sha256": "EF85BCF25EE40BF3424AC831FDE98501A237313A555FF9E3ADB00A29232211DA",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition, controller casting, scenes and effects only. Standing armed Emily body phases replicate through native Steam. Remote world VFX, AI and gameplay outcomes remain pending; source-only world effects intentionally retained. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-network-emily-v74.mp4": {
      "video": "mesmerize-network-emily-v74.mp4",
      "character": "emily",
      "label": "Mesmerize Steam remote body · mesmerize-network-emily-v74",
      "alias": "mesmerize_native_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-network-emily-v74/result.json",
      "visual_review": "mesmerize-network-emily-v74/visual-review.json",
      "proxy_sha256": "1AF1B4D81619C1E2E51BD5C95C45D82F4EF410FB708D238C6006F867E2EBD915",
      "actual_capture_hz": 27.494780793319414,
      "video_sha256": "DFC643FB54E688859FFC44731DF55F519AEDBE40758A195B20B3417F3B4A19D7",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition, controller casting, scenes and effects only. Standing armed Emily body phases replicate through native Steam. Remote world VFX, AI and gameplay outcomes remain pending; source-only world effects intentionally retained. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-network-emily-v74-source.mp4": {
      "video": "mesmerize-network-emily-v74-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-network-emily-v74",
      "alias": "mesmerize_native_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-network-emily-v74/result.json",
      "visual_review": "mesmerize-network-emily-v74/visual-review.json",
      "proxy_sha256": "1AF1B4D81619C1E2E51BD5C95C45D82F4EF410FB708D238C6006F867E2EBD915",
      "actual_capture_hz": 27.301255230125523,
      "video_sha256": "BDDBE9C5010ED44C023FABFCEB84A1D6164EA297510549455520291AD9588FD0",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition, controller casting, scenes and effects only. Standing armed Emily body phases replicate through native Steam. Remote world VFX, AI and gameplay outcomes remain pending; source-only world effects intentionally retained. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-emily-v75.mp4": {
      "video": "mesmerize-world-emily-v75.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-world-emily-v75",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-emily-v75/result.json",
      "visual_review": "mesmerize-world-emily-v75/visual-review.json",
      "proxy_sha256": "1AF1B4D81619C1E2E51BD5C95C45D82F4EF410FB708D238C6006F867E2EBD915",
      "actual_capture_hz": 27.340980187695518,
      "video_sha256": "4C7B41F9368EC19DABF3C70DA400F9113ED8C4148BFD49453024B1B9992DC16A",
      "checks_passed": true,
      "scope": "Bounded read-only committed world investigation with finite20s native equipment fixture. Preview entities do not establish commitment; late despawn may be fixture-driven. Native source Mesmerize acquisition, controller casting, scenes and effects only. No remote body, VFX, AI or gameplay outcome replication. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-visual-model-v76c.mp4": {
      "video": "mesmerize-visual-model-v76c.mp4",
      "character": "emily",
      "label": "Mesmerize isolated cosmetic model v76c",
      "alias": "mesmerize_visual_model_proof",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-visual-model-v76c/result.json",
      "visual_review": "mesmerize-visual-model-v76c/visual-review.json",
      "proxy_sha256": "978D06C43985E1D8D9CE4A1A5A17E873991FF967F5252C853B84DFA33C513288",
      "actual_capture_hz": 27.25,
      "video_sha256": "F2C356868B4DA8006BD83FAC0890506F65FC7850BC83D8DA32E15FCB222FD6BE",
      "checks_passed": false,
      "scope": "Finite cosmetic Mesmerize model and native animation lifecycle. No networking, particles, sound or native gameplay outcomes in this proof. Visual acceptance is separate."
    },
    "mesmerize-visual-model-v76d.mp4": {
      "video": "mesmerize-visual-model-v76d.mp4",
      "character": "emily",
      "label": "Mesmerize isolated cosmetic model v76d",
      "alias": "mesmerize_visual_model_proof",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-visual-model-v76d/result.json",
      "visual_review": "mesmerize-visual-model-v76d/visual-review.json",
      "proxy_sha256": "37EEBEDF471F30D78096A97ACA4BB4836EB1538692602A40FAFFD307E232EDFE",
      "actual_capture_hz": 26.993736951983298,
      "video_sha256": "DDCE99D0A414905D34E55014A22C293114EC1C76EA10E882D6AC0AB698AB4702",
      "checks_passed": true,
      "scope": "Finite cosmetic Mesmerize model and native animation lifecycle. Native particles and declared spatial sound accompany the model. Recording is silent. No networking or native gameplay outcomes in this proof. Visual acceptance is separate."
    },
    "mesmerize-world-network-emily-v78.mp4": {
      "video": "mesmerize-world-network-emily-v78.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-world-network-emily-v78",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v78/result.json",
      "visual_review": "mesmerize-world-network-emily-v78/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.285714285714285,
      "video_sha256": "F804DBA70F44D34CF0C3DAD4F346DA4081FB4BF487E8AFE5111A50E0BBADE9AA",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-network-emily-v78-source.mp4": {
      "video": "mesmerize-world-network-emily-v78-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-world-network-emily-v78",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v78/result.json",
      "visual_review": "mesmerize-world-network-emily-v78/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 26.535714285714285,
      "video_sha256": "8094C53B12E4A13841D3FD4F3DB2CA47B00925ED0BA7F37B0FEDBB4112A605CA",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-network-emily-v79.mp4": {
      "video": "mesmerize-world-network-emily-v79.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-world-network-emily-v79",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v79/result.json",
      "visual_review": "mesmerize-world-network-emily-v79/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 26.889153754469607,
      "video_sha256": "DB2D1A00A38C11CB77A690EEE84532ADA1D05BD7E4ACA25C9AE2533D6B398F17",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-network-emily-v79-source.mp4": {
      "video": "mesmerize-world-network-emily-v79-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-world-network-emily-v79",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v79/result.json",
      "visual_review": "mesmerize-world-network-emily-v79/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 26.285714285714285,
      "video_sha256": "BF91B1230B6A9BCD97DA2798D8E0DE925109D64D8341A16F10C16B3D91EF2786",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-network-emily-v80.mp4": {
      "video": "mesmerize-world-network-emily-v80.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-world-network-emily-v80",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v80/result.json",
      "visual_review": "mesmerize-world-network-emily-v80/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.261969336571283,
      "video_sha256": "015A871B98ED3A932B831839828CAEF9912B1CEA000F0700C44EDEB25F5B2823",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-network-emily-v80-source.mp4": {
      "video": "mesmerize-world-network-emily-v80-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-world-network-emily-v80",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v80/result.json",
      "visual_review": "mesmerize-world-network-emily-v80/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.05588533510692,
      "video_sha256": "B5A44D03A125E051009858A571CA69191B609169CE02428A1A976353ECD22672",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-network-emily-v81.mp4": {
      "video": "mesmerize-world-network-emily-v81.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-world-network-emily-v81",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v81/result.json",
      "visual_review": "mesmerize-world-network-emily-v81/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.38235294117647,
      "video_sha256": "89E8A9A25FEF20C33B4432320F5EDFA3E55646B2F6F7699142CD8E787857F299",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-world-network-emily-v81-source.mp4": {
      "video": "mesmerize-world-network-emily-v81-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-world-network-emily-v81",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-world-network-emily-v81/result.json",
      "visual_review": "mesmerize-world-network-emily-v81/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 26.852941176470587,
      "video_sha256": "79129556364C374F93578D6A37F71A6AC389004F865840B8E2A38529C675C13E",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-interrupt-source-v82.mp4": {
      "video": "mesmerize-interrupt-source-v82.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-interrupt-source-v82",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-interrupt-source-v82/result.json",
      "visual_review": "mesmerize-interrupt-source-v82/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.41429970617042,
      "video_sha256": "31340C2803D064AAA0C12F53A2FDF47597F0CD3FCF25969BD962A4B1E6F29B0C",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-interrupt-source-v82-source.mp4": {
      "video": "mesmerize-interrupt-source-v82-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-interrupt-source-v82",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-interrupt-source-v82/result.json",
      "visual_review": "mesmerize-interrupt-source-v82/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.029411764705884,
      "video_sha256": "737A92E03D74637D083E36AE6DAC85644483FC7D45987DCAB254ED3CF2E50B1B",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-interrupt-receiver-v82.mp4": {
      "video": "mesmerize-interrupt-receiver-v82.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-interrupt-receiver-v82",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-interrupt-receiver-v82/result.json",
      "visual_review": "mesmerize-interrupt-receiver-v82/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.294117647058822,
      "video_sha256": "ACFEA8713BCE85E9DFA92520769222E41AD28188B28DB070B2CFC6B9C2341085",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-interrupt-receiver-v82-source.mp4": {
      "video": "mesmerize-interrupt-receiver-v82-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-interrupt-receiver-v82",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-interrupt-receiver-v82/result.json",
      "visual_review": "mesmerize-interrupt-receiver-v82/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.208619000979432,
      "video_sha256": "A328BB1B060072871C31F03C26ADFAB3E32F630140477C24AD912428532BD3C0",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-moving-forward-v83.mp4": {
      "video": "mesmerize-moving-forward-v83.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-moving-forward-v83",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-moving-forward-v83/result.json",
      "visual_review": "mesmerize-moving-forward-v83/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 27.264705882352942,
      "video_sha256": "9725511D91D93FE31C51ACBBD8F410BE6F67430AEA495848510F340AF591E2BB",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed walking Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-moving-forward-v83-source.mp4": {
      "video": "mesmerize-moving-forward-v83-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-moving-forward-v83",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-moving-forward-v83/result.json",
      "visual_review": "mesmerize-moving-forward-v83/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 26.91176470588235,
      "video_sha256": "6CBAB84AEEF9B6DDB89D4BEC2024E0B63B24255419AB49A97AE4FA6E7307AE93",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed walking Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-moving-eyes-v84.mp4": {
      "video": "mesmerize-moving-eyes-v84.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-moving-eyes-v84",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-moving-eyes-v84/result.json",
      "visual_review": "mesmerize-moving-eyes-v84/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 26.941176470588236,
      "video_sha256": "B49A1BDBECC3E61A309899CDB3E172C8311F8387F99FD9E6DA2F1E8EE44BCE79",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed walking Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source eye ownership is inspected read-only. Remote eye/target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-moving-eyes-v84-source.mp4": {
      "video": "mesmerize-moving-eyes-v84-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-moving-eyes-v84",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-moving-eyes-v84/result.json",
      "visual_review": "mesmerize-moving-eyes-v84/visual-review.json",
      "proxy_sha256": "7E4CF5EB00FFA7D38DD9EFFA6087C4727657F33151BBE9F1B291B7DF30B9E936",
      "actual_capture_hz": 26.474045053868757,
      "video_sha256": "3CFA54E5017345EFCF833F875177EA270C334B975E727F85052A1FA7EF4699A4",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed walking Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source eye ownership is inspected read-only. Remote eye/target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-network-v85.mp4": {
      "video": "mesmerize-eye-network-v85.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-eye-network-v85",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-network-v85/result.json",
      "visual_review": "mesmerize-eye-network-v85/visual-review.json",
      "proxy_sha256": "CCA468CEECD8C9F78A26F627063AF9D1E082AC807332628AE7F189E922EFC1DC",
      "actual_capture_hz": 27.38235294117647,
      "video_sha256": "A1A63487914BDE56A2633FA461D3F88B03C3B913F8A587F2D0D3A0CDA20F05B9",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-owned eye incarnations and native transforms drive cosmetic model-owned eye effects through Steam. Target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-network-v85-source.mp4": {
      "video": "mesmerize-eye-network-v85-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-eye-network-v85",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-network-v85/result.json",
      "visual_review": "mesmerize-eye-network-v85/visual-review.json",
      "proxy_sha256": "CCA468CEECD8C9F78A26F627063AF9D1E082AC807332628AE7F189E922EFC1DC",
      "actual_capture_hz": 26.61764705882353,
      "video_sha256": "3F81EE9D1A51140356DFC8C940CD4EBA64711974920F19550DD98837DEC6F9CB",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-owned eye incarnations and native transforms drive cosmetic model-owned eye effects through Steam. Target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-network-v86.mp4": {
      "video": "mesmerize-eye-network-v86.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-eye-network-v86",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-network-v86/result.json",
      "visual_review": "mesmerize-eye-network-v86/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 26.470588235294116,
      "video_sha256": "2C44FDE26E38E7A68A5C47DB6A9EDDB6239E7C2D3A1889D03473E6ED2D7A0784",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-owned eye incarnations and native transforms drive cosmetic model-owned eye effects through Steam. Target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-network-v86-source.mp4": {
      "video": "mesmerize-eye-network-v86-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-eye-network-v86",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-network-v86/result.json",
      "visual_review": "mesmerize-eye-network-v86/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 23.38235294117647,
      "video_sha256": "BA7C37E176F7732FF4B7D7634589CB7AB0F0138414158398C5D7C2E933E3DAD5",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-owned eye incarnations and native transforms drive cosmetic model-owned eye effects through Steam. Target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-moving-v87.mp4": {
      "video": "mesmerize-eye-moving-v87.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-eye-moving-v87",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-moving-v87/result.json",
      "visual_review": "mesmerize-eye-moving-v87/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 27.323529411764707,
      "video_sha256": "7CB53CA21CD74F646D364D08564F143E75DFB99DF50FD3554E46B49FFF1E0830",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-owned eye incarnations and native transforms drive cosmetic model-owned eye effects through Steam. Target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-moving-v87-source.mp4": {
      "video": "mesmerize-eye-moving-v87-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-eye-moving-v87",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-moving-v87/result.json",
      "visual_review": "mesmerize-eye-moving-v87/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 24.08823529411765,
      "video_sha256": "53AB635EDF1B9210488F49B15C43DCC8BB781D90466B0DB47F4ED1FE2531D5B7",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-owned eye incarnations and native transforms drive cosmetic model-owned eye effects through Steam. Target beams and AI/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-source-pause-v88.mp4": {
      "video": "mesmerize-eye-source-pause-v88.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-eye-source-pause-v88",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-source-pause-v88/result.json",
      "visual_review": "mesmerize-eye-source-pause-v88/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 27.556427870461235,
      "video_sha256": "70401BA507481092B8D9FBDD3C583DEBD3DE344FC0D0212989A12A714A52D445",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-referenced eye effects are owned by the cosmetic model and retire on interruption. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-eye-source-pause-v88-source.mp4": {
      "video": "mesmerize-eye-source-pause-v88-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-eye-source-pause-v88",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-eye-source-pause-v88/result.json",
      "visual_review": "mesmerize-eye-source-pause-v88/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 26.319921491658487,
      "video_sha256": "561D9240B1DABD4F1558A9BC8FCBE3FBCF9E9949EC91F7DC46A953F3D0505973",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-referenced eye effects are owned by the cosmetic model and retire on interruption. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-targets-receiver-pause-v89.mp4": {
      "video": "mesmerize-targets-receiver-pause-v89.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-targets-receiver-pause-v89",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-targets-receiver-pause-v89/result.json",
      "visual_review": "mesmerize-targets-receiver-pause-v89/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 26.90873405299313,
      "video_sha256": "0CEFD82498EDFB0ADB9B6EA41DB169DF8C8B5FBBD3C813216E3864B9E2A54F7A",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-referenced eye effects are owned by the cosmetic model and retire on interruption. Native target/beam records are inspected read-only; no beam playback is added. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-targets-receiver-pause-v89-source.mp4": {
      "video": "mesmerize-targets-receiver-pause-v89-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-targets-receiver-pause-v89",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-targets-receiver-pause-v89/result.json",
      "visual_review": "mesmerize-targets-receiver-pause-v89/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 26.17271835132483,
      "video_sha256": "58C19A6EF9E13092EEB14AF76E3C266EEFB28BF93E86C1D00BB556A822E41D8D",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-referenced eye effects are owned by the cosmetic model and retire on interruption. Native target/beam records are inspected read-only; no beam playback is added. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-targets-receiver-pause-v90.mp4": {
      "video": "mesmerize-targets-receiver-pause-v90.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-targets-receiver-pause-v90",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-targets-receiver-pause-v90/result.json",
      "visual_review": "mesmerize-targets-receiver-pause-v90/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 27.441176470588236,
      "video_sha256": "E826BB1F54DDCD2BD06089319E17EB33061AE6E75CF0D0819639B53D40E64295",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-referenced eye effects are owned by the cosmetic model and retire on interruption. Native target/beam records are inspected read-only; no beam playback is added. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-targets-receiver-pause-v90-source.mp4": {
      "video": "mesmerize-targets-receiver-pause-v90-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-targets-receiver-pause-v90",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-targets-receiver-pause-v90/result.json",
      "visual_review": "mesmerize-targets-receiver-pause-v90/visual-review.json",
      "proxy_sha256": "DFA7B79B0E79DB53C3AC74C36A8131E91AC203C2961901DE02ECFB8760C36CEB",
      "actual_capture_hz": 26.268364348677768,
      "video_sha256": "943DB2835951683E0CC5FE8CDAE13DBC611B124CA8D9C83C16BD9CBD62516F3F",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Source-referenced eye effects are owned by the cosmetic model and retire on interruption. Native target/beam records are inspected read-only; no beam playback is added. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-exclusion-v91.mp4": {
      "video": "mesmerize-partner-exclusion-v91.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-partner-exclusion-v91",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-exclusion-v91/result.json",
      "visual_review": "mesmerize-partner-exclusion-v91/visual-review.json",
      "proxy_sha256": "2B724D2C3D9ED5FF8E8E438DA78CD6B6B0A36DDE2D0062D68EECD39B218ED598",
      "actual_capture_hz": 26.441176470588236,
      "video_sha256": "1D62C394C44AF95FA72FDD23897616EC258AB780B46A0094B0AF11E6484B9301",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-exclusion-v91-source.mp4": {
      "video": "mesmerize-partner-exclusion-v91-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-partner-exclusion-v91",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-exclusion-v91/result.json",
      "visual_review": "mesmerize-partner-exclusion-v91/visual-review.json",
      "proxy_sha256": "2B724D2C3D9ED5FF8E8E438DA78CD6B6B0A36DDE2D0062D68EECD39B218ED598",
      "actual_capture_hz": 20.31403336604514,
      "video_sha256": "E66CE97F73719DE38386AD426614DB3E1EA7D5047D5439533479FA6AFA3C9001",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-lifetime-v93.mp4": {
      "video": "mesmerize-partner-lifetime-v93.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-partner-lifetime-v93",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-lifetime-v93/result.json",
      "visual_review": "mesmerize-partner-lifetime-v93/visual-review.json",
      "proxy_sha256": "142FD73EA96E5556B9AF492B34BBE286D2DEAD8D9D507B1E775CBA386551DBA4",
      "actual_capture_hz": 27.647058823529413,
      "video_sha256": "C69D2D043AF69AECA54B3E30E6C14D0BB1A55D35A3E7027B1177743A73226703",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-lifetime-v93-source.mp4": {
      "video": "mesmerize-partner-lifetime-v93-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-partner-lifetime-v93",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-lifetime-v93/result.json",
      "visual_review": "mesmerize-partner-lifetime-v93/visual-review.json",
      "proxy_sha256": "142FD73EA96E5556B9AF492B34BBE286D2DEAD8D9D507B1E775CBA386551DBA4",
      "actual_capture_hz": 26.0549558390579,
      "video_sha256": "5D2C13E09499C2EF8FF6AC2227CA185C10475C2346B1444A72486506F1988527",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-current-owner-v94.mp4": {
      "video": "mesmerize-partner-current-owner-v94.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-partner-current-owner-v94",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-current-owner-v94/result.json",
      "visual_review": "mesmerize-partner-current-owner-v94/visual-review.json",
      "proxy_sha256": "A80A734EF3828C621645FE7CEB0EED6ED10C16EE46012F14339519474AD7932F",
      "actual_capture_hz": 26.264705882352942,
      "video_sha256": "DD9823810D1DA3220E5E08A6DB0FB0D9B5009AD59CCCD4A2F4CDD2F22ADD5D55",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-current-owner-v94-source.mp4": {
      "video": "mesmerize-partner-current-owner-v94-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-partner-current-owner-v94",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-current-owner-v94/result.json",
      "visual_review": "mesmerize-partner-current-owner-v94/visual-review.json",
      "proxy_sha256": "A80A734EF3828C621645FE7CEB0EED6ED10C16EE46012F14339519474AD7932F",
      "actual_capture_hz": 23.558823529411764,
      "video_sha256": "E27242D8227466F609DDA9C66327EC8F380FEE6DD965A47E61B6B2380BAECAAB",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-current-owner-v95.mp4": {
      "video": "mesmerize-partner-current-owner-v95.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-partner-current-owner-v95",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-current-owner-v95/result.json",
      "visual_review": "mesmerize-partner-current-owner-v95/visual-review.json",
      "proxy_sha256": "A80A734EF3828C621645FE7CEB0EED6ED10C16EE46012F14339519474AD7932F",
      "actual_capture_hz": 26.82664054848188,
      "video_sha256": "17AFF689FEA4BAB1CCF4C7DF5D3E38AF0A1C7C416FAAB5DFE3F76264569AC12F",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-partner-current-owner-v95-source.mp4": {
      "video": "mesmerize-partner-current-owner-v95-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-partner-current-owner-v95",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-partner-current-owner-v95/result.json",
      "visual_review": "mesmerize-partner-current-owner-v95/visual-review.json",
      "proxy_sha256": "A80A734EF3828C621645FE7CEB0EED6ED10C16EE46012F14339519474AD7932F",
      "actual_capture_hz": 23.840864440078587,
      "video_sha256": "BFF487F1DEB06D0FBFADF71441DA341C40830DE003E73BBDEDDB29545143E4D7",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-npc-positive-v99.mp4": {
      "video": "mesmerize-npc-positive-v99.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-npc-positive-v99",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-npc-positive-v99/result.json",
      "visual_review": "mesmerize-npc-positive-v99/visual-review.json",
      "proxy_sha256": "A80A734EF3828C621645FE7CEB0EED6ED10C16EE46012F14339519474AD7932F",
      "actual_capture_hz": 26.705882352941178,
      "video_sha256": "59098AA6EB755AB7133235F3F2A83718179723F60AB3777D560BB654F37AF829",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-npc-positive-v99-source.mp4": {
      "video": "mesmerize-npc-positive-v99-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-npc-positive-v99",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-npc-positive-v99/result.json",
      "visual_review": "mesmerize-npc-positive-v99/visual-review.json",
      "proxy_sha256": "A80A734EF3828C621645FE7CEB0EED6ED10C16EE46012F14339519474AD7932F",
      "actual_capture_hz": 23.27130264446621,
      "video_sha256": "16A70AEA77244D21122B2D6224015132FDED0B1A2FBA6F9E2E3BF363E33D8080",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-scoped-reader-v105.mp4": {
      "video": "mesmerize-scoped-reader-v105.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-scoped-reader-v105",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-scoped-reader-v105/result.json",
      "visual_review": "mesmerize-scoped-reader-v105/visual-review.json",
      "proxy_sha256": "F82EE3EBF650B7E7115376300C488480DF44AEAD375E5947FA0D0DB6FB49985F",
      "actual_capture_hz": 26.735294117647058,
      "video_sha256": "69A216E5FD657B513881B46AAFBA8DCFAF03E444E77CE416F218DBF0D20BEF74",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-scoped-reader-v105-source.mp4": {
      "video": "mesmerize-scoped-reader-v105-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-scoped-reader-v105",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-scoped-reader-v105/result.json",
      "visual_review": "mesmerize-scoped-reader-v105/visual-review.json",
      "proxy_sha256": "F82EE3EBF650B7E7115376300C488480DF44AEAD375E5947FA0D0DB6FB49985F",
      "actual_capture_hz": 26.61764705882353,
      "video_sha256": "67A89F7395FF265151299A43920824DF8BD32B74CC942C5368399BB83FC4E72E",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. AI distraction/target beams/gameplay outcome consistency remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-target-links-v107.mp4": {
      "video": "mesmerize-target-links-v107.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-target-links-v107",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-target-links-v107/result.json",
      "visual_review": "mesmerize-target-links-v107/visual-review.json",
      "proxy_sha256": "C83A86EFDA36C49F5D13977320DB7EC587A95EC937A28CFE4F864B10503B1CA3",
      "actual_capture_hz": 26.558823529411764,
      "video_sha256": "2B423A627DC8EA10F1CCCB465C76AE7526BE7A408BD1D3628B73BF4DBEEF9F2C",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-target-links-v107-source.mp4": {
      "video": "mesmerize-target-links-v107-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-target-links-v107",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-target-links-v107/result.json",
      "visual_review": "mesmerize-target-links-v107/visual-review.json",
      "proxy_sha256": "C83A86EFDA36C49F5D13977320DB7EC587A95EC937A28CFE4F864B10503B1CA3",
      "actual_capture_hz": 26.441176470588236,
      "video_sha256": "A87A5C105F79E12859D1F5A546427F784824C73AE30765670A2DD41D830E90C1",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-receiver-pause-v109.mp4": {
      "video": "mesmerize-links-receiver-pause-v109.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-links-receiver-pause-v109",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-receiver-pause-v109/result.json",
      "visual_review": "mesmerize-links-receiver-pause-v109/visual-review.json",
      "proxy_sha256": "C83A86EFDA36C49F5D13977320DB7EC587A95EC937A28CFE4F864B10503B1CA3",
      "actual_capture_hz": 27.11764705882353,
      "video_sha256": "74FF9C729A1762B518D61E4517393120B7DA9076792F5EB2DC26C19627363B43",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-receiver-pause-v109-source.mp4": {
      "video": "mesmerize-links-receiver-pause-v109-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-links-receiver-pause-v109",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-receiver-pause-v109/result.json",
      "visual_review": "mesmerize-links-receiver-pause-v109/visual-review.json",
      "proxy_sha256": "C83A86EFDA36C49F5D13977320DB7EC587A95EC937A28CFE4F864B10503B1CA3",
      "actual_capture_hz": 26.931297709923665,
      "video_sha256": "6743F4F136B06FDAAF389CFD5FEDFC05B63917DADD4C1D2D64A7CC8887FF354B",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-ai-receiver-pause-v113.mp4": {
      "video": "mesmerize-links-ai-receiver-pause-v113.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-links-ai-receiver-pause-v113",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-ai-receiver-pause-v113/result.json",
      "visual_review": "mesmerize-links-ai-receiver-pause-v113/visual-review.json",
      "proxy_sha256": "2B6ADDE97BC76B1F3D9AB4116B1C2815337E486A513552B26ED01671AFA7BBA2",
      "actual_capture_hz": 27.674190382728163,
      "video_sha256": "35A8D9AC4EE71CD8AD45F9D027288CD59D24EA826C50CE9977990427BDC7D6F6",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-ai-receiver-pause-v113-source.mp4": {
      "video": "mesmerize-links-ai-receiver-pause-v113-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-links-ai-receiver-pause-v113",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-ai-receiver-pause-v113/result.json",
      "visual_review": "mesmerize-links-ai-receiver-pause-v113/visual-review.json",
      "proxy_sha256": "2B6ADDE97BC76B1F3D9AB4116B1C2815337E486A513552B26ED01671AFA7BBA2",
      "actual_capture_hz": 27.526987242394505,
      "video_sha256": "F39F1CB97793C9FFB3006FF194D63940855C930DF623E71C28493B09F1171691",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-fixture-receiver-pause-v116.mp4": {
      "video": "mesmerize-links-fixture-receiver-pause-v116.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-links-fixture-receiver-pause-v116",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-fixture-receiver-pause-v116/result.json",
      "visual_review": "mesmerize-links-fixture-receiver-pause-v116/visual-review.json",
      "proxy_sha256": "845A307AF1090838D02B7F56D35B3A03F296F231C77C8A2E7D068684C90D14BB",
      "actual_capture_hz": 27.176470588235293,
      "video_sha256": "4864FDB4AF87E6D1A188C313E16CEE3EFD157AE1AB3C146D530A306114B7A677",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-fixture-receiver-pause-v116-source.mp4": {
      "video": "mesmerize-links-fixture-receiver-pause-v116-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-links-fixture-receiver-pause-v116",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-fixture-receiver-pause-v116/result.json",
      "visual_review": "mesmerize-links-fixture-receiver-pause-v116/visual-review.json",
      "proxy_sha256": "845A307AF1090838D02B7F56D35B3A03F296F231C77C8A2E7D068684C90D14BB",
      "actual_capture_hz": 27.352941176470587,
      "video_sha256": "48131DDAF83DF5D4B384D33E443F69962F0A7CF839492C5BD7F7427BAEDC5832",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-fixture-source-pause-v117.mp4": {
      "video": "mesmerize-links-fixture-source-pause-v117.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-links-fixture-source-pause-v117",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-fixture-source-pause-v117/result.json",
      "visual_review": "mesmerize-links-fixture-source-pause-v117/visual-review.json",
      "proxy_sha256": "845A307AF1090838D02B7F56D35B3A03F296F231C77C8A2E7D068684C90D14BB",
      "actual_capture_hz": 27.647058823529413,
      "video_sha256": "2A8EA58E17FDBD8676643EA242A3CA43D4B442016383769AA7B9955DD41736E0",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-links-fixture-source-pause-v117-source.mp4": {
      "video": "mesmerize-links-fixture-source-pause-v117-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-links-fixture-source-pause-v117",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-links-fixture-source-pause-v117/result.json",
      "visual_review": "mesmerize-links-fixture-source-pause-v117/visual-review.json",
      "proxy_sha256": "845A307AF1090838D02B7F56D35B3A03F296F231C77C8A2E7D068684C90D14BB",
      "actual_capture_hz": 27.002938295788443,
      "video_sha256": "19CD9EB9978F5AE382DE4B9A10255AAC973B56105777FBE96E285E3AFC5BB02A",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-active-resume-receiver-v120.mp4": {
      "video": "mesmerize-active-resume-receiver-v120.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-active-resume-receiver-v120",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-active-resume-receiver-v120/result.json",
      "visual_review": "mesmerize-active-resume-receiver-v120/visual-review.json",
      "proxy_sha256": "EA18C121334B5A90FE125EC73137071F3471A0840B8C0A6200634870B1E07FA4",
      "actual_capture_hz": 26.5,
      "video_sha256": "4EA44679CBE822C950AE4B5E7BA3D93003BA1B00459AEE955006BF7069AEF2C6",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Same-world native menu recovery uses new Steam stream tokens, the same pinned banks/avatars/native committed world and one reused cosmetic model. A current state3 snapshot enters the loop without replaying the cast or spawn animation. No second cast input is issued. Aiming cancellation, natural lifetime and shared NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-active-resume-receiver-v120-source.mp4": {
      "video": "mesmerize-active-resume-receiver-v120-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-active-resume-receiver-v120",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-active-resume-receiver-v120/result.json",
      "visual_review": "mesmerize-active-resume-receiver-v120/visual-review.json",
      "proxy_sha256": "EA18C121334B5A90FE125EC73137071F3471A0840B8C0A6200634870B1E07FA4",
      "actual_capture_hz": 25.58823529411765,
      "video_sha256": "F25957E6A90DC3B4401403C08490964FFE3A574D4609116D6F364FC6A6260126",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Same-world native menu recovery uses new Steam stream tokens, the same pinned banks/avatars/native committed world and one reused cosmetic model. A current state3 snapshot enters the loop without replaying the cast or spawn animation. No second cast input is issued. Aiming cancellation, natural lifetime and shared NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-active-resume-source-v121.mp4": {
      "video": "mesmerize-active-resume-source-v121.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-active-resume-source-v121",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-active-resume-source-v121/result.json",
      "visual_review": "mesmerize-active-resume-source-v121/visual-review.json",
      "proxy_sha256": "EA18C121334B5A90FE125EC73137071F3471A0840B8C0A6200634870B1E07FA4",
      "actual_capture_hz": 26.58823529411765,
      "video_sha256": "3167C7260F431389CFF3ABD65156A0E95898167B0C4DFB165D711B3518093A6A",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Same-world native menu recovery uses new Steam stream tokens, the same pinned banks/avatars/native committed world and one reused cosmetic model. A current state3 snapshot enters the loop without replaying the cast or spawn animation. No second cast input is issued. Aiming cancellation, natural lifetime and shared NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-active-resume-source-v121-source.mp4": {
      "video": "mesmerize-active-resume-source-v121-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-active-resume-source-v121",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-active-resume-source-v121/result.json",
      "visual_review": "mesmerize-active-resume-source-v121/visual-review.json",
      "proxy_sha256": "EA18C121334B5A90FE125EC73137071F3471A0840B8C0A6200634870B1E07FA4",
      "actual_capture_hz": 25.029411764705884,
      "video_sha256": "7A3B997987345972AA9053DCFF7AFBC034C150B250D10FB298AD6DAEBB1F6AB9",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Same-world native menu recovery uses new Steam stream tokens, the same pinned banks/avatars/native committed world and one reused cosmetic model. A current state3 snapshot enters the loop without replaying the cast or spawn animation. No second cast input is issued. Aiming cancellation, natural lifetime and shared NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-aim-cancel-emily-v123.mp4": {
      "video": "mesmerize-aim-cancel-emily-v123.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-aim-cancel-emily-v123",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-aim-cancel-emily-v123/result.json",
      "visual_review": "mesmerize-aim-cancel-emily-v123/visual-review.json",
      "proxy_sha256": "EA18C121334B5A90FE125EC73137071F3471A0840B8C0A6200634870B1E07FA4",
      "actual_capture_hz": 26.583333333333332,
      "video_sha256": "AC447C8DEC61020CCC93F4EFAF2A92139F221AC0D77233DC30F1186B7E602EBB",
      "checks_passed": true,
      "scope": "Standing armed Emily Mesmerize native aim cancellation via controller X while LT remains held. Confirms native first-person enter/aim provenance, remote body recovery and absence of a committed native/cosmetic power or mana cost. Visual acceptance, other stances, natural lifetime and shared NPC outcomes remain separate. Both isolated checkpoints restore and protected originals remain unchanged."
    },
    "mesmerize-aim-cancel-emily-v123-source.mp4": {
      "video": "mesmerize-aim-cancel-emily-v123-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-aim-cancel-emily-v123",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-aim-cancel-emily-v123/result.json",
      "visual_review": "mesmerize-aim-cancel-emily-v123/visual-review.json",
      "proxy_sha256": "EA18C121334B5A90FE125EC73137071F3471A0840B8C0A6200634870B1E07FA4",
      "actual_capture_hz": 26.5,
      "video_sha256": "1A230669546BB765811585E8B5F22F56A2C19840472C13926D3B9D50338A0991",
      "checks_passed": true,
      "scope": "Standing armed Emily Mesmerize native aim cancellation via controller X while LT remains held. Confirms native first-person enter/aim provenance, remote body recovery and absence of a committed native/cosmetic power or mana cost. Visual acceptance, other stances, natural lifetime and shared NPC outcomes remain separate. Both isolated checkpoints restore and protected originals remain unchanged."
    },
    "mesmerize-natural-expiry-emily-v125.mp4": {
      "video": "mesmerize-natural-expiry-emily-v125.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-natural-expiry-emily-v125",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-natural-expiry-emily-v125/result.json",
      "visual_review": "mesmerize-natural-expiry-emily-v125/visual-review.json",
      "proxy_sha256": "C179CFD8258A13000484A051D0EAFB4F6D1B8C6FDACD1F02E5CBAE3BCA6284E2",
      "actual_capture_hz": 26.92105263157895,
      "video_sha256": "B9FF9B45C68F17D782D3BE6699C2092D53A4A83EC9A5559ED2078F50A732DE58",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native countdown expiry is observed while a bounded 40-second equipment fixture remains active and the power stays selected. Source alone owns gameplay; the remote model and native particles retire from source lifecycle. Aim cancellation and menu recovery have separate recordings. Other stances, directions and authoritative NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-natural-expiry-emily-v125-source.mp4": {
      "video": "mesmerize-natural-expiry-emily-v125-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-natural-expiry-emily-v125",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-natural-expiry-emily-v125/result.json",
      "visual_review": "mesmerize-natural-expiry-emily-v125/visual-review.json",
      "proxy_sha256": "C179CFD8258A13000484A051D0EAFB4F6D1B8C6FDACD1F02E5CBAE3BCA6284E2",
      "actual_capture_hz": 26.07894736842105,
      "video_sha256": "8C6A7A104982FED983E3E920595CF8A3EB3696FF6BBFF4A8B2F6B271A8006D3A",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native countdown expiry is observed while a bounded 40-second equipment fixture remains active and the power stays selected. Source alone owns gameplay; the remote model and native particles retire from source lifecycle. Aim cancellation and menu recovery have separate recordings. Other stances, directions and authoritative NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-natural-expiry-emily-v129.mp4": {
      "video": "mesmerize-natural-expiry-emily-v129.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-natural-expiry-emily-v129",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-natural-expiry-emily-v129/result.json",
      "visual_review": "mesmerize-natural-expiry-emily-v129/visual-review.json",
      "proxy_sha256": "C179CFD8258A13000484A051D0EAFB4F6D1B8C6FDACD1F02E5CBAE3BCA6284E2",
      "actual_capture_hz": 27.023705004389814,
      "video_sha256": "91A67FF8BA1DC5391E6A279150A6345973311218C0EDC50702E77B124A797AC9",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native countdown expiry is observed while a bounded 40-second equipment fixture remains active and the power stays selected. Source alone owns gameplay; the remote model and native particles retire from source lifecycle. Aim cancellation and menu recovery have separate recordings. Other stances, directions and authoritative NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-natural-expiry-emily-v129-source.mp4": {
      "video": "mesmerize-natural-expiry-emily-v129-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-natural-expiry-emily-v129",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-natural-expiry-emily-v129/result.json",
      "visual_review": "mesmerize-natural-expiry-emily-v129/visual-review.json",
      "proxy_sha256": "C179CFD8258A13000484A051D0EAFB4F6D1B8C6FDACD1F02E5CBAE3BCA6284E2",
      "actual_capture_hz": 26.733977172958735,
      "video_sha256": "35655551F21FCDAD8CDF5A10981519E717C032516D1299B7AF3146FD0C8CF642",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and controller casting drive standing armed Emily body, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native countdown expiry is observed while a bounded 40-second equipment fixture remains active and the power stays selected. Source alone owns gameplay; the remote model and native particles retire from source lifecycle. Aim cancellation and menu recovery have separate recordings. Other stances, directions and authoritative NPC outcomes remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-source-observe-v127.mp4": {
      "video": "mesmerize-crouch-source-observe-v127.mp4",
      "character": "emily",
      "label": "Mesmerize native crouch observation · mesmerize-crouch-source-observe-v127",
      "alias": "mesmerize_native_crouch_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-source-observe-v127/result.json",
      "visual_review": "mesmerize-crouch-source-observe-v127/visual-review.json",
      "proxy_sha256": "C179CFD8258A13000484A051D0EAFB4F6D1B8C6FDACD1F02E5CBAE3BCA6284E2",
      "actual_capture_hz": 27.0,
      "video_sha256": "4A1AF72C2C4E81448D87F0ACAD16042500B02631524D80FF72422CC01C778EC6",
      "checks_passed": true,
      "scope": "Native crouched source Mesmerize acquisition and controller cast observation. Existing strict power reader deliberately remains standing-only; raw Midnight/player state evidence must be reviewed before enabling crouch transport. No remote body, VFX, AI or gameplay outcome replication. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-fast-observe-v131.mp4": {
      "video": "mesmerize-crouch-fast-observe-v131.mp4",
      "character": "emily",
      "label": "Mesmerize native crouch observation · mesmerize-crouch-fast-observe-v131",
      "alias": "mesmerize_native_crouch_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-fast-observe-v131/result.json",
      "visual_review": "mesmerize-crouch-fast-observe-v131/visual-review.json",
      "proxy_sha256": "C179CFD8258A13000484A051D0EAFB4F6D1B8C6FDACD1F02E5CBAE3BCA6284E2",
      "actual_capture_hz": 27.375,
      "video_sha256": "46533D44594C270C26B2BC09B11C489D77840D98B5C1609B024B3698BDDF7ADF",
      "checks_passed": true,
      "scope": "Native crouched source Mesmerize acquisition and controller cast observation. Existing strict power reader deliberately remains standing-only; raw Midnight/player state evidence must be reviewed before enabling crouch transport. No remote body, VFX, AI or gameplay outcome replication. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-network-emily-v134.mp4": {
      "video": "mesmerize-crouch-network-emily-v134.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-crouch-network-emily-v134",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-network-emily-v134/result.json",
      "visual_review": "mesmerize-crouch-network-emily-v134/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 26.294117647058822,
      "video_sha256": "379F0FF97DBC92B7DC2C5EA1ACE170E8214C9CAAC5DA1FC34274617412723364",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and crouched armed controller casting drive the complete-body Emily stance candidate, followed by native standing recovery, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-network-emily-v134-source.mp4": {
      "video": "mesmerize-crouch-network-emily-v134-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-crouch-network-emily-v134",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-network-emily-v134/result.json",
      "visual_review": "mesmerize-crouch-network-emily-v134/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 26.205882352941178,
      "video_sha256": "0A254BDDBC6E25ED7E3667EA8859C2F5BFFF27F9FD4EFB77233C30A8564892BB",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and crouched armed controller casting drive the complete-body Emily stance candidate, followed by native standing recovery, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-cancel-emily-v136.mp4": {
      "video": "mesmerize-crouch-cancel-emily-v136.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-crouch-cancel-emily-v136",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-cancel-emily-v136/result.json",
      "visual_review": "mesmerize-crouch-cancel-emily-v136/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 25.916666666666668,
      "video_sha256": "8531E4AD994316C9C47E2CFE6C8C4094188523729E6ED97FA475EE9070FE9BC6",
      "checks_passed": true,
      "scope": "Crouched armed Emily Mesmerize native aim cancellation via controller X while LT remains held. Confirms native first-person enter/aim provenance, remote body recovery and absence of a committed native/cosmetic power or mana cost. Visual acceptance and shared NPC outcomes remain incomplete; standing cancellation and natural lifetime have separate receipts. Both isolated checkpoints restore and protected originals remain unchanged."
    },
    "mesmerize-crouch-cancel-emily-v136-source.mp4": {
      "video": "mesmerize-crouch-cancel-emily-v136-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-crouch-cancel-emily-v136",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-cancel-emily-v136/result.json",
      "visual_review": "mesmerize-crouch-cancel-emily-v136/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 26.583333333333332,
      "video_sha256": "D8F4692968B733033A08B62E6BF9E7B0093037F10B5E651104BFC33EC82815A5",
      "checks_passed": true,
      "scope": "Crouched armed Emily Mesmerize native aim cancellation via controller X while LT remains held. Confirms native first-person enter/aim provenance, remote body recovery and absence of a committed native/cosmetic power or mana cost. Visual acceptance and shared NPC outcomes remain incomplete; standing cancellation and natural lifetime have separate receipts. Both isolated checkpoints restore and protected originals remain unchanged."
    },
    "mesmerize-crouch-network-emily-v138.mp4": {
      "video": "mesmerize-crouch-network-emily-v138.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-crouch-network-emily-v138",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-network-emily-v138/result.json",
      "visual_review": "mesmerize-crouch-network-emily-v138/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 27.147058823529413,
      "video_sha256": "8E3642F76E46D4CFA38BC52FF36DF1B8AA5E9E372735B422FD2BE7154B6CA546",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and crouched armed controller casting drive the complete-body Emily stance candidate, followed by native standing recovery, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-network-emily-v138-source.mp4": {
      "video": "mesmerize-crouch-network-emily-v138-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-crouch-network-emily-v138",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-network-emily-v138/result.json",
      "visual_review": "mesmerize-crouch-network-emily-v138/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 26.970588235294116,
      "video_sha256": "A550E1EC7A10BF4237DD744AFC39252EB0500EA8E3AB132B1FF2086B6F91C05C",
      "checks_passed": true,
      "scope": "Native source Mesmerize acquisition and crouched armed controller casting drive the complete-body Emily stance candidate, followed by native standing recovery, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-moving-emily-v140.mp4": {
      "video": "mesmerize-crouch-moving-emily-v140.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-crouch-moving-emily-v140",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-moving-emily-v140/result.json",
      "visual_review": "mesmerize-crouch-moving-emily-v140/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 26.970588235294116,
      "video_sha256": "4858CC233B6D3EE74436B07F53F3352A238BA9089AFBD94681E974FDE8CB438F",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and forward-moving crouched armed controller casting drive the complete-body Emily stance candidate, followed by native standing recovery, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. This take uses20-second equipment expiry; native lifetime, standing cancellation and crouched cancellation have separate receipts. Other directions and equipment contexts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-crouch-moving-emily-v140-source.mp4": {
      "video": "mesmerize-crouch-moving-emily-v140-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-crouch-moving-emily-v140",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouch-moving-emily-v140/result.json",
      "visual_review": "mesmerize-crouch-moving-emily-v140/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 26.941176470588236,
      "video_sha256": "495905F354A23ED858A9E789711828B1CBC392C98FF31C94168DACC403CDD4E0",
      "checks_passed": false,
      "scope": "Native source Mesmerize acquisition and forward-moving crouched armed controller casting drive the complete-body Emily stance candidate, followed by native standing recovery, source-committed world transform, three animation-only model scenes and exact native body particles on the observer through Steam. Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending. FX sound declarations retained but recordings silent. This take uses20-second equipment expiry; native lifetime, standing cancellation and crouched cancellation have separate receipts. Other directions and equipment contexts remain unverified. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged."
    },
    "mesmerize-holstered-source-v142.mp4": {
      "video": "mesmerize-holstered-source-v142.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-holstered-source-v142",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-holstered-source-v142/result.json",
      "visual_review": "mesmerize-holstered-source-v142/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 27.25,
      "video_sha256": "2BC3BE681894B0892D416265F8F7E1AC7DA2880ECB85D348D7E9D5D5300CBF2C",
      "checks_passed": true,
      "scope": "Source-only native holstered Mesmerize observation. Native X holsters, then one LT hold/release tests the actual draw/use behavior in the requested stance. Raw scene/equipment/mana evidence determines whether a cast occurs and which equipment context it uses. No remote integration or complete movement coverage claimed. Isolated native checkpoints and protected hashes restore."
    },
    "mesmerize-holstered-crouch-source-v144.mp4": {
      "video": "mesmerize-holstered-crouch-source-v144.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-holstered-crouch-source-v144",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-holstered-crouch-source-v144/result.json",
      "visual_review": "mesmerize-holstered-crouch-source-v144/visual-review.json",
      "proxy_sha256": "C00A8689AE6E16D9855882256ADF191AA4A79C508CC5B9D5C9A84B98BC4A45EA",
      "actual_capture_hz": 27.4375,
      "video_sha256": "B13FB6FF35EE1AA727CF424F12D0418A47130D36DD7E76C5764CCB9A34AEBB08",
      "checks_passed": true,
      "scope": "Source-only native holstered Mesmerize observation. Native X holsters, then one LT hold/release tests the actual draw/use behavior in the requested stance. Raw scene/equipment/mana evidence determines whether a cast occurs and which equipment context it uses. No remote integration or complete movement coverage claimed. Isolated native checkpoints and protected hashes restore."
    },
    "mesmerize-air-source-v151.mp4": {
      "video": "mesmerize-air-source-v151.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-air-source-v151",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-source-v151/result.json",
      "visual_review": "mesmerize-air-source-v151/visual-review.json",
      "proxy_sha256": "75EC7A32FCBE91D165D8EF2D842135FCDCBDC1A6225585B09E62B2B806DBBFB7",
      "actual_capture_hz": 27.0625,
      "video_sha256": "25DC55FD2FA180F746EB5D4218E003F387020F71A85C114AE768EB2B64946E27",
      "checks_passed": true,
      "scope": "Native source in-place jump while holding Mesmerize aim, releasing LT600ms after A. Raw player states and single-arm Midnight bindings determine actual airborne behavior. Current strict reader still refuses airborne Mesmerize, and no remote body/FX integration is enabled by this observation. Native mana/world behavior and final native floor are recorded. Both isolated checkpoints and protected save hashes restore."
    },
    "mesmerize-empty-standing-emily-v156.mp4": {
      "video": "mesmerize-empty-standing-emily-v156.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-empty-standing-emily-v156",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-standing-emily-v156/result.json",
      "visual_review": "mesmerize-empty-standing-emily-v156/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 25.352941176470587,
      "video_sha256": "CAC12110EF73DD31B4CC45571A4DFD915B815039A23051C8B3FA82634D3DB431",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-standing-emily-v156-source.mp4": {
      "video": "mesmerize-empty-standing-emily-v156-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-empty-standing-emily-v156",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-standing-emily-v156/result.json",
      "visual_review": "mesmerize-empty-standing-emily-v156/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 26.529411764705884,
      "video_sha256": "547C487A2CD1CBD4F42C497042929183E3926F0C23B20B68468B64886F7EA84A",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-crouched-emily-v158.mp4": {
      "video": "mesmerize-empty-crouched-emily-v158.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-empty-crouched-emily-v158",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-crouched-emily-v158/result.json",
      "visual_review": "mesmerize-empty-crouched-emily-v158/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 26.941176470588236,
      "video_sha256": "D37DBB38330D3C3010524EC37B7A4715BE75CD5A17FA2C74602C2C315A5DBE14",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-crouched-emily-v158-source.mp4": {
      "video": "mesmerize-empty-crouched-emily-v158-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-empty-crouched-emily-v158",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-crouched-emily-v158/result.json",
      "visual_review": "mesmerize-empty-crouched-emily-v158/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 27.08823529411765,
      "video_sha256": "535BDE2E6F26015286D33AD618C96CCBF1072DCBD69448DC5483722FDC569DF5",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-crouched-moving-emily-v159.mp4": {
      "video": "mesmerize-crouched-moving-emily-v159.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-crouched-moving-emily-v159",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouched-moving-emily-v159/result.json",
      "visual_review": "mesmerize-crouched-moving-emily-v159/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 26.676470588235293,
      "video_sha256": "E2734852E97828EB40FEF2F79C7E9A7FF3F06D4A1089E93AC33EC569774C7CE3",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-crouched-moving-emily-v159-source.mp4": {
      "video": "mesmerize-crouched-moving-emily-v159-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-crouched-moving-emily-v159",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-crouched-moving-emily-v159/result.json",
      "visual_review": "mesmerize-crouched-moving-emily-v159/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 26.058823529411764,
      "video_sha256": "C6158AD7BD2E6F83CFE4E0ABF7ECFF5AD66B9A89B15E76176B90456AA4119D4B",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-moving-emily-v160.mp4": {
      "video": "mesmerize-empty-moving-emily-v160.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-empty-moving-emily-v160",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-moving-emily-v160/result.json",
      "visual_review": "mesmerize-empty-moving-emily-v160/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 27.17369970559372,
      "video_sha256": "0EE8AE76F63266CF1DCC674DD0C2E735876640F0068FAA6B2707CC1AC1AF93D8",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-moving-emily-v160-source.mp4": {
      "video": "mesmerize-empty-moving-emily-v160-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-empty-moving-emily-v160",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-moving-emily-v160/result.json",
      "visual_review": "mesmerize-empty-moving-emily-v160/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 27.53183153770813,
      "video_sha256": "FEE41E4764C18F1CA4B32E7B860E1974FD657550A4511906DFBDA35A00F92843",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-release-source-v161.mp4": {
      "video": "mesmerize-air-release-source-v161.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-air-release-source-v161",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-release-source-v161/result.json",
      "visual_review": "mesmerize-air-release-source-v161/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 27.620041753653446,
      "video_sha256": "B048F463ED4217DD101C0EB6B09EBFF11C1B2E276B36A4E2E9D95E1EC15CD1A8",
      "checks_passed": true,
      "scope": "Native source in-place jump while holding Mesmerize aim, releasing LT180ms after A. Raw player states and single-arm Midnight bindings determine actual airborne behavior. Current strict reader still refuses airborne Mesmerize, and no remote body/FX integration is enabled by this observation. Native mana/world behavior and final native floor are recorded. Both isolated checkpoints and protected save hashes restore."
    },
    "mesmerize-air-entry-source-v163.mp4": {
      "video": "mesmerize-air-entry-source-v163.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-air-entry-source-v163",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-entry-source-v163/result.json",
      "visual_review": "mesmerize-air-entry-source-v163/visual-review.json",
      "proxy_sha256": "4F44E2A78577641C04F23073E68EB00D18114983557A68A9AA8CE7B970B8C52D",
      "actual_capture_hz": 27.244258872651358,
      "video_sha256": "A4D154F90B30282234B13E38AEFEE7DFE2895EE9BCFF6DCB503ECB10DC39D957",
      "checks_passed": true,
      "scope": "Native source in-place jump and Mesmerize aim begin together with1800ms hold; directly inspect airborne entry and later grounded release. Raw player states and single-arm Midnight bindings determine actual airborne behavior. Current strict reader still refuses airborne Mesmerize, and no remote body/FX integration is enabled by this observation. Native mana/world behavior and final native floor are recorded. Both isolated checkpoints and protected save hashes restore."
    },
    "mesmerize-air-entry-network-emily-v166.mp4": {
      "video": "mesmerize-air-entry-network-emily-v166.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-air-entry-network-emily-v166",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-entry-network-emily-v166/result.json",
      "visual_review": "mesmerize-air-entry-network-emily-v166/visual-review.json",
      "proxy_sha256": "9450A558099B8227A002FBB7F9A484678B86310118667E834C9528C5FB684C5B",
      "actual_capture_hz": 26.970588235294116,
      "video_sha256": "BA29946487072CA5A598DB2207FE390C78EA546DF2B3C33413295449FAD8C155",
      "checks_passed": false,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-entry-network-emily-v166-source.mp4": {
      "video": "mesmerize-air-entry-network-emily-v166-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-air-entry-network-emily-v166",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-entry-network-emily-v166/result.json",
      "visual_review": "mesmerize-air-entry-network-emily-v166/visual-review.json",
      "proxy_sha256": "9450A558099B8227A002FBB7F9A484678B86310118667E834C9528C5FB684C5B",
      "actual_capture_hz": 26.294117647058822,
      "video_sha256": "7F4A1FBD3D558CF631B0EA287A01C7EBA36589F762B0CBA3CC6851E180FD31B7",
      "checks_passed": false,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-entry-network-emily-v168.mp4": {
      "video": "mesmerize-air-entry-network-emily-v168.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-air-entry-network-emily-v168",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-entry-network-emily-v168/result.json",
      "visual_review": "mesmerize-air-entry-network-emily-v168/visual-review.json",
      "proxy_sha256": "FF25414CF01E0D9CAF8A973AFDB9569EEE4BAC269FF12ABA4E960753714FD28B",
      "actual_capture_hz": 26.88235294117647,
      "video_sha256": "88A08205E74DB72F5D3352E1F921E525B61BBF391B9D40942D32BF463694ECF7",
      "checks_passed": true,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-entry-network-emily-v168-source.mp4": {
      "video": "mesmerize-air-entry-network-emily-v168-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-air-entry-network-emily-v168",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-entry-network-emily-v168/result.json",
      "visual_review": "mesmerize-air-entry-network-emily-v168/visual-review.json",
      "proxy_sha256": "FF25414CF01E0D9CAF8A973AFDB9569EEE4BAC269FF12ABA4E960753714FD28B",
      "actual_capture_hz": 26.470588235294116,
      "video_sha256": "4743C9D4BB107C2CA54ECCED0FE4F210CFD2AB64B662B074D6154EB82DC0D0B7",
      "checks_passed": true,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-entry-network-emily-v171.mp4": {
      "video": "mesmerize-air-entry-network-emily-v171.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-air-entry-network-emily-v171",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-entry-network-emily-v171/result.json",
      "visual_review": "mesmerize-air-entry-network-emily-v171/visual-review.json",
      "proxy_sha256": "DDD62F5EB3D7CED26AC45D859C48C72C8CC9A0754A31DF1EEC7C1025FC605805",
      "actual_capture_hz": 26.764705882352942,
      "video_sha256": "338ABF195EB150E510CB9F5A8BF9D9B6B2BE0B50722C7CCED4AE73ABF2928DA6",
      "checks_passed": true,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-entry-network-emily-v171-source.mp4": {
      "video": "mesmerize-air-entry-network-emily-v171-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-air-entry-network-emily-v171",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-entry-network-emily-v171/result.json",
      "visual_review": "mesmerize-air-entry-network-emily-v171/visual-review.json",
      "proxy_sha256": "DDD62F5EB3D7CED26AC45D859C48C72C8CC9A0754A31DF1EEC7C1025FC605805",
      "actual_capture_hz": 26.08823529411765,
      "video_sha256": "03FC43C3507E02CDC897F3F273B9B420E34453B62239E735EA25EF4B2661A5E1",
      "checks_passed": true,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-release-network-emily-v173.mp4": {
      "video": "mesmerize-air-release-network-emily-v173.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-air-release-network-emily-v173",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-release-network-emily-v173/result.json",
      "visual_review": "mesmerize-air-release-network-emily-v173/visual-review.json",
      "proxy_sha256": "DDD62F5EB3D7CED26AC45D859C48C72C8CC9A0754A31DF1EEC7C1025FC605805",
      "actual_capture_hz": 27.061704211557295,
      "video_sha256": "61C926331B9ACDD36FE6B2BFA55430D99E1872F2F9ADE20E9B533A0CCD226C48",
      "checks_passed": true,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-air-release-network-emily-v173-source.mp4": {
      "video": "mesmerize-air-release-network-emily-v173-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-air-release-network-emily-v173",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-air-release-network-emily-v173/result.json",
      "visual_review": "mesmerize-air-release-network-emily-v173/visual-review.json",
      "proxy_sha256": "DDD62F5EB3D7CED26AC45D859C48C72C8CC9A0754A31DF1EEC7C1025FC605805",
      "actual_capture_hz": 26.705882352941178,
      "video_sha256": "0231C28E0399B53500562B951C6043F558F081C4F8CEA0D90CC1B541E152F69E",
      "checks_passed": true,
      "scope": "Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-air-entry-source-v175.mp4": {
      "video": "mesmerize-empty-air-entry-source-v175.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-empty-air-entry-source-v175",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-air-entry-source-v175/result.json",
      "visual_review": "mesmerize-empty-air-entry-source-v175/visual-review.json",
      "proxy_sha256": "DDD62F5EB3D7CED26AC45D859C48C72C8CC9A0754A31DF1EEC7C1025FC605805",
      "actual_capture_hz": 27.4375,
      "video_sha256": "6D963D8F0CC7E49636E9CCE572F68B094BBF23D70DAF611857DA9646AF72FD48",
      "checks_passed": true,
      "scope": "Native source sword is holstered before the in-place jump and Mesmerize aim begin together with1800ms hold; directly inspect airborne entry and later grounded release. Raw player states and single-arm Midnight bindings determine actual airborne behavior. Current strict reader still refuses holstered airborne Mesmerize, and no remote body/FX integration is enabled by this observation. Native mana/world behavior and final native floor are recorded. Both isolated checkpoints and protected save hashes restore."
    },
    "mesmerize-empty-air-release-source-v177.mp4": {
      "video": "mesmerize-empty-air-release-source-v177.mp4",
      "character": "emily",
      "label": "Native Mesmerize source observation · mesmerize-empty-air-release-source-v177",
      "alias": "mesmerize_native_observation",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-air-release-source-v177/result.json",
      "visual_review": "mesmerize-empty-air-release-source-v177/visual-review.json",
      "proxy_sha256": "DDD62F5EB3D7CED26AC45D859C48C72C8CC9A0754A31DF1EEC7C1025FC605805",
      "actual_capture_hz": 27.375,
      "video_sha256": "8EB6E5ED3ACC030B305033BBDAA3839CA8CF22A9205E64568AEF39D94CC116CE",
      "checks_passed": true,
      "scope": "Native source in-place jump while holding Mesmerize aim, releasing LT180ms after A. Raw player states and single-arm Midnight bindings determine actual airborne behavior. Current strict reader still refuses holstered airborne Mesmerize, and no remote body/FX integration is enabled by this observation. Native mana/world behavior and final native floor are recorded. Both isolated checkpoints and protected save hashes restore."
    },
    "mesmerize-empty-air-entry-network-v179.mp4": {
      "video": "mesmerize-empty-air-entry-network-v179.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-empty-air-entry-network-v179",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-air-entry-network-v179/result.json",
      "visual_review": "mesmerize-empty-air-entry-network-v179/visual-review.json",
      "proxy_sha256": "8E3CA92B8589CA623730D40987769A03A6D46B0B64EA3F4F38C2D5F7489A64B0",
      "actual_capture_hz": 26.794117647058822,
      "video_sha256": "4FEAE84A908667FC8787CD5FD9396BF4636FFE3EA7037D0D46A38CDF6E6225A6",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam; the holstered field distinguishes empty-hand and sword contexts. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-air-entry-network-v179-source.mp4": {
      "video": "mesmerize-empty-air-entry-network-v179-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-empty-air-entry-network-v179",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-air-entry-network-v179/result.json",
      "visual_review": "mesmerize-empty-air-entry-network-v179/visual-review.json",
      "proxy_sha256": "8E3CA92B8589CA623730D40987769A03A6D46B0B64EA3F4F38C2D5F7489A64B0",
      "actual_capture_hz": 26.823529411764707,
      "video_sha256": "2F9114545649DB3C20B23664D5DD0EB3C8A04708E251E97574AEE954D865B32E",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam; the holstered field distinguishes empty-hand and sword contexts. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-air-release-network-v181.mp4": {
      "video": "mesmerize-empty-air-release-network-v181.mp4",
      "character": "emily",
      "label": "Mesmerize Steam body and cosmetic world · mesmerize-empty-air-release-network-v181",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-air-release-network-v181/result.json",
      "visual_review": "mesmerize-empty-air-release-network-v181/visual-review.json",
      "proxy_sha256": "8E3CA92B8589CA623730D40987769A03A6D46B0B64EA3F4F38C2D5F7489A64B0",
      "actual_capture_hz": 26.558823529411764,
      "video_sha256": "43983B57D74FF7761DE15EC6A27209186E1E3304C4878879BDF0E929DBAB46A1",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam; the holstered field distinguishes empty-hand and sword contexts. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    },
    "mesmerize-empty-air-release-network-v181-source.mp4": {
      "video": "mesmerize-empty-air-release-network-v181-source.mp4",
      "character": "emily",
      "label": "Mesmerize native source · mesmerize-empty-air-release-network-v181",
      "alias": "mesmerize_native_world_network",
      "requirements": [
        "emily_mesmerize_cast"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "mesmerize-empty-air-release-network-v181/result.json",
      "visual_review": "mesmerize-empty-air-release-network-v181/visual-review.json",
      "proxy_sha256": "8E3CA92B8589CA623730D40987769A03A6D46B0B64EA3F4F38C2D5F7489A64B0",
      "actual_capture_hz": 27.291462217860648,
      "video_sha256": "CE87A14B9D8BDB9D214210B837D633538DD8E6B3B5DC1BC47F13F4B0DC92BB10",
      "checks_passed": true,
      "scope": "Native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam; the holstered field distinguishes empty-hand and sword contexts. The airCase field identifies the bounded in-place jump sequence. Explicit stance, holstered and move parameters identify this take. Native source power owns mana, targets and gameplay. Remote world/model/effects are cosmetic; positive NPC and repeatedly valid empty-target cases have distinct checks. Standing recovery, native sword selection, inventory/health, checkpoint cleanup and protected saves are verified separately from visual acceptance. Shared NPC authority and broader movement remain incomplete."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1332e/result.json",
    "coverage-v1332d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1339-emily-clone-spirit.json",
    "decision": "complete",
    "scope": "Emily casts Mesmerize without targets (power variants 16-18): the partner presents the enter, aim and release gestures and the native spirit appearing ahead of her, then its dismissal. The mesmerized behaviour of NPC targets is stage 4.",
    "takes": [
      {
        "take": "coverage-v1332e",
        "cases": [
          "client-mesmerize"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "client-mesmerize"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1332e/client-mesmerize-observer.mp4",
        "verdict": "Emily thrusts her hand forward; a dark, tendrilled spirit forms a few metres ahead of her and hovers there for the rest of the cast."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1332e/result.json](<../../../../work/natural-moveset/coverage-v1332e/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: mesmerize-native-emily-v70.mp4](<../../../../work/natural-moveset/mesmerize-native-emily-v70.mp4>)
- [in_game_recordings: mesmerize-native-emily-v71.mp4](<../../../../work/natural-moveset/mesmerize-native-emily-v71.mp4>)
- [in_game_recordings: mesmerize-owned-scenes-emily-v72.mp4](<../../../../work/natural-moveset/mesmerize-owned-scenes-emily-v72.mp4>)
- [in_game_recordings: mesmerize-network-emily-v73.mp4](<../../../../work/natural-moveset/mesmerize-network-emily-v73.mp4>)
- [in_game_recordings: mesmerize-network-emily-v73-source.mp4](<../../../../work/natural-moveset/mesmerize-network-emily-v73-source.mp4>)
- [in_game_recordings: mesmerize-world-emily-v75.mp4](<../../../../work/natural-moveset/mesmerize-world-emily-v75.mp4>)
- [in_game_recordings: mesmerize-network-emily-v74.mp4](<../../../../work/natural-moveset/mesmerize-network-emily-v74.mp4>)
- [in_game_recordings: mesmerize-network-emily-v74-source.mp4](<../../../../work/natural-moveset/mesmerize-network-emily-v74-source.mp4>)
- [in_game_recordings: mesmerize-visual-model-v76c.mp4](<../../../../work/natural-moveset/mesmerize-visual-model-v76c.mp4>)
- [in_game_recordings: mesmerize-visual-model-v76d.mp4](<../../../../work/natural-moveset/mesmerize-visual-model-v76d.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v78.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v78.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v78-source.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v78-source.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v79.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v79.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v79-source.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v79-source.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v80.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v80.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v80-source.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v80-source.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v81.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v81.mp4>)
- [in_game_recordings: mesmerize-world-network-emily-v81-source.mp4](<../../../../work/natural-moveset/mesmerize-world-network-emily-v81-source.mp4>)
- [in_game_recordings: mesmerize-interrupt-source-v82.mp4](<../../../../work/natural-moveset/mesmerize-interrupt-source-v82.mp4>)
- [in_game_recordings: mesmerize-interrupt-source-v82-source.mp4](<../../../../work/natural-moveset/mesmerize-interrupt-source-v82-source.mp4>)
- [in_game_recordings: mesmerize-interrupt-receiver-v82.mp4](<../../../../work/natural-moveset/mesmerize-interrupt-receiver-v82.mp4>)
- [in_game_recordings: mesmerize-interrupt-receiver-v82-source.mp4](<../../../../work/natural-moveset/mesmerize-interrupt-receiver-v82-source.mp4>)
- [in_game_recordings: mesmerize-moving-forward-v83.mp4](<../../../../work/natural-moveset/mesmerize-moving-forward-v83.mp4>)
- [in_game_recordings: mesmerize-moving-forward-v83-source.mp4](<../../../../work/natural-moveset/mesmerize-moving-forward-v83-source.mp4>)
- [in_game_recordings: mesmerize-moving-eyes-v84.mp4](<../../../../work/natural-moveset/mesmerize-moving-eyes-v84.mp4>)
- [in_game_recordings: mesmerize-moving-eyes-v84-source.mp4](<../../../../work/natural-moveset/mesmerize-moving-eyes-v84-source.mp4>)
- [in_game_recordings: mesmerize-eye-network-v85.mp4](<../../../../work/natural-moveset/mesmerize-eye-network-v85.mp4>)
- [in_game_recordings: mesmerize-eye-network-v85-source.mp4](<../../../../work/natural-moveset/mesmerize-eye-network-v85-source.mp4>)
- [in_game_recordings: mesmerize-eye-network-v86.mp4](<../../../../work/natural-moveset/mesmerize-eye-network-v86.mp4>)
- [in_game_recordings: mesmerize-eye-network-v86-source.mp4](<../../../../work/natural-moveset/mesmerize-eye-network-v86-source.mp4>)
- [in_game_recordings: mesmerize-eye-moving-v87.mp4](<../../../../work/natural-moveset/mesmerize-eye-moving-v87.mp4>)
- [in_game_recordings: mesmerize-eye-moving-v87-source.mp4](<../../../../work/natural-moveset/mesmerize-eye-moving-v87-source.mp4>)
- [in_game_recordings: mesmerize-eye-source-pause-v88.mp4](<../../../../work/natural-moveset/mesmerize-eye-source-pause-v88.mp4>)
- [in_game_recordings: mesmerize-eye-source-pause-v88-source.mp4](<../../../../work/natural-moveset/mesmerize-eye-source-pause-v88-source.mp4>)
- [in_game_recordings: mesmerize-targets-receiver-pause-v89.mp4](<../../../../work/natural-moveset/mesmerize-targets-receiver-pause-v89.mp4>)
- [in_game_recordings: mesmerize-targets-receiver-pause-v89-source.mp4](<../../../../work/natural-moveset/mesmerize-targets-receiver-pause-v89-source.mp4>)
- [in_game_recordings: mesmerize-targets-receiver-pause-v90.mp4](<../../../../work/natural-moveset/mesmerize-targets-receiver-pause-v90.mp4>)
- [in_game_recordings: mesmerize-targets-receiver-pause-v90-source.mp4](<../../../../work/natural-moveset/mesmerize-targets-receiver-pause-v90-source.mp4>)
- [in_game_recordings: mesmerize-partner-exclusion-v91.mp4](<../../../../work/natural-moveset/mesmerize-partner-exclusion-v91.mp4>)
- [in_game_recordings: mesmerize-partner-exclusion-v91-source.mp4](<../../../../work/natural-moveset/mesmerize-partner-exclusion-v91-source.mp4>)
- [in_game_recordings: mesmerize-partner-lifetime-v93.mp4](<../../../../work/natural-moveset/mesmerize-partner-lifetime-v93.mp4>)
- [in_game_recordings: mesmerize-partner-lifetime-v93-source.mp4](<../../../../work/natural-moveset/mesmerize-partner-lifetime-v93-source.mp4>)
- [in_game_recordings: mesmerize-partner-current-owner-v94.mp4](<../../../../work/natural-moveset/mesmerize-partner-current-owner-v94.mp4>)
- [in_game_recordings: mesmerize-partner-current-owner-v94-source.mp4](<../../../../work/natural-moveset/mesmerize-partner-current-owner-v94-source.mp4>)
- [in_game_recordings: mesmerize-partner-current-owner-v95.mp4](<../../../../work/natural-moveset/mesmerize-partner-current-owner-v95.mp4>)
- [in_game_recordings: mesmerize-partner-current-owner-v95-source.mp4](<../../../../work/natural-moveset/mesmerize-partner-current-owner-v95-source.mp4>)
- [in_game_recordings: mesmerize-npc-positive-v99.mp4](<../../../../work/natural-moveset/mesmerize-npc-positive-v99.mp4>)
- [in_game_recordings: mesmerize-npc-positive-v99-source.mp4](<../../../../work/natural-moveset/mesmerize-npc-positive-v99-source.mp4>)
- [in_game_recordings: mesmerize-scoped-reader-v105.mp4](<../../../../work/natural-moveset/mesmerize-scoped-reader-v105.mp4>)
- [in_game_recordings: mesmerize-scoped-reader-v105-source.mp4](<../../../../work/natural-moveset/mesmerize-scoped-reader-v105-source.mp4>)
- [in_game_recordings: mesmerize-target-links-v107.mp4](<../../../../work/natural-moveset/mesmerize-target-links-v107.mp4>)
- [in_game_recordings: mesmerize-target-links-v107-source.mp4](<../../../../work/natural-moveset/mesmerize-target-links-v107-source.mp4>)
- [in_game_recordings: mesmerize-links-receiver-pause-v109.mp4](<../../../../work/natural-moveset/mesmerize-links-receiver-pause-v109.mp4>)
- [in_game_recordings: mesmerize-links-receiver-pause-v109-source.mp4](<../../../../work/natural-moveset/mesmerize-links-receiver-pause-v109-source.mp4>)
- [in_game_recordings: mesmerize-links-ai-receiver-pause-v113.mp4](<../../../../work/natural-moveset/mesmerize-links-ai-receiver-pause-v113.mp4>)
- [in_game_recordings: mesmerize-links-ai-receiver-pause-v113-source.mp4](<../../../../work/natural-moveset/mesmerize-links-ai-receiver-pause-v113-source.mp4>)
- [in_game_recordings: mesmerize-links-fixture-receiver-pause-v116.mp4](<../../../../work/natural-moveset/mesmerize-links-fixture-receiver-pause-v116.mp4>)
- [in_game_recordings: mesmerize-links-fixture-receiver-pause-v116-source.mp4](<../../../../work/natural-moveset/mesmerize-links-fixture-receiver-pause-v116-source.mp4>)
- [in_game_recordings: mesmerize-links-fixture-source-pause-v117.mp4](<../../../../work/natural-moveset/mesmerize-links-fixture-source-pause-v117.mp4>)
- [in_game_recordings: mesmerize-links-fixture-source-pause-v117-source.mp4](<../../../../work/natural-moveset/mesmerize-links-fixture-source-pause-v117-source.mp4>)
- [in_game_recordings: mesmerize-active-resume-receiver-v120.mp4](<../../../../work/natural-moveset/mesmerize-active-resume-receiver-v120.mp4>)
- [in_game_recordings: mesmerize-active-resume-receiver-v120-source.mp4](<../../../../work/natural-moveset/mesmerize-active-resume-receiver-v120-source.mp4>)
- [in_game_recordings: mesmerize-active-resume-source-v121.mp4](<../../../../work/natural-moveset/mesmerize-active-resume-source-v121.mp4>)
- [in_game_recordings: mesmerize-active-resume-source-v121-source.mp4](<../../../../work/natural-moveset/mesmerize-active-resume-source-v121-source.mp4>)
- [in_game_recordings: mesmerize-aim-cancel-emily-v123.mp4](<../../../../work/natural-moveset/mesmerize-aim-cancel-emily-v123.mp4>)
- [in_game_recordings: mesmerize-aim-cancel-emily-v123-source.mp4](<../../../../work/natural-moveset/mesmerize-aim-cancel-emily-v123-source.mp4>)
- [in_game_recordings: mesmerize-natural-expiry-emily-v125.mp4](<../../../../work/natural-moveset/mesmerize-natural-expiry-emily-v125.mp4>)
- [in_game_recordings: mesmerize-natural-expiry-emily-v125-source.mp4](<../../../../work/natural-moveset/mesmerize-natural-expiry-emily-v125-source.mp4>)
- [in_game_recordings: mesmerize-natural-expiry-emily-v129.mp4](<../../../../work/natural-moveset/mesmerize-natural-expiry-emily-v129.mp4>)
- [in_game_recordings: mesmerize-natural-expiry-emily-v129-source.mp4](<../../../../work/natural-moveset/mesmerize-natural-expiry-emily-v129-source.mp4>)
- [in_game_recordings: mesmerize-crouch-source-observe-v127.mp4](<../../../../work/natural-moveset/mesmerize-crouch-source-observe-v127.mp4>)
- [in_game_recordings: mesmerize-crouch-fast-observe-v131.mp4](<../../../../work/natural-moveset/mesmerize-crouch-fast-observe-v131.mp4>)
- [in_game_recordings: mesmerize-crouch-network-emily-v134.mp4](<../../../../work/natural-moveset/mesmerize-crouch-network-emily-v134.mp4>)
- [in_game_recordings: mesmerize-crouch-network-emily-v134-source.mp4](<../../../../work/natural-moveset/mesmerize-crouch-network-emily-v134-source.mp4>)
- [in_game_recordings: mesmerize-crouch-cancel-emily-v136.mp4](<../../../../work/natural-moveset/mesmerize-crouch-cancel-emily-v136.mp4>)
- [in_game_recordings: mesmerize-crouch-cancel-emily-v136-source.mp4](<../../../../work/natural-moveset/mesmerize-crouch-cancel-emily-v136-source.mp4>)
- [in_game_recordings: mesmerize-crouch-network-emily-v138.mp4](<../../../../work/natural-moveset/mesmerize-crouch-network-emily-v138.mp4>)
- [in_game_recordings: mesmerize-crouch-network-emily-v138-source.mp4](<../../../../work/natural-moveset/mesmerize-crouch-network-emily-v138-source.mp4>)
- [in_game_recordings: mesmerize-crouch-moving-emily-v140.mp4](<../../../../work/natural-moveset/mesmerize-crouch-moving-emily-v140.mp4>)
- [in_game_recordings: mesmerize-crouch-moving-emily-v140-source.mp4](<../../../../work/natural-moveset/mesmerize-crouch-moving-emily-v140-source.mp4>)
- [in_game_recordings: mesmerize-holstered-source-v142.mp4](<../../../../work/natural-moveset/mesmerize-holstered-source-v142.mp4>)
- [in_game_recordings: mesmerize-holstered-crouch-source-v144.mp4](<../../../../work/natural-moveset/mesmerize-holstered-crouch-source-v144.mp4>)
- [in_game_recordings: mesmerize-air-source-v151.mp4](<../../../../work/natural-moveset/mesmerize-air-source-v151.mp4>)
- [in_game_recordings: mesmerize-empty-standing-emily-v156.mp4](<../../../../work/natural-moveset/mesmerize-empty-standing-emily-v156.mp4>)
- [in_game_recordings: mesmerize-empty-standing-emily-v156-source.mp4](<../../../../work/natural-moveset/mesmerize-empty-standing-emily-v156-source.mp4>)
- [in_game_recordings: mesmerize-empty-crouched-emily-v158.mp4](<../../../../work/natural-moveset/mesmerize-empty-crouched-emily-v158.mp4>)
- [in_game_recordings: mesmerize-empty-crouched-emily-v158-source.mp4](<../../../../work/natural-moveset/mesmerize-empty-crouched-emily-v158-source.mp4>)
- [in_game_recordings: mesmerize-crouched-moving-emily-v159.mp4](<../../../../work/natural-moveset/mesmerize-crouched-moving-emily-v159.mp4>)
- [in_game_recordings: mesmerize-crouched-moving-emily-v159-source.mp4](<../../../../work/natural-moveset/mesmerize-crouched-moving-emily-v159-source.mp4>)
- [in_game_recordings: mesmerize-empty-moving-emily-v160.mp4](<../../../../work/natural-moveset/mesmerize-empty-moving-emily-v160.mp4>)
- [in_game_recordings: mesmerize-empty-moving-emily-v160-source.mp4](<../../../../work/natural-moveset/mesmerize-empty-moving-emily-v160-source.mp4>)
- [in_game_recordings: mesmerize-air-release-source-v161.mp4](<../../../../work/natural-moveset/mesmerize-air-release-source-v161.mp4>)
- [in_game_recordings: mesmerize-air-entry-source-v163.mp4](<../../../../work/natural-moveset/mesmerize-air-entry-source-v163.mp4>)
- [in_game_recordings: mesmerize-air-entry-network-emily-v166.mp4](<../../../../work/natural-moveset/mesmerize-air-entry-network-emily-v166.mp4>)
- [in_game_recordings: mesmerize-air-entry-network-emily-v166-source.mp4](<../../../../work/natural-moveset/mesmerize-air-entry-network-emily-v166-source.mp4>)
- [in_game_recordings: mesmerize-air-entry-network-emily-v168.mp4](<../../../../work/natural-moveset/mesmerize-air-entry-network-emily-v168.mp4>)
- [in_game_recordings: mesmerize-air-entry-network-emily-v168-source.mp4](<../../../../work/natural-moveset/mesmerize-air-entry-network-emily-v168-source.mp4>)
- [in_game_recordings: mesmerize-air-entry-network-emily-v171.mp4](<../../../../work/natural-moveset/mesmerize-air-entry-network-emily-v171.mp4>)
- [in_game_recordings: mesmerize-air-entry-network-emily-v171-source.mp4](<../../../../work/natural-moveset/mesmerize-air-entry-network-emily-v171-source.mp4>)
- [in_game_recordings: mesmerize-air-release-network-emily-v173.mp4](<../../../../work/natural-moveset/mesmerize-air-release-network-emily-v173.mp4>)
- [in_game_recordings: mesmerize-air-release-network-emily-v173-source.mp4](<../../../../work/natural-moveset/mesmerize-air-release-network-emily-v173-source.mp4>)
- [in_game_recordings: mesmerize-empty-air-entry-source-v175.mp4](<../../../../work/natural-moveset/mesmerize-empty-air-entry-source-v175.mp4>)
- [in_game_recordings: mesmerize-empty-air-release-source-v177.mp4](<../../../../work/natural-moveset/mesmerize-empty-air-release-source-v177.mp4>)
- [in_game_recordings: mesmerize-empty-air-entry-network-v179.mp4](<../../../../work/natural-moveset/mesmerize-empty-air-entry-network-v179.mp4>)
- [in_game_recordings: mesmerize-empty-air-entry-network-v179-source.mp4](<../../../../work/natural-moveset/mesmerize-empty-air-entry-network-v179-source.mp4>)
- [in_game_recordings: mesmerize-empty-air-release-network-v181.mp4](<../../../../work/natural-moveset/mesmerize-empty-air-release-network-v181.mp4>)
- [in_game_recordings: mesmerize-empty-air-release-network-v181-source.mp4](<../../../../work/natural-moveset/mesmerize-empty-air-release-network-v181-source.mp4>)
## emily_shadow_walk_enter_move_attack_exit

Visual sweep 1260: accepted (emily accept). Shadow Walk entry, crawl and exit read correctly as functional coverage. The shadow material and attacks are not shown, and the native particles are subtle. Scope: body accepted; the shadow-walk transformed material is not applied and no attack is recorded. 550/551/552/554: Emily crouched and standing Shadow Walk entry, full-body crawl and requested crouched exit pass14 checks each through native Steam. Native crouched phase2 and trans_in_sneak/emily1000ms added; missing crouched fast sword holster repaired with exact equipment8/333ms. All six native particle resources follow/retire, no body-clock refusals, protected checkpoints/health/inventory retained. Paired temporal sheets viewed; clearer side view554 retained. Shadow material and full Shadow Walk coverage remain unfinished. Protocol58, bank59/336;544 material code disconnected.509 air landing and540 startup freshness remain open.

````json
{
  "id": "emily_shadow_walk_enter_move_attack_exit",
  "family": "powers",
  "characters": [
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_standing_sneaking_entry_crawl_exit_through_Steam",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "emily shadow walk enter move attack exit",
  "motions": [
    "shadow_walk_enter",
    "shadow_walk_idle",
    "shadow_walk_move_forward",
    "shadow_walk_attack_study",
    "shadow_walk_exit"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (emily accept). Shadow Walk entry, crawl and exit read correctly as functional coverage. The shadow material and attacks are not shown, and the native particles are subtle. Scope: body accepted; the shadow-walk transformed material is not applied and no attack is recorded. 550/551/552/554: Emily crouched and standing Shadow Walk entry, full-body crawl and requested crouched exit pass14 checks each through native Steam. Native crouched phase2 and trans_in_sneak/emily1000ms added; missing crouched fast sword holster repaired with exact equipment8/333ms. All six native particle resources follow/retire, no body-clock refusals, protected checkpoints/health/inventory retained. Paired temporal sheets viewed; clearer side view554 retained. Shadow material and full Shadow Walk coverage remain unfinished. Protocol58, bank59/336;544 material code disconnected.509 air landing and540 startup freshness remain open.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "shadow_walk",
      "character": "emily",
      "requirements": [
        "emily_shadow_walk_enter_move_attack_exit"
      ],
      "needs": [
        "native shadow form and effects",
        "entry/movement/attack/exit body handoff",
        "wall/rat-hole variants and restoration of equipment"
      ],
      "runtime_effects_verified": false,
      "partial_runtime_evidence": {
        "candidate": "candidate-shadowwalk-sneak-v550",
        "receipts": [
          "native-shadowwalk-sneak-v551/result.json",
          "native-shadowwalk-standing-v552/result.json",
          "native-shadowwalk-sneak-clear-v554/result.json"
        ],
        "scope": "Emily native standing/sneaking entry, forward/reverse crawl, requested crouched exit, six owned native particles and first-person preservation.",
        "remaining": [
          "Correct whole-body Shadow Walk appearance; ordinary human body is still visible.",
          "Dedicated sideways/backward/turning motion and continuous hand/foot contact review.",
          "Attack and victim contact/outcomes, upgraded sprint, wall and rat-hole traversal.",
          "Relevant interruptions and recovery; entry/crawl/requested exit tests do not establish all lifecycle cases.",
          "509 airborne landing dropout and540 stream-start freshness refusal remain separate unresolved issues."
        ],
        "visual_accepted": false,
        "visual_limit": "554 side view improves entry/recovery; body leaves the frame during mid-travel."
      }
    }
  ],
  "in_game_recordings": [
    "native-shadowwalk-standing-v516b.mp4",
    "native-shadowwalk-state-v520.mp4",
    "native-shadowwalk-scenes-v522.mp4",
    "native-shadowwalk-body-v525.mp4",
    "native-shadowwalk-body-v525-source.mp4",
    "native-shadowwalk-body-v527.mp4",
    "native-shadowwalk-body-v527-source.mp4",
    "native-shadowwalk-travel-v529.mp4",
    "native-shadowwalk-travel-v529-source.mp4",
    "native-shadowwalk-travel-v532.mp4",
    "native-shadowwalk-travel-v532-source.mp4",
    "native-shadowwalk-fx-v535.mp4",
    "native-shadowwalk-fx-v535-source.mp4",
    "native-shadowwalk-material-v540b.mp4",
    "native-shadowwalk-material-v540b-source.mp4",
    "native-shadowwalk-material-source-v542.mp4",
    "native-shadowwalk-material-v545.mp4",
    "native-shadowwalk-material-v545-source.mp4",
    "native-shadowwalk-crouch-source-v547b.mp4",
    "native-shadowwalk-sneak-v549.mp4",
    "native-shadowwalk-sneak-v549-source.mp4",
    "native-shadowwalk-sneak-v551.mp4",
    "native-shadowwalk-sneak-v551-source.mp4",
    "native-shadowwalk-standing-v552.mp4",
    "native-shadowwalk-standing-v552-source.mp4",
    "native-shadowwalk-sneak-clear-v554.mp4",
    "native-shadowwalk-sneak-clear-v554-source.mp4"
  ],
  "native_effects_status": "partial_six_native_cosmetic_particles_follow_and_retire",
  "complete": true,
  "evidence": [
    "native-shadowwalk-sneak-v551/result.json",
    "native-shadowwalk-standing-v552/result.json",
    "native-shadowwalk-sneak-clear-v554/result.json",
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1332c/result.json",
    "coverage-v1359e/result.json",
    "takedown-variants-v1254zd/result.json"
  ],
  "remaining_behaviors": [
    "Correct whole-body Shadow Walk appearance; ordinary human body is still visible.",
    "Dedicated sideways/backward/turning motion and continuous hand/foot contact review.",
    "Attack and victim contact/outcomes, upgraded sprint, wall and rat-hole traversal.",
    "Relevant interruptions and recovery; entry/crawl/requested exit tests do not establish all lifecycle cases.",
    "509 airborne landing dropout and540 stream-start freshness refusal remain separate unresolved issues."
  ],
  "completion": {
    "audit": "completion-audit-v1393-shadow-walk.json",
    "decision": "complete",
    "scope": "Emily's Shadow Walk on the partner: entry, the crawl while moving, the form's kill and choke, and the exit, in the native black material silhouette, recovering to her ordinary body.",
    "takes": [
      {
        "take": "coverage-v1359e",
        "cases": [
          "client-shadowwalk"
        ]
      },
      {
        "take": "takedown-variants-v1254zd",
        "cases": [
          "client-swback-lethal",
          "client-swback-choke"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1359e/client-shadowwalk-observer.mp4",
        "verdict": "Emily drops from standing into the Shadow Walk crawl and becomes a matte black silhouette with the native smoke around her, crawls forward on all fours, then rises and is back in her ordinary colours as the form ends."
      },
      {
        "video": "takedown-variants-v1254zd/swback-client-lethal-observer.mp4",
        "verdict": "From the form behind the guard, Emily as a black silhouette seizes him and throws him overhead; he crashes down dead and she straightens in her ordinary colours. Her sword's ring guard stays lit in her hand during the throw (the sword is a separate entity outside the material)."
      },
      {
        "video": "takedown-variants-v1254zd/swback-client-choke-observer.mp4",
        "verdict": "From the form, Emily as a black silhouette pulls the guard down to his knees from behind, then holds him down in her ordinary colours until he goes limp."
      }
    ],
    "lifecycle": "See method."
  }
}
````

- [evidence: native-shadowwalk-sneak-v551/result.json](<../../../../work/natural-moveset/native-shadowwalk-sneak-v551/result.json>)
- [evidence: native-shadowwalk-standing-v552/result.json](<../../../../work/natural-moveset/native-shadowwalk-standing-v552/result.json>)
- [evidence: native-shadowwalk-sneak-clear-v554/result.json](<../../../../work/natural-moveset/native-shadowwalk-sneak-clear-v554/result.json>)
- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1332c/result.json](<../../../../work/natural-moveset/coverage-v1332c/result.json>)
- [evidence: coverage-v1359e/result.json](<../../../../work/natural-moveset/coverage-v1359e/result.json>)
- [evidence: takedown-variants-v1254zd/result.json](<../../../../work/natural-moveset/takedown-variants-v1254zd/result.json>)
- [in_game_recordings: native-shadowwalk-standing-v516b.mp4](<../../../../work/natural-moveset/native-shadowwalk-standing-v516b.mp4>)
- [in_game_recordings: native-shadowwalk-state-v520.mp4](<../../../../work/natural-moveset/native-shadowwalk-state-v520.mp4>)
- [in_game_recordings: native-shadowwalk-scenes-v522.mp4](<../../../../work/natural-moveset/native-shadowwalk-scenes-v522.mp4>)
- [in_game_recordings: native-shadowwalk-body-v525.mp4](<../../../../work/natural-moveset/native-shadowwalk-body-v525.mp4>)
- [in_game_recordings: native-shadowwalk-body-v525-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-body-v525-source.mp4>)
- [in_game_recordings: native-shadowwalk-body-v527.mp4](<../../../../work/natural-moveset/native-shadowwalk-body-v527.mp4>)
- [in_game_recordings: native-shadowwalk-body-v527-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-body-v527-source.mp4>)
- [in_game_recordings: native-shadowwalk-travel-v529.mp4](<../../../../work/natural-moveset/native-shadowwalk-travel-v529.mp4>)
- [in_game_recordings: native-shadowwalk-travel-v529-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-travel-v529-source.mp4>)
- [in_game_recordings: native-shadowwalk-travel-v532.mp4](<../../../../work/natural-moveset/native-shadowwalk-travel-v532.mp4>)
- [in_game_recordings: native-shadowwalk-travel-v532-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-travel-v532-source.mp4>)
- [in_game_recordings: native-shadowwalk-fx-v535.mp4](<../../../../work/natural-moveset/native-shadowwalk-fx-v535.mp4>)
- [in_game_recordings: native-shadowwalk-fx-v535-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-fx-v535-source.mp4>)
- [in_game_recordings: native-shadowwalk-material-v540b.mp4](<../../../../work/natural-moveset/native-shadowwalk-material-v540b.mp4>)
- [in_game_recordings: native-shadowwalk-material-v540b-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-material-v540b-source.mp4>)
- [in_game_recordings: native-shadowwalk-material-source-v542.mp4](<../../../../work/natural-moveset/native-shadowwalk-material-source-v542.mp4>)
- [in_game_recordings: native-shadowwalk-material-v545.mp4](<../../../../work/natural-moveset/native-shadowwalk-material-v545.mp4>)
- [in_game_recordings: native-shadowwalk-material-v545-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-material-v545-source.mp4>)
- [in_game_recordings: native-shadowwalk-crouch-source-v547b.mp4](<../../../../work/natural-moveset/native-shadowwalk-crouch-source-v547b.mp4>)
- [in_game_recordings: native-shadowwalk-sneak-v549.mp4](<../../../../work/natural-moveset/native-shadowwalk-sneak-v549.mp4>)
- [in_game_recordings: native-shadowwalk-sneak-v549-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-sneak-v549-source.mp4>)
- [in_game_recordings: native-shadowwalk-sneak-v551.mp4](<../../../../work/natural-moveset/native-shadowwalk-sneak-v551.mp4>)
- [in_game_recordings: native-shadowwalk-sneak-v551-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-sneak-v551-source.mp4>)
- [in_game_recordings: native-shadowwalk-standing-v552.mp4](<../../../../work/natural-moveset/native-shadowwalk-standing-v552.mp4>)
- [in_game_recordings: native-shadowwalk-standing-v552-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-standing-v552-source.mp4>)
- [in_game_recordings: native-shadowwalk-sneak-clear-v554.mp4](<../../../../work/natural-moveset/native-shadowwalk-sneak-clear-v554.mp4>)
- [in_game_recordings: native-shadowwalk-sneak-clear-v554-source.mp4](<../../../../work/natural-moveset/native-shadowwalk-sneak-clear-v554-source.mp4>)
## power_upgrade_variants

Blink Assault kick/slash body studies only. Other upgrades, transformed bodies, effects and action/state variants are not covered by these clips. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.

````json
{
  "id": "power_upgrade_variants",
  "family": "powers",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "native_runtime_completion_audit",
  "visual_validation": "accepted_completion_audit",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "power upgrade variants",
  "motions": [
    "blink_assault_kick_sequence",
    "blink_assault_slash_sequence"
  ],
  "deliverable_kind": "animation",
  "notes": "Blink Assault kick/slash body studies only. Other upgrades, transformed bodies, effects and action/state variants are not covered by these clips. Completion also requires native power effects/entities, synchronized phase and targets, cleanup and no duplicate outcomes. First-person-only effects must retain native viewer scope.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "power_variants",
      "character": "both",
      "requirements": [
        "power_upgrade_variants",
        "power_interruptions_and_no_mana"
      ],
      "needs": [
        "applicable upgrades",
        "no-mana and invalid-target rejection",
        "arbitrary interruption and absence of orphaned effects"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [],
  "native_effects_status": "not_verified",
  "completion": {
    "audit": "completion-audit-v1511-power-upgrades.json",
    "decision": "complete",
    "scope": "Both roles: every power upgrade that changes the caster's presented action or the power's presented entities and effects (Blink Assault, Shockwave, Greater Windblast, Possession of humans, Transposition, Twin Shade, Bigger, Follow and Double Swarm, Far Reach object pull), with numeric and first-person upgrades unchanged in presentation. World outcomes of upgrades are stage 4 (#117, #115).",
    "takes": [
      {
        "take": "coverage-v1503a",
        "cases": [
          "host-upgshockwave2"
        ]
      },
      {
        "take": "coverage-v1503b",
        "cases": [
          "host-upgwbgreater"
        ]
      },
      {
        "take": "coverage-v1503c",
        "cases": [
          "host-upgswarmfollow"
        ]
      },
      {
        "take": "coverage-v1504a",
        "cases": [
          "host-upgswarmdouble"
        ]
      },
      {
        "take": "coverage-v1506a",
        "cases": [
          "host-upgswarmdouble2"
        ]
      },
      {
        "take": "coverage-v1497a",
        "cases": [
          "client-upgtwinshade2"
        ]
      },
      {
        "take": "coverage-v1487a",
        "cases": [
          "client-upgtransposition"
        ]
      },
      {
        "take": "mission-coverage-v1492a",
        "cases": [
          "host-takedown"
        ]
      },
      {
        "take": "coverage-v1332f",
        "cases": [
          "host-possesshuman"
        ]
      },
      {
        "take": "coverage-v1359b",
        "cases": [
          "host-blinkkick"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1503a/host-upgshockwave2-observer.mp4",
        "verdict": "Emily's window: Corvo drops in and lands crouched inside a large swirling wind blast while a wave runs along the floor around him. Accepted."
      },
      {
        "video": "coverage-v1503b/host-upgwbgreater-observer.mp4",
        "verdict": "Emily's window: Corvo thrusts his hand out and heavy white wind streaks fly from it, wider than the plain blast. Accepted."
      },
      {
        "video": "coverage-v1503c/host-upgswarmfollow-observer.mp4",
        "verdict": "Emily's window (product session): Corvo summons the swarm and the rats run after him as he walks off. Accepted."
      },
      {
        "video": "coverage-v1506a/host-upgswarmdouble2-observer.mp4",
        "verdict": "Emily's window: each cast throws a smoke burst and a pack of rats; after the second both packs, about thirty rats, swarm around Corvo's feet. Accepted."
      },
      {
        "video": "coverage-v1497a/client-upgtwinshade2-observer.mp4",
        "verdict": "Corvo's window: Emily casts, the first shade appears beside her and walks off, she turns and casts again and the second shade rises in smoke; both are shown at once (status). Accepted."
      },
      {
        "video": "coverage-v1487a/client-upgtransposition-observer.mp4",
        "verdict": "Corvo's window: Emily casts, the shade rises, and she reappears at the shade's place. Accepted."
      },
      {
        "video": "mission-coverage-v1492a/host-takedown-source.mp4",
        "verdict": "Corvo's own film on Edge of the World: the possession cast, the vortex, the possessed walk and the overseer staggering with a hand to his head after the exit; the partner side of possessing a human is coverage-v1332f. Accepted."
      },
      {
        "video": "coverage-v1332f/host-possesshuman-observer.mp4",
        "verdict": "Emily's window: Corvo thrusts his arm toward the guard and vanishes; only the guard is seen, then Corvo reappears beside him (audit v1341). Accepted."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true,
  "evidence": [
    "coverage-v1503a/result.json",
    "coverage-v1503b/result.json",
    "coverage-v1503c/result.json",
    "coverage-v1504a/result.json",
    "coverage-v1506a/result.json",
    "coverage-v1497a/result.json",
    "coverage-v1487a/result.json",
    "mission-coverage-v1492a/result.json",
    "coverage-v1332f/result.json",
    "coverage-v1359b/result.json"
  ],
  "visual_validation_before": "not_accepted",
  "runtime_binding_before": "not_verified"
}
````

- [evidence: coverage-v1503a/result.json](<../../../../work/natural-moveset/coverage-v1503a/result.json>)
- [evidence: coverage-v1503b/result.json](<../../../../work/natural-moveset/coverage-v1503b/result.json>)
- [evidence: coverage-v1503c/result.json](<../../../../work/natural-moveset/coverage-v1503c/result.json>)
- [evidence: coverage-v1504a/result.json](<../../../../work/natural-moveset/coverage-v1504a/result.json>)
- [evidence: coverage-v1506a/result.json](<../../../../work/natural-moveset/coverage-v1506a/result.json>)
- [evidence: coverage-v1497a/result.json](<../../../../work/natural-moveset/coverage-v1497a/result.json>)
- [evidence: coverage-v1487a/result.json](<../../../../work/natural-moveset/coverage-v1487a/result.json>)
- [evidence: mission-coverage-v1492a/result.json](<../../../../work/natural-moveset/mission-coverage-v1492a/result.json>)
- [evidence: coverage-v1332f/result.json](<../../../../work/natural-moveset/coverage-v1332f/result.json>)
- [evidence: coverage-v1359b/result.json](<../../../../work/natural-moveset/coverage-v1359b/result.json>)
## power_interruptions_and_no_mana

Visual sweep 1260: accepted (corvo accept). Corvo's Bend Time out-of-mana failures and pause recovery read correctly. A late relaxed-stance switch after the resume tests should be checked, and no Emily or other-power interruption recordings exist (outside the bound scope). Corvo standing/crouched and drawn/holstered Bend Time failures pass thirteen checks each with automatic finite native hand effects in v48. Source-confirmed event and delayed body clock prevent duplicate and late playback. Other characters, powers, air/slide, arbitrary interruption phases, precise tattoo alignment and seamless visibility remain open; this requirement is not complete.

````json
{
  "id": "power_interruptions_and_no_mana",
  "family": "powers",
  "characters": [
    "corvo",
    "emily"
  ],
  "authoring": "candidates",
  "runtime_binding": "partial_native_Corvo_Bend_Time_failure_and_same_world_pause",
  "visual_validation": "accepted_sweep_1260_functional_owner",
  "native_reference_family_count": 217,
  "priority": 3,
  "title": "power interruptions and no mana",
  "motions": [
    "power_doppelganger_cancel",
    "power_mesmerize_cancel",
    "power_bend_time_cancel",
    "power_devouring_swarm_cancel",
    "power_blink_cancel",
    "power_possession_cancel",
    "power_far_reach_cancel",
    "power_domino_cancel"
  ],
  "deliverable_kind": "animation",
  "notes": "Visual sweep 1260: accepted (corvo accept). Corvo's Bend Time out-of-mana failures and pause recovery read correctly. A late relaxed-stance switch after the resume tests should be checked, and no Emily or other-power interruption recordings exist (outside the bound scope). Corvo standing/crouched and drawn/holstered Bend Time failures pass thirteen checks each with automatic finite native hand effects in v48. Source-confirmed event and delayed body clock prevent duplicate and late playback. Other characters, powers, air/slide, arbitrary interruption phases, precise tattoo alignment and seamless visibility remain open; this requirement is not complete.",
  "phase_status": "complete",
  "power_visual_requirements": [
    {
      "id": "power_variants",
      "character": "both",
      "requirements": [
        "power_upgrade_variants",
        "power_interruptions_and_no_mana"
      ],
      "needs": [
        "applicable upgrades",
        "no-mana and invalid-target rejection",
        "arbitrary interruption and absence of orphaned effects"
      ],
      "runtime_effects_verified": false
    }
  ],
  "in_game_recordings": [
    "failure-fx-receiver-stop-v51.mp4",
    "failure-fx-receiver-stop-v51-source.mp4",
    "failure-fx-source-resume-v51.mp4",
    "failure-fx-source-resume-v51-source.mp4",
    "failure-fx-receiver-resume-v51.mp4",
    "failure-fx-receiver-resume-v51-source.mp4",
    "outofmana-corvo-standing-armed-fx-v48.mp4",
    "outofmana-corvo-standing-armed-fx-v48-source.mp4",
    "outofmana-corvo-crouched-armed-fx-v48.mp4",
    "outofmana-corvo-crouched-armed-fx-v48-source.mp4",
    "outofmana-corvo-standing-empty-fx-v48.mp4",
    "outofmana-corvo-standing-empty-fx-v48-source.mp4",
    "outofmana-corvo-crouched-empty-fx-v48.mp4",
    "outofmana-corvo-crouched-empty-fx-v48-source.mp4",
    "outofmana-hand-fx-corvo-v47.mp4",
    "outofmana-hand-fx-emily-v47.mp4",
    "outofmana-corvo-standing-armed-v46.mp4",
    "outofmana-corvo-standing-armed-v46-source.mp4",
    "outofmana-corvo-standing-empty-v46.mp4",
    "outofmana-corvo-standing-empty-v46-source.mp4",
    "outofmana-corvo-crouched-armed-v46.mp4",
    "outofmana-corvo-crouched-armed-v46-source.mp4",
    "outofmana-corvo-crouched-empty-v46.mp4",
    "outofmana-corvo-crouched-empty-v46-source.mp4",
    "native-bendtime-corvo-no-mana-crouched-armed-v45.mp4",
    "native-bendtime-corvo-no-mana-standing-empty-v45.mp4",
    "native-bendtime-corvo-no-mana-crouched-empty-v45.mp4",
    "outofmana-corvo-standing-armed-front-v45.mp4",
    "outofmana-corvo-standing-armed-front-v45-source.mp4",
    "outofmana-corvo-standing-armed-v45-v1.mp4",
    "outofmana-corvo-standing-armed-v45-v1-source.mp4",
    "bendtime-receiver-stop-v44.mp4",
    "bendtime-receiver-stop-v44-source.mp4",
    "bendtime-source-pause-v44.mp4",
    "bendtime-source-pause-v44-source.mp4",
    "bendtime-receiver-pause-v44.mp4",
    "bendtime-receiver-pause-v44-source.mp4",
    "bendtime-source-resume-v44.mp4",
    "bendtime-source-resume-v44-source.mp4",
    "bendtime-receiver-resume-v44.mp4",
    "bendtime-receiver-resume-v44-source.mp4",
    "bendtime-receiver-resume-v44-v2.mp4",
    "bendtime-receiver-resume-v44-v2-source.mp4",
    "native-bendtime-corvo-no-mana-v44.mp4",
    "native-bendtime-corvo-no-mana-v44-v2.mp4"
  ],
  "native_effects_status": "not_verified",
  "native_integration": {
    "bendtime-receiver-stop-v44.mp4": {
      "video": "bendtime-receiver-stop-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-stop-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-stop-v44/result.json",
      "visual_review": "bendtime-receiver-stop-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.38888888888889,
      "video_sha256": "203FE37BA8BC3F1F76E0E74E3D850E2821293F88680EF34E8C8EADF042B58561",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-stop-v44-source.mp4": {
      "video": "bendtime-receiver-stop-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-stop-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-stop-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.34011090573013,
      "video_sha256": "BB3D5F34FCA0AD9628C52AD4B7DD08D2E1CA9FB00667F0A10EC5B7CDC359FCCD",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-pause-v44.mp4": {
      "video": "bendtime-source-pause-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · source-pause-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-pause-v44/result.json",
      "visual_review": "bendtime-source-pause-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "903BE6034DE9951FD1CCF03B2DD6AD5DDEA89E040B96B768FFB1DE0E44CF7A79",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-pause-v44-source.mp4": {
      "video": "bendtime-source-pause-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · source-pause-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-pause-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.22222222222222,
      "video_sha256": "9A32ADFC7CD055B30F0F4E9396AFBF6A4EEA4F455656B3CCCCC5E75A249A8E4F",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-pause-v44.mp4": {
      "video": "bendtime-receiver-pause-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-pause-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-pause-v44/result.json",
      "visual_review": "bendtime-receiver-pause-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.44916820702403,
      "video_sha256": "610B1ECF5F1D331A492BB57773C0804BE28EEF3F86A566612951936307011700",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-pause-v44-source.mp4": {
      "video": "bendtime-receiver-pause-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-pause-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-pause-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.217068645640076,
      "video_sha256": "A8A66FBB4CC056DE4C2464A99A25CD589F1746AC93AD49F31567E5391E5894F0",
      "checks_passed": true,
      "scope": "Actual native Bend Time cast interruption and ownership retirement. Resume is deliberately disarmed for this test. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-resume-v44.mp4": {
      "video": "bendtime-source-resume-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · source-resume-v44",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-resume-v44/result.json",
      "visual_review": "bendtime-source-resume-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.72222222222222,
      "video_sha256": "6FC7E4B54CFDFC1B75F819C8F1AFA43A58FCD02753158ED637785D886476DFF3",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-source-resume-v44-source.mp4": {
      "video": "bendtime-source-resume-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · source-resume-v44 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-source-resume-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.22222222222222,
      "video_sha256": "091307A958980F854FA4B990ED3F0683C838C5562ED65D401AB95862A11C65DA",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44.mp4": {
      "video": "bendtime-receiver-resume-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44 · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44/result.json",
      "visual_review": "bendtime-receiver-resume-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.22222222222222,
      "video_sha256": "5A27735C82275C1EB96A8F2B4C7D81B24E691E2215777C44E3521EFB61C83E57",
      "checks_passed": false,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44-source.mp4": {
      "video": "bendtime-receiver-resume-v44-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44 · source · failed check",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 25.78558225508318,
      "video_sha256": "1BDF823F63EAAB4DFF3CF253E414B27A28680F67CAF43659E79AA9A5F03F3F5A",
      "checks_passed": false,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44-v2.mp4": {
      "video": "bendtime-receiver-resume-v44-v2.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44-v2",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44-v2/result.json",
      "visual_review": "bendtime-receiver-resume-v44-v2/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 27.38888888888889,
      "video_sha256": "BEF1A23891BDD93864A06860466BC8DF8B38F5538D3550A3E618C00AC28FD9DC",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "bendtime-receiver-resume-v44-v2-source.mp4": {
      "video": "bendtime-receiver-resume-v44-v2-source.mp4",
      "character": "corvo",
      "label": "Bend Time · receiver-resume-v44-v2 · source",
      "alias": "native_phase_bend_time_in",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "bendtime-receiver-resume-v44-v2/result.json",
      "visual_review": null,
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": "F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3",
      "actual_capture_hz": 26.950092421441774,
      "video_sha256": "D6D6E2EFF460B32889450791C3FA93D38C4D4CB4470F601C28C3DDD793BEBF7D",
      "checks_passed": true,
      "scope": "Actual native Bend Time interruption and ownership retirement; same-world automatic recovery only when automaticResume is true. No shared time authority, death/load or full movement acceptance inferred."
    },
    "native-bendtime-corvo-no-mana-v44.mp4": {
      "video": "native-bendtime-corvo-no-mana-v44.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-v44 · failed check",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-v44/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-v44/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": null,
      "actual_capture_hz": 26.125,
      "video_sha256": "7BF586EBADD27D542C9FAC85510DBA90F07566E93495FF81316C2CF9040C9568",
      "checks_passed": false,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "native-bendtime-corvo-no-mana-v44-v2.mp4": {
      "video": "native-bendtime-corvo-no-mana-v44-v2.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-v44-v2",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-v44-v2/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-v44-v2/visual-review.json",
      "proxy_sha256": "DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024",
      "bank_sha256": null,
      "actual_capture_hz": 26.0,
      "video_sha256": "7333427C4C8747CAAD8FC1C8944759D5C0A04AF0ABF83826BD8A8463B31CB530",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "outofmana-corvo-standing-armed-v45-v1.mp4": {
      "video": "outofmana-corvo-standing-armed-v45-v1.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v45-v1",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v45-v1/result.json",
      "visual_review": "outofmana-corvo-standing-armed-v45-v1/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 27.5,
      "video_sha256": "CA2B3C3BBA0BB71E7C5B8A80E6671173A3AE5E02FD70B42B88FFD985F7FF07C3",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-v45-v1-source.mp4": {
      "video": "outofmana-corvo-standing-armed-v45-v1-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v45-v1 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v45-v1/result.json",
      "visual_review": null,
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 23.064066852367688,
      "video_sha256": "8211A0509C538B31E26CE22584F97EB2375CEED3DC5837BD4A50775EEBF44482",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-front-v45.mp4": {
      "video": "outofmana-corvo-standing-armed-front-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-front-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-front-v45/result.json",
      "visual_review": "outofmana-corvo-standing-armed-front-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 27.57660167130919,
      "video_sha256": "940D056A4CF1A013AA0101944301E7F3338DE5CCD3261277FA5AF5FE8FE9950D",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-front-v45-source.mp4": {
      "video": "outofmana-corvo-standing-armed-front-v45-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-front-v45 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-front-v45/result.json",
      "visual_review": null,
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": "1B97B5989EC715B2898FB6AED3F0F45AB170DFA8A9949C4D729B32A8849258B8",
      "actual_capture_hz": 22.29050279329609,
      "video_sha256": "3BC97F3F1DAC504E0978F34BA467E03DD19D76141B985B7623AFDCD60140A933",
      "checks_passed": true,
      "scope": "Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "native-bendtime-corvo-no-mana-standing-empty-v45.mp4": {
      "video": "native-bendtime-corvo-no-mana-standing-empty-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-standing-empty-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-standing-empty-v45/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-standing-empty-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": null,
      "actual_capture_hz": 25.905292479108635,
      "video_sha256": "B4947B3394622D33D8FAE4844EABA829C3033057331994053BC8E74A673134E9",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "native-bendtime-corvo-no-mana-crouched-empty-v45.mp4": {
      "video": "native-bendtime-corvo-no-mana-crouched-empty-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-crouched-empty-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-crouched-empty-v45/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-crouched-empty-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": null,
      "actual_capture_hz": 26.083333333333332,
      "video_sha256": "CFB522FC1F8F19BEB52919BC8AC9B6315952087DC76279D8BD2E0E42FED23707",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "outofmana-corvo-standing-armed-v46.mp4": {
      "video": "outofmana-corvo-standing-armed-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v46/result.json",
      "visual_review": "outofmana-corvo-standing-armed-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.333333333333332,
      "video_sha256": "5F2E568D34908BFF2EA94A7F0B4737F56287C5D81C72AAE220B033AE0BA81AF4",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-v46-source.mp4": {
      "video": "outofmana-corvo-standing-armed-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.083333333333332,
      "video_sha256": "5D76489440394EB427F5DD6894B61CEC136BEBB71F44FA5C32FD0DC61AF41C25",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-v46.mp4": {
      "video": "outofmana-corvo-standing-empty-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-v46/result.json",
      "visual_review": "outofmana-corvo-standing-empty-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.583333333333332,
      "video_sha256": "FB217C93125A8EFF652CAC02229885867BDD65D49FABB95E3C787B345DA6033E",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-v46-source.mp4": {
      "video": "outofmana-corvo-standing-empty-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.75,
      "video_sha256": "10A1550541D571F5D5D98530D73AAAC580CD01CC0A08610A5B5345E66813C651",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-v46.mp4": {
      "video": "outofmana-corvo-crouched-armed-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-v46/result.json",
      "visual_review": "outofmana-corvo-crouched-armed-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.74373259052925,
      "video_sha256": "22DC69024D5B05788396012A72C3896529587645C3693CAB32BCC52EFEDA65C3",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-v46-source.mp4": {
      "video": "outofmana-corvo-crouched-armed-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.583333333333332,
      "video_sha256": "004F8A556FC29C7A6E1CA367CCCF1A1D16639AABC98D5E9523536DA12E3A3E3B",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-v46.mp4": {
      "video": "outofmana-corvo-crouched-empty-v46.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-v46",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-v46/result.json",
      "visual_review": "outofmana-corvo-crouched-empty-v46/visual-review.json",
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.24233983286908,
      "video_sha256": "0C86365C5EB85EF4B92542E5026E1B89EED5D6DE1B2FD44E040B8FB50401BB76",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-v46-source.mp4": {
      "video": "outofmana-corvo-crouched-empty-v46-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-v46 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-v46/result.json",
      "visual_review": null,
      "proxy_sha256": "AA8C3D2A9A9BCA738D37F6C0464BF523395C42F440FB2419D617381DCF3C990D",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.651810584958216,
      "video_sha256": "B666B412B92B3DCAD2AD93E64568E180B1A73191EA8398524CCA2EBCD9C4E435",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Native time dilation remains source-local; no remote power activation or animation damage. Original files unchanged."
    },
    "native-bendtime-corvo-no-mana-crouched-armed-v45.mp4": {
      "video": "native-bendtime-corvo-no-mana-crouched-armed-v45.mp4",
      "character": "corvo",
      "label": "Bend Time · native-corvo-no-mana-crouched-armed-v45",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "native-bendtime-corvo-no-mana-crouched-armed-v45/result.json",
      "visual_review": "native-bendtime-corvo-no-mana-crouched-armed-v45/visual-review.json",
      "proxy_sha256": "296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7",
      "bank_sha256": null,
      "actual_capture_hz": 26.083333333333332,
      "video_sha256": "FE38AE6FA392739619CE7514DD4000E232F5BB27F285726B5841EAA7C9051209",
      "checks_passed": true,
      "scope": "Native BendTime insufficient-mana investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified."
    },
    "outofmana-hand-fx-corvo-v47.mp4": {
      "video": "outofmana-hand-fx-corvo-v47.mp4",
      "character": "corvo",
      "label": "Native failure hand FX · corvo · manual",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-hand-fx-corvo-v47/result.json",
      "visual_review": "outofmana-hand-fx-corvo-v47/visual-review.json",
      "proxy_sha256": "5957834D38E30B3DFD3DD57FE2BEFFCFF55898AF4D0468A8A4839B8E167F6109",
      "bank_sha256": null,
      "actual_capture_hz": 27.666666666666668,
      "video_sha256": "71323C7A62E62914570372190B9D3DF1E7DC309733C1130FB16754814FBBE2E3",
      "checks_passed": true,
      "scope": "Isolated manual native out-of-mana hand particle probe on the role-specific remote avatar. Exact native FX/particles, owned left wrist transform, finite 1600ms lease and cancellation. No automatic network trigger, first-person callback, power activation, resource write or damage. Wrist-centred adaptation of the native tattoo socket; placement/scale and camera effects require actual visual review."
    },
    "outofmana-hand-fx-emily-v47.mp4": {
      "video": "outofmana-hand-fx-emily-v47.mp4",
      "character": "emily",
      "label": "Native failure hand FX · emily · manual",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-hand-fx-emily-v47/result.json",
      "visual_review": "outofmana-hand-fx-emily-v47/visual-review.json",
      "proxy_sha256": "5957834D38E30B3DFD3DD57FE2BEFFCFF55898AF4D0468A8A4839B8E167F6109",
      "bank_sha256": null,
      "actual_capture_hz": 27.555555555555557,
      "video_sha256": "F86C4EB3F6521F98D07C2F3AA7751D55DE234CED98190AE498DAD2CA8B04DEEC",
      "checks_passed": true,
      "scope": "Isolated manual native out-of-mana hand particle probe on the role-specific remote avatar. Exact native FX/particles, owned left wrist transform, finite 1600ms lease and cancellation. No automatic network trigger, first-person callback, power activation, resource write or damage. Wrist-centred adaptation of the native tattoo socket; placement/scale and camera effects require actual visual review."
    },
    "outofmana-corvo-standing-armed-fx-v48.mp4": {
      "video": "outofmana-corvo-standing-armed-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-fx-v48/result.json",
      "visual_review": "outofmana-corvo-standing-armed-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.333333333333332,
      "video_sha256": "29CF40DC881FA9755E664329DB1BCDA91DC9B0A7A4BB1577614C0D03C580DB4F",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-standing-armed-fx-v48-source.mp4": {
      "video": "outofmana-corvo-standing-armed-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-armed-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-armed-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.568245125348188,
      "video_sha256": "5098EA60E060C0627510FDFB0FEA05C13DDAB87666319AA5612317043EEF084F",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-fx-v48.mp4": {
      "video": "outofmana-corvo-crouched-armed-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-fx-v48/result.json",
      "visual_review": "outofmana-corvo-crouched-armed-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.416666666666668,
      "video_sha256": "CDE13F2614BF013EB585FAAA84CB5DF949492938CD7F197F065C56D49D382782",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-armed-fx-v48-source.mp4": {
      "video": "outofmana-corvo-crouched-armed-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-armed-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-armed-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 23.102493074792243,
      "video_sha256": "10111547C333BA2B33227ABD68B902D8759DA160AE8E70E41F83193E038C022C",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-fx-v48.mp4": {
      "video": "outofmana-corvo-standing-empty-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-fx-v48/result.json",
      "visual_review": "outofmana-corvo-standing-empty-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.5,
      "video_sha256": "D5FCC2D7FD32247AEFC078489015EA2EF1B5DF63FC18DF53D94509F9C42D7A19",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-standing-empty-fx-v48-source.mp4": {
      "video": "outofmana-corvo-standing-empty-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-standing-empty-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-standing-empty-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.25,
      "video_sha256": "2B7D1FB74E09941F987DA5FAE44F531B43951AB32BBC73CCE5F056D946F22440",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-fx-v48.mp4": {
      "video": "outofmana-corvo-crouched-empty-fx-v48.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-fx-v48",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-fx-v48/result.json",
      "visual_review": "outofmana-corvo-crouched-empty-fx-v48/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.166666666666668,
      "video_sha256": "F0EEDD86982A62D54FB60CBBFBCD19ACAD2FA9B4D119C66DD361DFE6D8B6477E",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "outofmana-corvo-crouched-empty-fx-v48-source.mp4": {
      "video": "outofmana-corvo-crouched-empty-fx-v48-source.mp4",
      "character": "corvo",
      "label": "Bend Time · outofmana-corvo-crouched-empty-fx-v48 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "outofmana-corvo-crouched-empty-fx-v48/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 24.333333333333332,
      "video_sha256": "BDE378B84B2BEEBCE20EB14593CE35E2DDC46A22BCCD1F1ABE866FD546490145",
      "checks_passed": true,
      "scope": "Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed. Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs. Original files unchanged."
    },
    "failure-fx-receiver-stop-v51.mp4": {
      "video": "failure-fx-receiver-stop-v51.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-stop-v51",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-stop-v51/result.json",
      "visual_review": "failure-fx-receiver-stop-v51/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.646076794657763,
      "video_sha256": "862D28466121E21E8FA85E4B3A1A2935FD633CFD5B68508C536C0FB8AE22254E",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-receiver-stop-v51-source.mp4": {
      "video": "failure-fx-receiver-stop-v51-source.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-stop-v51 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-stop-v51/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 26.1,
      "video_sha256": "704580F6BC709E502E54268AD47C76B5DFDCD87CB4A066740F85F7F214792511",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-source-resume-v51.mp4": {
      "video": "failure-fx-source-resume-v51.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-source-resume-v51",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-source-resume-v51/result.json",
      "visual_review": "failure-fx-source-resume-v51/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.15,
      "video_sha256": "16820A0FCCE5BE319D742804E4F2D57D4E6F7D20F94BE21EFDD9DDFDEE140047",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-source-resume-v51-source.mp4": {
      "video": "failure-fx-source-resume-v51-source.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-source-resume-v51 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-source-resume-v51/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 25.45,
      "video_sha256": "392173FFD5212BEF63BEB2F52B9BCCAC2929A379F7E2D2F273F373B47EF7FF6D",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-receiver-resume-v51.mp4": {
      "video": "failure-fx-receiver-resume-v51.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-resume-v51",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-resume-v51/result.json",
      "visual_review": "failure-fx-receiver-resume-v51/visual-review.json",
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 27.6,
      "video_sha256": "CBA0E3CCD59F85DC767B2B9F27AA23DF68581D78D48DE179A7AC0A2C5099CA19",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    },
    "failure-fx-receiver-resume-v51-source.mp4": {
      "video": "failure-fx-receiver-resume-v51-source.mp4",
      "character": "corvo",
      "label": "Bend Time · failure-fx-receiver-resume-v51 · source",
      "alias": "native_out_of_mana_standing_armed",
      "requirements": [
        "corvo_bend_time_cast_end",
        "power_interruptions_and_no_mana"
      ],
      "gameplay_bound": false,
      "complete": false,
      "visual_accepted": false,
      "evidence": "failure-fx-receiver-resume-v51/result.json",
      "visual_review": null,
      "proxy_sha256": "B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486",
      "bank_sha256": "6DE3B2439C63CD6C24B864751FD0AB4E44936EEB74DFECF31CC4DD2FC6DA1CFA",
      "actual_capture_hz": 26.056572379367722,
      "video_sha256": "81C4DEB60A69F80BAE6C3318E2141D526F3D0DAA3D06352763A1F6B37450DA46",
      "checks_passed": true,
      "scope": "Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open."
    }
  },
  "evidence": [
    "../movement-integration/visual-sweep-v1260/powers-review.json",
    "coverage-v1417a/result.json",
    "coverage-v1425c/result.json",
    "coverage-v1434a/result.json",
    "coverage-v1438a/result.json",
    "coverage-v1350a/result.json",
    "coverage-v1353a/result.json",
    "coverage-v1332d/result.json"
  ],
  "completion": {
    "audit": "completion-audit-v1439-power-interruptions.json",
    "decision": "complete",
    "scope": "Both roles: the native failed cast without mana under the selected power, an aim cut off by a knockdown hit, an aim cancelled with X, and a cast with no valid target, each presented on the partner and ending without a leftover power pose or effect.",
    "takes": [
      {
        "take": "coverage-v1417a",
        "cases": [
          "client-nomana3"
        ]
      },
      {
        "take": "coverage-v1425c",
        "cases": [
          "host-nomana4"
        ]
      },
      {
        "take": "coverage-v1434a",
        "cases": [
          "client-labpowerblast"
        ]
      },
      {
        "take": "coverage-v1438a",
        "cases": [
          "host-labpowerblast"
        ]
      },
      {
        "take": "coverage-v1350a",
        "cases": [
          "host-blinkcancel"
        ]
      },
      {
        "take": "coverage-v1353a",
        "cases": [
          "client-farreachcancel"
        ]
      },
      {
        "take": "coverage-v1332d",
        "cases": [
          "host-possession",
          "client-domino"
        ]
      }
    ],
    "visual": [
      {
        "video": "coverage-v1417a/client-nomana3-observer.mp4",
        "verdict": "Emily twists slightly and flicks her left hand at the hip, the native out-of-mana response, then settles."
      },
      {
        "video": "coverage-v1425c/host-nomana4-observer.mp4",
        "verdict": "Corvo flicks his left hand out at the hip with the native out-of-mana wisps curling around it, then the hand drops back."
      },
      {
        "video": "coverage-v1434a/client-labpowerblast-observer.mp4",
        "verdict": "Emily holds her left hand out aiming Far Reach; the blast throws her back toward the pillar and she recovers with no Far Reach effect left."
      },
      {
        "video": "coverage-v1438a/host-labpowerblast-observer.mp4",
        "verdict": "Corvo holds his left arm out aiming Blink; the blast throws him back against the pillar, he recovers standing, and no Blink follows."
      },
      {
        "video": "coverage-v1350a/host-blinkcancel-observer.mp4",
        "verdict": "Corvo throws his left arm out to aim, drops it again at the cancel and stays where he stands; a few seconds later he aims again and vanishes in a blue burst."
      },
      {
        "video": "coverage-v1353a/client-farreachcancel-observer.mp4",
        "verdict": "Emily reaches out with her left hand and holds it, lowers it again without moving when the aim is cancelled, then reaches once more and is pulled away."
      },
      {
        "video": "coverage-v1332d/host-possession-observer.mp4",
        "verdict": "Corvo raises his left arm up and out to aim Possession at nothing, holds it, then lowers it and stands; nothing is possessed."
      },
      {
        "video": "coverage-v1332d/client-domino-observer.mp4",
        "verdict": "Emily makes the Domino cast with no target: a short gesture of her left hand at her side and a shift of stance, then she settles (dark, small at this distance)."
      }
    ],
    "lifecycle": "See method."
  },
  "complete": true
}
````

- [evidence: ../movement-integration/visual-sweep-v1260/powers-review.json](<../../../../work/movement-integration/visual-sweep-v1260/powers-review.json>)
- [evidence: coverage-v1417a/result.json](<../../../../work/natural-moveset/coverage-v1417a/result.json>)
- [evidence: coverage-v1425c/result.json](<../../../../work/natural-moveset/coverage-v1425c/result.json>)
- [evidence: coverage-v1434a/result.json](<../../../../work/natural-moveset/coverage-v1434a/result.json>)
- [evidence: coverage-v1438a/result.json](<../../../../work/natural-moveset/coverage-v1438a/result.json>)
- [evidence: coverage-v1350a/result.json](<../../../../work/natural-moveset/coverage-v1350a/result.json>)
- [evidence: coverage-v1353a/result.json](<../../../../work/natural-moveset/coverage-v1353a/result.json>)
- [evidence: coverage-v1332d/result.json](<../../../../work/natural-moveset/coverage-v1332d/result.json>)
- [in_game_recordings: failure-fx-receiver-stop-v51.mp4](<../../../../work/natural-moveset/failure-fx-receiver-stop-v51.mp4>)
- [in_game_recordings: failure-fx-receiver-stop-v51-source.mp4](<../../../../work/natural-moveset/failure-fx-receiver-stop-v51-source.mp4>)
- [in_game_recordings: failure-fx-source-resume-v51.mp4](<../../../../work/natural-moveset/failure-fx-source-resume-v51.mp4>)
- [in_game_recordings: failure-fx-source-resume-v51-source.mp4](<../../../../work/natural-moveset/failure-fx-source-resume-v51-source.mp4>)
- [in_game_recordings: failure-fx-receiver-resume-v51.mp4](<../../../../work/natural-moveset/failure-fx-receiver-resume-v51.mp4>)
- [in_game_recordings: failure-fx-receiver-resume-v51-source.mp4](<../../../../work/natural-moveset/failure-fx-receiver-resume-v51-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-fx-v48.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-fx-v48.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-fx-v48-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-fx-v48-source.mp4>)
- [in_game_recordings: outofmana-hand-fx-corvo-v47.mp4](<../../../../work/natural-moveset/outofmana-hand-fx-corvo-v47.mp4>)
- [in_game_recordings: outofmana-hand-fx-emily-v47.mp4](<../../../../work/natural-moveset/outofmana-hand-fx-emily-v47.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v46.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v46-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-v46.mp4>)
- [in_game_recordings: outofmana-corvo-standing-empty-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-empty-v46-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-v46.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-armed-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-armed-v46-source.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-v46.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-v46.mp4>)
- [in_game_recordings: outofmana-corvo-crouched-empty-v46-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-crouched-empty-v46-source.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-crouched-armed-v45.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-crouched-armed-v45.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-standing-empty-v45.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-standing-empty-v45.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-crouched-empty-v45.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-crouched-empty-v45.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-front-v45.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-front-v45.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-front-v45-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-front-v45-source.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v45-v1.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v45-v1.mp4>)
- [in_game_recordings: outofmana-corvo-standing-armed-v45-v1-source.mp4](<../../../../work/natural-moveset/outofmana-corvo-standing-armed-v45-v1-source.mp4>)
- [in_game_recordings: bendtime-receiver-stop-v44.mp4](<../../../../work/natural-moveset/bendtime-receiver-stop-v44.mp4>)
- [in_game_recordings: bendtime-receiver-stop-v44-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-stop-v44-source.mp4>)
- [in_game_recordings: bendtime-source-pause-v44.mp4](<../../../../work/natural-moveset/bendtime-source-pause-v44.mp4>)
- [in_game_recordings: bendtime-source-pause-v44-source.mp4](<../../../../work/natural-moveset/bendtime-source-pause-v44-source.mp4>)
- [in_game_recordings: bendtime-receiver-pause-v44.mp4](<../../../../work/natural-moveset/bendtime-receiver-pause-v44.mp4>)
- [in_game_recordings: bendtime-receiver-pause-v44-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-pause-v44-source.mp4>)
- [in_game_recordings: bendtime-source-resume-v44.mp4](<../../../../work/natural-moveset/bendtime-source-resume-v44.mp4>)
- [in_game_recordings: bendtime-source-resume-v44-source.mp4](<../../../../work/natural-moveset/bendtime-source-resume-v44-source.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44-source.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44-v2.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44-v2.mp4>)
- [in_game_recordings: bendtime-receiver-resume-v44-v2-source.mp4](<../../../../work/natural-moveset/bendtime-receiver-resume-v44-v2-source.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-v44.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-v44.mp4>)
- [in_game_recordings: native-bendtime-corvo-no-mana-v44-v2.mp4](<../../../../work/natural-moveset/native-bendtime-corvo-no-mana-v44-v2.mp4>)

