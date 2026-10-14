# State record: NaturalFullBodyMoveset

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "at": "2026-10-10T17:52:04.450299+02:00",
  "complete_moveset": false,
  "functional_requirements": 105,
  "requirements_with_candidates": 100,
  "paired_role_recordings": 482,
  "actual_game_recordings": [
    {
      "video": "native-corvo-1.mp4",
      "character": "corvo",
      "actual_capture_hz": 10.824064107164215,
      "container_fps": 30,
      "timing": "Actual capture intervals, no speedup",
      "scope": "Native D2 pose review. World-root translation and rotation intentionally remain controlled by the local player; clips are not gameplay bound."
    },
    {
      "video": "native-emily-1.mp4",
      "character": "emily",
      "actual_capture_hz": 10.925025511735399,
      "container_fps": 30,
      "timing": "Actual capture intervals, no speedup",
      "scope": "Native D2 pose review. World-root translation and rotation intentionally remain controlled by the local player; clips are not gameplay bound."
    },
    {
      "video": "gait-emily-video-1.mp4",
      "character": "emily",
      "alias": null,
      "actual_capture_hz": 8.392101551480959,
      "container_fps": 30,
      "timing": "Actual capture intervals, no speedup",
      "scope": "Actual player inputs, native Steam root stream, authored native walk/run and directional blending. Phase advances with travelled distance."
    },
    {
      "video": "stance-emily-video-2.mp4",
      "character": "emily",
      "alias": "crouch_sequence",
      "actual_capture_hz": 7.591733445803459,
      "container_fps": 30,
      "timing": "Actual capture intervals, no speedup",
      "scope": "Actual D2 pose-only sequence on the native remote character. Actor root stays still; this recording evaluates body, native correctives and weapon attachment, not travelling foot contact."
    },
    {
      "video": "powers-emily-video-1.mp4",
      "character": "emily",
      "alias": "power_cast_showcase",
      "actual_capture_hz": 9.925990422289944,
      "container_fps": 30,
      "timing": "Actual capture intervals, no speedup",
      "scope": "Actual D2 pose-only sequence on the native remote character. Actor root stays still; this recording evaluates body, native correctives and weapon attachment, not travelling foot contact.",
      "chapters": [
        {
          "name": "windblast",
          "seconds": 0.424,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "doppelganger",
          "seconds": 2.175,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "mesmerize",
          "seconds": 5.19,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "bend time",
          "seconds": 7.992,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "devouring swarm",
          "seconds": 9.434,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "dark vision",
          "seconds": 12.176,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "blink",
          "seconds": 13.711,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "possession",
          "seconds": 14.958,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "far reach",
          "seconds": 16.45,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        },
        {
          "name": "domino",
          "seconds": 18.436,
          "timing_precision": "Approximate; 60ms telemetry cadence"
        }
      ],
      "sequence_completed": true,
      "clip_sha256": "80ee6d0e523b3921a5a1d92769e4904a805a9d81c3e60dacc285bc81b6923f72"
    },
    {
      "video": "blink-assault-emily-video-1.mp4",
      "character": "emily",
      "alias": "blink_assault_kick_sequence",
      "actual_capture_hz": 6.431459022418228,
      "container_fps": 30,
      "timing": "Actual capture intervals, no speedup",
      "scope": "Actual D2 pose-only sequence on the native remote character. Actor root stays still; this recording evaluates body, native correctives and weapon attachment, not travelling foot contact."
    },
    {
      "video": "trap-wall-emily-video-1.mp4",
      "character": "emily",
      "alias": "springrazor_wall_place_sequence",
      "actual_capture_hz": 7.056936647955093,
      "container_fps": 30,
      "timing": "Actual capture intervals, no speedup",
      "scope": "Actual D2 pose-only sequence on the native remote character. Actor root stays still; this recording evaluates body, native correctives and weapon attachment, not travelling foot contact."
    },
    {
      "video": "integrated-stance-corvo-v1.mp4",
      "character": "corvo",
      "label": "Corvo · Live walking, crouching and recovery",
      "actual_capture_hz": 29.710526315789473,
      "container_fps": "1129/38",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Actual player inputs and native Steam movement drive the candidate whole-body standing/crouched bank. Native attack, block and airborne presentation take priority. Effects, other actions and complete moveset integration remain pending.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.122,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk forward",
          "seconds": 2.159,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk backward",
          "seconds": 3.54,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk left",
          "seconds": 4.923,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk right",
          "seconds": 6.055,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand to crouch",
          "seconds": 7.532,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched idle",
          "seconds": 9.319,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 10.561,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward",
          "seconds": 11.929,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch left",
          "seconds": 13.363,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch right",
          "seconds": 14.765,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward left",
          "seconds": 16.136,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward right",
          "seconds": 17.474,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward right",
          "seconds": 18.748,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward left",
          "seconds": 20.038,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch stop",
          "seconds": 21.545,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch to stand",
          "seconds": 22.728,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native block takes priority",
          "seconds": 24.655,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sword attack takes priority",
          "seconds": 26.331,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native jump takes priority",
          "seconds": 28.349,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 30.461,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run forward",
          "seconds": 31.736,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run backward",
          "seconds": 33.035,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final idle",
          "seconds": 34.65,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "F94E4F1DE4ACFB7553DE184344011BE421287FEBBAB3DFBBE457F8F7BCBAE964",
      "bank_sha256": "C5926EA6EF7B26E23576F5AB69BB01A8A532088E6FD840AB0B3E1BB52817628C",
      "sha256": "66dffeb92da12ad980a00c7885964d3e579e03e1ad58b3e93924bc38ff47cc1a",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "1129/38",
        "duration": "38.000000",
        "nb_frames": "1129"
      },
      "evidence": "integrated-stance-corvo-v1/take.json",
      "requirements": [
        "armed_idle",
        "walk_8_directions",
        "run_8_directions",
        "crouch_idle",
        "crouch_8_directions",
        "stand_to_crouch",
        "crouch_to_stand",
        "crouch_stop_start",
        "reverse_and_direction_changes",
        "gait_speed_changes"
      ],
      "tests": [
        "stance-bank-v1-test-2/result.json",
        "stance-bank-v1-priority/result.json"
      ],
      "limitations": [
        "Partial coverage per linked requirement; the video is not acceptance of every variant.",
        "Dark Dunwall room; close-up weapon grip, terrain contacts and arbitrary interruptions need further review.",
        "Attack, block and jump show existing native presentation priority, not completion of those action families.",
        "Persistent test opt-in retires on pause/load; automatic session lifecycle integration remains pending."
      ]
    },
    {
      "video": "integrated-stance-emily-v1.mp4",
      "character": "emily",
      "label": "Emily · Live walking, crouching and recovery",
      "actual_capture_hz": 29.68421052631579,
      "container_fps": "564/19",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Actual player inputs and native Steam movement drive the candidate whole-body standing/crouched bank. Native attack, block and airborne presentation take priority. Effects, other actions and complete moveset integration remain pending.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.123,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk forward",
          "seconds": 2.161,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk backward",
          "seconds": 3.522,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk left",
          "seconds": 4.929,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Walk right",
          "seconds": 6.063,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand to crouch",
          "seconds": 7.553,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched idle",
          "seconds": 9.34,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 10.534,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward",
          "seconds": 11.951,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch left",
          "seconds": 13.363,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch right",
          "seconds": 14.747,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward left",
          "seconds": 16.131,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward right",
          "seconds": 17.423,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward right",
          "seconds": 18.723,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward left",
          "seconds": 20.052,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch stop",
          "seconds": 21.56,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch to stand",
          "seconds": 22.772,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native block takes priority",
          "seconds": 24.632,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sword attack takes priority",
          "seconds": 26.345,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native jump takes priority",
          "seconds": 28.38,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 30.444,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run forward",
          "seconds": 31.734,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run backward",
          "seconds": 33.022,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final idle",
          "seconds": 34.628,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "F94E4F1DE4ACFB7553DE184344011BE421287FEBBAB3DFBBE457F8F7BCBAE964",
      "bank_sha256": "316EAA92A2B85AE856AC561DD64F304CD3F50A57DAD4CECB5C97ADDCD95A8B20",
      "sha256": "92a3e0d23f434a883db8d2c4ead146ae363d0106f5b74c0d9efd62f6035ffe9b",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "564/19",
        "duration": "38.000000",
        "nb_frames": "1128"
      },
      "evidence": "integrated-stance-emily-v1/take.json",
      "requirements": [
        "armed_idle",
        "walk_8_directions",
        "run_8_directions",
        "crouch_idle",
        "crouch_8_directions",
        "stand_to_crouch",
        "crouch_to_stand",
        "crouch_stop_start",
        "reverse_and_direction_changes",
        "gait_speed_changes"
      ],
      "tests": [
        "stance-bank-v1-test-2/result.json",
        "stance-bank-v1-priority/result.json"
      ],
      "limitations": [
        "Partial coverage per linked requirement; the video is not acceptance of every variant.",
        "Dark Dunwall room; close-up weapon grip, terrain contacts and arbitrary interruptions need further review.",
        "Attack, block and jump show existing native presentation priority, not completion of those action families.",
        "Persistent test opt-in retires on pause/load; automatic session lifecycle integration remains pending."
      ]
    },
    {
      "video": "integrated-lean-corvo-v1.mp4",
      "character": "corvo",
      "label": "Corvo · Live standing/crouched lean and recovery",
      "actual_capture_hz": 27.657894736842106,
      "container_fps": "1051/38",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Exact native Leaning state and read-only horizontal lean amplitude travel over Steam and select authored whole-body balance shifts. Native movement releases the lean. Local first-person camera and controls remain untouched.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.122,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native standing lean left",
          "seconds": 2.656,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits left lean",
          "seconds": 5.643,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Return forward",
          "seconds": 7.136,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native standing lean right",
          "seconds": 9.141,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits right lean",
          "seconds": 12.157,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Return forward",
          "seconds": 13.635,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand to crouch",
          "seconds": 15.638,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native crouched lean left",
          "seconds": 18.145,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits crouched left lean",
          "seconds": 21.155,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 22.662,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native crouched lean right",
          "seconds": 24.635,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits crouched right lean",
          "seconds": 27.659,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 29.126,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch to stand",
          "seconds": 31.136,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 34.636,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "6C4FEEC151EFA5527352522864791281A83C4D5B4323AEB0E4EBD1FEB2F8CEC7",
      "bank_sha256": "AD45A693D3F4D81AB587ECC05A429889F96FC4D3B7AB0599D4B9AEF44FB3D45A",
      "sha256": "00c1e8449df323d632aaa2d6d83d8048e67aa92cffee8d5ff76536c024b2ca57",
      "capture": {
        "width": 800,
        "height": 540,
        "avg_frame_rate": "1051/38",
        "duration": "38.000000",
        "nb_frames": "1051"
      },
      "evidence": "integrated-lean-corvo-v1/take.json",
      "requirements": [
        "stand_lean_left_right",
        "crouch_lean_left_right"
      ],
      "tests": [
        "lean-bank-v1-test/result.json"
      ],
      "limitations": [
        "Partial Dunwall coverage, no general visual acceptance.",
        "Vertical peeking, collision-limited lean, sword interruptions and simultaneous direction reversals need further runtime coverage.",
        "Dark room limits close-up hand and equipment inspection."
      ]
    },
    {
      "video": "integrated-lean-emily-v1.mp4",
      "character": "emily",
      "label": "Emily · Live standing/crouched lean and recovery",
      "actual_capture_hz": 27.973684210526315,
      "container_fps": "1063/38",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Exact native Leaning state and read-only horizontal lean amplitude travel over Steam and select authored whole-body balance shifts. Native movement releases the lean. Local first-person camera and controls remain untouched.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.111,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native standing lean left",
          "seconds": 2.651,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits left lean",
          "seconds": 5.616,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Return forward",
          "seconds": 7.129,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native standing lean right",
          "seconds": 9.112,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits right lean",
          "seconds": 12.147,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Return forward",
          "seconds": 13.657,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand to crouch",
          "seconds": 15.619,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native crouched lean left",
          "seconds": 18.124,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits crouched left lean",
          "seconds": 21.135,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 22.641,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native crouched lean right",
          "seconds": 24.619,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Movement exits crouched right lean",
          "seconds": 27.64,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 29.13,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch to stand",
          "seconds": 31.112,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 34.654,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "6C4FEEC151EFA5527352522864791281A83C4D5B4323AEB0E4EBD1FEB2F8CEC7",
      "bank_sha256": "1DDE5C8A0D1793597C3C9C85039ABD13D1029CB1D24E214458F5F2C05E6245DC",
      "sha256": "acb18111c8350aa6e0b83b0be224a3cdc58aec100925f2338b419555e12fe301",
      "capture": {
        "width": 800,
        "height": 540,
        "avg_frame_rate": "1063/38",
        "duration": "38.000000",
        "nb_frames": "1063"
      },
      "evidence": "integrated-lean-emily-v1/take.json",
      "requirements": [
        "stand_lean_left_right",
        "crouch_lean_left_right"
      ],
      "tests": [
        "lean-bank-v1-test/result.json"
      ],
      "limitations": [
        "Partial Dunwall coverage, no general visual acceptance.",
        "Vertical peeking, collision-limited lean, sword interruptions and simultaneous direction reversals need further runtime coverage.",
        "Dark room limits close-up hand and equipment inspection."
      ]
    },
    {
      "video": "integrated-sprint-corvo-v2.mp4",
      "character": "corvo",
      "label": "Corvo · Live sprinting, crouching and recovery",
      "actual_capture_hz": 27.736842105263158,
      "container_fps": "527/19",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Actual native sprint/standing/crouch inputs and Steam semantic state select the whole-body movement bank. Steps follow network-controlled travel. Root timestamps and gradual buffer recovery prevent arrival bursts from compressing movement time. Native block/slash presentation takes priority. Complete movement, terrain, equipment and effects remain unfinished.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.125,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sprint forward",
          "seconds": 2.664,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run backward recovery",
          "seconds": 5.136,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing stop",
          "seconds": 7.623,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sprint diagonal left",
          "seconds": 9.666,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run back right recovery",
          "seconds": 12.153,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing stop",
          "seconds": 14.656,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sprint diagonal right",
          "seconds": 16.646,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run back left recovery",
          "seconds": 19.165,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand to crouch",
          "seconds": 21.663,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 23.624,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward",
          "seconds": 25.667,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch to stand",
          "seconds": 28.161,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native block priority",
          "seconds": 30.639,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native slash priority",
          "seconds": 32.629,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 34.623,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "6C4FEEC151EFA5527352522864791281A83C4D5B4323AEB0E4EBD1FEB2F8CEC7",
      "bank_sha256": "AD45A693D3F4D81AB587ECC05A429889F96FC4D3B7AB0599D4B9AEF44FB3D45A",
      "sha256": "70b89338277a33d04e362bd484651fbb17200f017b64be755e17c870c85eb7ba",
      "capture": {
        "width": 800,
        "height": 540,
        "avg_frame_rate": "527/19",
        "duration": "38.000000",
        "nb_frames": "1054"
      },
      "evidence": "integrated-sprint-corvo-v2/take.json",
      "requirements": [
        "sprint_forward_and_diagonals",
        "start_stop_each_gait",
        "gait_speed_changes",
        "reverse_and_direction_changes",
        "stand_to_crouch",
        "crouch_to_stand",
        "crouch_stop_start"
      ],
      "tests": [
        "sprint-bank-v1-underrun/result.json",
        "stance-bank-v4-regression-2/result.json",
        "stance-bank-v4-priority/result.json",
        "lean-bank-v1-test/result.json"
      ],
      "limitations": [
        "Partial coverage per linked requirement; the video is not acceptance of every variant.",
        "Dark Dunwall room; close-up weapon grip, terrain contacts and arbitrary interruptions need further review.",
        "Attack, block and jump show existing native presentation priority, not completion of those action families.",
        "Persistent test opt-in retires on pause/load; automatic session lifecycle integration remains pending.",
        "Wide view retains sprint travel; initial body is near the right edge and local sword. Later crouch is partly occluded by native furniture."
      ]
    },
    {
      "video": "integrated-sprint-emily-v2.mp4",
      "character": "emily",
      "label": "Emily · Live sprinting, crouching and recovery",
      "actual_capture_hz": 28.13157894736842,
      "container_fps": "1069/38",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Actual native sprint/standing/crouch inputs and Steam semantic state select the whole-body movement bank. Steps follow network-controlled travel. Root timestamps and gradual buffer recovery prevent arrival bursts from compressing movement time. Native block/slash presentation takes priority. Complete movement, terrain, equipment and effects remain unfinished.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.116,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sprint forward",
          "seconds": 2.644,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run backward recovery",
          "seconds": 5.131,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing stop",
          "seconds": 7.665,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sprint diagonal left",
          "seconds": 9.666,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run back right recovery",
          "seconds": 12.14,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing stop",
          "seconds": 14.634,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native sprint diagonal right",
          "seconds": 16.626,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Run back left recovery",
          "seconds": 19.163,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand to crouch",
          "seconds": 21.657,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch forward",
          "seconds": 23.631,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch backward",
          "seconds": 25.619,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch to stand",
          "seconds": 28.148,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native block priority",
          "seconds": 30.637,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native slash priority",
          "seconds": 32.623,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 34.644,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "6C4FEEC151EFA5527352522864791281A83C4D5B4323AEB0E4EBD1FEB2F8CEC7",
      "bank_sha256": "1DDE5C8A0D1793597C3C9C85039ABD13D1029CB1D24E214458F5F2C05E6245DC",
      "sha256": "daa6502de951aa0463b0599127b50854d3efe31c0872da819fe7e935781449f1",
      "capture": {
        "width": 800,
        "height": 540,
        "avg_frame_rate": "1069/38",
        "duration": "38.000000",
        "nb_frames": "1069"
      },
      "evidence": "integrated-sprint-emily-v2/take.json",
      "requirements": [
        "sprint_forward_and_diagonals",
        "start_stop_each_gait",
        "gait_speed_changes",
        "reverse_and_direction_changes",
        "stand_to_crouch",
        "crouch_to_stand",
        "crouch_stop_start"
      ],
      "tests": [
        "sprint-bank-v1-underrun/result.json",
        "stance-bank-v4-regression-2/result.json",
        "stance-bank-v4-priority/result.json",
        "lean-bank-v1-test/result.json"
      ],
      "limitations": [
        "Partial coverage per linked requirement; the video is not acceptance of every variant.",
        "Dark Dunwall room; close-up weapon grip, terrain contacts and arbitrary interruptions need further review.",
        "Attack, block and jump show existing native presentation priority, not completion of those action families.",
        "Persistent test opt-in retires on pause/load; automatic session lifecycle integration remains pending.",
        "Wide view retains sprint travel; initial body is near the right edge and local sword. Later crouch is partly occluded by native furniture."
      ]
    },
    {
      "video": "integrated-air-corvo-v5.mp4",
      "character": "corvo",
      "label": "Corvo · Standing, held, crouched and moving jumps",
      "actual_capture_hz": 29.615384615384617,
      "container_fps": "385/13",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Read-only native Jumping/Falling states and native start/apex heights control the remote full-body air bank. Rising pose follows replicated root height. Native ground contact starts authored standing/running/crouched landing recovery; local first-person physics and controls remain unchanged.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.127,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing jump",
          "seconds": 2.654,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Held jump",
          "seconds": 6.148,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch",
          "seconds": 10.166,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched jump and landing",
          "seconds": 12.134,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 16.127,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Move into jump",
          "seconds": 18.131,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Moving jump",
          "seconds": 18.427,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Landing recovery",
          "seconds": 22.125,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final idle",
          "seconds": 24.148,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "A38195E8EB05E6F8BB6153289B6BDCE6F29488142D5D8B7A58CBD04DF4075B70",
      "bank_sha256": "3FAC85F7CF38B8F8DAF40281EF09FC3859E65ED7C4DB4732BDD73C906FC54A59",
      "sha256": "8bff2a3b0263cb30a5995a2ad25361f3c4ca6d67b53250b54f0becc37fee7100",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "385/13",
        "duration": "26.000000",
        "nb_frames": "770"
      },
      "evidence": "integrated-air-corvo-v5/take.json",
      "requirements": [
        "standing_jump_takeoff",
        "running_jump_takeoff",
        "crouch_jump_transition",
        "rising_airborne",
        "fall_loop",
        "soft_landing",
        "moving_landing"
      ],
      "tests": [
        "air-bank-v4-semantic/result.json",
        "air-bank-v2-priority/result.json"
      ],
      "limitations": [
        "Ordinary Dunwall jumps only; no complete family or visual acceptance.",
        "Double jump, high falls, hard landings, slopes, airborne attacks and arbitrary interruptions remain unverified.",
        "Moving takeoff uses observed speed blend; a short walking start does not certify maximum-speed running jumps."
      ]
    },
    {
      "video": "integrated-air-emily-v5.mp4",
      "character": "emily",
      "label": "Emily · Standing, held, crouched and moving jumps",
      "actual_capture_hz": 29.653846153846153,
      "container_fps": "771/26",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": true,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Read-only native Jumping/Falling states and native start/apex heights control the remote full-body air bank. Rising pose follows replicated root height. Native ground contact starts authored standing/running/crouched landing recovery; local first-person physics and controls remain unchanged.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.131,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing jump",
          "seconds": 2.641,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Held jump",
          "seconds": 6.161,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch",
          "seconds": 10.148,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched jump and landing",
          "seconds": 12.178,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing recovery",
          "seconds": 16.166,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Move into jump",
          "seconds": 18.141,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Moving jump",
          "seconds": 18.452,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Landing recovery",
          "seconds": 22.137,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final idle",
          "seconds": 24.177,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "A38195E8EB05E6F8BB6153289B6BDCE6F29488142D5D8B7A58CBD04DF4075B70",
      "bank_sha256": "CA7B5A3EE7FE4E12BAA5053B47CE6E5268D9E918E93D58245A9D9316B4B75FFD",
      "sha256": "4fb7c4f702e78f69a488d39b9096f220730b9dc7e3875fe83db476b0feb1756f",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "771/26",
        "duration": "26.000000",
        "nb_frames": "771"
      },
      "evidence": "integrated-air-emily-v5/take.json",
      "requirements": [
        "standing_jump_takeoff",
        "running_jump_takeoff",
        "crouch_jump_transition",
        "rising_airborne",
        "fall_loop",
        "soft_landing",
        "moving_landing"
      ],
      "tests": [
        "air-bank-v4-semantic/result.json",
        "air-bank-v2-priority/result.json"
      ],
      "limitations": [
        "Ordinary Dunwall jumps only; no complete family or visual acceptance.",
        "Double jump, high falls, hard landings, slopes, airborne attacks and arbitrary interruptions remain unverified.",
        "Moving takeoff uses observed speed blend; a short walking start does not certify maximum-speed running jumps."
      ]
    },
    {
      "video": "integrated-combat-corvo-v1.mp4",
      "character": "corvo",
      "label": "Corvo · Native-timed sword combos, heavy attack and guards",
      "actual_capture_hz": 29.612903225806452,
      "container_fps": "918/31",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": false,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Actual native sword scene semantics over Steam drive authored standing/crouched body attacks and guards. Root placement and local first-person controls remain native. Visual acceptance, hits, air combat and other movement families remain pending.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.135,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native short slash",
          "seconds": 2.453,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Combo first slash",
          "seconds": 5.359,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Combo second slash",
          "seconds": 5.889,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Combo third slash",
          "seconds": 6.477,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Heavy windup, hold and release",
          "seconds": 8.963,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing guard and release",
          "seconds": 12.461,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch",
          "seconds": 16.266,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched slash",
          "seconds": 18.458,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched guard and release",
          "seconds": 21.566,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Return to standing",
          "seconds": 25.466,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final armed idle",
          "seconds": 28.476,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "D3EF81227796790A8373903E189BC2833E44CA469A82FC1F6402FF9BFEEC6452",
      "bank_sha256": "FB7A204B273EBD3A52150A0003A19BCB307E65FF91BD4207F2A1176C32130B1B",
      "sha256": "6bfb7f22c0f8e8ebb7ca64e48a28041d6e8686594a44e863b2a200416ec5b8fd",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "918/31",
        "duration": "31.000000",
        "nb_frames": "918"
      },
      "evidence": "integrated-combat-corvo-v1/take.json",
      "requirements": [
        "light_attack_variants",
        "heavy_attack_variants",
        "crouched_attack",
        "block_enter_hold_exit"
      ],
      "tests": [
        "combat-bank-v1-test/result.json"
      ],
      "limitations": [
        "Both-role isolated native-input and Steam timing checks pass; no complete combat-family or visual acceptance.",
        "Short/alternating slashes, one heavy windup/hold/release, one standing/crouched guard quadrant. Other directional parries, counters, victim contacts and hit reactions are unfinished.",
        "Airborne combat and moving-foot contact remain unfinished. Heavy hold currently retains a static authored windup pose.",
        "Complete body remains visible, but this camera distance does not establish fine grip/contact acceptance."
      ]
    },
    {
      "video": "integrated-combat-emily-v1.mp4",
      "character": "emily",
      "label": "Emily · Native-timed sword combos, heavy attack and guards",
      "actual_capture_hz": 29.677419354838708,
      "container_fps": "920/31",
      "timing": "Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.",
      "kind": "game",
      "travelling": false,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Actual native sword scene semantics over Steam drive authored standing/crouched body attacks and guards. Root placement and local first-person controls remain native. Visual acceptance, hits, air combat and other movement families remain pending.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.123,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Native short slash",
          "seconds": 2.444,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Combo first slash",
          "seconds": 5.365,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Combo second slash",
          "seconds": 5.895,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Combo third slash",
          "seconds": 6.422,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Heavy windup, hold and release",
          "seconds": 8.936,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Standing guard and release",
          "seconds": 12.433,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch",
          "seconds": 16.239,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched slash",
          "seconds": 18.466,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched guard and release",
          "seconds": 21.547,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Return to standing",
          "seconds": 25.443,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final armed idle",
          "seconds": 28.456,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "D3EF81227796790A8373903E189BC2833E44CA469A82FC1F6402FF9BFEEC6452",
      "bank_sha256": "DB17268E8F3FC072AF3E00AFADCD20D8065F6F5AB4575AAA7B31D0BE04FA37C8",
      "sha256": "4cde14132b064643b713e77b36918716a2cb335cc63bedb4f35dfd566de7c011",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "920/31",
        "duration": "31.000000",
        "nb_frames": "920"
      },
      "evidence": "integrated-combat-emily-v1/take.json",
      "requirements": [
        "light_attack_variants",
        "heavy_attack_variants",
        "crouched_attack",
        "block_enter_hold_exit"
      ],
      "tests": [
        "combat-bank-v1-test/result.json"
      ],
      "limitations": [
        "Both-role isolated native-input and Steam timing checks pass; no complete combat-family or visual acceptance.",
        "Short/alternating slashes, one heavy windup/hold/release, one standing/crouched guard quadrant. Other directional parries, counters, victim contacts and hit reactions are unfinished.",
        "Airborne combat and moving-foot contact remain unfinished. Heavy hold currently retains a static authored windup pose.",
        "Complete body remains visible, but this camera distance does not establish fine grip/contact acceptance."
      ]
    },
    {
      "video": "integrated-equipment-close-corvo-v3.mp4",
      "character": "corvo",
      "label": "Corvo · Native sword draw/holster, standing and crouched",
      "actual_capture_hz": 29.633333333333333,
      "container_fps": "889/30",
      "timing": "Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.",
      "kind": "game",
      "travelling": false,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Native first-person inputs and exact owned equipment scenes drive whole-body draw/holster and body-relative native clone sword handoff over Steam.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.122,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Holster sword",
          "seconds": 2.373,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 5.462,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 6.925,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Draw sword at body-relative hand contact",
          "seconds": 8.724,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch",
          "seconds": 11.631,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched holster",
          "seconds": 13.739,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 16.843,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 18.225,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched draw",
          "seconds": 20.138,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand",
          "seconds": 23.061,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Guard after equipment change",
          "seconds": 25.45,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final recovery",
          "seconds": 27.947,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "0E32032996386A453EA1D99A4BB4C3C96D52A2F61478802F85C6700BBEABB3B3",
      "bank_sha256": "8F43991F3D542AF08DA3DF0135C8E311E9E7B5BDD21175577A3D9185E846A63A",
      "sha256": "efc851426b1b54bc2bf9cbd2ad80acfbd9c30349e173c5fd7795e649eb0f94ff",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "889/30",
        "duration": "30.000000",
        "nb_frames": "889"
      },
      "evidence": "integrated-equipment-close-corvo-v3/take.json",
      "requirements": [
        "sword_draw_holster",
        "sword_ready_idle",
        "stand_to_crouch",
        "crouch_to_stand"
      ],
      "tests": [
        "equipment-bank-v3-test/result.json",
        "equipment-pause-v3/result.json",
        "equipment-v3-priority/result.json"
      ],
      "visual_review": {
        "contact_sheet": "integrated-equipment-close-corvo-v3/contact.jpg",
        "observations": [
          "Full body visible in native Dunwall room.",
          "Crouched equipment posture no longer adds the complete crouch bend twice.",
          "Native sword changes between the hand and hip at the authored reach."
        ],
        "accepted": false
      },
      "limitations": [
        "One stationary view per role; fine finger contact, moving draw/holster and arbitrary interruption need additional visual review.",
        "Crouched variants use a bounded posture adaptation of the native standing motion.",
        "No general family or complete movement acceptance from this take.",
        "Pause cleanup is verified; automatic session rearm and full death/load lifecycle remain unfinished."
      ]
    },
    {
      "video": "integrated-equipment-close-emily-v3.mp4",
      "character": "emily",
      "label": "Emily · Native sword draw/holster, standing and crouched",
      "actual_capture_hz": 29.666666666666668,
      "container_fps": "89/3",
      "timing": "Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.",
      "kind": "game",
      "travelling": false,
      "gameplay_bound": true,
      "complete": false,
      "visual_accepted": false,
      "scope": "Native first-person inputs and exact owned equipment scenes drive whole-body draw/holster and body-relative native clone sword handoff over Steam.",
      "chapters": [
        {
          "name": "Armed standing",
          "seconds": 1.124,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Holster sword",
          "seconds": 2.352,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 5.447,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 6.927,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Draw sword at body-relative hand contact",
          "seconds": 8.765,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouch",
          "seconds": 11.668,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched holster",
          "seconds": 13.74,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 16.85,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stationary grip inspection",
          "seconds": 18.232,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Crouched draw",
          "seconds": 20.129,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Stand",
          "seconds": 23.05,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Guard after equipment change",
          "seconds": 25.457,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        },
        {
          "name": "Final recovery",
          "seconds": 27.928,
          "timing_precision": "Command issue relative to recorder launch; native response and recorder initialization add latency."
        }
      ],
      "dll_sha256": "0E32032996386A453EA1D99A4BB4C3C96D52A2F61478802F85C6700BBEABB3B3",
      "bank_sha256": "674A9EF1004960E80D1B91932A5C5062A68FD3598E05DED0E2D51F330EAE8AD9",
      "sha256": "15fa00b8da00a5f290a1349358d10d20852f5f0a2ee40b0794c37a42932034a0",
      "capture": {
        "width": 944,
        "height": 540,
        "avg_frame_rate": "89/3",
        "duration": "30.000000",
        "nb_frames": "890"
      },
      "evidence": "integrated-equipment-close-emily-v3/take.json",
      "requirements": [
        "sword_draw_holster",
        "sword_ready_idle",
        "stand_to_crouch",
        "crouch_to_stand"
      ],
      "tests": [
        "equipment-bank-v3-test/result.json",
        "equipment-pause-v3/result.json",
        "equipment-v3-priority/result.json"
      ],
      "visual_review": {
        "contact_sheet": "integrated-equipment-close-emily-v3/contact.jpg",
        "observations": [
          "Full body visible in native Dunwall room.",
          "Crouched equipment posture no longer adds the complete crouch bend twice.",
          "Native sword changes between the hand and hip at the authored reach."
        ],
        "accepted": false
      },
      "limitations": [
        "One stationary view per role; fine finger contact, moving draw/holster and arbitrary interruption need additional visual review.",
        "Crouched variants use a bounded posture adaptation of the native standing motion.",
        "No general family or complete movement acceptance from this take.",
        "Pause cleanup is verified; automatic session rearm and full death/load lifecycle remain unfinished."
      ]
    }
  ],
  "native_sources_sampled": 1889,
  "first_person_constraints": false,
  "unchanged_live_dll": "CF60C5EAB9A08573F128F2EF3958F9237BC92FCDC5B551682601DF44C6BDBD46",
  "remaining": [
    {
      "id": "ragdoll_handoff",
      "family": "combat"
    },
    {
      "id": "scripted_scene_pose_handoff",
      "family": "interactions"
    },
    {
      "id": "cutscene_enter_exit",
      "family": "interactions"
    },
    {
      "id": "map_load_checkpoint_recovery",
      "family": "interactions"
    },
    {
      "id": "incapacitation_and_rescue_if_supported",
      "family": "interactions"
    }
  ],
  "limitations": [
    "Authored candidates are not visual acceptance or complete game-state bindings.",
    "Right game-window capture remains blocked by monitor crop error; left capture recovered.",
    "Two 39.73-second native reading clips exceed the current 30-second in-game review duration limit; offline recordings are complete.",
    "Offline native helper/cloth preview approximations differ from actual D2 rendering.",
    "Contact targets, held props, paired victims, character powers, many transitions and contextual variants remain incomplete."
  ],
  "review": "natural-moveset/review.html",
  "catalog": "natural-moveset/review-catalog.json",
  "integrity": "natural-moveset/authoring-pass-integrity.json",
  "stable_milestone": "natural-moveset/milestone-CF60",
  "tests": [
    "window-v5-roundtrip.json",
    "slide-v4-roundtrip.json",
    "ammo-changes-v1-roundtrip.json",
    "window-choke-v2-roundtrip.json",
    "top-transition-v1-roundtrip.json",
    "water-exit-v3-roundtrip.json",
    "trap-surface-v1-roundtrip.json",
    "carry-release-v3-roundtrip.json",
    "ladder-top-entry-v1-roundtrip.json",
    "body-lift-v2-roundtrip.json",
    "body-throw-v1-roundtrip.json",
    "gaits-corvo-2/result.json",
    "gaits-emily-1/result.json",
    "gait-priority-1/result.json",
    "stance-pose-test-1/result.json",
    "stance-roundtrip.json",
    "mixamo-v5-pose-test/result.json",
    "mixamo-v5-roundtrip.json",
    "review-feedback-export-test.json",
    "guard-v1-roundtrip.json",
    "shadow-v1-roundtrip.json",
    "mechanisms-v4-roundtrip.json",
    "carry-v3-roundtrip.json",
    "object-carry-v1-roundtrip.json",
    "item-transitions-v1-roundtrip.json",
    "ladder-v2-roundtrip.json",
    "water-v1-roundtrip.json",
    "contextual-v1-roundtrip.json",
    "crouched-pairs-v1-roundtrip.json",
    "blink-assault-v1-roundtrip.json"
  ]
}
````
