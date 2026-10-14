# State record: MovesetExpansion

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "scope": "Full remote moveset work in progress; first candidate bank covers armed idle + eight walking directions on both native rigs",
  "complete": false,
  "default_enabled": false,
  "nonrenewable_lease_seconds": 60,
  "coverage": "work/moveset-growth/coverage.json",
  "functional_items": 105,
  "source_aliases": 1942,
  "native_first_person_changed": false,
  "network": "Existing native Steam root and action streams",
  "banks": [
    {
      "role": "corvo",
      "sha256": "4d4f17ecf482f4b3b3f3c04900b83e9aca8323a3850b0f0b8cbf5b4cd1db42a5",
      "layout_hash": 61698903717286687,
      "bytes": 6556028,
      "joints": 215
    },
    {
      "role": "emily",
      "sha256": "904f6b9080dd0453e4979d05f2c206b7853b9b16b9900c493a296cd836ef8d3c",
      "layout_hash": 9573391624217783168,
      "bytes": 6677996,
      "joints": 219
    }
  ],
  "tests": {
    "direction_checks": 18,
    "priority_checks": 13,
    "bank_validation": "Both actual banks plus malformed inputs and phase math; full existing build regression suite passed"
  },
  "videos": [
    "work/moveset-growth/corvo-walking.mp4",
    "work/moveset-growth/emily-walking.mp4"
  ],
  "turn_feedback": [
    {
      "name": "turn_left_180",
      "requested_unreal_yaw": -180,
      "actual_unreal_yaw": 179.9961749019481,
      "wrapped_error_degrees": 0.0038250980519478617,
      "d2_playback": false
    },
    {
      "name": "turn_left_90",
      "requested_unreal_yaw": -90,
      "actual_unreal_yaw": -90.01385349840155,
      "wrapped_error_degrees": 0.013853498401545039,
      "d2_playback": false
    },
    {
      "name": "turn_right_180",
      "requested_unreal_yaw": 180,
      "actual_unreal_yaw": -179.709512978004,
      "wrapped_error_degrees": 0.2904870219960003,
      "d2_playback": false
    },
    {
      "name": "turn_right_90",
      "requested_unreal_yaw": 90,
      "actual_unreal_yaw": 90.1306230047333,
      "wrapped_error_degrees": 0.1306230047333088,
      "d2_playback": false
    }
  ],
  "turn_candidates": {
    "retargeted_roles": [
      "corvo",
      "emily"
    ],
    "directions": [
      90,
      -90,
      180,
      -180
    ],
    "deployed": false,
    "blocker": "Foot/knee continuity under root-following turns; source-timed right90 promising, remaining candidates fail continuity"
  },
  "native_fullbody_source_candidates": "work/moveset-growth/native-fullbody-candidates.json",
  "remaining": "Dedicated transitions/foot polish, running/sprinting sources, turn runtime, crouch/lean, traversal, combat variants, ranged/items, paired actions, powers and interactions",
  "previous_milestone": "work/animgen-feasibility/milestone-092E",
  "ghidra_note": "Dishonored2.exe 0x140540a90 post comment saved"
}
````
