# Complete functional movement requirements

Captured 2026-10-19T05:49:10.313219+02:00; source [review-catalog.json](<../../../work/natural-moveset/review-catalog.json>).

105 requirements: 93 complete (decided by a completion audit; see each item's `completion` record), 95 runtime-bound (fully or partially), 87 with accepted visuals, 2 not applicable or excluded by policy. Candidate material and recordings do not establish completion.

## Acceptance contract

````json
{
  "version": 1,
  "requested_delivery": "Complete Corvo/Emily moveset library ported to Dishonored 2, with actual in-game recordings in the review dashboard. Finish the movement phase before moving to other co-op work.",
  "scope": "Every applicable functional movement requirement in review-catalog.json, including transitions, contextual actions, equipment and powers.",
  "complete": false,
  "completion_requires": [
    "Complete-body motions look natural on each applicable native character rig, including hands, knees, calf helpers and attachments.",
    "Native gameplay states trigger the correct remote motion automatically through the native Steam connection.",
    "World position remains network controlled; foot cadence and trajectory agree with travel, contact and native gameplay timing.",
    "Entry, hold, completion, cancellation, interruption and recovery work, including relevant stance, weapon and direction variants.",
    "Native equipment, props, victim alignment and physics handoffs appear correctly where required.",
    "Powers include their native world VFX, spawned entities or transformed representation, with phase, target and cleanup synchronized.",
    "First-person-only targeting and screen effects remain scoped to their native viewer; remote presentation does not affect the other player's camera.",
    "Remote presentation cannot inflict duplicate damage or friendly fire, spend mana twice, or create duplicate gameplay entities.",
    "Native first-person animations, hands, camera and controls are preserved.",
    "Each applicable action and character has actual D2 recording evidence in the dashboard with the tested build and source data identified.",
    "Recordings preserve real elapsed timing; playback quality is inspected visually, not inferred from joint matrices or capture counts.",
    "Pause, death, checkpoint reload, map changes and peer loss retire animation/effect ownership cleanly and restore usable native state."
  ],
  "evidence_rules": [
    "An authoring preview or fixed-frame pose publication is not proof of gameplay integration.",
    "A native effect resource existing in the installation is not proof of remote effect playback or synchronization.",
    "Corvo/Emily comparison adaptations do not imply that both characters receive every power in the campaign.",
    "No requirement is marked finished solely because it has a donor clip or a numerical test."
  ],
  "power_cases": [
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
        "remaining": "Stance/movement/equipment/upgrades/dynamic targets, full lifecycle and final visual acceptance.",
        "latest_regression": {
          "recording": "blink-regression-v26-v2.mp4",
          "evidence": "blink-regression-v26-v2/result.json",
          "dll": "E8E73BE946D2DC0C7E9801B04E70A6ED264FDE7A8E7FCBFF794B75FF22A1C99A",
          "checks": [
            "Native Steam stream and v26 bank ready before the cast",
            "corvo native cast phases 1,2,3 drive remote whole-body presentation",
            "Native phase end restores standing movement",
            "Native X cancellation does not teleport or pull the source",
            "Both players/avatar health, source first-person sword, remote native sword and peer power quantity preserved",
            "Native Blink travel creates exactly one network departure and one arrival; aim cancellation creates neither",
            "Repeated reliable arrival packet cannot replay effects",
            "Remote Blink uses a discrete endpoint transfer with no eye-height body jump (max endpoint error 0.0136110005877573 m)"
          ]
        }
      }
    },
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
        "remaining": "Stance/movement/equipment/upgrades/dynamic targets, full lifecycle and final visual acceptance.",
        "latest_regression": {
          "recording": "grapple-regression-v26-v2.mp4",
          "evidence": "grapple-regression-v26-v2/result.json",
          "dll": "E8E73BE946D2DC0C7E9801B04E70A6ED264FDE7A8E7FCBFF794B75FF22A1C99A",
          "checks": [
            "Native Steam stream and v26 bank ready before the cast",
            "emily native cast phases 4,5,6,7 drive remote whole-body presentation",
            "Native phase end restores standing movement",
            "Native X cancellation does not teleport or pull the source",
            "Both players/avatar health, source first-person sword, remote native sword and peer power quantity preserved",
            "Native Far Reach cast creates exactly one remote native tether and one impact; aim cancellation creates neither",
            "Remote authored left wrist drives native tether endpoints; native end retires the owned beam",
            "Duplicate reliable Far Reach end packet cannot replay tether or impact"
          ]
        }
      }
    },
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
      "runtime_effects_verified": false,
      "partial_runtime_evidence": {
        "scope": "Both roles standing/crouched, drawn/holstered; stationary and forward/backward walking. Native cast clock, private reveal, recovery, equipment/health/cleanup.",
        "recordings": [
          "darkvision-corvo-v25-v2.mp4",
          "darkvision-emily-v25-v2.mp4"
        ],
        "natural_expiry": "native-darkvision-corvo-v4/result.json",
        "remaining": "Sideways/run/slide/water, moving and double jumps, upgrades, lifecycle and full continuous visual approval.",
        "context_recordings": [
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
          "darkvision-emily-crouched-empty-moving-v26-v1-source.mp4"
        ],
        "context_tests": [
          "darkvision-corvo-crouched-v26-v1/result.json",
          "darkvision-corvo-empty-v26-v1/result.json",
          "darkvision-corvo-crouched-empty-v26-v1/result.json",
          "darkvision-corvo-moving-v26-v3/result.json",
          "darkvision-corvo-crouched-moving-v26-v1/result.json",
          "darkvision-corvo-empty-moving-v26-v1/result.json",
          "darkvision-corvo-crouched-empty-moving-v26-v1/result.json",
          "darkvision-emily-crouched-v26-v1/result.json",
          "darkvision-emily-empty-v26-v1/result.json",
          "darkvision-emily-crouched-empty-v26-v2/result.json",
          "darkvision-emily-moving-v26-v1/result.json",
          "darkvision-emily-crouched-moving-v26-v1/result.json",
          "darkvision-emily-empty-moving-v26-v1/result.json",
          "darkvision-emily-crouched-empty-moving-v26-v1/result.json"
        ],
        "context_checks": 134,
        "airborne": {
          "recordings": [
            "darkvision-corvo-air-v26-v3.mp4",
            "darkvision-corvo-air-v26-v3-source.mp4",
            "darkvision-emily-air-v26-v2.mp4",
            "darkvision-emily-air-v26-v2-source.mp4",
            "darkvision-corvo-air-empty-v26-v1.mp4",
            "darkvision-corvo-air-empty-v26-v1-source.mp4",
            "darkvision-emily-air-empty-v26-v1.mp4",
            "darkvision-emily-air-empty-v26-v1-source.mp4"
          ],
          "tests": [
            "darkvision-corvo-air-v26-v3/result.json",
            "darkvision-emily-air-v26-v2/result.json",
            "darkvision-corvo-air-empty-v26-v1/result.json",
            "darkvision-emily-air-empty-v26-v1/result.json"
          ],
          "checks": 48,
          "scope": "Both roles, drawn and holstered, two ordinary jump casts each.",
          "dll": "48A0E8F493C17097455CECF5416AD1DDC095DFC02885FD0DCBC79C50EEB337FD"
        }
      }
    },
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
    },
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
    },
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
      "runtime_effects_verified": false,
      "partial_runtime_evidence": {
        "scope": "Corvo Windblast ground stance/equipment/travel body integration. Native source controller casts and scene timing drive the remote whole body through Steam. Observed basic native cast FX drives one separate world-space presentation effect on the remote clone, preserving native position/axis and body timing. Original private screen distortion is excluded. Replay, finite cleanup and health are checked. Impacts, world physics outcomes, upgrades and broader lifecycle are not certified; visual quality requires separate review.",
        "recordings": [
          "windblast-corvo-network-v28-v1.mp4",
          "windblast-corvo-network-v28-v1-source.mp4",
          "windblast-corvo-empty-network-v28-v1.mp4",
          "windblast-corvo-empty-network-v28-v1-source.mp4"
        ],
        "tests": [
          "windblast-corvo-network-v28-v1/result.json",
          "windblast-corvo-empty-network-v28-v1/result.json"
        ],
        "dll": "80E9E28879F84E427BF05BFEC45357430EE181376C9A1669CA28D4A5E9670F8A",
        "checks": [
          "Native Steam stream and exact v28 stance/equipment bank ready before the cast",
          "Two native cast events drive remote whole-body Windblast animation through Steam",
          "Both casts recover to original stance with network body ownership retained",
          "First-person and remote native sword retain the original drawn/holstered state",
          "Full-body cast retains original native crouch/standing stance throughout",
          "Exactly two uniquely identified native source effects each create one finite remote world effect through Steam",
          "Both clone-owned remote effects preserve the exact independently read native source origin and orientation",
          "Replayed reliable packet is received and rejected without a second effect",
          "Native cleanup restores original power quantity and selection",
          "Both players and remote avatars retain native health; no animation damage events",
          "Protected original executable, Steam API and save hashes unchanged"
        ],
        "remaining": [
          "Standing stationary basic Corvo Windblast only.",
          "Fine hand contact and continuous visual quality still require acceptance.",
          "Crouched, travelling, aerial, upgraded and impact variants remain incomplete.",
          "Remote physics/damage outcomes are outside this cosmetic effect proof.",
          "Recordings contain no audio; sound is not audibly verified."
        ],
        "airborne": {
          "recordings": [
            "windblast-corvo-air-network-v28-v1.mp4",
            "windblast-corvo-air-network-v28-v1-source.mp4",
            "windblast-corvo-air-empty-moving-v28-v1.mp4",
            "windblast-corvo-air-empty-moving-v28-v1-source.mp4"
          ],
          "tests": [
            "windblast-corvo-air-network-v28-v1/result.json",
            "windblast-corvo-air-empty-moving-v28-v1/result.json"
          ],
          "dll": "D662AA6718B0F42A643CA1B53D310B3B58C45671F248C937CE2E36974E29A2EB",
          "checks": 27,
          "remaining": [
            "Two ordinary jumping basic casts per take, with the explicitly recorded equipment/travel context.",
            "Double-jump casting, upgrades, physics impacts and full lifecycle remain unverified.",
            "Selected temporal frames inspected; complete continuous motion quality is not accepted.",
            "No recorded audio; sound is not audibly verified."
          ]
        }
      }
    },
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
    },
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
    },
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
    },
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
    },
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
      "runtime_effects_verified": false
    },
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
  "conditional_exclusions": {
    "incapacitation_and_rescue_if_supported": "Shared death/checkpoint recovery remains the requested policy; no partner-rescue mechanic is planned."
  }
}
````

