# carry-preparation-findings-v17.json

Original: [work/natural-moveset/carry-preparation-findings-v17.json](<../../../../work/natural-moveset/carry-preparation-findings-v17.json>)

SHA-256: `905B3AD7DB0C159EE0878448963DF1B5F6B6082ED14E668DAD8B5B56AA0347E9`; 2240 bytes.

## Original saved record

````json
{
  "build": "287E50874C2CCB796844BAE2C1360091EA11E4F1D6AB174FF1B7666D7DC350F4",
  "baseline_failure": "native-pickup-stream-corvo-v16/stream-validation.json",
  "cases": {
    "corvo": {
      "evidence": "native-pickup-stream-corvo-v17/stream-validation.json",
      "video": "native-pickup-stream-corvo-v17/native.mp4",
      "checks": 10,
      "transitions": [
        {
          "ms": 1,
          "ready": true,
          "result": "ready",
          "state": 0,
          "carry": 0
        },
        {
          "ms": 1878,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 1
        },
        {
          "ms": 2813,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 2
        }
      ],
      "actual_capture_hz": 28.25
    },
    "emily": {
      "evidence": "native-pickup-stream-emily-v17/stream-validation.json",
      "video": "native-pickup-stream-emily-v17/native.mp4",
      "checks": 10,
      "transitions": [
        {
          "ms": 1,
          "ready": true,
          "result": "ready",
          "state": 0,
          "carry": 0
        },
        {
          "ms": 1781,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 1
        },
        {
          "ms": 2829,
          "ready": true,
          "result": "ready",
          "state": 14,
          "carry": 2
        }
      ],
      "actual_capture_hz": 27.25
    }
  },
  "confirmed": "Native phase1 Carry45 owns its target before a scene exists. Running0+Carry45+Equip53 is a valid temporary grounded state. The previous reader rejected it and the peer stopped as stale.",
  "fix": "Strict read-only preparation reader plus existing unique authored target resolution. Send grounded presentation until a validated native carry scene begins. Existing 250ms timeout stays unchanged.",
  "remaining": [
    "No authored remote lifting clip connected during native In.",
    "Remote victim pickup and snapshot blending remain separate experiments.",
    "Crouched preparation has not been observed and remains refused."
  ],
  "complete": false
}
````
