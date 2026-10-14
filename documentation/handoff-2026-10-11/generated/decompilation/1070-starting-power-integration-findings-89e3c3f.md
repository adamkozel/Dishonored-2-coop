# starting-power-integration-findings.json

Original: [work/movement-integration/starting-power-integration-findings.json](<../../../../work/movement-integration/starting-power-integration-findings.json>)

SHA-256: `DBD3D51881AB1FCD79E3F3494B52D497D86058238772BB8B8C82C645CF77C42D`; 1460 bytes.

## Original saved record

````json
{
  "dll_sha256": "3EC71AF4416817FDB70057CC50C7603B30B14F0371A6CE756DF0D7671A2FA47F",
  "bank": "D2MOVE15 / 126 clips",
  "action_protocol": "14 / 236 bytes",
  "blink_fx_protocol": "1 / 116 bytes; native Steam reliable events",
  "complete": false,
  "visual_accepted": false,
  "body_checks": 13,
  "tests": [
    "integrated-blink-fx-v1/result.json",
    "integrated-power-emily-v3/result.json",
    "blink-fx-stream-stop-v1/result.json",
    "blink-fx-pause-v1/result.json"
  ],
  "confirmed": [
    "Both starting powers drive native-role body phases and recover through native cancellation.",
    "Blink state12 eye/foot relationship and completion bit correlate with actual native casts.",
    "Blink departure and arrival effects use the native FX manager with exact owner/ID/declaration identity.",
    "Successful Blink produces two FX events; aim cancellation produces none and duplicate arrival cannot replay effects.",
    "Stopping the stream and native pause/resume retire owned Blink effects without deferred replay; 11 checks pass on the same DLL."
  ],
  "pending": [
    "Far Reach tether/impact/target representation.",
    "Other powers and starting-power variants.",
    "Power interruption/lifecycle matrix, broader movement regressions and final visual acceptance."
  ],
  "rollback": "work/movement-integration/verified-blink-network-3EC7",
  "protected_files_unchanged": true,
  "lifecycle_checks": 11
}

````
