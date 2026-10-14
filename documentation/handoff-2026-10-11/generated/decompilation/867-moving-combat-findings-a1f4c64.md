# moving-combat-findings.json

Original: [work/movement-integration/moving-combat-findings.json](<../../../../work/movement-integration/moving-combat-findings.json>)

SHA-256: `B26896928D9A76F7EFB7F6022F904148E4B72BBA09252EB547966A62B255D9F1`; 954 bytes.

## Original saved record

````json
{
  "complete": false,
  "visual_accepted": false,
  "dll": "A0B648425DA7C8EB9D5C138BF0550434E124E6574D0085F5021D14691C346471",
  "bank": "D2MOVE16 / 141 clips",
  "tests": [
    "integrated-moving-combat-corvo-v1/result.json",
    "integrated-moving-combat-emily-v1/result.json"
  ],
  "checks": 16,
  "confirmed": [
    "Both roles perform forward, side, backward and crouched moving sword strikes and forward moving guard.",
    "Movement cadence continues during the authored sword action, with stopped recovery.",
    "Movement semantics use the same delayed timeline as root and native scene phases.",
    "Health, native first person, sword attachment and protected files stay unchanged."
  ],
  "pending": [
    "Close foot/hand review and remaining direction, speed, action and contact cases.",
    "Full movement integration, lifecycle and acceptance."
  ],
  "rollback": "work/movement-integration/candidate-moving-A0B6"
}
````
