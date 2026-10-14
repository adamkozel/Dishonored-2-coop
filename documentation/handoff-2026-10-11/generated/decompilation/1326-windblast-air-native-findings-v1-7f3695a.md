# windblast-air-native-findings-v1.json

Original: [work/natural-moveset/windblast-air-native-findings-v1.json](<../../../../work/natural-moveset/windblast-air-native-findings-v1.json>)

SHA-256: `718031DC1FDB5F4802C6050A364DAAE5385FEC4495351B8D8045FB42006B0F2A`; 15550 bytes.

## Original saved record

````json
{
  "exact_game_sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "source_test": "native-windblast-corvo-air-v2/result.json",
  "source_dll": "80E9E28879F84E427BF05BFEC45357430EE181376C9A1669CA28D4A5E9670F8A",
  "confirmed": [
    "Native jump before LT produces actual Windblast casts during Jump and Fall.",
    "One owned 1000ms Cast_Ou_Shockwave scene persists through landing.",
    "Native UsePower63 overlays both Jump2 and Fall3; the prior ground-only motion gate incorrectly rejected these combinations.",
    "Native effect source uses the same exact world declaration and parent ownership in the air."
  ],
  "hypotheses": [],
  "evidence": [
    {
      "time": 1.985,
      "native_state_ids": [
        2
      ],
      "native_state_classes": [
        "0x1c85028"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 16,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": []
    },
    {
      "time": 2.086,
      "native_state_ids": [
        2,
        63
      ],
      "native_state_classes": [
        "0x1c85028",
        "0x1c80ed8"
      ],
      "old_motion_reader_valid": false,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 116,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 2.191,
      "native_state_ids": [
        3,
        63
      ],
      "native_state_classes": [
        "0x1c845c8",
        "0x1c80ed8"
      ],
      "old_motion_reader_valid": false,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 216,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 2.366,
      "native_state_ids": [
        3,
        63
      ],
      "native_state_classes": [
        "0x1c845c8",
        "0x1c80ed8"
      ],
      "old_motion_reader_valid": false,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 358,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 2.483,
      "native_state_ids": [
        0,
        63
      ],
      "native_state_classes": [
        "0x1c85628",
        "0x1c80ed8"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 508,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 2.594,
      "native_state_ids": [
        0,
        63
      ],
      "native_state_classes": [
        "0x1c85628",
        "0x1c80ed8"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 600,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 2.705,
      "native_state_ids": [
        0,
        63
      ],
      "native_state_classes": [
        "0x1c85628",
        "0x1c80ed8"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 700,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 2.81,
      "native_state_ids": [
        0
      ],
      "native_state_classes": [
        "0x1c85628"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 816,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 2.917,
      "native_state_ids": [
        0
      ],
      "native_state_classes": [
        "0x1c85628"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 219,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 925,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 6.072,
      "native_state_ids": [
        2
      ],
      "native_state_classes": [
        "0x1c85028"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 220,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 0,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274
      ]
    },
    {
      "time": 6.171,
      "native_state_ids": [
        2,
        63
      ],
      "native_state_classes": [
        "0x1c85028",
        "0x1c80ed8"
      ],
      "old_motion_reader_valid": false,
      "scenes": [
        {
          "id": 220,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 133,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274,
        561
      ]
    },
    {
      "time": 6.299,
      "native_state_ids": [
        3
      ],
      "native_state_classes": [
        "0x1c845c8"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 220,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 241,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274,
        561
      ]
    },
    {
      "time": 6.427,
      "native_state_ids": [
        3
      ],
      "native_state_classes": [
        "0x1c845c8"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 220,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 383,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274,
        561
      ]
    },
    {
      "time": 6.543,
      "native_state_ids": [
        0
      ],
      "native_state_classes": [
        "0x1c85628"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 220,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 500,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274,
        561
      ]
    },
    {
      "time": 6.645,
      "native_state_ids": [
        0
      ],
      "native_state_classes": [
        "0x1c85628"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 220,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 608,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274,
        561
      ]
    },
    {
      "time": 6.911,
      "native_state_ids": [
        0
      ],
      "native_state_classes": [
        "0x1c85628"
      ],
      "old_motion_reader_valid": true,
      "scenes": [
        {
          "id": 220,
          "asset": "midnight/gameplay/player/base/powers/power_windblast_cast.midnightscene",
          "subscene": "Cast_Ou_Shockwave",
          "duration": 1000,
          "clock": 858,
          "bindings": [
            {
              "actor": "0x26ac2783e90",
              "handle": 67862,
              "reciprocal": true
            },
            {
              "actor": "0x26a98e3efc0",
              "handle": 1,
              "reciprocal": true
            }
          ]
        }
      ],
      "native_effect_ids": [
        274,
        561
      ]
    }
  ],
  "implementation_candidate": {
    "dll": "D662AA6718B0F42A643CA1B53D310B3B58C45671F248C937CE2E36974E29A2EB",
    "shadow_protocol": 24,
    "bank": 28,
    "unit_checks": [
      "Exact native scene + reciprocal left-arm/player binding",
      "Jump/Fall with UsePower preserves motion and scoped air metadata",
      "A caller without an owned scene cannot request the overlay air path",
      "Old semantic packet version and stale apex refused"
    ]
  },
  "complete": false,
  "runtime_integration": {
    "dll": "D662AA6718B0F42A643CA1B53D310B3B58C45671F248C937CE2E36974E29A2EB",
    "tests": [
      "windblast-corvo-air-network-v28-v1/result.json",
      "windblast-corvo-air-empty-moving-v28-v1/result.json"
    ],
    "checks": 27,
    "recordings": [
      "windblast-corvo-air-network-v28-v1.mp4",
      "windblast-corvo-air-network-v28-v1-source.mp4",
      "windblast-corvo-air-empty-moving-v28-v1.mp4",
      "windblast-corvo-air-empty-moving-v28-v1-source.mp4"
    ],
    "visual_inspected": true,
    "visual_accepted": false
  },
  "remaining": [
    "Two ordinary jumping basic casts per take, with the explicitly recorded equipment/travel context.",
    "Double-jump casting, upgrades, physics impacts and full lifecycle remain unverified.",
    "Selected temporal frames inspected; complete continuous motion quality is not accepted.",
    "No recorded audio; sound is not audibly verified."
  ]
}
````
