# ranged-draw-overlap-findings-v1.json

Original: [work/natural-moveset/ranged-draw-overlap-findings-v1.json](<../../../../work/natural-moveset/ranged-draw-overlap-findings-v1.json>)

SHA-256: `00D69DD1EA309BF246B9F25D5ED238CFBE42F457AD6A400196C5A4AFD6ABEF9C`; 3450 bytes.

## Original saved record

````json
{
  "complete": false,
  "visual_accepted": false,
  "reason": "Native crouched Draw65/EquipChange64/53 overlap was not recognized by the bounded state reader.",
  "original_take": "ranged-emily-empty-crossbow-cache-d58d-v1",
  "original_build": "D58DEA2F56D7DFC7D2ACDACFF7A1A1612564FA5925A52AC8BF48E99EDB60684B",
  "failed_samples": [
    {
      "index": 87,
      "ms": 15193,
      "player": {
        "tick": 267618328,
        "game": "0x210f9c49050",
        "player": "0x21118a2f730",
        "machine": "0x2113d61b100",
        "mode": "0x21159ccde10",
        "stance": 1,
        "defaultMode": true,
        "motionValid": false,
        "motion": 0,
        "position": [
          -33.5,
          -153,
          66.4036484
        ],
        "active": [
          {
            "id": 6,
            "vtableRva": "0x1c84258",
            "type": ".?AVarkPlayerStateSneaking@@"
          },
          {
            "id": 65,
            "vtableRva": "0x1c78a68",
            "type": ".?AVarkPlayerStateDraw@@",
            "sequence24Research": 0
          },
          {
            "id": 64,
            "vtableRva": "0x1c79cf8",
            "type": ".?AVarkPlayerStateEquipChange@@",
            "sequence24Research": 237
          },
          {
            "id": 53,
            "vtableRva": "0x1c79cf8",
            "type": ".?AVarkPlayerStateEquipChange@@",
            "sequence24Research": 238
          }
        ],
        "keyholeSceneValid": false,
        "keyholeReaderStage": 1,
        "landingSceneValid": false,
        "combatSceneValid": false,
        "vaultSceneValid": false,
        "traversalSceneValid": false,
        "traversalReaderStage": 1,
        "equipmentSceneValid": false,
        "powerSceneValid": false,
        "rangedSceneValid": false,
        "vaultSensorResearch": {
          "front38": 0,
          "clearF0": 0,
          "minSpeed70": 3.6500001,
          "valuesF4to10c": [
            0,
            0,
            0,
            0,
            0,
            0,
            0
          ],
          "velocityCandidate33c": [
            0,
            0,
            9.12696123e-06
          ]
        }
      }
    }
  ],
  "candidate_build": "05377F9EAB5EEBA983CD35C9C3FC8E1AA3F349EDC24CDB42E0C8F15FD3B78065",
  "candidate_archive": "work/movement-integration/candidate-ranged-draw-0537-v1",
  "change": "Exact grounded four-state combination only; class, unique ID, mode ownership, native physical stance and owned ranged scene/weapon/left-arm validation retained.",
  "unit_tests": "work/movement-integration/d2_world_identity_tests-ranged-draw-overlap-v1.log",
  "runtime_replay": {
    "passed": true,
    "takes": [
      "ranged-emily-empty-crossbow-cache-0537-v1",
      "ranged-emily-empty-gun-cache-0537-v1",
      "ranged-corvo-standing-crossbow-0537-v1"
    ],
    "checks": 27,
    "exact_four_state_resampled": false,
    "scope": "Original exact four-state combination reproduced in memory tests; subsequent game sequences pass without recapturing that short overlap."
  },
  "ghidra": {
    "program": "Dishonored2.exe",
    "bookmark": "141c78a68 / ConfirmedNativeDrawOverlap",
    "xrefs": [
      "1408d3006",
      "1408d3014"
    ],
    "function": "1408d46e0 returns runtime class metadata DAT1439bcdd0; RTTI name observed independently at runtime"
  }
}
````