## Requirements

| id | family | authoring | runtime binding | visual validation | phase |
|---|---|---|---|---|---|
| stand_idle | locomotion | candidates | partial_native_steam_batched_locomotion_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| armed_idle | locomotion | candidates | partial_native_steam_batched_locomotion_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| walk_8_directions | locomotion | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| run_8_directions | locomotion | candidates | isolated_Dunwall_both_roles | accepted_functional_both_roles | complete |
| sprint_forward_and_diagonals | locomotion | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| start_stop_each_gait | locomotion | candidates | partial_native_steam_batched_locomotion_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| gait_speed_changes | locomotion | candidates | partial_native_steam_batched_locomotion_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| turn_in_place_90_180 | locomotion | candidates | partial_native_steam_stance_turns_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| moving_turns | locomotion | candidates | partial_native_steam_batched_locomotion_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| reverse_and_direction_changes | locomotion | candidates | partial_native_steam_batched_locomotion_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| slopes_and_stairs | locomotion | candidates | partial_native_steam_dunwall_stairs_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouch_idle | stance | candidates | partial_native_manual_and_automatic_crouch_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouch_8_directions | stance | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| stand_to_crouch | stance | candidates | partial_native_manual_and_automatic_crouch_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouch_to_stand | stance | candidates | partial_native_manual_and_automatic_crouch_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouched_turns | stance | candidates | partial_native_steam_stance_turns_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouch_stop_start | stance | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| stand_lean_left_right | stance | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouch_lean_left_right | stance | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| ledge_peek | stance | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| keyhole_enter_look_exit | stance | candidates | partial_native_keyhole_phases_through_Steam | bounded_actual_game_recordings_inspected | complete |
| standing_jump_takeoff | traversal | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| running_jump_takeoff | traversal | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouch_jump_transition | traversal | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| double_jump | traversal | candidates | partial_native_double_jump_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| rising_airborne | traversal | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| fall_loop | traversal | candidates | isolated_Dunwall_both_roles | accepted_functional_both_roles_long_fall | complete |
| soft_landing | traversal | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| hard_landing | traversal | candidates | native_light_landing_standing_and_crouched_verified_dunwall | accepted_functional_both_roles_long_fall_landing | complete |
| moving_landing | traversal | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| vault_low_obstacle | traversal | candidates | partial_isolated_Dunwall_native_vault | accepted_sweep_1260_functional_both_roles | complete |
| climb_high_ledge | traversal | candidates | partial_isolated_Dunwall_native_high_mantle | accepted_sweep_1260_functional_both_roles | complete |
| ledge_hang_and_pull_up | traversal | candidates | partial_isolated_Dunwall_low_mantle | accepted_sweep_1260_functional_both_roles | complete |
| window_enter_exit | traversal | candidates | native_confined_low_mantle_through_climbable_eotw_window_both_roles | not_accepted | complete |
| slide_enter_loop_exit | traversal | candidates | isolated_Dunwall_both_roles | accepted_functional_both_roles | complete |
| slide_to_crouch_or_stand | traversal | candidates | isolated_Dunwall_both_roles | accepted_functional_both_roles | complete |
| ladder_up_down_entry_exit | traversal | candidates | partial_native_steam_chain_climb_both_roles_mission_map | bounded_actual_game_frames_inspected | complete |
| chain_rope_climb | traversal | candidates | blocked_native_fixture_missing_components | not_accepted | complete |
| swim_idle_directions | traversal | candidates | partial_native_steam_swimming_both_roles_mission_map | bounded_actual_game_frames_inspected | complete |
| dive_surface_water_exit | traversal | candidates | partial_native_steam_water_exit_both_roles_mission_map | bounded_actual_game_frames_inspected | complete |
| sword_draw_holster | combat | candidates | partial_native_steam_batched_locomotion_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| sword_ready_idle | combat | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| light_attack_variants | combat | candidates | partial_native_steam_target_bearing_standing_slash | accepted_sweep_1260_functional_both_roles | complete |
| heavy_attack_variants | combat | candidates | partial_native_steam_standing_and_crouched_combat | accepted_functional_both_roles_crouched_heavy_retake | complete |
| moving_attack | combat | candidates | partial_native_moving_combat_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crouched_attack | combat | candidates | partial_native_steam_standing_and_crouched_combat | accepted_sweep_1260_functional_both_roles | complete |
| airborne_attack | combat | candidates | partial_native_airborne_combat_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| block_enter_hold_exit | combat | candidates | isolated_Dunwall_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| parry_directions | combat | candidates | partial_native_steam_block_impact_and_timed_parry_both_roles | not_accepted | complete |
| parry_recovery | combat | candidates | partial_native_steam_block_impact_and_timed_parry_both_roles | not_accepted | complete |
| counterattack | combat | candidates | partial_native_counter_kill_finisher_shared_host_source | not_accepted | work_remaining |
| hit_reactions_directions | combat | candidates | native_network_standing_stun_partial_verified | accepted_sweep_1260_functional_both_roles | complete |
| stagger_knockdown_recovery | combat | candidates | not_verified | not_accepted | work_remaining |
| kick_or_shove_if_present | combat | candidates | partial_native_steam_player_kick_outcome_host_source_mission_map | not_accepted | work_remaining |
| death_variants | combat | candidates | partial_native_steam_npc_death_and_corpse_both_peers_mission_map | accepted_functional_both_roles | complete |
| ragdoll_handoff | combat | missing | partial_native_steam_npc_corpse_authority_mission_map | accepted_functional_both_roles | complete |
| pistol_draw_aim_fire_reload_holster | ranged_and_items | candidates | partial_native_steam_ranged_five_phase_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| crossbow_draw_aim_fire_reload_holster | ranged_and_items | candidates | partial_native_steam_ranged_five_phase_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| bolt_or_ammo_switch | ranged_and_items | candidates | not_verified | not_accepted | complete |
| grenade_equip_aim_throw | ranged_and_items | candidates | native_network_ground_grenade_partial_verified | accepted_functional_both_roles_bank_1266 | complete |
| mine_or_razor_place | ranged_and_items | candidates | partial_native_steam_trap_body_and_held_prop | accepted_sweep_1260_functional_both_roles | complete |
| heart_equip_use_holster | ranged_and_items | candidates | partial_native_steam_ground_heart_body_prop | accepted_sweep_1260_functional_both_roles | complete |
| health_elixir | ranged_and_items | candidates | native_health_elixir_drink_reach_presented_both_roles | accepted_functional_both_roles_hand_to_mouth | complete |
| mana_elixir | ranged_and_items | candidates | native_network_movement_during_mana_use_partial_verified | not_accepted | complete |
| item_switch_interruptions | ranged_and_items | candidates | partial_native_steam_wheel_interruption_and_quickslot_switch_both_roles_mission_map | bounded_actual_game_frames_inspected | complete |
| lethal_takedown_front_back_sides | paired_actions | candidates | partial_census_lethals_back_drop_slide_finishers_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| nonlethal_takedown_front_back_sides | paired_actions | candidates | partial_census_chokes_back_both_roles_sides_emily_front_corvo | accepted_sweep_1260_functional_both_roles | complete |
| crouched_takedown | paired_actions | candidates | native_crouched_rear_choke_and_fast_back_lethal_presented_both_roles | accepted_client_choke_client_lethal_host_choke_host_lethal | complete |
| aerial_drop_kill | paired_actions | candidates | native_drop_lethal_presented_and_shared_both_roles_product | accepted_functional_both_roles_fast_back_donor | complete |
| aerial_nonlethal_takedown | paired_actions | candidates | native_drop_choke_presented_and_knockout_shared_both_roles_product | accepted_functional_both_roles_choke_donor | complete |
| sliding_takedown | paired_actions | candidates | native_slide_choke_and_lethal_presented_and_shared_both_roles_product | accepted_functional_both_roles_choke_and_lethal | complete |
| wall_and_window_variants | paired_actions | candidates | not_verified | not_accepted | work_remaining |
| enemy_body_size_variants | paired_actions | candidates | not_verified | not_accepted | work_remaining |
| paired_victim_alignment | paired_actions | candidates | shared_victim_and_outcome_both_roles_choke_and_lethal_product | accepted_host_and_client_sourced_rear_choke_and_lethal_victims | complete |
| struggle_and_escape | paired_actions | candidates | partial_native_steam_rear_choke_actor_and_breakout | accepted_sweep_1260_functional_both_roles | work_remaining |
| carry_body_lift_idle_move_drop_throw | paired_actions | candidates | bounded_native_source_paired_pickup_both_roles | accepted_functional_both_roles_v38 | complete |
| corvo_blink_aim_cancel_release_arrive | powers | candidates | partial_native_Blink_body_and_world_FX | accepted_sweep_1260_functional_owner | complete |
| corvo_blink_pull_and_kick_variants | powers | candidates | not_verified | not_accepted | complete |
| corvo_dark_vision_cast_end | powers | candidates | partial_native_Dark_Vision_stance_equipment_and_travel_through_Steam | bounded_actual_game_frames_inspected | complete |
| corvo_possession_enter_loop_exit | powers | candidates | native_empty_target_entry_aim_remote_body_verified_target_pending | accepted_sweep_1260_functional_owner | complete |
| corvo_bend_time_cast_end | powers | candidates | partial_native_Bend_Time_stance_equipment_air_and_pause_through_Steam | accepted_sweep_1260_functional_owner | complete |
| corvo_windblast_cast | powers | candidates | partial_native_steam_windblast_body_and_finite_world_fx | accepted_sweep_1260_functional_owner | complete |
| corvo_devouring_swarm_cast | powers | candidates | native_swarm_grounded_contexts_incrementally_verified_world_pending | accepted_sweep_1260_functional_owner | complete |
| emily_far_reach_aim_cancel_release_arrive | powers | candidates | partial_native_Far_Reach_body_and_world_FX | accepted_sweep_1260_functional_owner | complete |
| emily_far_reach_pull_variants | powers | candidates | not_verified | not_accepted | work_remaining |
| emily_dark_vision_cast_end | powers | candidates | partial_native_Dark_Vision_stance_equipment_and_travel_through_Steam | bounded_actual_game_frames_inspected | complete |
| emily_domino_mark_link_cancel | powers | candidates | native_domino_standing_armed_body_and_mark_partial_verified | accepted_sweep_1260_functional_owner | complete |
| emily_doppelganger_cast | powers | candidates | grounded_armed_and_holstered_scoped | not_accepted | complete |
| emily_mesmerize_cast | powers | candidates | native_observed_mesmerize_body_and_cosmetic_world_contexts_partial_verified | accepted_sweep_1260_functional_owner | complete |
| emily_shadow_walk_enter_move_attack_exit | powers | candidates | partial_native_standing_sneaking_entry_crawl_exit_through_Steam | accepted_sweep_1260_functional_owner | complete |
| power_upgrade_variants | powers | candidates | not_verified | not_accepted | work_remaining |
| power_interruptions_and_no_mana | powers | candidates | partial_native_Corvo_Bend_Time_failure_and_same_world_pause | accepted_sweep_1260_functional_owner | work_remaining |
| door_open_close_locked | interactions | candidates | native_ordinary_door_continuous_manual_and_death_checkpoint_recovery | accepted_functional_both_roles_crouched_rise | complete |
| lever_button_crank_wheel | interactions | candidates | native_use_state75_real_mechanisms_generic_lever_crank_wheel_valve_both_roles | accepted_sweep_1260_functional_both_roles | complete |
| pickup_loot_steal | interactions | candidates | native_pickup_reach_presented_both_roles_product | accepted_functional_both_roles_generic_reach | complete |
| key_use | interactions | candidates | native_use_state75_real_lock_both_roles | accepted_functional_both_roles | complete |
| read_object_or_note | interactions | candidates | partial_native_note_visible_root_hold_and_resume_both_roles | accepted_functional_both_roles_held_reach_reading | complete |
| eat_drink | interactions | candidates | native_food_reach_presented_both_roles_product | accepted_functional_both_roles_generic_reach | complete |
| carry_object_place_throw | interactions | candidates | partial_native_hold_object_gesture_both_roles_product | accepted_functional_both_roles_held_reach | work_remaining |
| shop_dialogue_gestures | interactions | candidates | not_verified | not_accepted | complete |
| sit_stand | interactions | candidates | not_verified | not_accepted | not_applicable |
| scripted_scene_pose_handoff | interactions | missing | partial_native_scene_exit_blend_to_locomotion_measured_window_mantle_both_roles | accepted_functional_both_roles_use_scene_handback | complete |
| cutscene_enter_exit | interactions | missing | native_cinematic5_partner_retired_and_restored_both_roles | accepted_mayhew_scene_both_windows | complete |
| map_load_checkpoint_recovery | interactions | missing | partial_native_steam_map_load_and_checkpoint_restart_both_peers | accepted_functional_both_roles | complete |
| incapacitation_and_rescue_if_supported | interactions | missing | not_verified | not_accepted | excluded_by_policy |

## Full per-family records

- [locomotion: 11 requirements](<requirements/locomotion.md>)
- [stance: 10 requirements](<requirements/stance.md>)
- [traversal: 19 requirements](<requirements/traversal.md>)
- [combat: 16 requirements](<requirements/combat.md>)
- [ranged_and_items: 9 requirements](<requirements/ranged_and_items.md>)
- [paired_actions: 11 requirements](<requirements/paired_actions.md>)
- [powers: 16 requirements](<requirements/powers.md>)
- [interactions: 13 requirements](<requirements/interactions.md>)
